#pragma once

#include "nu2api/nucore/common.h"
#include "nu2api/nucore/numemory.h"

class NuSoundStreamDesc;
class NuSoundBufferCallback;
template <typename T> class NuSoundWeakPtr;

class NuSoundSource {

  public:
    enum class FeedType : u32 {
        ZERO = 0,
        STREAMING = 1,
    };

    enum class SourceType : u32 {
        ZERO = 0,
        STREAMING = 1,
    };

  public:
    FeedType feed_type;
    SourceType source_type;
    u16 name_length;
    u16 name_capacity;
    const char *name;
    NuSoundStreamDesc *stream_desc;
    i32 field_0x18;
    i32 field_0x1c;

  public:
    NuSoundSource(const char *file, SourceType source_type, FeedType feed_type);
    virtual ~NuSoundSource();

    static void operator delete(void *allocation) {
        NuMemoryGet()->GetThreadMem()->BlockFree(allocation, 0);
    }

    virtual const char *GetName() const;
    virtual NuSoundSource *GetEncodedSource();

    void SetStreamDesc(NuSoundStreamDesc *desc);
    NuSoundStreamDesc *GetStreamDesc() const {
        return this->stream_desc;
    }

    // Source virtuals the voice layer dispatches through (the original went
    // through the source vtable; NuSoundSample / NuSoundStreamingSample and
    // NuSoundDecoder override them).
    virtual bool OpenStream(bool loop) {
        (void)loop;
        return true;
    }
    virtual void CloseStream() {
    }
    virtual bool IsStreamOpen() const = 0;
    virtual void RequestBuffer(bool loop, NuSoundWeakPtr<NuSoundBufferCallback> callback) = 0;
    virtual u32 GetMaxBufferSize() {
        return 0;
    }
    virtual void Lock() {
    }
    virtual void Unlock() {
    }
    virtual bool IsLocked() const = 0;
    virtual u32 GetNumInitialBuffers() const;
    virtual void VoiceReference();
    virtual void VoiceRelease();

    // Number of buffers Play() requests before starting the hardware voice
    // (the original read this global through an inline accessor).
    static const u32 sNumInitialBuffers[2];
};
