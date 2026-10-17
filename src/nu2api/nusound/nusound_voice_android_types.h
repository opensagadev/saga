#pragma once

// Types shared by nu2api/nusound/nusound_voice_android.cpp and nu2api/nusound/nusound_android.cpp.

namespace {
    struct OpenSLBufferQueueLocator {
        u32 locator_type;
        u32 num_buffers;
    };
    struct OpenSLPcmFormat {
        u32 format_type;
        u32 channels;
        u32 sample_rate;
        u32 bits;
        u32 container_bits;
        u32 speaker_mask;
        u32 endianness;
    };
    struct OpenSLDataEndpoint {
        void *locator;
        void *format;
    };
    struct OpenSLOutputMixLocator {
        u32 locator_type;
        void *output_mix;
    };
}
