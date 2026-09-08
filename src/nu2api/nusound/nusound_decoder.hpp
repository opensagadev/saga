#pragma once

#include "nu2api/nucore/android/NuThread_android.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nusound/nusound_buffer.hpp"
#include "nu2api/nusound/nusound_source.hpp"
#include "nu2api/nusound/nusound_sync.hpp"
#include "nu2api/nusound/nusound_weakptr.hpp"

class NuSoundBufferCallback;
class NuSoundDecodeThread;
class NuSoundStreamDesc;

// The decoder IS a sound source: it wraps the original source, decodes its
// encoded data into ring buffers and feeds voices from those buffers.
// Transcribed from libTTapp.so 0x31eed0 (ctor) / 0x31ec90 (OpenStream) /
// 0x31fa90 (RequestBuffer). The class is abstract: the stream-descriptor
// vtable's Decode slot is __cxa_pure_virtual and is implemented by the
// format-specific subclasses (NuSoundDecoderOGG).
class NuSoundDecoder : public NuSoundSource {
  public:
    NuSoundDecoder(char const *name, NuSoundSource *source);
    virtual ~NuSoundDecoder();

    void CloseStream();
    const char *GetName() const override;
    NuSoundSource *GetEncodedSource() override;
    static void Initialise();
    bool IsLocked() const override;
    bool IsStreamOpen() const override;
    void Lock();
    bool OpenStream(bool loop) override;
    static void Shutdown();
    void Unlock();
    void VoiceReference() override;
    void VoiceRelease() override;
    u32 GetNumInitialBuffers() const override;
    u32 GetMaxBufferSize() override;
    unsigned int GetNumRingBuffers() const;
    void RequestBuffer(bool loop, NuSoundWeakPtr<NuSoundBufferCallback> callback) override;

    // libTTapp.so @0x11e90d0: the singleton decode thread created by Initialise.
    static NuSoundDecodeThread *sDecodeThread;

    friend class NuSoundDecodeThread;

    // One decoded chunk handed to the voice layer. The base class decodes up
    // to two ring buffers upfront in OpenStream; further chunks are decoded
    // through the decode thread as the voice consumes them.
    virtual u64 Decode(NuSoundSource &source, NuSoundBuffer &buffer, bool loop) = 0;
    virtual void Reset();

  protected:
    NuSoundSource *source;    // +0x20: wrapped source
    NuSoundBuffer buffers[2]; // +0x24: two inline 0x40-byte ring buffers
    u32 buffer_size;          // bytes per ring buffer
    i32 ring_count;           // buffers filled so far
    i32 decode_pos;           // next buffer index to decode
    i32 consumed_pos;         // next buffer index to hand out
    i32 buffers_started;
    u64 decoded_bytes; // bytes decoded since stream start
    u32 field_0xc0;
    u32 field_0xc4;
    u64 total_decoded_bytes; // +0xc8
    u32 field_0xd0;
    i32 field_0xd4;   // atomic count of queued decode requests
    bool stream_open; // +0xd8
    bool closing;     // +0xd9
    u8 padding_0xda[2];
    NuSoundMutex decode_mutex;    // +0xdc: decode-completion sync pair
    NuSoundCondition decode_cond; // +0xe0
    bool decode_done;             // +0xe4
    bool decode_broadcast;        // +0xe5: manual-reset/broadcast mode
    u8 padding_0xe6[2];
};

// libTTapp.so: the async decode worker. RequestDecode parks a 0x1c-byte
// loader entry {decoder, buffer, weakptr node, loop} in a 128-slot ring
// (head index tracked atomically at thread+0xe08) and signals the thread's
// semaphore (thread+0xe0c). The worker pops entries, re-validates the
// callback weakptr, decodes the buffer through the decoder's virtual Decode
// and hands it to the callback's SubmitBuffer under the sample critical
// section (0x31f190 ThreadFunc / 0x31f670 RequestDecode).
class NuSoundDecodeThread {
  public:
    // One queued decode request (libTTapp loader entry, stride 0x1c).
    struct Loader {
        NuSoundDecoder *decoder;                        // +0x00
        NuSoundBuffer *buffer;                          // +0x04
        NuSoundWeakPtr<NuSoundBufferCallback> callback; // +0x08 (node)
        bool loop;                                      // +0x18
    };

    union LoaderSlot {
        u32 alignment;
        u8 storage[sizeof(Loader)];
    };

    NuSoundDecodeThread();
    ~NuSoundDecodeThread();

    static void ThreadFunc(void *self_);
    void Shutdown();
    void RequestDecode(NuSoundDecoder &, NuSoundBuffer &, NuSoundWeakPtr<NuSoundBufferCallback>, bool);

    NuThread *thread;            // +0x000
    LoaderSlot loaders[128];     // +0x004; raw storage, not automatically destroyed
    u32 tail_index;              // +0xe04, producer (RequestDecode) write index
    u32 head_index;              // +0xe08, consumer (ThreadFunc) read index
    NuThreadSemaphore semaphore; // +0xe0c

    static i32 sThreadPriority;
    static NuThreadSemaphore sShutdownSemaphore;
};
