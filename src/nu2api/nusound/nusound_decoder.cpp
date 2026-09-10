#include "nusound_decoder.hpp"

#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nusound/nusound_buffer.hpp"
#include "nu2api/nusound/nusound_sample.hpp"
#include "nu2api/nusound/nusound_voice.hpp"
#include "nu2api/nusound/nusound_system.hpp"

#include <cstring>
#include <new>

// libTTapp.so NuSoundDecoder::sDecodeThread @0x11e90d0.
NuSoundDecodeThread *NuSoundDecoder::sDecodeThread = NULL;
i32 NuSoundDecodeThread::sThreadPriority = 2;
NuThreadSemaphore NuSoundDecodeThread::sShutdownSemaphore(1);

// libTTapp.so 0x31eed0: the decoder wraps the source, carries two ring
// buffer objects and starts closed. The source's stream descriptor (the
// loaded NuSoundHeaderOGG) is shared with the decoder so the decode path can
// ov_read() through the handle the loader opened.
NuSoundDecoder::NuSoundDecoder(char const *name, NuSoundSource *wrapped)
    : NuSoundSource(NULL, SourceType::STREAMING, NuSoundSource::FeedType::STREAMING) {
    // The decode-sync pair the decode thread raises per completed request
    // (device offsets +0xdc/+0xe0/+0xe4).
    NuSoundMutexInit(&this->decode_mutex);
    NuSoundConditionInit(&this->decode_cond);
    (void)name;
    this->source = wrapped;

    this->ring_count = 0;
    this->consumed_pos = 0;
    this->decode_pos = 0;
    this->field_0xd4 = 0;
    this->buffer_size = 0x2000;

    NuSoundStreamDesc *desc = wrapped->GetStreamDesc();
    this->decode_broadcast = false;
    this->decode_done = false;
    this->field_0xc0 = 0;
    this->field_0xc4 = 0;
    this->total_decoded_bytes = 0;
    this->decoded_bytes = 0;
    this->stream_open = false;

    this->SetStreamDesc(desc);
    this->field_0x1c = 1;
}

// libTTapp.so 0x31ede0 / 0x31ee70.
NuSoundDecoder::~NuSoundDecoder() {
    NuSoundMutexDestroy(&this->decode_mutex);
    NuSoundConditionDestroy(&this->decode_cond);
}

__attribute__((weak)) void NuSoundDecoder::Reset() {
}

// libTTapp.so 0x31ec90: fill the ring buffers upfront. Each buffer is
// allocated at the decoder's buffer size and decoded via the virtual Decode;
// the loop stops after the first two buffers or once the whole stream has
// been decoded (the remaining chunks decode on demand through the decode
// thread as the voice consumes the ring).
bool NuSoundDecoder::OpenStream(bool loop) {
    u64 total = 0;
    u64 decoded = 0;
    this->source->OpenStream(loop);

    NuSoundStreamDesc *desc = this->source->GetStreamDesc();
    total = desc->GetDecodedLengthBytes();
    this->ring_count = 0;
    this->decode_pos = 0;
    this->consumed_pos = 0;

    for (u32 i = 0; decoded < total; i++) {
        NuSoundBuffer &buffer = this->buffers[i];

        if (buffer.Allocate(this->buffer_size, NuSoundSystem::MemoryDiscipline::DECODER) != 1) {
            this->CloseStream();
            return false;
        }

        u64 got = this->Decode(*this->source, buffer, loop);
        decoded += got;
        this->ring_count++;
        this->decode_pos++;
        this->buffers_started++;

        if (i >= 1) {
            break;
        }
    }

    this->stream_open = true;
    return true;
}

// libTTapp.so 0x31e9c0.
bool NuSoundDecoder::IsStreamOpen() const {
    return this->source->IsStreamOpen() && this->stream_open;
}

const char *NuSoundDecoder::GetName() const {
    return this->source != NULL ? this->source->GetName() : "";
}

NuSoundSource *NuSoundDecoder::GetEncodedSource() {
    return this->source;
}

// libTTapp.so 0x31ebb0: stop new decode work, drain requests already queued,
// release each decoder-pool ring buffer, then close the wrapped stream.
void NuSoundDecoder::CloseStream() {
    this->closing = true;

    while (this->field_0xd4 > 0) {
        NuSoundMutexLock(&this->decode_mutex);
        while (this->decode_done == false) {
            NuSoundConditionWait(&this->decode_cond, &this->decode_mutex);
        }
        this->decode_done = this->decode_broadcast;
        NuSoundMutexUnlock(&this->decode_mutex);
    }

    for (i32 i = 0; i < this->ring_count; i++) {
        this->buffers[i].Free();
    }

    this->source->CloseStream();
    this->stream_open = false;
}

// libTTapp.so 0x31f0e0: allocate the singleton decode thread.
void NuSoundDecoder::Initialise() {
    // 0xe1c is the device-side object size; the host NuThreadSemaphore and
    // weakptr types differ in size, so size the block from the real object.
    NuSoundDecodeThread *thread = (NuSoundDecodeThread *)NuSoundSystem::_AllocMemory(
        NuSoundSystem::MemoryDiscipline::SCRATCH, (u32)sizeof(NuSoundDecodeThread), 4,
        "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/android/nusound_decoder.cpp:69");

    if (thread != NULL) {
        new (thread) NuSoundDecodeThread();
    }

    NuSoundDecoder::sDecodeThread = thread;
}

// libTTapp.so 0x31fe00.
void NuSoundDecoder::Shutdown() {
    if (NuSoundDecoder::sDecodeThread != NULL) {
        NuSoundDecoder::sDecodeThread->Shutdown();

        NuSoundDecodeThread *thread = NuSoundDecoder::sDecodeThread;
        if (thread != NULL) {
            thread->~NuSoundDecodeThread();
            NU_FREE(thread);
        }

        NuSoundDecoder::sDecodeThread = NULL;
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
    for (i32 i = 0; i < this->ring_count; i++) {
        this->buffers[i].Lock();
    }
}

// libTTapp.so 0x31eaf0.
void NuSoundDecoder::Unlock() {
    for (i32 i = 0; i < this->ring_count; i++) {
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
            NuSoundWeakPtrListNode::sPtrAccessLock.Lock();
            NuSoundBufferCallback *cb = (NuSoundBufferCallback *)callback.obj;
            if (cb != NULL) {
                cb->SubmitBuffer(&ready);
            }
            NuSoundWeakPtrListNode::sPtrAccessLock.Unlock();
        }

        this->consumed_pos++;
        return;
    }

    NuSoundBuffer &buffer = this->buffers[this->decode_pos % this->ring_count];
    NuSoundDecoder::sDecodeThread->RequestDecode(
        *this, buffer, NuSoundWeakPtr<NuSoundBufferCallback>((NuSoundBufferCallback *)callback.obj), (loop & 1) != 0);
    this->decode_pos++;

    this->consumed_pos++;
}

// ---------------------------------------------------------------------------
// the decode thread
// ---------------------------------------------------------------------------

// libTTapp.so 0x31f010: 128 zeroed loader slots behind a 128-signal
// semaphore, then the worker thread (priority 2, cafe/xbox core 2).
NuSoundDecodeThread::NuSoundDecodeThread() : semaphore(128) {
    memset(this->loaders, 0, sizeof(this->loaders));
    this->tail_index = 0;
    this->head_index = 0;
    this->thread = NuCore::m_threadManager->CreateThread(NuSoundDecodeThread::ThreadFunc, this,
                                                         NuSoundDecodeThread::sThreadPriority, "NuSoundDecodeThread", 0,
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

    LoaderSlot &slot = this->loaders[this->tail_index % 128];
    new (&slot) Loader{&decoder, &buffer, callback, loop};
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

        Loader &entry = *reinterpret_cast<Loader *>(&self->loaders[self->head_index % 128]);
        Loader request(entry);
        entry.~Loader();
        __sync_fetch_and_add(&self->head_index, 1);

        NuSoundDecoder *decoder = request.decoder;
        NuSoundBuffer *buffer = request.buffer;
        bool loop = request.loop;

        // libTTapp.so 0x31f4d3: a closed decoder drops the request
        // (pending count--) without decoding.
        if (decoder == NULL) {
            NuSoundDecodeThread::sShutdownSemaphore.Signal();
            return;
        }

        if (decoder->closing == false) {
            // Decode owns the destination buffer lock. ThreadFunc in the target
            // dispatches directly through the decoder vtable here; it does not
            // take a second NuSoundBuffer lock around that call.
            decoder->Decode(*decoder->GetEncodedSource(), *buffer, loop);
            decoder->buffers_started++;

            NuSoundWeakPtrListNode::sPtrAccessLock.Lock();
            NuSoundBufferCallback *callback = (NuSoundBufferCallback *)request.callback.obj;
            if (callback != NULL) {
                callback->SubmitBuffer(buffer);
            }
            NuSoundWeakPtrListNode::sPtrAccessLock.Unlock();
        }

        // libTTapp.so 0x31f1c8: the request is complete; drop the pending
        // count (a locked atomic sub on device) and raise the decode-done
        // flag through the decoder's decode-sync pair (mutex +0xdc,
        // cond +0xe0, done flag +0xe4). This never touches the decoder's
        // lifetime lock mutex, which the owning voice holds.
        __sync_fetch_and_sub(&decoder->field_0xd4, 1);
        NuSoundMutexLock(&decoder->decode_mutex);
        if (decoder->decode_done == false) {
            decoder->decode_done = true;
            if (decoder->decode_broadcast) {
                NuSoundConditionBroadcast(&decoder->decode_cond);
            } else {
                NuSoundConditionSignal(&decoder->decode_cond);
            }
        }
        NuSoundMutexUnlock(&decoder->decode_mutex);
    }
}

// libTTapp.so 0x31fc90.
void NuSoundDecodeThread::Shutdown() {
    LoaderSlot &slot = this->loaders[this->tail_index % 128];
    new (&slot) Loader();
    __sync_fetch_and_add(&this->tail_index, 1);

    this->semaphore.Signal();
    NuSoundDecodeThread::sShutdownSemaphore.Wait();
}
