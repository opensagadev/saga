#include "deflate.h"

#include <string.h>

#include "decomp.h"

DECOMP_ASSERT(sizeof(DEFHUFFMAN) == 0x7e4, "DEFHUFFMAN size");
DECOMP_ASSERT(offsetof(DEFHUFFMAN, base_code) == 0x420, "DEFHUFFMAN threshold table offset");
DECOMP_ASSERT(offsetof(DEFHUFFMAN, num_codes) == 0x464, "DEFHUFFMAN code count offset");

static inline i32 ReverseBits(i32 x) {
    x = (x & 0xaaaa) >> 1 | (x & 0x5555) << 1;
    x = (x & 0xcccc) >> 2 | (x & 0x3333) << 2;
    x = (x & 0xf0f0) >> 4 | (x & 0xf0f) << 4;
    x = (x & 0xff00) >> 8 | (x & 0xff) << 8;
    return x;
}

u32 BuildHuffmanTree(DEFHUFFMAN *tree, u8 *codeLengths, i32 symbolCount) {
    u32 nextCode[16];
    u32 lengthCount[17];
    u32 symbolTableIndex;
    i32 code;

    memset(lengthCount, 0, sizeof(lengthCount));
    memset(tree->fast_lookup, -1, sizeof(tree->fast_lookup));

    for (i32 i = 0; i < symbolCount; i++) {
        lengthCount[codeLengths[i]]++;
    }

    lengthCount[0] = 0;

    nextCode[1] = 0;

    tree->first_code[1] = 0;
    tree->num_codes[1] = 0;
    tree->base_code[1] = lengthCount[1] << 15;

    for (u32 len = 2; len < 16; len++) {
        nextCode[len] = (nextCode[len - 1] + lengthCount[len - 1]) << 1;

        tree->first_code[len] = nextCode[len];
        tree->num_codes[len] = tree->num_codes[len - 1] + lengthCount[len - 1];
        tree->base_code[len] = (nextCode[len] + lengthCount[len]) << (0x10 - len);
    }

    tree->base_code[16] = 0x10000;

    for (i32 i = 0; i < symbolCount; i++) {
        i32 value = codeLengths[i];

        if (value != 0) {
            code = nextCode[value];
            symbolTableIndex = (code - tree->first_code[value]) + tree->num_codes[value];

            tree->symbols[symbolTableIndex] = value;
            tree->symbol_index[symbolTableIndex] = i;

            if (value < 10) {
                i32 j = ReverseBits(code) >> (0x10 - value);

                do {
                    tree->fast_lookup[j] = (i16)symbolTableIndex;
                    j = j + (1 << value);
                } while (j < 512);
            }

            nextCode[value] = code + 1;
        }
    }

    return 1;
}

i32 InflateBuffer(char *buffer, i32 decodedSize, char *readBuffer, i32 readBufferSize) {
    DEFLATECONTEXT ctx;

    ctx.read_buffer = (u8 *)readBuffer;
    ctx.read_buffer_end = (u8 *)readBuffer + readBufferSize;

    if (Inflate(&ctx, buffer, decodedSize) == 0) {
        return -1;
    } else {
        return ctx.current_pos - ctx.start_pos;
    }
}

i32 Inflate(DEFLATECONTEXT *ctx, char *buffer, i32 size) {
    ctx->start_pos = buffer;
    ctx->current_pos = buffer;
    ctx->end_pos = buffer + size;
    return DecodeDeflated(ctx);
}

static u8 DefaultLengths[288] = {0};
static u8 DefaultDistances[32] = {0};

void InitHuffmanDefaults() {
    for (i32 i = 0; i < 144; i++) {
        DefaultLengths[i] = 8;
    }

    for (i32 i = 144; i < 256; i++) {
        DefaultLengths[i] = 9;
    }

    for (i32 i = 256; i < 280; i++) {
        DefaultLengths[i] = 7;
    }

    for (i32 i = 280; i < 288; i++) {
        DefaultLengths[i] = 8;
    }

    for (i32 i = 0; i < 32; i++) {
        DefaultDistances[i] = 5;
    }
}

#define DROPBITS(ctx, n)                                                                                               \
    ({                                                                                                                 \
        (ctx)->bit_buffer >>= (n);                                                                                     \
        (ctx)->num_bits_available -= (n);                                                                              \
    })

#define PEEKBITS(ctx, n)                                                                                               \
    ({                                                                                                                 \
        if ((ctx)->num_bits_available < (n)) {                                                                         \
            do {                                                                                                       \
                u32 byte = 0;                                                                                          \
                if ((ctx)->read_buffer < (ctx)->read_buffer_end) {                                                     \
                    byte = *ctx->read_buffer++;                                                                        \
                }                                                                                                      \
                                                                                                                       \
                (ctx)->bit_buffer |= byte << (ctx)->num_bits_available;                                                \
                (ctx)->num_bits_available += 8;                                                                        \
            } while ((ctx)->num_bits_available < 25);                                                                  \
        }                                                                                                              \
                                                                                                                       \
        (ctx)->bit_buffer & ((1 << (n)) - 1);                                                                          \
    })

static void FillBits(DEFLATECONTEXT *ctx) {
    u32 byte;

    do {
        if (ctx->read_buffer < ctx->read_buffer_end) {
            byte = *ctx->read_buffer++;
        } else {
            byte = 0;
        }

        ctx->bit_buffer |= byte << ctx->num_bits_available;
        ctx->num_bits_available += 8;
    } while (ctx->num_bits_available < 25);
}

#define READBITS(ctx, n)                                                                                               \
    ({                                                                                                                 \
        if ((ctx)->num_bits_available < (n)) {                                                                         \
            FillBits((ctx));                                                                                           \
        }                                                                                                              \
                                                                                                                       \
        u32 bits = (ctx)->bit_buffer & ((1 << (n)) - 1);                                                               \
        DROPBITS(ctx, n);                                                                                              \
        bits;                                                                                                          \
    })

#define READHUFFMANSYMBOL(ctx, tree)                                                                                   \
    ({                                                                                                                 \
        i32 symbol;                                                                                                    \
                                                                                                                       \
        if (ctx->num_bits_available < 0x10) {                                                                          \
            PEEKBITS(ctx, 0x10);                                                                                       \
        }                                                                                                              \
                                                                                                                       \
        u32 bits = ctx->bit_buffer;                                                                                    \
                                                                                                                       \
        u32 lookupIndex = (tree).fast_lookup[bits & 0x1ff];                                                            \
                                                                                                                       \
        if (lookupIndex != 0xffff) {                                                                                   \
            /* fast path: 9-bit lookup */                                                                              \
            i32 symbolLength = (tree).symbols[lookupIndex];                                                            \
            i32 symbolIndex = (tree).symbol_index[lookupIndex];                                                        \
                                                                                                                       \
            DROPBITS(ctx, symbolLength);                                                                               \
                                                                                                                       \
            symbol = symbolIndex;                                                                                      \
        } else {                                                                                                       \
                                                                                                                       \
            /* slow path: bit-by-bit traversal */                                                                      \
            i32 rev = ReverseBits(bits);                                                                               \
                                                                                                                       \
            i32 len = 10;                                                                                              \
            while (rev >= (tree).base_code[len]) {                                                                     \
                len++;                                                                                                 \
            }                                                                                                          \
                                                                                                                       \
            i32 index = (rev >> (16 - len)) - (tree).first_code[len] + (tree).num_codes[len];                          \
                                                                                                                       \
            DROPBITS(ctx, len);                                                                                        \
            symbol = (tree).symbol_index[index];                                                                       \
        }                                                                                                              \
        symbol;                                                                                                        \
    })

static inline i32 CtxReadHuffmanSymbol(DEFLATECONTEXT *ctx, DEFHUFFMAN *tree) {
    return READHUFFMANSYMBOL(ctx, *tree);
}

static i32 LengthBase[31] = {3,  4,  5,  6,  7,  8,  9,  10,  11,  13,  15,  17,  19,  23, 27, 31,
                             35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0,  0};
static i32 LengthExtraBits[31] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
                                  3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 0, 0};
static i32 DistanceBase[32] = {1,    2,    3,    4,    5,    7,     9,     13,    17,  25,   33,
                               49,   65,   97,   129,  193,  257,   385,   513,   769, 1025, 1537,
                               2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577, 0,   0};
static i32 DistanceExtraBits[32] = {0, 0, 0, 0, 1, 1, 2,  2,  3,  3,  4,  4,  5,  5,  6, 6,
                                    7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 0, 0};

i32 DecodeDeflatedBlock(DEFLATECONTEXT *ctx) {
    while (true) {
        i32 symbol = CtxReadHuffmanSymbol(ctx, &ctx->length_tree);

        if (symbol < 256) {
            // Literal byte
            *ctx->current_pos++ = symbol;
        } else {
            if (symbol == 256) {
                return 1;
            }
            symbol -= 257;

            i32 length = LengthBase[symbol];
            if (LengthExtraBits[symbol] != 0) {
                length += READBITS(ctx, LengthExtraBits[symbol]);
            }

            symbol = CtxReadHuffmanSymbol(ctx, &ctx->distance_tree);
            i32 distance = DistanceBase[symbol];
            if (DistanceExtraBits[symbol] != 0) {
                distance += READBITS(ctx, DistanceExtraBits[symbol]);
            }

            char *src = ctx->current_pos - distance;
            while (length--) {
                *ctx->current_pos++ = *src++;
            }
        }
    }
}

i32 DecodeUncompressedBlock(DEFLATECONTEXT *ctx) {
    i32 alignment = ctx->num_bits_available & 7;
    if (alignment != 0) {
        READBITS(ctx, alignment);
    }

    u8 bytes[sizeof(ctx->bit_buffer)];
    i32 count = 0;
    i32 available_bits = ctx->num_bits_available;
    u32 buffered_bits = ctx->bit_buffer;
    while (available_bits > 0) {
        bytes[count++] = buffered_bits;
        buffered_bits >>= 8;
        available_bits -= 8;
    }
    ctx->bit_buffer = buffered_bits;
    ctx->num_bits_available = available_bits;
    while (count < 2) {
        bytes[count++] = ctx->read_buffer < ctx->read_buffer_end ? *ctx->read_buffer++ : 0;
    }

    i32 length = bytes[0] + (bytes[1] << 8);
    if (count != 2) {
        memcpy(ctx->current_pos, bytes + 2, count - 2);
        ctx->current_pos += count - 2;
        length -= count - 2;
    }
    if (ctx->read_buffer + length > ctx->read_buffer_end) {
        return 0;
    }

    memmove(ctx->current_pos, ctx->read_buffer, length);
    ctx->read_buffer += length;
    ctx->current_pos += length;
    return 1;
}

enum {
    BLOCK_DYNAMIC_HUFFMAN = 0,
    BLOCK_FIXED_HUFFMAN = 1,
    BLOCK_UNCOMPRESSED = 2,
};

i32 DecodeDeflated(DEFLATECONTEXT *ctx) {
    u32 is_final_block;
    i32 type;

    ctx->num_bits_available = 0;
    ctx->bit_buffer = 0;

    do {
        is_final_block = READBITS(ctx, 1);
        type = READBITS(ctx, 2);

        switch (type) {
            default:
                if (!DecompressHuffmanTrees(ctx)) {
                    return 0;
                }
                break;
            case BLOCK_FIXED_HUFFMAN:
                if (DefaultDistances[0x1f] == '\0') {
                    InitHuffmanDefaults();
                }

                if (!BuildHuffmanTree(&ctx->length_tree, DefaultLengths, 0x120)) {
                    return 0;
                }

                if (!BuildHuffmanTree(&ctx->distance_tree, DefaultDistances, 0x20)) {
                    return 0;
                }
                break;
            case BLOCK_UNCOMPRESSED:
                if (!DecodeUncompressedBlock(ctx)) {
                    return 0;
                }
                continue;
        }

        if (!DecodeDeflatedBlock(ctx)) {
            return 0;
        }
    } while (!is_final_block);

    return 1;
}

static u8 LengthDeZigZag[19] = {0x10, 0x11, 0x12, 0x00, 0x08, 0x07, 0x09, 0x06, 0x0a, 0x05,
                                0x0b, 0x04, 0x0c, 0x03, 0x0d, 0x02, 0x0e, 0x01, 0x0f};

i32 DecompressHuffmanTrees(DEFLATECONTEXT *ctx) {
    i32 hlit = READBITS(ctx, 5) + 257;
    i32 hdist = READBITS(ctx, 5) + 1;
    i32 hclen = READBITS(ctx, 4) + 4;

    u8 codeLengths[19];
    memset(codeLengths, 0, sizeof(codeLengths));

    for (i32 i = 0; i < hclen; i++) {
        i32 length = READBITS(ctx, 3);
        codeLengths[LengthDeZigZag[i]] = length;
    }

    if (!BuildHuffmanTree(&ctx->temp_code_length, codeLengths, 19)) {
        LOG_WARN("failed to build code length huffman tree");
        return false;
    }

    // The five-bit counts can describe 288 literal/length and 32 distance codes,
    // including reserved literal counts; a final repeat can extend another 137 entries.
    u8 allCodeLengths[288 + 32 + 137];

    i32 i = 0;
    do {
        i32 symbol = READHUFFMANSYMBOL(ctx, ctx->temp_code_length);

        if (symbol < 16) {
            allCodeLengths[i++] = symbol;
        } else if (symbol == 16) {
            symbol = READBITS(ctx, 2) + 3;
            memset(allCodeLengths + i, allCodeLengths[i - 1], symbol);
            i += symbol;
        } else if (symbol == 17) {
            symbol = READBITS(ctx, 3) + 3;
            memset(allCodeLengths + i, 0, symbol);
            i += symbol;
        } else {
            symbol = READBITS(ctx, 7) + 11;
            memset(allCodeLengths + i, 0, symbol);
            i += symbol;
        }
    } while (i < hlit + hdist);

    if (!BuildHuffmanTree(&ctx->length_tree, allCodeLengths, hlit)) {
        LOG_WARN("failed to build length huffman tree");
        return 0;
    }

    if (!BuildHuffmanTree(&ctx->distance_tree, allCodeLengths + hlit, hdist)) {
        LOG_WARN("failed to build distance huffman tree");
        return 0;
    }

    return true;
}
