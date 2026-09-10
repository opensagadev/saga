#pragma once

#include "nu2api/nucore/fixed_width.h"
#include "nu2api/nusound/nusound_loader.hpp"

#include <vorbis/vorbisfile.h>

class NuSoundHeaderOGG : public NuSoundStreamDesc {
  public:
    u16 format_id;
    u16 num_channels;
    u32 sample_rate;
    u32 samples_per_second;
    u16 block_size;
    u16 bits_per_channel;
    u16 extended_data_size;
    u8 extended_data[0x16];
    OggVorbis_File ogg_file;
    u64 encoded_length_bytes;
    u64 decoded_length_bytes;
    u64 length_samples;
    f32 length_seconds;

    NuSoundHeaderOGG() = default;

    DataFormat GetDecodedDataFormat() const;
    u64 GetEncodedLengthBytes() const;
    u64 GetLengthSamples() const;
    f32 GetLengthSeconds() const;
    u64 GetDataOffset() const;
    u32 GetNumChannels() const;
    u32 GetSampleRate() const;
    u32 GetBitsPerChannel() const;
    u32 GetBlockSize() const;
    DataFormat GetEncodedDataFormat() const;
    u64 GetDecodedLengthBytes() const;
    u16 GetInterleaveSize() const;
    u16 GetFormatID() const;
    u16 GetExtendedDataSize() const;
    void *GetExtendedData() const;
};

class NuSoundLoaderOGG : public NuSoundLoader {
  public:
    class OGGFileCallbacks {
      private:
        NUFILE file;

      public:
        ~OGGFileCallbacks() {
        }

        virtual void SetFile(NUFILE file);
        virtual i32 Read(void *dest, u32 size);
        virtual i32 Seek(i32 offset, u32 origin);
        virtual void Close();
        virtual i32 GetPosition() const;
        virtual NUFILE GetFile() const;
    };

    NuSoundLoaderOGG();
    ~NuSoundLoaderOGG();

    NuSoundStreamDesc::DataFormat GetDecodedDataFormat();
    NuSoundStreamDesc *CreateHeader();

    bool SeekPCMSample(u64 index);
    bool SeekTime(f64 seconds);
    virtual i32 ReadHeader(NuSoundStreamDesc *header);
    i32 OpenFileForStreaming(const char *path, bool flag);
    bool SeekRawData(u64 position);
    void Close();

    static int OggCallbackClose(void *callbacks);
    static int OggCallbackSeek(void *callbacks, i64 offset, i32 origin); // NOLINT(google-runtime-int)
    static long OggCallbackTell(void *callbacks);                        // NOLINT(google-runtime-int)

  private:
    OggVorbis_File field_0x1c;
    OGGFileCallbacks file_callbacks;
    void *buffer;

    void FreeMemoryBuffer();

    static usize OggCallbackRead(void *dest, usize count, usize size, void *callbacks);
};
