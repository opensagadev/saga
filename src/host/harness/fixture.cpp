#include "host/harness/fixture.hpp"

#include "decomp.h"
#include "gameframework/export.h"
#include "gameframework/saveload.h"
#include "globals.h"

#include <cstdio>

namespace {
    static_assert(sizeof(SaveLoad) == 0x2028, "Save header layout");
    static_assert(sizeof(GAMESAVE_s) == 0x7e58, "Game save layout");

    constexpr i32 kMaximumFixtureSize = 16 * 1024 * 1024;

    bool read_fixture(const char *path, GAMESAVE_s &game) {
        FILE *file = fopen(path, "rb");
        if (file == nullptr) {
            return false;
        }

        SaveLoad header{};
        GAMESAVE_s candidate{};
        u32 checksum = 0;
        u32 slot_code = 0;
        bool valid = fseek(file, 0, SEEK_END) == 0;
        const auto length = valid ? ftell(file) : -1;
        valid = valid && length >= 0 && length <= kMaximumFixtureSize && fseek(file, 0, SEEK_SET) == 0;
        valid = valid && fread(&header, sizeof(header), 1, file) == 1;
        valid = valid && header.field0_0x0 == 0x52474d48 && header.field1_0x4 == 1 &&
                header.size == static_cast<i32>(sizeof(header)) && header.extradata_offset >= 0;
        if (valid) {
            const size_t expected =
                sizeof(header) + static_cast<size_t>(header.extradata_offset) + sizeof(candidate) + sizeof(u32) * 2;
            valid = static_cast<size_t>(length) == expected && fseek(file, header.extradata_offset, SEEK_CUR) == 0;
        }
        valid = valid && fread(&candidate, sizeof(candidate), 1, file) == 1;
        valid = valid && fread(&checksum, sizeof(checksum), 1, file) == 1;
        valid = valid && fread(&slot_code, sizeof(slot_code), 1, file) == 1;
        valid = valid && static_cast<u32>(ChecksumSaveData(&candidate, sizeof(candidate))) == checksum;
        valid = fclose(file) == 0 && valid;
        if (valid) {
            game = candidate;
        }
        return valid;
    }
} // namespace

bool host_read_game_fixture(const char *path, GAMESAVE_s &game) {
    if (read_fixture(path, game)) {
        return true;
    }
    fprintf(stderr, "fixture: invalid game save or checksum: %s\n", path);
    return false;
}
