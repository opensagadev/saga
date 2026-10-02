#include "gamelib/util/CRC16.h"

CRC16 CRC16::instance;
u32 CRC16::crcTable[256];

CRC16::CRC16() {
    for (u32 i = 0; i < 256; ++i) {
        u32 value = 0;
        u32 input = i << 8;
        for (i32 bit = 0; bit < 8; ++bit) {
            const bool carry = ((value ^ input) & 0x8000) != 0;
            value = carry ? ((value << 1) ^ 0x1021) & 0xffff : (value << 1) & 0xffff;
            input <<= 1;
        }
        crcTable[i] = value;
    }
}

u32 CRC16::hash(unsigned char const *data, i32 length) {
    u32 result = 0xffffffffu;
    if (length > 0) {
        unsigned char const *end = data + length;
        result = 0xffff;
        do {
            result = ((result << 8) ^ crcTable[((result >> 8) ^ *data) & 0xff]) & 0xffff;
            ++data;
        } while (data != end);
    }
    return result;
}

u32 CRC16::hashInverse(unsigned char const *data, i32 length) {
    u32 result = 0xffffffffu;
    unsigned char const *cursor = data + length;
    if (length > 0) {
        result = 0xffff;
        do {
            --cursor;
            result = ((result << 8) ^ crcTable[((result >> 8) ^ *cursor) & 0xff]) & 0xffff;
        } while (cursor != data);
    }
    return result;
}
