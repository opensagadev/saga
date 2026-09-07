// NuSoundStreamingSample / NuSoundStreamer — decompiled from libTTapp.so
// (nu2api.2013/nusound/nusound_streaming_sample.cpp, nusound_streamer.cpp).

#include "nu2api/nusound/nusound_streamer.hpp"
#include "nu2api/nusound/nusound_voice.hpp"

#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nusound/nusound_loader.hpp"

#include <new>

NuList<NuSoundStreamer *> NuSoundStreamer::sStreamers;
i32 NuSoundStreamer::sThreadPriority = 2;
i32 NuSoundStreamer::sThreadStackSize = 0x8000;
NUTHREAD_CORE NuSoundStreamer::sThreadCoreId = {.value = 0};

// ---------------------------------------------------------------------------
// NuSoundStreamer
// ---------------------------------------------------------------------------

NuSoundStreamer::NuSoundStreamer() : queue1(), queue2(), semaphore(32) {
    this->running = true;
    thread =
        NuCore::m_threadManager->CreateThread(ThreadFunc, this, sThreadPriority, "NuSoundStreamThread",
                                              sThreadStackSize, sThreadCoreId.cafe_core, sThreadCoreId.xbox360_core);

    NuListNode<NuSoundStreamer *> *node = NU_ALLOC_T(NuListNode<NuSoundStreamer *>, 1, "", NUMEMORY_CATEGORY_NONE);
    if (node != NULL) {
        new (node) NuListNode<NuSoundStreamer *>(NULL, NULL, this);
    }

    sStreamers.Append(node);
}

NuSoundStreamer::~NuSoundStreamer() {
    this->running = false;

    for (NuListNodeBase *node = sStreamers.Head(); node != sStreamers.Tail();) {
        NuListNodeBase *next = node->GetNext();
        if (static_cast<NuListNode<NuSoundStreamer *> *>(node)->value == this) {
            sStreamers.Remove(node);
        }
        node = next;
    }
}

void NuSoundStreamer::RequestCue(NuSoundStreamingSample *streaming_sample, bool loop, f32 start_offset,
                                 bool weak_flag) {
    streaming_sample->AddedToThreadQueue();
    streaming_sample->streamer = this;

    QueueElement element;
    element.message = QueueElement::Message::OPEN_SAMPLE;
    element.sample = streaming_sample;
    element.loop = loop;
    element.start_offset = start_offset;
    element.buffer = NULL;
    element.weak_ptr.Set(NULL);
    element.weak_flag = weak_flag;

    this->queue1.Push(element);

    this->semaphore.Signal();
}

void NuSoundStreamer::RequestFill(NuSoundStreamingSample *sample, NuSoundBuffer *buffer, bool loop,
                                  NuSoundWeakPtr<NuSoundBufferCallback> callback) {
    QueueElement element;
    element.message = QueueElement::Message::FILL_STREAM_BUFFER;
    element.sample = sample;
    element.loop = loop;
    element.start_offset = 0.0f;
    element.buffer = buffer;
    element.weak_ptr.Set((NuSoundBufferCallback *)callback.obj);
    element.weak_flag = false;

    this->queue2.Push(element);

    this->semaphore.Signal();
}

void NuSoundStreamer::RequestClose(NuSoundStreamingSample *sample) {
    sample->AddedToThreadQueue();

    QueueElement element;
    element.message = QueueElement::Message::CLOSE_SAMPLE;
    element.sample = sample;
    element.loop = false;
    element.start_offset = 0.0f;
    element.buffer = NULL;
    element.weak_ptr.Set(NULL);
    element.weak_flag = false;

    this->queue1.Push(element);

    this->semaphore.Signal();
}

void NuSoundStreamer::RequestReCue(NuSoundStreamingSample *sample, bool loop, f32 start_offset) {
    sample->AddedToThreadQueue();

    QueueElement element;
    element.message = QueueElement::Message::RECUE_SAMPLE;
    element.sample = sample;
    element.loop = loop;
    element.start_offset = start_offset;
    element.buffer = NULL;
    element.weak_ptr.Set(NULL);
    element.weak_flag = false;

    this->queue1.Push(element);

    this->semaphore.Signal();
}

void NuSoundStreamer::ShutdownThread() {
    QueueElement element;
    element.message = QueueElement::Message::SHUTDOWN;
    element.sample = NULL;
    element.loop = false;
    element.start_offset = 0.0f;
    element.buffer = NULL;
    element.weak_ptr.Set(NULL);
    element.weak_flag = false;

    this->queue1.Push(element);

    this->semaphore.Signal();

    while (this->running) {
        NuThreadSleep(1);
    }

    NuSoundWeakPtrListNode::sPtrListLock.Lock();
    NuSoundWeakPtrListNode::sPtrListLock.Unlock();
}

void NuSoundStreamer::ShutdownAll() {
    for (NuListNodeBase *node = sStreamers.Head(); node != sStreamers.Tail(); node = node->GetNext()) {
        ((NuListNode<NuSoundStreamer *> *)node)->value->ShutdownThread();
    }
}

void NuSoundStreamer::ThreadFunc(void *self) {
    NuSoundStreamer *streamer = (NuSoundStreamer *)self;
    if (streamer->running == false) {
        return;
    }

    do {
        streamer->semaphore.Wait();

        QueueElement element = streamer->queue2.Empty() ? streamer->queue1.Pop() : streamer->queue2.Pop();

        switch (element.message) {
            case QueueElement::Message::OPEN_SAMPLE:
                element.sample->Open(element.start_offset, element.loop, element.weak_flag);
                element.sample->RemovedFromThreadQueue();
                break;

            case QueueElement::Message::FILL_STREAM_BUFFER: {
                NuSoundLoader *loader = element.sample->file_loader;
                loader->FillStreamBuffer(element.buffer, element.loop);

                NuSoundBuffer::Context &context = element.buffer->GetCurrentContext();
                if (context.size2 != 0) {
                    NuSoundWeakPtrListNode::sPtrAccessLock.Lock();
                    NuSoundBufferCallback *callback = (NuSoundBufferCallback *)element.weak_ptr.obj;
                    if (callback != NULL) {
                        callback->SubmitBuffer(element.buffer);
                    }
                    NuSoundWeakPtrListNode::sPtrAccessLock.Unlock();
                }
                break;
            }

            case QueueElement::Message::CLOSE_SAMPLE:
                element.sample->Close();
                element.sample->RemovedFromThreadQueue();
                break;

            case QueueElement::Message::RECUE_SAMPLE:
                element.sample->ReCue(element.start_offset, element.loop);
                element.sample->RemovedFromThreadQueue();
                break;

            case QueueElement::Message::SHUTDOWN:
                streamer->running = false;
                break;
        }

    } while (streamer->running != false);
}

// ---------------------------------------------------------------------------
// NuSoundStreamingSample
// ---------------------------------------------------------------------------

NuSoundStreamingSample::NuSoundStreamingSample(const char *file)
    : NuSoundSample(file, NuSoundSource::FeedType::STREAMING) {
    this->some_count = 0;
    this->field8_0x90 = 0;
    this->streamer = NULL;
    this->file_loader = NULL;
    this->field_0x88 = 0;
    this->field_0x1c = 1;
}

NuSoundStreamingSample::~NuSoundStreamingSample() {
    if (this->file_loader != NULL) {
        NuSoundSystem::ReleaseFileLoader(this->file_loader);
        this->file_loader = NULL;
    }
}

i32 NuSoundStreamingSample::Open(f32 start_offset, bool loop, bool weak_flag) {
    LoadState load_state = GetLoadState();
    if (GetResourceCount() == 0 || load_state == LoadState::STREAM_READY) {
        return 0;
    }

    NuSoundStreamDesc *desc = NULL;
    i32 error = 0;
    i32 open_result = 0;

    if (this->sound_buffer1 == NULL) {
        u32 stream_buffer_size = NuSoundSystem::GetStreamBufferSize();

        this->sound_buffer1 = NU_ALLOC_T(NuSoundBuffer, 1, "", NUMEMORY_CATEGORY_NUSOUND);
        if (this->sound_buffer1 != NULL) {
            new (this->sound_buffer1) NuSoundBuffer();
        }

        if (this->sound_buffer1->Allocate(stream_buffer_size / 2, NuSoundSystem::MemoryDiscipline::SAMPLE) != 1) {
            goto alloc_error;
        }

        this->sound_buffer2 = NU_ALLOC_T(NuSoundBuffer, 1, "", NUMEMORY_CATEGORY_NUSOUND);
        if (this->sound_buffer2 != NULL) {
            new (this->sound_buffer2) NuSoundBuffer();
        }

        if (this->sound_buffer2->Allocate(stream_buffer_size / 2, NuSoundSystem::MemoryDiscipline::SAMPLE) != 1) {
            goto alloc_error;
        }

        this->field_0x88 = 1;
    }

    this->file_loader = NuSoundSystem::CreateFileLoader(this->file_type);
    desc = this->file_loader->CreateHeader();
    if (desc == NULL) {
        NuSoundSystem::ReleaseFileLoader(this->file_loader);
        this->file_loader = NULL;
        return 3;
    }

    this->SetStreamDesc(desc);

    open_result = this->file_loader->OpenForStreaming(this->name, start_offset, desc, weak_flag);
    if (open_result != 1) {
        switch (open_result) {
            case 2:
                error = 1;
                break;
            case 3:
            case 4:
                error = 2;
                break;
            case 5:
                error = 3;
                break;
            default:
                error = 1;
                break;
        }
        goto open_error;
    }

    {
        NuSoundBuffer::Context context;
        context.flags &= ~2;
        context.flags |= 1;

        context = this->file_loader->FillStreamBuffer(this->sound_buffer1, loop);

        if (context.size2 != 0) {
            this->some_count++;
        }
        this->sound_buffer1->SetCurrentContext(context);
        u32 first_flags = context.flags;

        if (context.size2 == 0 && this->some_count == 0) {
            this->file_loader->CloseStream();
            this->sound_buffer1->SetCurrentContext(context);
            error = (first_flags & 2) != 0 ? 4 : 2;
            goto open_error;
        }

        context.flags &= ~1u;
        if ((first_flags & 2) == 0) {
            context = this->file_loader->FillStreamBuffer(this->sound_buffer2, loop);

            if (context.size2 != 0) {
                this->some_count++;
            } else if (this->some_count == 0) {
                this->file_loader->CloseStream();
                this->sound_buffer2->SetCurrentContext(context);
                error = 4;
                goto open_error;
            }
            this->sound_buffer2->SetCurrentContext(context);
        }
    }

    this->SetLoadState(LoadState::STREAM_READY);
    this->SetLastErrorState(ErrorState::NONE);
    return 0;

open_error:
    NuSoundSystem::ReleaseFileLoader(this->file_loader);
    this->file_loader = NULL;
    NuSoundSystem::FreeMemory(NuSoundSystem::MemoryDiscipline::SCRATCH, (usize)desc, 0);
    this->SetStreamDesc(NULL);
    this->SetLoadState(LoadState::NOT_LOADED);
    this->SetLastErrorState(static_cast<ErrorState>(error));
    return error;

alloc_error:
    if (this->sound_buffer1 != NULL) {
        if (this->sound_buffer1->IsAllocated()) {
            this->sound_buffer1->Free();
        }

        this->sound_buffer1->~NuSoundBuffer();

        NU_FREE(this->sound_buffer1);

        this->sound_buffer1 = NULL;
    }

    if (this->sound_buffer2 != NULL) {
        if (this->sound_buffer2->IsAllocated()) {
            this->sound_buffer2->Free();
        }

        this->sound_buffer2->~NuSoundBuffer();

        NU_FREE(this->sound_buffer2);

        this->sound_buffer2 = NULL;
    }

    return 3;
}

void NuSoundStreamingSample::Close() {
    if (GetResourceCount() == 0) {
        if (this->file_loader != NULL) {
            this->file_loader->CloseStream();
            NuSoundSystem::ReleaseFileLoader(this->file_loader);
            this->file_loader = NULL;
        }

        NuSoundStreamDesc *desc = this->GetStreamDesc();
        if (desc != NULL) {
            this->file_loader->ReleaseHeader(desc);
            this->SetStreamDesc(NULL);
        }

        this->some_count = 0;
        this->field8_0x90 = 0;
        this->SetLoadState(LoadState::NOT_LOADED);

        if (this->field_0x88 != 0) {
            if (this->sound_buffer1 != NULL) {
                this->sound_buffer1->Free();
                NuSoundBuffer *buffer = this->sound_buffer1;
                if (buffer != NULL) {
                    buffer->~NuSoundBuffer();
                    NU_FREE(buffer);
                }
            }
        }
        this->sound_buffer1 = NULL;

        if (this->field_0x88 != 0) {
            if (this->sound_buffer2 != NULL) {
                this->sound_buffer2->Free();
                NuSoundBuffer *buffer = this->sound_buffer2;
                if (buffer != NULL) {
                    buffer->~NuSoundBuffer();
                    NU_FREE(buffer);
                }
            }
        }
        this->sound_buffer2 = NULL;
    }

    this->SetLastErrorState(ErrorState::NONE);
}

i32 NuSoundStreamingSample::ReCue(f32 start_offset, bool loop) {
    this->some_count = 0;
    this->field8_0x90 = 0;

    this->file_loader->SeekTime(start_offset);

    NuSoundBuffer::Context context;
    context.flags &= ~2;
    context.flags |= 1;

    this->sound_buffer1->Lock();
    context = this->file_loader->FillStreamBuffer(this->sound_buffer1, loop);

    if (context.size2 != 0) {
        this->some_count++;
    } else if ((context.flags & 2) == 0) {
        this->file_loader->CloseStream();
        return 2;
    }
    this->sound_buffer1->SetCurrentContext(context);
    context.flags &= ~1;
    this->sound_buffer1->Unlock();

    if ((context.flags & 2) == 0) {
        this->sound_buffer2->Lock();
        context = this->file_loader->FillStreamBuffer(this->sound_buffer2, loop);

        if (context.size2 != 0) {
            this->some_count++;
        } else if ((context.flags & 2) == 0) {
            this->file_loader->CloseStream();
            return 2;
        }
        this->sound_buffer2->SetCurrentContext(context);
        context.flags &= ~1;
        this->sound_buffer2->Unlock();
    }

    return 0;
}

bool NuSoundStreamingSample::IsLocked() const {
    if (this->sound_buffer1 != NULL && this->sound_buffer1->IsLocked()) {
        return true;
    }
    if (this->sound_buffer2 != NULL && this->sound_buffer2->IsLocked()) {
        return true;
    }
    return false;
}

// libTTapp.so 0x326f10: streaming samples are open at STREAM_READY, unlike
// resident samples, whose inherited implementation tests LOADED.
bool NuSoundStreamingSample::IsStreamOpen() const {
    return GetLoadState() == LoadState::STREAM_READY;
}

void NuSoundStreamingSample::Lock() {
    this->sound_buffer1->Lock();
    this->sound_buffer2->Lock();
}

void NuSoundStreamingSample::Unlock() {
    this->sound_buffer1->Unlock();
    this->sound_buffer2->Unlock();
}

void NuSoundStreamingSample::RequestBuffer(bool loop, NuSoundWeakPtr<NuSoundBufferCallback> callback) {
    if (this->field8_0x90 < this->some_count) {
        // A buffer already holds decoded data: hand it to the voice directly.
        NuSoundBuffer *buffer = (&this->sound_buffer1)[this->field8_0x90 % 2];
        NuSoundWeakPtrListNode::sPtrAccessLock.Lock();
        if (callback.obj != NULL) {
            ((NuSoundBufferCallback *)callback.obj)->SubmitBuffer(buffer);
        }
        NuSoundWeakPtrListNode::sPtrAccessLock.Unlock();
    } else {
        // The next slot in the ring still has to be filled on the streamer
        // thread; the voice gets it once the fill completes.
        NuSoundBuffer *buffer = (&this->sound_buffer1)[this->some_count % 2];

        NuSoundWeakPtr<NuSoundBufferCallback> local;
        local.Set((NuSoundBufferCallback *)callback.obj);

        this->streamer->RequestFill(this, buffer, loop, local);
        this->some_count++;
    }

    this->field8_0x90++;
}
