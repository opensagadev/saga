#include "nusound_decoder_ogg.hpp"

#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nusound/nusound_buffer.hpp"
#include "nu2api/nusound/nusound_system.hpp"

#include <new>

#include <string.h>

// libTTapp.so 0x32e630: the OGG decoder extends the base decoder with the
// streaming reader subobject and a 0x2b000-byte ring buffer size.
NuSoundDecoderOGG::NuSoundDecoderOGG(char const *name, NuSoundSource *wrapped) : NuSoundDecoder(name, wrapped) {
    this->field_0x108 = 0;

    new (&this->read_callbacks) OGGReadCallbacksDecoder();

    this->ring_read_pos = 0;
    this->ring_write_pos = 0;
    for (u32 i = 0; i < 4; i++) {
        this->encoded_buffers[i] = NULL;
    }
    this->locked_buffer = NULL;
    this->ogg_loop = false;

    this->read_callbacks.SetDecoder(this);

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
// The decoder datasource never seeks.  The original callback returns the
// requested offset unchanged; vorbisfile ignores it on this streaming path.
i32 NuSoundDecoderOGG::OGGReadCallbacksDecoder::Seek(i32 offset, u32 origin) {
    (void)origin;
    return offset;
}

void NuSoundDecoderOGG::OGGReadCallbacksDecoder::Close() {
}

int NuSoundDecoderOGG::OGGReadCallbacksDecoder::GetPosition() const {
    return 0;
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

    u32 copied = 0;
    while (size != 0) {
        if (this->decoder->locked_buffer == NULL) {
            memset(dest, 0, size);
            return (int)size;
        }

        this->decoder->locked_buffer->Lock();
        NuSoundBuffer::Context &context = this->decoder->locked_buffer->GetCurrentContext();

        u32 read_available = (u32)context.read_size - this->position;
        u32 boundary_available = context.size3 != 0 ? (u32)context.size3 - this->position : read_available;
        u32 take = size < read_available ? size : read_available;
        if (boundary_available < take) {
            take = boundary_available;
        }

        memmove(dest, (u8 *)this->decoder->locked_buffer->GetAddress() + this->position, take);
        this->position += take;
        size -= take;
        copied += take;
        dest = (u8 *)dest + take;

        this->decoder->locked_buffer->Unlock();
        if (size == 0 || ((context.flags & 2) != 0 && !this->decoder->ogg_loop)) {
            return (int)copied;
        }

        if (context.size3 != 0 && context.size3 < context.read_size) {
            context.size3 = 0;
            break;
        }

        // Decode holds a persistent lock while vorbisfile consumes the
        // encoded buffer. The original drops that second lock here.
        this->decoder->locked_buffer->Unlock();

        if (this->decoder->ring_read_pos == this->decoder->ring_write_pos) {
            this->decoder->locked_buffer->Lock();
            this->position = 0;
            return (int)copied;
        }

        this->decoder->locked_buffer = this->decoder->encoded_buffers[this->decoder->ring_read_pos % 4];
        __sync_fetch_and_add(&this->decoder->ring_read_pos, 1);

        NuSoundWeakPtr<NuSoundBufferCallback> callback;
        callback.Set(this->decoder);
        this->decoder->source->RequestBuffer(false, callback);

        this->decoder->locked_buffer->Lock();
        this->position = 0;
    }
    return (int)copied;
}

// libTTapp.so 0x32e730: decode the next chunk of the stream into the given
// buffer. Returns the number of decoded bytes written (the buffer context's
// read_size). Encoded buffers arrive through the four-entry ring at +0x11c;
// the callback reader at +0x10c exposes that ring to vorbisfile.
u64 NuSoundDecoderOGG::Decode(NuSoundSource &source, NuSoundBuffer &buffer, bool loop) {
    this->ogg_loop = loop;

    if (this->locked_buffer == NULL) {
        for (i32 i = 0; i < source.GetNumInitialBuffers(); i++) {
            NuSoundWeakPtr<NuSoundBufferCallback> callback;
            callback.Set(this);
            source.RequestBuffer(loop, callback);
        }
        this->locked_buffer = this->encoded_buffers[this->ring_read_pos % 4];
        __sync_fetch_and_add(&this->ring_read_pos, 1);
    }

    NuSoundStreamDesc *desc = source.GetStreamDesc();
    NuSoundBuffer::Context &context = buffer.GetCurrentContext();

    if (this->locked_buffer != NULL) {
        this->locked_buffer->Lock();
    }

    buffer.Lock();

    context.flags &= ~0x3u;
    context.size2 = 0;

    desc->GetNumChannels();
    memset(buffer.GetAddress(), 0, buffer.GetBufferSize());

    char *dest = (char *)buffer.GetAddress();
    u32 buffer_bytes = (u32)buffer.GetBufferSize();

    if (this->decoded_bytes == 0) {
        context.flags |= 1;
    }

    i32 block_size = desc->GetBlockSize();
    i32 rounded = (i32)buffer_bytes / block_size * block_size;

    u64 total = desc->GetDecodedLengthBytes();

    i32 chunk = rounded;
    if (loop == false) {
        i32 remaining = (i32)total - (i32)this->decoded_bytes;
        if (remaining < chunk) {
            chunk = remaining;
        }
    }

    u32 got = this->DecodeOggChunk(dest, (u32)chunk);

    this->total_decoded_bytes += got;
    this->decoded_bytes += chunk;
    context.size2 += got;

    total = desc->GetDecodedLengthBytes();
    if (this->decoded_bytes == total) {
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
    i32 bits_per_sample = (i32)desc->GetBitsPerChannel();
    NuSoundHeaderOGG *header = (NuSoundHeaderOGG *)desc;
    OggVorbis_File *ogg = &header->ogg_file;
    void *saved_datasource = ogg->datasource;
    ogg->datasource = &this->read_callbacks;

    u32 decoded = 0;
    u32 block_size = desc->GetBlockSize();

    if (size != 0 && block_size < size && ogg->ready_state != 0) {
        i32 rounded_bits = bits_per_sample + 7;
        if (bits_per_sample >= 0) {
            rounded_bits = bits_per_sample;
        }
        i32 bytes_per_sample = rounded_bits >> 3;
        char *cursor = dest;

        do {
            int bitstream = 0;
            NuIOS_IsLowEndDevice();
            int ret = ov_read(ogg, cursor, (int)(size - decoded), 0, bytes_per_sample, 1, &bitstream);

            if (ret < 1) {
                if (ret == 0) {
                    if (!this->ogg_loop) {
                        break;
                    }
                    NuIOS_IsLowEndDevice();
                    ov_raw_seek(ogg, 0);
                }
            } else {
                decoded += ret;
                cursor += ret;
            }

            if (size <= decoded || size - decoded <= block_size || ogg->ready_state == 0) {
                break;
            }
        } while (true);
    }

    ogg->datasource = saved_datasource;

    // Channel permutation passes (Vorbis orders multichannel material
    // differently from WAVE). The original advances one interleaved frame at
    // a time, i.e. by the channel count in 16-bit samples.
    if (desc->GetNumChannels() == 3) {
        u32 i = 0;
        while (true) {
            i32 channel_bits = (i32)desc->GetBitsPerChannel();
            i32 rounded_bits = channel_bits + 7;
            if (channel_bits >= 0) {
                rounded_bits = channel_bits;
            }
            if (decoded / (u32)(rounded_bits >> 3) <= i) {
                break;
            }
            u16 tmp = *(u16 *)&dest[2 + i * 2];
            *(u16 *)&dest[2 + i * 2] = *(u16 *)&dest[4 + i * 2];
            *(u16 *)&dest[4 + i * 2] = tmp;
            i += desc->GetNumChannels();
        }
        return decoded;
    }

    if (desc->GetNumChannels() == 6) {
        u32 i = 0;
        while (true) {
            i32 channel_bits = (i32)desc->GetBitsPerChannel();
            i32 rounded_bits = channel_bits + 7;
            if (channel_bits >= 0) {
                rounded_bits = channel_bits;
            }
            if (decoded / (u32)(rounded_bits >> 3) <= i) {
                break;
            }
            u16 tmp = *(u16 *)&dest[2 + i * 2];
            *(u16 *)&dest[2 + i * 2] = *(u16 *)&dest[4 + i * 2];
            *(u16 *)&dest[4 + i * 2] = tmp;

            tmp = *(u16 *)&dest[6 + i * 2];
            *(u16 *)&dest[6 + i * 2] = *(u16 *)&dest[10 + i * 2];
            *(u16 *)&dest[10 + i * 2] = tmp;

            tmp = *(u16 *)&dest[8 + i * 2];
            *(u16 *)&dest[8 + i * 2] = *(u16 *)&dest[10 + i * 2];
            *(u16 *)&dest[10 + i * 2] = tmp;
            i += desc->GetNumChannels();
        }
        return decoded;
    }

    return decoded;
}
