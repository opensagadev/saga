#include "decomp.h"

// RefPack decompression is a separate optimized library routine in retail,
// after the network emulation module and before the Ogg bitstream library.
i32 unref(unsigned char *source, unsigned char *destination) {
    unsigned char *output_start = destination;
    unsigned char *match;
    i32 first;
    unsigned char second;
    unsigned char third;
    unsigned char fourth;
    u32 run;

    for (;;) {
        first = *source++;
        if ((first & 0x80) == 0) {
            second = *source++;
            run = first & 3;
            while (run--)
                *destination++ = *source++;
            match = destination - 1 - (((first & 0x60) << 3) + second);
            run = ((first & 0x1c) >> 2) + 3 - 1;
            do {
                *destination++ = *match++;
            } while (run--);
        } else if ((first & 0x40) == 0) {
            second = *source++;
            third = *source++;
            run = second >> 6;
            while (run--)
                *destination++ = *source++;
            match = destination - 1 - (((second & 0x3f) << 8) + third);
            run = (first & 0x3f) + 4 - 1;
            do {
                *destination++ = *match++;
            } while (run--);
        } else if ((first & 0x20) == 0) {
            second = *source++;
            third = *source++;
            fourth = *source++;
            run = first & 3;
            while (run--)
                *destination++ = *source++;
            match = destination - 1 - (((first & 0x10) << 12) + (second << 8) + third);
            run = ((first & 0x0c) << 6) + fourth + 5 - 1;
            do {
                *destination++ = *match++;
            } while (run--);
        } else {
            run = ((first & 0x1f) << 2) + 4;
            if (run <= 0x70) {
                while (run--)
                    *destination++ = *source++;
            } else {
                run = first & 3;
                while (run--)
                    *destination++ = *source++;
                return destination - output_start;
            }
        }
    }
}
