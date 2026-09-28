#include "gamelib/crc/crc.h"

static i32 g_crc_initialised = 0;
i32 *g_crc_table = NULL;

#define CRC32_POLY 0x04C11DB7

static inline char CRC_ToUpper(char c) {
    if (static_cast<u8>(c - 'a') <= static_cast<u8>('z' - 'a')) {
        return c - ('a' - 'A');
    }
    return c;
}

void CRC_Init(VARIPTR *buffer_start) {
    if (g_crc_initialised) {
        return;
    }

    g_crc_table = reinterpret_cast<i32 *>(ALIGN(buffer_start->addr, alignof(i32)));
    buffer_start->addr = reinterpret_cast<usize>(g_crc_table + 0x100);

    for (u32 i = 0; i < 0x100; i++) {
        u32 crc = (i & 0x80U) != 0 ? (i << 25) ^ CRC32_POLY : i << 25;

        if ((crc & 0x80000000U) != 0) {
            crc = (crc << 1) ^ CRC32_POLY;
        } else {
            crc <<= 1;
        }
        if ((crc & 0x80000000U) != 0) {
            crc = (crc << 1) ^ CRC32_POLY;
        } else {
            crc <<= 1;
        }
        if ((crc & 0x80000000U) != 0) {
            crc = (crc << 1) ^ CRC32_POLY;
        } else {
            crc <<= 1;
        }
        if ((crc & 0x80000000U) != 0) {
            crc = (crc << 1) ^ CRC32_POLY;
        } else {
            crc <<= 1;
        }
        if ((crc & 0x80000000U) != 0) {
            crc = (crc << 1) ^ CRC32_POLY;
        } else {
            crc <<= 1;
        }
        if ((crc & 0x80000000U) != 0) {
            crc = (crc << 1) ^ CRC32_POLY;
        } else {
            crc <<= 1;
        }
        if ((crc & 0x80000000U) != 0) {
            crc = (crc << 1) ^ CRC32_POLY;
        } else {
            crc <<= 1;
        }
        g_crc_table[i] = crc;
    }

    g_crc_initialised = 1;
}

u32 CRC_Process(const void *data, u32 size) {
    if (size == 0) {
        return 0;
    }

    const u8 *cursor = static_cast<const u8 *>(data);
    const u8 *end = cursor + size;
    u32 crc = 0;
    do {
        const u32 table_index = *cursor++ ^ (crc >> 24);
        crc = (crc << 8) ^ g_crc_table[table_index];
    } while (cursor != end);
    return crc;
}

u32 CRC_ProcessString(const char *str) {
    const char *cursor = str;
    char c = *cursor;
    if (__builtin_expect(c == '\0', 0))
        return 0;
    ++cursor;
    const i32 *table = g_crc_table;
    u32 crc = 0;
    do {
        const u32 table_index = static_cast<u32>(static_cast<i32>(c)) ^ (crc >> 24);
        ++cursor;
        c = cursor[-1];
        crc = (crc << 8) ^ table[table_index];
    } while (c != '\0');
    return crc;
}

u32 CRC_ProcessStringN(const char *str, u32 size) {
    char c = *str;
    if (__builtin_expect(c == '\0', 0) || size == 0)
        return 0;
    const char *cursor = str;
    const char *end = cursor + size;
    const i32 *table = g_crc_table;
    u32 crc = 0;
    do {
        const u32 table_index = static_cast<u32>(static_cast<i32>(c)) ^ (crc >> 24);
        crc = (crc << 8) ^ table[table_index];
        c = cursor[1];
        if (c == '\0')
            break;
        ++cursor;
    } while (cursor != end);
    return crc;
}

u32 CRC_ProcessStringIgnoreCase(const char *str) {
    const char *cursor = str;
    char c = *cursor;
    if (__builtin_expect(c == '\0', 0))
        return 0;
    ++cursor;
    const i32 *table = g_crc_table;
    u32 crc = 0;
    do {
        const u32 table_index = static_cast<u32>(static_cast<i32>(CRC_ToUpper(c))) ^ (crc >> 24);
        ++cursor;
        c = cursor[-1];
        crc = (crc << 8) ^ table[table_index];
    } while (c != '\0');
    return crc;
}

u32 CRC_ProcessStringNIgnoreCase(const char *str, u32 size) {
    volatile u32 crc = 0;
    for (u32 i = 0; str[i] != '\0' && i < size; ++i) {
        const u32 table_index = static_cast<u32>(static_cast<i32>(CRC_ToUpper(str[i]))) ^ (crc >> 24);
        crc = (crc << 8) ^ g_crc_table[table_index];
    }
    return crc;
}
