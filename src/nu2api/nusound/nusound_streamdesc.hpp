#pragma once

// NuSoundStreamDesc — decompiled from libTTapp.so
// (nu2api.2013/nusound/nusound_stream_desc.h). Common header description for
// a loadable / streamable sound file: format, rates, lengths and the encoded
// data position. Concrete loaders (WAV, OGG) subclass it.

#include "nu2api/nucore/common.h"

class NuSoundStreamDesc {
  public:
    enum class DataFormat {
        ZERO = 0,
        THREE = 3,
    };

  public:
    virtual ~NuSoundStreamDesc() {
    }
    virtual DataFormat GetDecodedDataFormat() const = 0;
    virtual u64 GetEncodedLengthBytes() const = 0;
    virtual u64 GetLengthSamples() const = 0;
    virtual f32 GetLengthSeconds() const = 0;
    virtual u64 GetDataOffset() const = 0;
    virtual u32 GetNumChannels() const = 0;
    virtual u32 GetSampleRate() const = 0;
    virtual u32 GetBitsPerChannel() const = 0;
    virtual u32 GetBlockSize() const = 0;
    virtual DataFormat GetEncodedDataFormat() const {
        return GetDecodedDataFormat();
    }
    virtual u64 GetDecodedLengthBytes() const {
        return GetEncodedLengthBytes();
    }
    virtual bool DecodeStreamOnOpen() const;
    virtual i32 GetLoopStart() const;
    virtual i32 GetLoopEnd() const;
    virtual u16 GetInterleaveSize() const {
        return 0;
    }
    virtual u16 GetFormatID() const {
        return 1;
    }
    virtual u16 GetExtendedDataSize() const {
        return 0;
    }
    virtual void *GetExtendedData() const {
        return NULL;
    }
};
