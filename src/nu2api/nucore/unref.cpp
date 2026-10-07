#include "decomp.h"

// RefPack decompression is a separate optimized library routine in retail,
// after the network emulation module and before the Ogg bitstream library.
i32 unref(unsigned char *source, unsigned char *destination) {
    unsigned char *output_start = destination;
    for (;;) {
        const i32 control = *source;
        if ((control & 0x80) == 0) {
            const u8 offset_byte = source[1];
            source += 2;
            const i32 literal_count = control & 3;
            for (i32 i = 0; i < literal_count; ++i)
                *destination++ = *source++;
            unsigned char *match = destination - (((control & 0x60) << 3) + offset_byte + 1);
            const i32 length = ((control & 0x1c) >> 2) + 3;
            for (i32 i = 0; i < length; ++i)
                destination[i] = match[i];
            destination += length;
        } else if ((control & 0x40) == 0) {
            const u8 first = source[1];
            const u8 second = source[2];
            source += 3;
            const i32 literal_count = first >> 6;
            for (i32 i = 0; i < literal_count; ++i)
                *destination++ = *source++;
            unsigned char *match = destination - (((first & 0x3f) << 8) + second + 1);
            const i32 length = (control & 0x3f) + 4;
            for (u32 i = 0; i < static_cast<u32>(length); ++i)
                destination[i] = match[i];
            destination += length;
        } else if ((control & 0x20) == 0) {
            const u8 first = source[1];
            const u8 second = source[2];
            const u8 third = source[3];
            source += 4;
            const i32 literal_count = control & 3;
            for (i32 i = 0; i < literal_count; ++i)
                *destination++ = *source++;
            unsigned char *match = destination - (((control & 0x10) << 12) + (first << 8) + second + 1);
            const i32 length = ((control & 0x0c) << 6) + third + 5;
            for (u32 i = 0; i < static_cast<u32>(length); ++i)
                destination[i] = match[i];
            destination += length;
        } else {
            ++source;
            i32 literal_count = ((control & 0x1f) << 2) + 4;
            if (literal_count > 0x70) {
                literal_count = control & 3;
                for (i32 i = 0; i < literal_count; ++i)
                    *destination++ = *source++;
                return destination - output_start;
            }
            for (i32 i = 0; i < literal_count; ++i)
                *destination++ = *source++;
        }
    }
}
