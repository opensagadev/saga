#include "nusound_decoder_ogg.hpp"

#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nusound/nusound_buffer.hpp"
#include "nu2api/nusound/nusound_system.hpp"

#include <new>

#include <string.h>

// libTTapp.so 0x32e630: the OGG decoder extends the base decoder with the
// streaming reader subobject and a 0x2b000-byte ring buffer size.
NuSoundDecoderOGG::NuSoundDecoderOGG(char const *name, NuSoundSource *wrapped)
    : NuSoundDecoder(name, wrapped), field_0x108(0) {
    this->ring_write_pos = 0;
    this->ring_read_pos = 0;
    this->read_callbacks.SetDecoder(this);
    this->locked_buffer = NULL;
    this->ogg_loop = false;

    this->buffer_size = 0x2b000;
}

NuSoundDecoderOGG::~NuSoundDecoderOGG() {
}

// libTTapp.so 0x32e1a0.
void NuSoundDecoderOGG::Reset() {
}

// libTTapp.so 0x32e1e0.
void NuSoundDecoderOGG::SubmitBuffer(NuSoundBuffer *buffer) {
    this->encoded_buffers[this->ring_write_pos % 4] = buffer;
    __sync_fetch_and_add(&this->ring_write_pos, 1);
}

// libTTapp.so 0x32e5f0 / 0x32e620.
NuSoundDecoderOGG::OGGReadCallbacksDecoder::OGGReadCallbacksDecoder() {
    this->position = 0;
}

void NuSoundDecoderOGG::OGGReadCallbacksDecoder::SetDecoder(NuSoundDecoderOGG *decoder) {
    this->decoder = decoder;
}

// libTTapp.so 0x32e1b0 / 0x32e1c0 / 0x32e1d0.
void NuSoundDecoderOGG::OGGReadCallbacksDecoder::Seek(int origin, unsigned int offset) {
    (void)origin;
    (void)offset;
}

void NuSoundDecoderOGG::OGGReadCallbacksDecoder::Close() {
}

int NuSoundDecoderOGG::OGGReadCallbacksDecoder::GetPosition() const {
    return (int)this->position;
}

// libTTapp.so 0x32ec10: serves the vorbisfile reader with encoded bytes from
// the decoder's streaming ring, blocking on the decode thread when the ring
// runs dry and looping the stream at EOF when requested.
int NuSoundDecoderOGG::OGGReadCallbacksDecoder::Read(void *dest, unsigned int size) {
    memset(dest, 0, size);
    this->decoder->GetEncodedSource();
    if (size == 0) {
        return 0;
    }

    u8 *out = (u8 *)dest;
    u32 remaining = size;
    u32 copied = 0;

    while (remaining != 0 && this->decoder->locked_buffer != NULL) {
        NuSoundBuffer *buffer = this->decoder->locked_buffer;
        buffer->Lock();
        NuSoundBuffer::Context &context = buffer->GetCurrentContext();

        u32 available = (u32)context.read_size - this->position;
        u32 boundary = context.size3 != 0 ? (u32)context.size3 - this->position : available;
        u32 take = remaining < available ? remaining : available;
        if (take > boundary)
            take = boundary;
        memmove(out, (u8 *)buffer->GetAddress() + this->position, take);
        this->position += take;
        out += take;
        remaining -= take;
        copied += take;

        buffer->Unlock();

        if (remaining == 0 || ((context.flags & 2) != 0 && !this->decoder->ogg_loop)) {
            return (int)copied;
        }

        if (context.size3 != 0 && context.size3 < context.read_size) {
            context.size3 = 0;
            return (int)copied;
        }

        {
            // Decode() keeps the current encoded buffer locked while
            // vorbisfile reads it.  At a buffer boundary the original drops
            // that persistent lock before moving to the next ring entry.
            buffer->Unlock();

            if (this->decoder->ring_read_pos == this->decoder->ring_write_pos) {
                // A refill is still in flight.  The original re-cues the
                // current buffer from its start and restores the persistent
                // lock, returning whatever was copied this call rather than
                // reporting a false end-of-stream to vorbisfile.
                buffer->Lock();
                this->position = 0;
                return (int)copied;
            }
            NuSoundBuffer *next_buffer = this->decoder->encoded_buffers[this->decoder->ring_read_pos % 4];
            __sync_fetch_and_add(&this->decoder->ring_read_pos, 1);
            this->decoder->locked_buffer = next_buffer;

            {
                this->decoder->source->RequestBuffer(this->decoder->ogg_loop,
                                                     NuSoundWeakPtr<NuSoundBufferCallback>(this->decoder));
            }

            this->decoder->locked_buffer->Lock();
            this->position = 0;
        }
    }

    memset(out, 0, remaining);
    return (int)remaining;
}

// libTTapp.so 0x32e730: decode the next chunk of the stream into the given
// buffer. Returns the number of decoded bytes written (the buffer context's
// size2 at context offset +8). Encoded buffers arrive through the four-entry ring at +0x11c;
// the callback reader at +0x10c exposes that ring to vorbisfile.
u64 NuSoundDecoderOGG::Decode(NuSoundSource &source, NuSoundBuffer &buffer, bool loop) {
    this->ogg_loop = loop;

    if (this->locked_buffer == NULL) {
        for (i32 i = 0; i < (i32)source.GetNumInitialBuffers(); i++) {
            source.RequestBuffer(loop, NuSoundWeakPtr<NuSoundBufferCallback>(this));
        }
        NuSoundBuffer *initial_buffer = this->encoded_buffers[this->ring_read_pos % 4];
        __sync_fetch_and_add(&this->ring_read_pos, 1);
        this->locked_buffer = initial_buffer;
    }

    NuSoundBuffer::Context &context = buffer.GetCurrentContext();
    NuSoundStreamDesc *desc = source.GetStreamDesc();

    if (this->locked_buffer != NULL) {
        this->locked_buffer->Lock();
    }

    buffer.Lock();

    context.flags &= ~0x3u;
    context.size2 = 0;

    desc->GetNumChannels();
    memset(buffer.GetAddress(), 0, buffer.GetBufferSize());

    char *dest = (char *)buffer.GetAddress();
    i32 buffer_bytes = (i32)buffer.GetBufferSize();

    if (this->decoded_bytes == 0) {
        context.flags |= 1;
    }

    i32 block_size = (i32)desc->GetBlockSize();
    i32 rounded = (buffer_bytes / block_size) * block_size;

    u64 total = desc->GetDecodedLengthBytes();

    i32 chunk = rounded;
    if (loop == false) {
        i32 remaining = (i32)((u32)total - (u32)this->decoded_bytes);
        if (remaining < chunk) {
            chunk = remaining;
        }
    }

    u32 got = this->DecodeOggChunk(dest, chunk);

    this->total_decoded_bytes += got;
    u64 decoded_end = this->decoded_bytes + (i64)chunk;
    this->decoded_bytes = decoded_end;
    context.size2 += got;

    total = desc->GetDecodedLengthBytes();
    if (decoded_end == total) {
        context.flags |= 2;
        this->decoded_bytes = 0;
    }

    if (this->locked_buffer != NULL) {
        this->locked_buffer->Unlock();
    }

    buffer.Unlock();

    return context.size2;
}

// libTTapp.so 0x32e310: ov_read() the encoded stream until the destination
// holds `size` bytes (or the stream ends / loops), then apply the original's
// channel permutation passes for 3- and 6-channel material. As in the
// original, the header's vorbis handle temporarily uses the decoder callback
// object as its datasource while this chunk is decoded.
u32 NuSoundDecoderOGG::DecodeOggChunk(char *dest, unsigned int size) {
    NuSoundStreamDesc *desc = this->GetStreamDesc();

    u32 bits_per_sample = desc->GetBitsPerChannel();

    NuSoundHeaderOGG *header = (NuSoundHeaderOGG *)desc;
    OggVorbis_File *ogg = &header->ogg_file;
    void *saved_datasource = ogg->datasource;
    ogg->datasource = &this->read_callbacks;
    u32 decoded = 0;

    u32 block_size = desc->GetBlockSize();
    i32 bytes_per_sample = (i32)bits_per_sample / 8;
    int bitstream = 0;

    if (size != 0 && block_size < size) {
        char *cursor = dest;
        char *end = dest + size;

        while (cursor < end && block_size < (u32)(end - cursor) && ogg->ready_state != 0) {
            NuIOS_IsLowEndDevice();
            int ret = ov_read(ogg, cursor, (int)(end - cursor), 0, bytes_per_sample, 1, &bitstream);

            if (ret <= 0) {
                if (ret < 0) {
                    // libTTapp.so 0x32e598: negative returns (OV_HOLE and
                    // friends) just loop around; the next ov_read recovers.
                    continue;
                }
                if (this->ogg_loop) {
                    // Stream finished and the voice wants a loop: restart it.
                    NuIOS_IsLowEndDevice();
                    ov_raw_seek(ogg, 0);
                    continue;
                }
                break;
            }

            decoded += ret;
            cursor += ret;
        }
    }

    // Channel permutation passes (Vorbis orders multichannel material
    // differently from WAVE): 3-channel streams rotate the first samples of
    // each frame, 6-channel streams apply the 5.1 reordering.
    ogg->datasource = saved_datasource;
    if (desc->GetNumChannels() == 3) {
        for (u32 i = 0; i < decoded / ((i32)desc->GetBitsPerChannel() / 8); i += desc->GetNumChannels()) {
            u16 tmp = *(u16 *)&dest[2 + i * 2];
            *(u16 *)&dest[2 + i * 2] = *(u16 *)&dest[4 + i * 2];
            *(u16 *)&dest[4 + i * 2] = tmp;
        }
    } else if (desc->GetNumChannels() == 6) {
        for (u32 i = 0; i < decoded / ((i32)desc->GetBitsPerChannel() / 8); i += desc->GetNumChannels()) {
            u16 tmp = *(u16 *)&dest[2 + i * 2];
            *(u16 *)&dest[2 + i * 2] = *(u16 *)&dest[4 + i * 2];
            *(u16 *)&dest[4 + i * 2] = tmp;

            tmp = *(u16 *)&dest[6 + i * 2];
            *(u16 *)&dest[6 + i * 2] = *(u16 *)&dest[10 + i * 2];
            *(u16 *)&dest[10 + i * 2] = tmp;

            tmp = *(u16 *)&dest[8 + i * 2];
            *(u16 *)&dest[8 + i * 2] = *(u16 *)&dest[10 + i * 2];
            *(u16 *)&dest[10 + i * 2] = tmp;
        }
    }

    return decoded;
}
