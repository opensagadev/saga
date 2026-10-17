#pragma once

#include "decomp.h"

// Types shared by nu2api/nusound/nusound3_include.cpp and legoapi/audio/audio.cpp.

// Stereo stream slot (0x10 bytes in the original):
//   +0x0 stream            the streaming sample playing in this slot
//   +0x4 pending_start     PlayStereoV sets it to 1, and NuSound3Update
//                          clears it once the voice is created
//   +0x8 volume            the PS2 volume (0..16383) as raw bits
//   +0xc field_0xc         the voice/loader loop flag (PlayStereoV key 0xb)
//   +0xd field_0xd         started flag: set once the voice began playing
struct NuSoundStream {
    NuSoundStreamingSample *stream;
    u8 pending_start;
    u8 padding_0x5[3];
    i32 ps2volume;
    u8 field_0xc;
    u8 field_0xd;
};
// The single stereo-stream voice shared by all stream slots. This is a class
// member in the original ABI, even though the slot bookkeeping lives here.
class NuSound3Stream {
  public:
    static NuSoundWeakPtr<NuSoundVoice> mVoice;
};
// 0x44-byte voice wrapper the NuSound3 API keeps per playing source. The
// elists link these while they are pending playback / active / pending
// destruction.
struct NuSound3Voice {
    NuSound3Voice *intrusive_prev;
    NuSound3Voice *intrusive_next;
    NuSoundWeakPtr<NuSoundVoice> weak_ptr;
    NuSoundSource *source;
    f32 pitch;     // +0x1c
    i32 volume;    // +0x20 raw volume bits
    f32 falloff_a; // +0x24
    f32 falloff_b; // +0x28
    bool loop;
    nuvec_s position;
    nuvec_s *source_position;
    i32 pause_counter; // frames in the update sweep
};
// NuSound3Voice is itself the intrusive list node in the original. NuEList
// owns two embedded sentinels, pointers to those sentinels, and a count; it
// does not allocate a second wrapper node around each voice.
template <> class NuEList<NuSound3Voice, DefaultElist> {
  private:
    struct Links {
        NuSound3Voice *prev;
        NuSound3Voice *next;

        Links() : prev(NULL), next(NULL) {
        }
    };

    Links begin_sentinel;
    Links end_sentinel;

  public:
    NuSound3Voice *begin;
    NuSound3Voice *end;
    i32 length;

    NuEList();

    ~NuEList() {
        while (length != 0) {
            NuSound3Voice *voice = begin->intrusive_next;
            voice->intrusive_prev->intrusive_next = voice->intrusive_next;
            voice->intrusive_next->intrusive_prev = voice->intrusive_prev;
            length--;
            voice->intrusive_prev = NULL;
            voice->intrusive_next = NULL;
            delete voice;
        }
    }

    NuSound3Voice *Front() const {
        return begin->intrusive_next;
    }

    NuSound3Voice *End() const {
        return end;
    }
};

DECOMP_ASSERT(sizeof(NuSound3Stream::mVoice) == 0x10, "NuSound3Stream voice pointer size");
DECOMP_ASSERT(sizeof(NuSound3Voice) == 0x44, "NuSound3Voice size");
DECOMP_ASSERT(sizeof(NuSoundStream) == 0x10, "NuSoundStream size");
