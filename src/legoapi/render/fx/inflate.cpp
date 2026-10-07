#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include <vorbis/codec.h>
#include <ogg/ogg.h>
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

abi_ulong GetMatchLength(unsigned char *, unsigned char *, abi_ulong);
u32 HashString(unsigned char *);

static i32 *HashTable;
static i32 *LinkArray;

void ImplodePutI(void *, u32, i32) {
    STUBBED();
}

void refpack_init() {
    constexpr usize hash_table_size = 0x40000;
    constexpr usize link_array_size = 0x10000;

    HashTable = NULL;
    if (*theMemoryManager.end_cell - *theMemoryManager.cursor_cell > hash_table_size) {
        usize allocation = ALIGN(*theMemoryManager.cursor_cell, 0x10);
        *theMemoryManager.cursor_cell = allocation + hash_table_size;
        HashTable = reinterpret_cast<i32 *>(allocation);
        memset(HashTable, 0, hash_table_size);
        theMemoryManager.allocated += hash_table_size;
        theMemoryManager.remaining -= hash_table_size;
        theMemoryManager.high_water = *theMemoryManager.cursor_cell;
    }

    LinkArray = NULL;
    if (*theMemoryManager.end_cell - *theMemoryManager.cursor_cell > link_array_size) {
        usize allocation = ALIGN(*theMemoryManager.cursor_cell, 0x10);
        *theMemoryManager.cursor_cell = allocation + link_array_size;
        LinkArray = reinterpret_cast<i32 *>(allocation);
        memset(LinkArray, 0, link_array_size);
        theMemoryManager.allocated += link_array_size;
        theMemoryManager.remaining -= link_array_size;
        theMemoryManager.high_water = *theMemoryManager.cursor_cell;
    }
}

void ImplodeFReadMem(unsigned char *, i32) {
    STUBBED();
}

void ImplodeMakeTree(i32, u16 *, unsigned char *, u16 *) {
    STUBBED();
}

void ImplodePutByteToMem(unsigned char) {
    STUBBED();
}

i32 refpack(unsigned char *source, abi_long source_size, unsigned char *destination) {
    for (u32 index = 0; index < 0x10000; ++index) {
        HashTable[index] = -1;
    }

    unsigned char *output = destination;
    if (source_size <= 0) {
        *output = 0xfc;
        return 1;
    }

    unsigned char *input_start = source;
    unsigned char *literal_start = source;
    u32 literal_count = 0;

    while (source_size > 0) {
        const u32 hash = HashString(source);
        const i32 input_index = source - input_start;
        i32 candidate_index = HashTable[hash];
        const i32 chain_limit = input_index >= 0x4000 ? input_index - 0x3fff : 0;
        if (candidate_index < chain_limit) {
            LinkArray[input_index & 0x3fff] = candidate_index;
            HashTable[hash] = input_index;
            ++source;
            --source_size;
            ++literal_count;
            continue;
        }
        u32 match_length = 2;
        u32 match_offset = 0;
        u32 command_size = 2;

        do {
            unsigned char *candidate = input_start + candidate_index;
            if (candidate[match_length] == source[match_length]) {
                const u32 length = GetMatchLength(source, candidate, MIN(source_size, 0x404));
                if (length > match_length) {
                    const u32 offset = input_index - 1 - candidate_index;
                    u32 size;
                    if (length <= 0xa && offset <= 0x3ff) {
                        size = 2;
                    } else if (length <= 0x43 && offset <= 0x3fff) {
                        size = 3;
                    } else {
                        size = 4;
                    }
                    if (length + 4 - size > match_length + 4 - command_size) {
                        match_length = length;
                        match_offset = offset;
                        command_size = size;
                        if (length > 0x403) {
                            break;
                        }
                    }
                }
            }
            candidate_index = LinkArray[candidate_index & 0x3fff];
        } while (candidate_index >= chain_limit);

        if (command_size < match_length) {
            while (literal_count > 3) {
                const u32 count = MIN(literal_count & ~3u, 0x70u);
                *output++ = (count >> 2) - 0x21;
                memmove(output, literal_start, count);
                output += count;
                literal_start += count;
                literal_count -= count;
            }

            if (command_size == 2) {
                *output++ = ((match_length - 3) << 2) + ((match_offset >> 8) << 5) + literal_count;
                *output++ = match_offset;
            } else if (command_size == 3) {
                *output++ = match_length + 0x7c;
                *output++ = (literal_count << 6) + (match_offset >> 8);
                *output++ = match_offset;
            } else {
                *output++ = 0xc0 + ((match_offset >> 16) << 4) + (((match_length - 5) >> 8) << 2) + literal_count;
                *output++ = match_offset >> 8;
                *output++ = match_offset;
                *output++ = match_length - 5;
            }

            if (literal_count != 0) {
                memmove(output, literal_start, literal_count);
                output += literal_count;
            }
            LinkArray[input_index & 0x3fff] = HashTable[hash];
            HashTable[hash] = input_index;
            source += match_length;
            source_size -= match_length;
            literal_start = source;
            literal_count = 0;
        } else {
            LinkArray[input_index & 0x3fff] = HashTable[hash];
            HashTable[hash] = input_index;
            ++source;
            --source_size;
            ++literal_count;
        }
    }

    while (literal_count > 3) {
        const u32 count = MIN(literal_count & ~3u, 0x70u);
        *output++ = (count >> 2) - 0x21;
        memmove(output, literal_start, count);
        output += count;
        literal_start += count;
        literal_count -= count;
    }
    *output++ = literal_count - 4;
    if (literal_count != 0) {
        memmove(output, literal_start, literal_count);
        output += literal_count;
    }
    return output - destination;
}
