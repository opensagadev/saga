#include "gamelib/util/CRC16.h"

CRC16 CRC16::instance;
u32 CRC16::crcTable[256];

CRC16::CRC16() {
    for (u32 i = 0; i < 256; ++i) {
        u32 value = 0;
        u32 input = i << 8;
        for (i32 bit = 0; bit < 8; ++bit) {
            if (((value ^ input) & 0x8000) != 0) {
                value = (value << 1) ^ 0x1021;
            } else {
                value <<= 1;
            }
            input <<= 1;
            value &= 0xffff;
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
            u32 input = *data++;
            result = ((result << 8) ^ crcTable[((result >> 8) ^ input) & 0xff]) & 0xffff;
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
