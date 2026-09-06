#include "decomp_assert.h"
#include "nusound_decoder.hpp"

#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nusound/nusound_buffer.hpp"
#include "nu2api/nusound/nusound_sample.hpp"
#include "nu2api/nusound/nusound_voice.hpp"
#include "nu2api/nusound/nusound_system.hpp"

#include <new>
#include <string.h>

// libTTapp.so NuSoundDecoder::sDecodeThread @0x11e90d0.
NuSoundDecodeThread *NuSoundDecoder::sDecodeThread = NULL;
// Original static initializer at 0xde53d passes a maximum of one signal.
NuThreadSemaphore NuSoundDecodeThread::sShutdownSemaphore(1);
i32 NuSoundDecodeThread::sThreadPriority = 2;

// libTTapp.so 0x31eed0: the decoder wraps the source, carries two ring
// buffer objects and starts closed. The source's stream descriptor (the
// loaded NuSoundHeaderOGG) is shared with the decoder so the decode path can
// ov_read() through the handle the loader opened.
NuSoundDecoder::NuSoundDecoder(char const *name, NuSoundSource *wrapped)
    : NuSoundSource(NULL, SourceType::STREAMING, NuSoundSource::FeedType::STREAMING) {
    (void)name;
    DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NuSoundBuffer) == 0x40, "decoder ring buffer stride");
    DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NuSoundDecoder) == 0xe8, "decoder callback base offset");
    DECOMP_ASSERT(sizeof(void *) != 4 || __builtin_offsetof(NuSoundDecoder, buffers) == 0x24, "embedded decoder buffers");
    DECOMP_ASSERT(sizeof(void *) != 4 || __builtin_offsetof(NuSoundDecoder, decoded_bytes) == 0xb8, "decoder byte counter");
    DECOMP_ASSERT(sizeof(void *) != 4 || __builtin_offsetof(NuSoundDecoder, total_decoded_bytes) == 0xc8, "decoder total byte counter");
    DECOMP_ASSERT(sizeof(void *) != 4 || __builtin_offsetof(NuSoundDecoder, decode_mutex) == 0xdc, "decoder completion mutex");
    // The decode-sync pair the decode thread raises per completed request
    // (device offsets +0xdc/+0xe0/+0xe4).
    pthread_mutex_init(&this->decode_mutex, NULL);
    pthread_cond_init(&this->decode_cond, NULL);
    this->source = wrapped;
    this->ring_count = 0;
    this->consumed_pos = 0;
    this->decode_pos = 0;
    this->field_0xd4 = 0;
    this->buffer_size = 0x2000;
    this->field_0xe5 = false;
    this->decode_done = false;
    this->field_0xc0 = 0;
    this->field_0xc4 = 0;
    this->total_decoded_bytes = 0;
    this->decoded_bytes = 0;
    this->stream_open = false;

    this->SetStreamDesc(wrapped->GetStreamDesc());
    this->field_0x1c = 1;
}

// libTTapp.so 0x31ede0 / 0x31ee70.
NuSoundDecoder::~NuSoundDecoder() {
    pthread_mutex_destroy(&this->decode_mutex);
    pthread_cond_destroy(&this->decode_cond);
}

__attribute__((weak)) void NuSoundDecoder::Reset() {
}

// libTTapp.so 0x31ec90: fill the ring buffers upfront. Each buffer is
// allocated at the decoder's buffer size and decoded via the virtual Decode;
// the loop stops after the first two buffers or once the whole stream has
// been decoded (the remaining chunks decode on demand through the decode
// thread as the voice consumes the ring).
bool NuSoundDecoder::OpenStream(bool loop) {
    u64 decoded = 0;
    this->source->OpenStream(loop);

    NuSoundStreamDesc *desc = this->source->GetStreamDesc();
    u64 total = desc->GetDecodedLengthBytes();
    this->ring_count = 0;
    this->consumed_pos = 0;
    this->decode_pos = 0;

    NuSoundBuffer *ring_buffers = this->buffers;
    for (i32 i = 0; decoded < total && i < 2; i++) {
        NuSoundBuffer &buffer = ring_buffers[i];

        if (buffer.Allocate(this->buffer_size, NuSoundSystem::MemoryDiscipline::DECODER) != 1) {
            this->CloseStream();
            return false;
        }

        u64 got = this->Decode(*this->source, buffer, loop);
        decoded += got;
        this->ring_count++;
        this->decode_pos++;
        this->buffers_started++;

    }

    this->stream_open = true;
    return true;
}

// libTTapp.so 0x31e9c0.
bool NuSoundDecoder::IsStreamOpen() const {
    return this->source->IsStreamOpen() && this->stream_open;
}

// libTTapp.so 0x31ebb0.
void NuSoundDecoder::CloseStream() {
    this->closing = true;
    while ((i32)this->field_0xd4 > 0) {
        pthread_mutex_lock(&this->decode_mutex);
        while (!this->decode_done) {
            pthread_cond_wait(&this->decode_cond, &this->decode_mutex);
        }
        this->decode_done = this->field_0xe5;
        pthread_mutex_unlock(&this->decode_mutex);
    }
    for (i32 i = 0; i < (i32)this->ring_count; ++i) {
        this->buffers[i].Free();
    }
    this->source->CloseStream();
    this->stream_open = false;
}

// libTTapp.so 0x31f0e0: allocate the singleton decode thread.
void NuSoundDecoder::Initialise() {
    // 0xe1c is the device-side object size; the host NuThreadSemaphore and
    // weakptr types differ in size, so size the block from the real object.
    NuSoundDecodeThread *thread = (NuSoundDecodeThread *)NuMemoryGet()->GetThreadMem()->_BlockAlloc(
        (u32)sizeof(NuSoundDecodeThread), 4, 1, "", 7);

    if (thread != NULL) {
        new (thread) NuSoundDecodeThread();
    }

    NuSoundDecoder::sDecodeThread = thread;
}

// libTTapp.so 0x31fe00.
void NuSoundDecoder::Shutdown() {
    if (sDecodeThread != NULL) {
        sDecodeThread->Shutdown();
        NuSoundDecodeThread *thread = sDecodeThread;
        if (thread != NULL) {
            thread->~NuSoundDecodeThread();
            NuMemoryGet()->GetThreadMem()->BlockFree(thread, 0);
        }
        sDecodeThread = NULL;
    }
}

bool NuSoundDecoder::IsLocked() const {
    return this->source->IsLocked();
}

// libTTapp.so 0x31eb50: the decoder lock is what keeps the ring buffers'
// device addresses valid. The voice holds it for its whole lifetime (the
// NuSoundVoice ctor calls the source's Lock slot), so every ring buffer
// stays locked at count >= 1 and SubmitBuffer always hands the device a
// live address while the voice exists.
void NuSoundDecoder::Lock() {
    for (i32 i = 0; i < (i32)this->ring_count; i++) {
        this->buffers[i].Lock();
    }
}

// libTTapp.so 0x31eaf0.
void NuSoundDecoder::Unlock() {
    for (i32 i = 0; i < (i32)this->ring_count; i++) {
        this->buffers[i].Unlock();
    }
}

// libTTapp.so 0x31eab0 / 0x31ea40: voice bookkeeping on the wrapped source.
void NuSoundDecoder::VoiceReference() {
    NuSoundSource::VoiceReference();
    this->source->VoiceReference();
}

void NuSoundDecoder::VoiceRelease() {
    this->Reset();
    this->consumed_pos = 0;
    this->decode_pos = 0;
    this->buffers_started = 0;
    NuSoundSource::VoiceRelease();
    this->source->VoiceRelease();
}

// libTTapp.so 0x31ea10: min(ring buffers, sNumInitialBuffersByType[type]).
// Every source's type field is 1 (the original's NuSoundSource ctor hardcodes
// it), so the table's second entry, 2, is the cap.
u32 NuSoundDecoder::GetNumInitialBuffers() const {
    const u32 type = (u32)this->feed_type;
    const u32 cap = NuSoundSource::sNumInitialBuffers[type];
    return this->ring_count < cap ? this->ring_count : cap;
}

// libTTapp.so 0x31f000.
unsigned int NuSoundDecoder::GetNumRingBuffers() const {
    return this->ring_count;
}

// libTTapp.so 0x31fe80.
u32 NuSoundDecoder::GetMaxBufferSize() {
    return this->buffer_size;
}

// libTTapp.so 0x31fa90: hand the voice the next ring buffer. When a decoded
// buffer is ahead of the consume cursor and still holds data, it is handed to
// the caller's SubmitBuffer callback under the sample critical section and
// the consume cursor advances. Otherwise the request goes to the decode
// thread (RequestDecode) and returns immediately; the thread decodes the
// buffer and performs the same SubmitBuffer hand-off on its own stack.
void NuSoundDecoder::RequestBuffer(bool loop, NuSoundWeakPtr<NuSoundBufferCallback> callback) {
    if (this->decode_pos > this->consumed_pos) {
        NuSoundBuffer &ready = this->buffers[this->consumed_pos % this->ring_count];
        NuSoundBuffer::Context &context = ready.GetCurrentContext();

        if (context.size2 != 0) {
            pthread_mutex_lock(&NuSoundSample::sCriticalSection);
            NuSoundBufferCallback *cb = (NuSoundBufferCallback *)callback.obj;
            if (cb != NULL) {
                cb->SubmitBuffer(&ready);
            }
            pthread_mutex_unlock(&NuSoundSample::sCriticalSection);
        }

        this->consumed_pos++;
        return;
    }

    // libTTapp.so 0x31fb08: queue buffers[decode_pos % ring_count] on the
    // decode thread and return; the thread performs the decode and the
    // SubmitBuffer hand-off. The ring slot was already allocated by the
    // OpenStream prefill, and the consume cursor advances on the caller side.
    {
        NuSoundBuffer &buffer = this->buffers[this->decode_pos % this->ring_count];
        NuSoundDecoder::sDecodeThread->RequestDecode(*this, buffer, callback, loop);
    }
    this->decode_pos++;
    this->consumed_pos++;
}

// ---------------------------------------------------------------------------
// the decode thread
// ---------------------------------------------------------------------------

// libTTapp.so 0x31f010: 128 zeroed loader slots behind a 128-signal
// semaphore, then the worker thread (priority 2, cafe/xbox core 2).
NuSoundDecodeThread::NuSoundDecodeThread() : semaphore(128) {
    DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(Loader) == 0x1c, "decode queue entry stride");
    DECOMP_ASSERT(sizeof(void *) != 4 || __builtin_offsetof(NuSoundDecodeThread, loaders) == 4, "decode queue offset");
    DECOMP_ASSERT(sizeof(void *) != 4 || __builtin_offsetof(NuSoundDecodeThread, semaphore) == 0xe0c, "decode semaphore offset");
    DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NuSoundDecodeThread) == 0xe1c, "decode thread allocation size");
    memset(this->loaders, 0, sizeof(this->loaders));
    this->tail_index = 0;
    this->head_index = 0;
    this->thread = NuCore::m_threadManager->CreateThread(NuSoundDecodeThread::ThreadFunc, this, sThreadPriority, "NuSoundDecode", 0,
                                                         NUTHREADCAFECORE_UNKNOWN_2, NUTHREADXBOX360CORE_UNKNOWN_2);
}

NuSoundDecodeThread::~NuSoundDecodeThread() {
}

// libTTapp.so 0x31f670: bump the decoder's pending-request count, park
// {decoder, buffer, callback, loop} in the next loader slot and signal the
// worker. The callback weakptr is copied, which re-links its list node into
// the callback's weak list so the worker can re-validate the callback.
void NuSoundDecodeThread::RequestDecode(NuSoundDecoder &decoder, NuSoundBuffer &buffer,
                                        NuSoundWeakPtr<NuSoundBufferCallback> callback, bool loop) {
    __sync_fetch_and_add(&decoder.field_0xd4, 1);

    Loader request = {&decoder, &buffer, callback, loop};
    Loader &entry = this->loaders[this->tail_index % 128];
    new (&entry) Loader(request);
    __sync_fetch_and_add(&this->tail_index, 1);

    this->semaphore.Signal();
}

// libTTapp.so 0x31f190: pop one loader entry, re-validate the callback
// weakptr, decode the buffer through the decoder's virtual Decode and hand
// it to the callback's SubmitBuffer under the sample critical section.
void NuSoundDecodeThread::ThreadFunc(void *self_) {
    NuSoundDecodeThread *self = (NuSoundDecodeThread *)self_;

    while (true) {
        self->semaphore.Wait();

        Loader &entry = self->loaders[self->head_index % 128];
        NuSoundDecoder *decoder = entry.decoder;
        NuSoundBuffer *buffer = entry.buffer;
        NuSoundWeakPtr<NuSoundBufferCallback> callback;
        callback.Set((NuSoundBufferCallback *)entry.callback.obj);
        bool loop = entry.loop;
        entry.~Loader();
        __sync_fetch_and_add(&self->head_index, 1);

        if (decoder == NULL) {
            sShutdownSemaphore.Signal();
            return;
        }

        // 0x31f4df: closing requests skip Decode but still signal completion.
        if (!decoder->closing) {
            decoder->Decode(*decoder->GetEncodedSource(), *buffer, loop);
            decoder->buffers_started++;
            pthread_mutex_lock(&NuSoundSample::sCriticalSection);
            if (callback.obj != NULL) {
                ((NuSoundBufferCallback *)callback.obj)->SubmitBuffer(buffer);
            }
            pthread_mutex_unlock(&NuSoundSample::sCriticalSection);
        }

        // libTTapp.so 0x31f1c8: the request is complete; drop the pending
        // count (a locked atomic sub on device) and raise the decode-done
        // flag through the decoder's decode-sync pair (mutex +0xdc,
        // cond +0xe0, done flag +0xe4).
        __sync_fetch_and_sub(&decoder->field_0xd4, 1);
        pthread_mutex_lock(&decoder->decode_mutex);
        if (decoder->decode_done == false) {
            decoder->decode_done = true;
            if (decoder->field_0xe5) {
                pthread_cond_broadcast(&decoder->decode_cond);
            } else {
                pthread_cond_signal(&decoder->decode_cond);
            }
        }
        pthread_mutex_unlock(&decoder->decode_mutex);
    }
}

// libTTapp.so 0x31fc90.
void NuSoundDecodeThread::Shutdown() {
    NuSoundWeakPtrListNode::sPtrListLock.Lock();
    NuSoundWeakPtrListNode::sPtrListLock.Unlock();
    Loader &entry = this->loaders[this->tail_index % 128];
    new (&entry) Loader;
    entry.decoder = NULL;
    entry.buffer = NULL;
    entry.callback.Set(NULL);
    entry.loop = false;
    __sync_fetch_and_add(&this->tail_index, 1);
    this->semaphore.Signal();
    sShutdownSemaphore.Wait();
    NuSoundWeakPtrListNode::sPtrListLock.Lock();
    NuSoundWeakPtrListNode::sPtrListLock.Unlock();
}
