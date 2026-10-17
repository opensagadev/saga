#ifndef GAMELIB_UTIL_NETINPUTSTREAM_H
#define GAMELIB_UTIL_NETINPUTSTREAM_H
#pragma once

#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/gameapi_edtools_types.h"

struct NetInputStream : EdInputStream {
    NetMessage *message;

    NetInputStream() : message(NULL) {
        mode = 1;
    }

    virtual ~NetInputStream() {}

    i32 Eat(i32 size, i32 count) override {
        i32 bytes = size * count;
        if (message->data != NULL) {
            message->read_offset += bytes;
        }
        return bytes;
    }

    i32 SerialiseBuffer(void *buffer, i32 size, i32 count) override {
        i32 bytes = size * count;
        NetMessage &input = *message;
        if (input.data != NULL) {
            memmove(buffer, input.data->bytes + input.read_offset, bytes);
            if (input.swap_endianness && size > 1) {
                u8 *next = static_cast<u8 *>(buffer);
                for (i32 i = 0; i < count; ++i) {
                    switch (size) {
                        case 2:
                            EdFileSwapEndianess16(next);
                            next += 2;
                            break;
                        case 4:
                            EdFileSwapEndianess32(next);
                            next += 4;
                            break;
                    }
                }
            }
            input.read_offset += bytes;
        }
        return bytes;
    }

    char const *BeginBlock(char const *name) override {
        return name;
    }

    void EndBlock() override {}
};

DECOMP_ASSERT(sizeof(NetInputStream) == 0x24, "NetInputStream ABI");
DECOMP_ASSERT(offsetof(NetInputStream, message) == 0x20, "NetInputStream message offset");

#endif // GAMELIB_UTIL_NETINPUTSTREAM_H
