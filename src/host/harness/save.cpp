#include "host/harness/save.hpp"
#include "host/harness/save_names.hpp"

#include "globals.h"
#include "gameframework/export.h"
#include "gameframework/saveload.h"

#include <cerrno>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#endif

extern void NewGame();
extern u16 MakeSaveHash();
extern char *slotname(i32 index);

namespace {
    static_assert(sizeof(SaveLoad) == 0x2028, "Save header layout");
    static_assert(sizeof(GAMESAVE_s) == 0x7e58, "Game save layout");
    static_assert(sizeof(SUPEROPTIONS_s) == 0x18, "Options save layout");

    enum class Kind { unsigned_integer, signed_integer, floating, bytes, bitmask };
    struct Property {
        std::string name;
        size_t offset;
        size_t size;
        Kind kind;
    };
    struct Save {
        std::vector<u8> bytes;
        size_t payload = sizeof(SaveLoad);
        size_t payload_size = sizeof(GAMESAVE_s);
    };

    struct EnumValue {
        std::string name;
        u32 value;
    };
    struct EnumValues {
        std::vector<EnumValue> values;
        bool flags = false;
    };

    std::vector<std::string> bit_names(const Property &p) {
        std::vector<std::string> names(p.size * 8);
        for (size_t bit = 0; bit < names.size(); ++bit)
            names[bit] = "BIT_" + std::to_string(bit);
        if (p.name == "shop_character_purchased_bits") {
            for (const auto &entry : kShopCharacterBits)
                if (entry.bit < names.size())
                    names[entry.bit] = entry.name;
        } else if (p.name == "extra_unlocked_bits" || p.name == "extra_purchased_bits") {
            for (size_t bit = 0; bit < names.size() && bit < SAVE_EXTRA_COUNT; ++bit)
                names[bit] = Cheat[bit].name;
        } else if (p.name == "hint_completion_bits") {
            const size_t bank_bits = names.size() / 2;
            for (size_t bank = 0; bank < 2; ++bank)
                for (const auto &entry : kTutorialHintBits)
                    if (entry.bit < bank_bits)
                        names[bank * bank_bits + entry.bit] =
                            std::string(bank == 0 ? "console." : "touch.") + entry.name;
        } else if (p.name == "shop_gold_brick_purchased_bits") {
            for (size_t bit = 0; bit < 14 && bit < names.size(); ++bit)
                names[bit] = "GOLD_BRICK_" + std::to_string(bit);
        }
        return names;
    }

    std::string mask_hex(const u8 *bytes, size_t size) {
        static const char hex[] = "0123456789abcdef";
        std::string result = "0x";
        for (size_t i = size; i; --i) {
            result += hex[bytes[i - 1] >> 4];
            result += hex[bytes[i - 1] & 15];
        }
        return result;
    }

    // Full-width decimal/hex parsing, independent of the host integer width.
    bool mask_number(const std::string &text, std::vector<u8> &bytes) {
        const bool hex = text.compare(0, 2, "0x") == 0;
        const size_t start = hex ? 2 : 0;
        if (start == text.size())
            return false;
        std::fill(bytes.begin(), bytes.end(), 0);
        for (size_t i = start; i < text.size(); ++i) {
            const char c = text[i];
            unsigned digit = c >= '0' && c <= '9'   ? c - '0'
                             : c >= 'a' && c <= 'f' ? c - 'a' + 10
                             : c >= 'A' && c <= 'F' ? c - 'A' + 10
                                                    : 255;
            const unsigned radix = hex ? 16 : 10;
            if (digit >= radix)
                return false;
            unsigned carry = digit;
            for (u8 &byte : bytes) {
                carry += byte * radix;
                byte = static_cast<u8>(carry);
                carry >>= 8;
            }
            if (carry)
                return false;
        }
        return true;
    }

    bool assign_mask(u8 *destination, const Property &p, const std::string &value) {
        const auto names = bit_names(p);
        std::vector<u8> result(p.size, 0), part(p.size, 0);
        size_t start = 0;
        for (;;) {
            const size_t end = value.find('|', start);
            const std::string token = value.substr(start, end == std::string::npos ? end : end - start);
            bool found = token == "NONE";
            for (size_t bit = 0; !found && bit < names.size(); ++bit)
                if (token == names[bit] || token == "BIT_" + std::to_string(bit)) {
                    result[bit / 8] |= u8(1) << (bit % 8);
                    found = true;
                }
            if (!found) {
                if (!mask_number(token, part))
                    return false;
                for (size_t i = 0; i < p.size; ++i)
                    result[i] |= part[i];
            }
            if (end == std::string::npos)
                break;
            start = end + 1;
        }
        memcpy(destination, result.data(), p.size);
        return true;
    }

    EnumValues enum_values(const std::string &name) {
        EnumValues e;
        if (name.find("extra_unlocked_bits[") == 0 || name.find("extra_purchased_bits[") == 0) {
            const size_t start = name.find('[');
            const size_t word = strtoul(name.c_str() + start + 1, nullptr, 10);
            e.values.push_back({"NONE", 0});
            for (size_t i = word * 32; i < (word + 1) * 32 && i < SAVE_EXTRA_COUNT; ++i)
                e.values.push_back({Cheat[i].name, u32(1) << (i % 32)});
            e.flags = true;
        } else if (name.find("shop_hint_purchased_bits[") == 0 || name.find("shop_character_purchased_bits[") == 0 ||
                   name.find("hint_completion_bits[") == 0 || name == "shop_gold_brick_purchased_bits") {
            e.values.push_back({"NONE", 0});
            for (size_t i = 0; i < 32; ++i)
                e.values.push_back({"BIT_" + std::to_string(i), SAVE_BIT_0 << i});
            e.flags = true;
        } else if (name == "hub_build_flags") {
            e.values = {{"NONE", 0},
                        {"BUILD_0", SAVE_HUB_BUILD_0},
                        {"BUILD_1", SAVE_HUB_BUILD_1},
                        {"BUILD_2", SAVE_HUB_BUILD_2},
                        {"BUILD_3", SAVE_HUB_BUILD_3},
                        {"BUILD_4", SAVE_HUB_BUILD_4},
                        {"BUILD_5", SAVE_HUB_BUILD_5},
                        {"BUILD_6", SAVE_HUB_BUILD_6},
                        {"LEVEL_BUILD", SAVE_HUB_LEVEL_BUILD}};
            e.flags = true;
        } else if (name == "reward_flags") {
            e.values = {{"NONE", 0},
                        {"100_PERCENT", SAVE_REWARD_100_PERCENT},
                        {"ALL_GOLD_BRICKS", SAVE_REWARD_ALL_GOLD_BRICKS}};
            e.flags = true;
        } else if (name.find("character_save[") == 0) {
            e.values = {{"NONE", 0}, {"AVAILABLE", SAVE_CHARACTER_AVAILABLE}, {"UNLOCKED", SAVE_CHARACTER_UNLOCKED}};
            e.flags = true;
        } else if (name.find(".arcade_flags") != std::string::npos) {
            e.values = {{"NONE", 0},
                        {"BATTLE", SAVE_ARCADE_BATTLE},
                        {"COLLECT", SAVE_ARCADE_COLLECT},
                        {"HUNT", SAVE_ARCADE_HUNT}};
            e.flags = true;
        } else if (name.find("episode_save[") == 0 && name.find(".flags") != std::string::npos) {
            e.values = {{"NONE", 0}, {"SUPERSTORY_COMPLETE", SAVE_SUPERSTORY_COMPLETE}};
            e.flags = true;
        } else if (name == "suit_flags") {
            e.values = {{"NONE", 0},
                        {"SHADOW", SAVE_SUIT_SHADOW},
                        {"GLIDE", SAVE_SUIT_GLIDE},
                        {"DEMOLITION", SAVE_SUIT_DEMOLITION},
                        {"SONAR", SAVE_SUIT_SONAR},
                        {"WATER", SAVE_SUIT_WATER},
                        {"TECHNOLOGY", SAVE_SUIT_TECHNOLOGY},
                        {"MAGNET", SAVE_SUIT_MAGNET},
                        {"ATTRACT", SAVE_SUIT_ATTRACT},
                        {"ALL", SAVE_SUIT_ALL}};
            e.flags = true;
        } else if (name == "options.store_pack_flags") {
            e.values = {{"NONE", 0},
                        {"EPISODE_II", SAVE_PACK_EPISODE_II},
                        {"EPISODE_III", SAVE_PACK_EPISODE_III},
                        {"EPISODE_IV", SAVE_PACK_EPISODE_IV},
                        {"EPISODE_V", SAVE_PACK_EPISODE_V},
                        {"EPISODE_VI", SAVE_PACK_EPISODE_VI},
                        {"ARCADE", SAVE_PACK_ARCADE},
                        {"BONUS", SAVE_PACK_BONUS},
                        {"BOUNTY", SAVE_PACK_BOUNTY},
                        {"CHALLENGE", SAVE_PACK_CHALLENGE},
                        {"JEDI", SAVE_PACK_JEDI},
                        {"SITH", SAVE_PACK_SITH}};
            e.flags = true;
        } else if (name == "options.store_bundle_flags") {
            e.values = {{"NONE", 0},
                        {"PREQUEL", SAVE_BUNDLE_PREQUEL},
                        {"ORIGINAL", SAVE_BUNDLE_ORIGINAL},
                        {"COMPLETE", SAVE_BUNDLE_COMPLETE}};
            e.flags = true;
        } else if (name == "options.touch_controls") {
            e.values = {{"VIRTUAL_CONSOLE", SAVE_VIRTUAL_CONSOLE}, {"TOUCH", SAVE_TOUCH}};
        } else if (name.find("_use_saved_name") != std::string::npos) {
            e.values = {{"CHARACTER_DEFAULT", SAVE_CHARACTER_DEFAULT_NAME}, {"SAVED_NAME", SAVE_CUSTOM_NAME}};
        } else if (name == "indy_unlocked" || name.find(".completed[") != std::string::npos ||
                   (name.find("area_save[") == 0 && (name.find("complete") != std::string::npos ||
                                                     name.find("red_brick_collected") != std::string::npos))) {
            e.values = {{"INCOMPLETE", SAVE_INCOMPLETE}, {"COMPLETE", SAVE_COMPLETE}};
        } else if (name == "options.music_enabled" || name == "options.dpad_locked" ||
                   name == "options_save.player1_rumble" || name == "options_save.player2_rumble" ||
                   name == "options_save.surround_sound" || name == "options_save.music_enabled" ||
                   name == "options_save.widescreen") {
            e.values = {{"OFF", SAVE_OFF}, {"ON", SAVE_ON}};
        }
        return e;
    }

    std::string enum_text(const EnumValues &e, u32 value) {
        for (const auto &entry : e.values)
            if (entry.value == value)
                return entry.name;
        std::string result;
        if (e.flags)
            for (const auto &entry : e.values)
                if (entry.value && (value & entry.value) == entry.value) {
                    if (!result.empty())
                        result += '|';
                    result += entry.name;
                    value &= ~entry.value;
                }
        if (result.empty() || value) {
            if (!result.empty())
                result += '|';
            result += std::to_string(value);
        }
        return result;
    }

    int fail(const std::string &message) {
        fprintf(stderr, "save: %s\n", message.c_str());
        return 1;
    }

    void usage() {
        puts("Usage: saga_native save list [path] [property-filter | --filter <text>]\n"
             "       saga_native save schema [path] [--options] [--filter <text>] [--tsv]\n"
             "       saga_native save edit [path] [--output <path>] [--params <file>] [key=value ...]\n"
             "       saga_native save create [path] [--from <save> | --options] [--params <file>] [key=value ...]\n"
             "\nBare 'save' lists the default save. Schema explains every property without needing a file.\n"
             "Use --path <path> in place of [path] when a filename resembles an option or assignment.\n"
             "\nList prints every stored byte through named properties or byte[N] (absolute file offset).\n"
             "Integers accept decimal or 0x hex; floats accept decimal; byte arrays use hex:0011aaff.\n"
             "Flags also accept schema enum names, joined with | for bit masks; unknown bits remain numeric.\n"
             "Fixed byte arrays also accept text:NAME (zero-padded, with room for a terminator).\n"
             "--params reads key=value lines, including list output; # starts a whole-line comment.\n"
             "Checksum and slot_code are refreshed using game routines; --keep-derived preserves them.\n"
             "Create refuses existing output. Edit writes atomically; --output refuses existing output.\n"
             "Without --from, create calls NewGame without asset configuration; asset-dependent defaults\n"
             "must be supplied as parameters. --options creates a zero-initialized SuperOptions save.");
        printf("Default path (relative to working directory): res/SavedGames/%s\n", slotname(0));
    }

    const char *description(std::string name) {
        // Descriptions apply to every index; actual offsets/types remain driven
        // by properties() and the canonical reconstructed structures.
        for (size_t begin = name.find('['); begin != std::string::npos; begin = name.find('[', begin + 2)) {
            const size_t end = name.find(']', begin);
            if (end == std::string::npos)
                break;
            name.erase(begin + 1, end - begin - 1);
        }
        static const struct {
            const char *name;
            const char *text;
        } explanations[] = {
            {"field_0x0", "Unidentified byte cleared by NewGame; no field-specific consumer established."},
            {"field_0x2", "Two unidentified bytes before the options record; cleared by NewGame."},
            {"options_save.field7_0x7", "Initialized to zero by InitGameBeforeConfig; no further meaning established."},
            {"options_save.field8_0x8", "Initialized to zero by InitGameBeforeConfig; no further meaning established."},
            {"options_save.field9_0x9",
             "Unidentified option byte preserved by whole-record copies; no field-specific use established."},
            {"options_save.field10_0xa",
             "Unidentified option byte preserved by whole-record copies; no field-specific use established."},
            {"level_save[].reserved_0x51",
             "Two unidentified bytes after the pickup count; preserved without assigning gameplay meanings."},
            {"area_save[].reserved_0x7", "Byte before the aligned challenge-time float; no recovered gameplay use."},
            {"reserved_0x7c2a", "Two bytes before the aligned gameplay-time float; no recovered gameplay use."},
            {"field_0x7c9f",
             "Last byte before mission storage; retained separately from the recovered customizer prefix."},
            {"options.field9_0x16", "Two trailing SuperOptions bytes; no field-specific consumer established."},
            {"difficulty", "AI difficulty threshold; NewGame initializes 5. Used by ResetAICreatures, "
                           "ManageGameObjects, KillGameObject and ReleaseHearts; not a format version."},
            {"options_save.player1_rumble", "Player 1 controller vibration toggle."},
            {"options_save.player2_rumble", "Player 2 controller vibration toggle."},
            {"options_save.surround_sound", "Dolby Pro Logic/surround toggle; passed to NuSound3SetDPL."},
            {"options_save.sound_volume", "Sound effects volume, menu range 0..10; multiplied by master_volume/10."},
            {"options_save.music_volume", "Music volume, menu range 0..10; multiplied by master_volume/10."},
            {"options_save.master_volume", "Master volume, menu range 0..10; scales effects, music and cutscenes."},
            {"options_save.music_enabled", "Per-game music toggle checked by GamePlayMusic."},
            {"options_save.widescreen", "Widescreen video toggle."},
            {"options_save.brightness", "Brightness, menu range 0..10; passed to NuVideoSetBrightness divided by 10."},
            {"level_save[].minikit_names[]", "Ten fixed 8-byte pickup-name slots per level. First minikit_count slots "
                                             "are used by GizmoPickups_Reset and SuperCounter_AnyCollected."},
            {"level_save[].minikit_count", "Number of stored pickup names, gameplay range 0..10. Record stride 84; 366 "
                                           "slots fit before area storage."},
            {"level_save[].arcade_flags",
             "Arcade modes won; Arcade_AwardPoint sets one bit per mode. All three bits complete the area."},
            {"level_save_padding",
             "Three bytes between the level-record capacity and area storage; no recovered field use."},
            {"shop_hint_purchased_bits", "96-bit shop-hint purchase set, stored as three u32s. Original ShopHintTab "
                                         "contains only its -1 sentinel, so no named shop entries are established."},
            {"shop_character_purchased_bits",
             "128-bit purchase set, stored as four u32s. Named bits follow the 90 buy_in_shop entries in the shipped "
             "collection configuration, not character IDs. Remaining bits are preserved."},
            {"extra_unlocked_bits", "64-bit extra-unlock set, stored as two u32s. Names index the original 44-entry "
                                    "Cheat table; codes/red bricks unlock extras."},
            {"extra_purchased_bits", "64-bit extra-purchase set, stored as two u32s. Named independently of unlocks "
                                     "and the non-persisted enabled-cheat state."},
            {"shop_gold_brick_purchased_bits",
             "Purchased shop gold bricks: bit i is shop brick i; original completion calculation scans 14 entries."},
            {"suit_flags", "Suit availability mask retained from the shared Batman engine. Separate from SuperOptions "
                           "store entitlements."},
            {"gold_bricks", "Earned gold-brick total; AddToGoldBricks caps it at GOLDBRICKPOINTS."},
            {"reward_flags", "One-time completion reward callbacks already fired; bit 0 also enables SuperWeirdo."},
            {"hub_build_flags", "Hub construction progress bits used by Hub_Update and GizBuildIt_FinishFn_Game; "
                                "meanings are indexed by hub build data."},
            {"indy_unlocked", "Indiana Jones unlock marker tested by IndyUnlocked_UpdateHint and the hub."},
            {"gameplay_seconds", "Accumulated gameplay time in seconds; GameTiming and hub time display."},
            {"mission_save.best_times[]",
             "Twenty mission best-time floats in seconds; mission index follows the configured mission table."},
            {"mission_save.completed[]", "Twenty mission completion bytes, following the 80-byte time array."},
            {"area_save[].minikit_complete",
             "Complete minikit-set reward marker, independent of the count at the next byte."},
            {"area_save[].red_brick_collected",
             "Area power/red brick collected marker; BuyAllShopExtras also sets it for each extra's associated area."},
            {"options.store_pack_flags", "Eleven individual store entitlements; names recovered from original "
                                         "StorePack data. Initial value 65535 enables all, including reserved bits."},
            {"options.store_bundle_flags", "Three purchase-bundle bits; original StoreBundle data names prequel, "
                                           "original trilogy and complete. Initial value 255."},
            {"options.dpad_locked", "Virtual D-pad position lock; toggled by its lock button callback."},
            {"customizer.primary_use_saved_name",
             "GameObj_GetName selects the stored primary name when nonzero; otherwise the character's text-table name. "
             "NewGame sets SAVED_NAME."},
            {"customizer.secondary_use_saved_name",
             "GameObj_GetName selects the stored secondary name when nonzero; otherwise the character's text-table "
             "name. NewGame sets SAVED_NAME."},
            {"header.field0_0x0", "Envelope magic: must remain 0x52474d48 (HMGR in file byte order)."},
            {"header.field1_0x4", "Envelope version: must remain 1."},
            {"header.size", "Envelope size in bytes: must remain 8232 (0x2028)."},
            {"header.extradata_offset",
             "Bytes skipped after the header before the payload. Edits must preserve this layout."},
            {"extra_prefix", "Opaque bytes skipped by the game loader before the payload; preserved verbatim."},
            {"coins", "Stored stud/coin balance, as an unsigned integer."},
            {"completion", "Completion points, not a percentage. Displayed percent is points * 100 / game "
                           "COMPLETIONPOINTS; also feeds slot_code."},
            {"character_save[]", "Character ID state: AVAILABLE is owned/playable (Collection_Got); UNLOCKED is "
                                 "CollectIDUnlocked. NewGame sets both for defaults."},
            {"hint_completion_bits",
             "192-bit tutorial set: console IDs 0..95 at bits 0..95; touch IDs 0..95 at bits 96..191. Names identify "
             "original Hints_LSW callback and console text ID. Stored as six u32s."},
            {"area_save[].complete", "Area availability/progression marker used when choosing the next area; NewGame "
                                     "sets starting areas to 1."},
            {"area_save[].area_complete", "Area completion marker; game completion paths set this to 1."},
            {"area_save[].story_buildup_complete", "Story-mode stud-buildup completion marker; normally 0 or 1."},
            {"area_save[].freeplay_buildup_complete", "Free-play stud-buildup completion marker; normally 0 or 1."},
            {"area_save[].minikit_count",
             "Stored area minikit count. The utility enforces byte width, not gameplay consistency."},
            {"area_save[].challenge_trial_time",
             "Challenge trial time in seconds; NewGame obtains the starting value from area data."},
            {"episode_save[].superstory_time_limit", "Stored best Super Story time in seconds; initialized to 3600 for "
                                                     "episodes 0..5 and reduced by better times."},
            {"episode_save[].superstory_score_target",
             "Stored best Super Story score; initialized to 100000 for episodes 0..5 and increased by better scores."},
            {"episode_save[].flags", "Super Story status word. The low byte becomes 1 on first completion; remaining "
                                     "bits are preserved as numeric values."},
            {"customizer.pieces[]",
             "Nine primary custom-character piece IDs, interpreted using the game's customizer data."},
            {"customizer.secondary_pieces[]",
             "Nine secondary custom-character piece IDs, interpreted using the game's customizer data."},
            {"customizer.primary_name",
             "Primary custom-character name: 32-byte buffer. text:NAME zero-pads and requires room for a terminator."},
            {"customizer.secondary_name", "Secondary custom-character name: 32-byte buffer. text:NAME zero-pads and "
                                          "requires room for a terminator."},
            {"options.touch_controls",
             "Touch control selection: 0 selects runtime control mode 1, nonzero selects mode 2."},
            {"options.left_control_x",
             "Saved left touch-control horizontal position; coordinate scale/range is not yet established."},
            {"options.left_control_y",
             "Saved left touch-control vertical position; coordinate scale/range is not yet established."},
            {"options.right_control_x",
             "Saved right touch-control horizontal position; coordinate scale/range is not yet established."},
            {"options.right_control_y",
             "Saved right touch-control vertical position; coordinate scale/range is not yet established."},
            {"options.music_enabled", "Music enabled flag: 0 disables music, nonzero enables it."},
            {"checksum", "Derived: ChecksumSaveData(payload), sum of little-endian u32 words plus 0x5c0999 modulo "
                         "2^32. Editing requires --keep-derived."},
            {"slot_code", "Derived: sign-extended i16 MakeSaveHash (completion points); options use 0xffffffff. "
                          "Editing requires --keep-derived."},
            {"byte[]", "Unidentified/padding byte at an absolute file offset. Raw byte[N] aliases are also available "
                       "for every named field."},
        };
        for (const auto &entry : explanations)
            if (name == entry.name)
                return entry.text;
        return "Unknown/reserved field: storage layout is recovered, but its meaning is not established. Preserved "
               "unless explicitly edited.";
    }

    std::vector<std::string> wrap_cell(std::string text, size_t width) {
        std::vector<std::string> lines;
        while (text.size() > width) {
            size_t split = text.rfind(' ', width);
            if (split == std::string::npos || split == 0)
                split = width;
            lines.push_back(text.substr(0, split));
            text.erase(0, split);
            while (!text.empty() && text[0] == ' ')
                text.erase(0, 1);
        }
        lines.push_back(text);
        return lines;
    }

    void schema_border() {
        const size_t widths[] = {44, 12, 24, 52};
        putchar('+');
        for (size_t width : widths) {
            for (size_t i = 0; i < width + 2; ++i)
                putchar('-');
            putchar('+');
        }
        putchar('\n');
    }

    void schema_row(const std::string &name, const std::string &type, const std::string &layout,
                    const std::string &details) {
        const size_t widths[] = {44, 12, 24, 52};
        const std::vector<std::string> cells[] = {wrap_cell(name, widths[0]), wrap_cell(type, widths[1]),
                                                  wrap_cell(layout, widths[2]), wrap_cell(details, widths[3])};
        size_t height = 1;
        for (const auto &cell : cells)
            if (cell.size() > height)
                height = cell.size();
        for (size_t line = 0; line < height; ++line) {
            putchar('|');
            for (size_t column = 0; column < 4; ++column)
                printf(" %-*s |", static_cast<int>(widths[column]),
                       line < cells[column].size() ? cells[column][line].c_str() : "");
            putchar('\n');
        }
        schema_border();
    }

    void schema(const Save &save, const std::vector<Property> &fields, const std::string &filter, bool tsv = false) {
        struct Group {
            Property field;
            std::string family;
            size_t first_index, last_index, count, stride;
            bool matches;
        };
        std::vector<Group> groups;
        for (const Property &p : fields) {
            const size_t begin = p.name.find('['), end = p.name.find(']');
            const bool indexed = begin != std::string::npos && end != std::string::npos;
            const std::string family = indexed ? p.name.substr(0, begin) + "[]" + p.name.substr(end + 1) : p.name;
            const size_t index = indexed ? strtoul(p.name.substr(begin + 1, end - begin - 1).c_str(), nullptr, 10) : 0;
            bool merged = false;
            if (indexed && p.name.find("extra_unlocked_bits[") != 0 && p.name.find("extra_purchased_bits[") != 0)
                for (Group &group : groups) {
                    if (group.family != family || group.field.kind != p.kind || group.field.size != p.size ||
                        index != group.last_index + 1 || p.offset <= group.field.offset)
                        continue;
                    const size_t stride = (p.offset - group.field.offset) / group.count;
                    if ((p.offset - group.field.offset) % group.count != 0 ||
                        (group.count > 1 && stride != group.stride))
                        continue;
                    group.stride = stride;
                    group.last_index = index;
                    ++group.count;
                    group.matches = group.matches || p.name.find(filter) != std::string::npos;
                    merged = true;
                    break;
                }
            if (!merged)
                groups.push_back({p, family, index, index, 1, 0, p.name.find(filter) != std::string::npos});
        }
        printf("# %s schema; little-endian; payload offset=0x%zx; payload bytes=%zu\n",
               save.payload_size == sizeof(GAMESAVE_s) ? "GAMESAVE_s" : "SUPEROPTIONS_s", save.payload,
               save.payload_size);
        puts("# Offsets are absolute file offsets. Indices are zero-based. Ranges are storage limits, not gameplay "
             "guarantees.\n"
             "# Byte arrays also accept member[index]=0..255. Any stored byte accepts byte[absolute-offset]=0..255.\n"
             "# Array ranges are inclusive. Offset is for the first element; stride and element_bytes are in bytes.");
        if (tsv)
            puts("property\ttype\tfile_offset\telement_bytes\tcount\tstride\taccepted_values\tdescription");
        else {
            schema_border();
            schema_row("Property", "Type", "Layout (bytes)", "Description / accepted values");
        }
        for (const Group &group : groups) {
            const Property &p = group.field;
            std::string name = p.name;
            if (group.count > 1) {
                const size_t begin = name.find('['), end = name.find(']');
                name.replace(begin + 1, end - begin - 1,
                             std::to_string(group.first_index) + ".." + std::to_string(group.last_index));
            }
            if (!group.matches && name.find(filter) == std::string::npos)
                continue;
            std::string type, range;
            if (p.kind == Kind::bitmask) {
                type = "bits" + std::to_string(p.size * 8);
                range = "0..(2^" + std::to_string(p.size * 8) +
                        "-1); NONE, decimal, 0x hex, or names joined with |; enum mapping (zero-based bit): ";
                const auto names = bit_names(p);
                for (size_t bit = 0; bit < names.size(); ++bit) {
                    if (bit)
                        range += ", ";
                    if (names[bit] == "BIT_" + std::to_string(bit)) {
                        const size_t first = bit;
                        while (bit + 1 < names.size() && names[bit + 1] == "BIT_" + std::to_string(bit + 1))
                            ++bit;
                        range += std::to_string(first);
                        if (bit != first)
                            range += ".." + std::to_string(bit);
                        range += "=unidentified (BIT_n)";
                    } else
                        range += std::to_string(bit) + "=" + names[bit];
                }
            } else if (p.kind == Kind::bytes) {
                type = "bytes[" + std::to_string(p.size) + "]";
                range = "hex: with " + std::to_string(p.size * 2) + " hex digits; text: with at most " +
                        std::to_string(p.size - 1) + " bytes";
            } else if (p.kind == Kind::floating) {
                type = "f32";
                range = "finite float32 (raw byte aliases preserve all bit patterns)";
            } else {
                const bool signed_value = p.kind == Kind::signed_integer;
                type = (signed_value ? "i" : "u") + std::to_string(p.size * 8);
                const u64 limit = u64(1) << (p.size * 8 - (signed_value ? 1 : 0));
                range = (signed_value ? "-" + std::to_string(limit) : "0") + ".." + std::to_string(limit - 1);
            }
            const EnumValues values = p.kind == Kind::bitmask ? EnumValues{} : enum_values(p.name);
            if (!values.values.empty()) {
                range += "; ";
                for (size_t i = 0; i < values.values.size(); ++i) {
                    if (i)
                        range += ", ";
                    range += std::string(values.values[i].name) + "=" + std::to_string(values.values[i].value);
                }
                if (values.flags)
                    range += "; combine with |; unknown bits accept numeric values";
            }
            if (tsv) {
                printf("%s\t%s\t0x%zx\t%zu\t%zu\t%zu\t%s\t%s\n", name.c_str(), type.c_str(), p.offset, p.size,
                       group.count, group.stride, range.c_str(), description(p.name));
            } else {
                char layout[128];
                snprintf(layout, sizeof(layout), "Offset 0x%zx; size %zu; count %zu; stride %zu", p.offset, p.size,
                         group.count, group.stride);
                schema_row(name, type, layout, std::string(description(p.name)) + " Values: " + range + ".");
            }
        }
    }

    u32 read_word(const Save &save, size_t offset) {
        u32 value;
        memcpy(&value, &save.bytes[offset], sizeof(value));
        return value;
    }

    void write_word(Save &save, size_t offset, u32 value) {
        memcpy(&save.bytes[offset], &value, sizeof(value));
    }

    bool validate(Save &save) {
        if (save.bytes.size() < sizeof(SaveLoad) + 8)
            return false;
        SaveLoad header;
        memcpy(&header, save.bytes.data(), sizeof(header));
        if (header.field0_0x0 != 0x52474d48 || header.field1_0x4 != 1 || header.size != sizeof(header) ||
            header.extradata_offset < 0)
            return false;
        save.payload = sizeof(header) + static_cast<size_t>(header.extradata_offset);
        if (save.payload > save.bytes.size() - 8)
            return false;
        save.payload_size = save.bytes.size() - save.payload - 8;
        return save.payload_size == sizeof(GAMESAVE_s) || save.payload_size == sizeof(SUPEROPTIONS_s);
    }

    bool read_save(const char *path, Save &save) {
        FILE *file = fopen(path, "rb");
        if (!file)
            return false;
        bool ok = fseek(file, 0, SEEK_END) == 0;
        const auto length = ok ? ftell(file) : -1;
        // Bound allocation before parsing untrusted offsets; ordinary saves are about 40 KiB.
        ok = length >= 0 && length <= 16 * 1024 * 1024 && fseek(file, 0, SEEK_SET) == 0;
        if (ok) {
            save.bytes.resize(static_cast<size_t>(length));
            ok = fread(save.bytes.data(), 1, save.bytes.size(), file) == save.bytes.size();
        }
        if (fclose(file) != 0)
            ok = false;
        return ok && validate(save);
    }

    u32 checksum(const Save &save) {
        // The original routine reads u32s. Copy to aligned storage even when
        // the file's extra-data prefix leaves its payload unaligned.
        std::vector<u32> aligned(save.payload_size / sizeof(u32));
        memcpy(aligned.data(), &save.bytes[save.payload], save.payload_size);
        return static_cast<u32>(ChecksumSaveData(aligned.data(), static_cast<i32>(save.payload_size)));
    }

    void derive(Save &save) {
        write_word(save, save.payload + save.payload_size, checksum(save));
        u32 code = 0xffffffffu;
        if (save.payload_size == sizeof(Game)) {
            memcpy(&Game, &save.bytes[save.payload], sizeof(Game));
            // SaveSystemInitialise stores MakeSaveHash as an i16 callback.
            code = static_cast<u32>(static_cast<i32>(static_cast<i16>(MakeSaveHash())));
        }
        write_word(save, save.bytes.size() - 4, code);
    }

    std::vector<Property> properties(const Save &save) {
        std::vector<Property> result;
        auto add = [&](const std::string &name, size_t offset, size_t size, Kind kind) {
            result.push_back({name, offset, size, kind});
        };
#define HEADER(member)                                                                                                 \
    add("header." #member, offsetof(SaveLoad, member), sizeof(((SaveLoad *)0)->member), Kind::signed_integer)
        HEADER(field0_0x0);
        HEADER(field1_0x4);
        HEADER(size);
        HEADER(field3_0xc);
        HEADER(field4_0x10);
        HEADER(extradata_offset);
        HEADER(field7_0x28);
        HEADER(field9_0x828);
        HEADER(field11_0x1028);
        HEADER(field13_0x1828);
#undef HEADER
#define HEADER_BYTES(member)                                                                                           \
    add("header." #member, offsetof(SaveLoad, member), sizeof(((SaveLoad *)0)->member), Kind::bytes)
        HEADER_BYTES(field6_0x18);
        HEADER_BYTES(field8_0x2a);
        HEADER_BYTES(field10_0x82a);
        HEADER_BYTES(field12_0x102a);
        HEADER_BYTES(field14_0x182a);
#undef HEADER_BYTES
        if (save.payload > sizeof(SaveLoad))
            add("extra_prefix", sizeof(SaveLoad), save.payload - sizeof(SaveLoad), Kind::bytes);
        const size_t base = save.payload;
        if (save.payload_size == sizeof(GAMESAVE_s)) {
#define GAME(member, kind)                                                                                             \
    add(#member, base + offsetof(GAMESAVE_s, member), sizeof(((GAMESAVE_s *)0)->member), Kind::kind)
            GAME(field_0x0, unsigned_integer);
            GAME(difficulty, unsigned_integer);
            GAME(field_0x2, bytes);
            GAME(shop_gold_brick_purchased_bits, bitmask);
            GAME(shop_hint_purchased_bits, bitmask);
            GAME(shop_character_purchased_bits, bitmask);
            GAME(extra_unlocked_bits, bitmask);
            GAME(extra_purchased_bits, bitmask);
            GAME(hint_completion_bits, bitmask);
            GAME(suit_flags, unsigned_integer);
            GAME(level_save_padding, bytes);
            GAME(coins, unsigned_integer);
            GAME(completion, unsigned_integer);
            GAME(gold_bricks, unsigned_integer);
            GAME(reward_flags, unsigned_integer);
            GAME(hub_build_flags, unsigned_integer);
            GAME(indy_unlocked, unsigned_integer);
            GAME(reserved_0x7c2a, bytes);
            GAME(gameplay_seconds, floating);
            GAME(field_0x7c9f, unsigned_integer);
#undef GAME
#define OPTION(member)                                                                                                 \
    add("options_save." #member, base + offsetof(GAMESAVE_s, options_save) + offsetof(OPTIONSSAVE, member),            \
        sizeof(((OPTIONSSAVE *)0)->member), Kind::unsigned_integer)
            OPTION(player1_rumble);
            OPTION(player2_rumble);
            OPTION(surround_sound);
            OPTION(sound_volume);
            OPTION(music_volume);
            OPTION(master_volume);
            OPTION(music_enabled);
            OPTION(field7_0x7);
            OPTION(field8_0x8);
            OPTION(field9_0x9);
            OPTION(field10_0xa);
            OPTION(widescreen);
            OPTION(brightness);
#undef OPTION
            for (size_t i = 0; i < sizeof(Game.level_records) / sizeof(Game.level_records[0]); ++i) {
                const std::string prefix = "level_save[" + std::to_string(i) + "].";
                const size_t offset = base + offsetof(GAMESAVE_s, level_records) + i * sizeof(LEVELSAVE_s);
                for (size_t j = 0; j < sizeof(LEVELSAVE_s::minikit_names) / sizeof(LEVELSAVE_s::minikit_names[0]); ++j)
                    add(prefix + "minikit_names[" + std::to_string(j) + "]",
                        offset + offsetof(LEVELSAVE_s, minikit_names) + j * sizeof(LEVELSAVE_s::minikit_names[0]),
                        sizeof(LEVELSAVE_s::minikit_names[0]), Kind::bytes);
#define LEVEL(member, kind)                                                                                            \
    add(prefix + #member, offset + offsetof(LEVELSAVE_s, member), sizeof(((LEVELSAVE_s *)0)->member), Kind::kind)
                LEVEL(minikit_count, unsigned_integer);
                LEVEL(reserved_0x51, bytes);
                LEVEL(arcade_flags, unsigned_integer);
#undef LEVEL
            }
            for (size_t i = 0; i < sizeof(MISSIONSAVE::best_times) / sizeof(f32); ++i) {
                const size_t offset = base + offsetof(GAMESAVE_s, mission_save);
                add("mission_save.best_times[" + std::to_string(i) + "]",
                    offset + offsetof(MISSIONSAVE, best_times) + i * sizeof(f32), sizeof(f32), Kind::floating);
                add("mission_save.completed[" + std::to_string(i) + "]", offset + offsetof(MISSIONSAVE, completed) + i,
                    1, Kind::unsigned_integer);
            }
            for (size_t i = 0; i < sizeof(((GAMESAVE_s *)0)->character_save); ++i)
                add("character_save[" + std::to_string(i) + "]", base + offsetof(GAMESAVE_s, character_save) + i, 1,
                    Kind::unsigned_integer);
            for (size_t i = 0; i < sizeof(Game.area_save) / sizeof(AREASAVE_s); ++i) {
                const std::string prefix = "area_save[" + std::to_string(i) + "].";
                const size_t offset = base + offsetof(GAMESAVE_s, area_save) + i * sizeof(AREASAVE_s);
#define AREA(member, kind)                                                                                             \
    add(prefix + #member, offset + offsetof(AREASAVE_s, member), sizeof(((AREASAVE_s *)0)->member), Kind::kind)
                AREA(complete, unsigned_integer);
                AREA(area_complete, unsigned_integer);
                AREA(story_buildup_complete, unsigned_integer);
                AREA(freeplay_buildup_complete, unsigned_integer);
                AREA(minikit_complete, unsigned_integer);
                AREA(minikit_count, unsigned_integer);
                AREA(red_brick_collected, unsigned_integer);
                AREA(reserved_0x7, unsigned_integer);
                AREA(challenge_trial_time, floating);
#undef AREA
            }
            for (size_t i = 0; i < sizeof(Game.episode_save) / sizeof(EPISODESAVE_s); ++i) {
                const std::string prefix = "episode_save[" + std::to_string(i) + "].";
                const size_t offset = base + offsetof(GAMESAVE_s, episode_save) + i * sizeof(EPISODESAVE_s);
#define EPISODE(member, kind)                                                                                          \
    add(prefix + #member, offset + offsetof(EPISODESAVE_s, member), sizeof(((EPISODESAVE_s *)0)->member), Kind::kind)
                EPISODE(superstory_time_limit, floating);
                EPISODE(superstory_score_target, signed_integer);
                EPISODE(flags, unsigned_integer);
#undef EPISODE
            }
            const size_t custom = base + offsetof(GAMESAVE_s, customizer);
            for (size_t i = 0; i < 9; ++i)
                add("customizer.pieces[" + std::to_string(i) + "]",
                    custom + offsetof(CUSTOMISESAVE_s, pieces) + i * sizeof(i16), sizeof(i16), Kind::signed_integer);
            for (size_t i = 0; i < 9; ++i)
                add("customizer.secondary_pieces[" + std::to_string(i) + "]",
                    custom + offsetof(CUSTOMISESAVE_s, secondary_pieces) + i * sizeof(i16), sizeof(i16),
                    Kind::signed_integer);
#define CUSTOM(member, kind)                                                                                           \
    add("customizer." #member, custom + offsetof(CUSTOMISESAVE_s, member), sizeof(((CUSTOMISESAVE_s *)0)->member),     \
        Kind::kind)
            CUSTOM(field_0x12, bytes);
            CUSTOM(primary_name, bytes);
            CUSTOM(primary_use_saved_name, unsigned_integer);
            CUSTOM(field_0x35, bytes);
            CUSTOM(secondary_name, bytes);
            CUSTOM(secondary_use_saved_name, unsigned_integer);
            CUSTOM(field_0x4a, bytes);
            CUSTOM(field_0x6d, bytes);
#undef CUSTOM
        } else {
#define OPTIONS(member, kind)                                                                                          \
    add("options." #member, base + offsetof(SUPEROPTIONS_s, member), sizeof(((SUPEROPTIONS_s *)0)->member), Kind::kind)
            OPTIONS(store_pack_flags, unsigned_integer);
            OPTIONS(touch_controls, unsigned_integer);
            OPTIONS(dpad_locked, unsigned_integer);
            OPTIONS(left_control_x, floating);
            OPTIONS(left_control_y, floating);
            OPTIONS(right_control_x, floating);
            OPTIONS(right_control_y, floating);
            OPTIONS(music_enabled, unsigned_integer);
            OPTIONS(store_bundle_flags, unsigned_integer);
            OPTIONS(field9_0x16, bytes);
#undef OPTIONS
        }
        add("checksum", base + save.payload_size, 4, Kind::unsigned_integer);
        add("slot_code", save.bytes.size() - 4, 4, Kind::unsigned_integer);
        // Keep padding and presently unidentified bytes visible and editable.
        std::vector<bool> covered(save.bytes.size(), false);
        for (const Property &p : result)
            for (size_t i = 0; i < p.size; ++i)
                covered[p.offset + i] = true;
        for (size_t i = 0; i < covered.size(); ++i)
            if (!covered[i])
                add("byte[" + std::to_string(i) + "]", i, 1, Kind::unsigned_integer);
        return result;
    }

    bool number(const std::string &text, u64 &value) {
        if (text.empty() || text[0] == '-' || text[0] == '+' || text[0] == ' ')
            return false;
        char *end;
        errno = 0;
        value = strtoull(text.c_str(), &end, text.compare(0, 2, "0x") == 0 ? 16 : 10);
        return errno == 0 && end != text.c_str() && *end == 0;
    }

    bool assign(Save &save, const std::vector<Property> &fields, const std::string &assignment) {
        const size_t equals = assignment.find('=');
        if (equals == std::string::npos)
            return false;
        std::string name = assignment.substr(0, equals), value = assignment.substr(equals + 1);
        if (name == "save_version")
            name = "difficulty";
        if (name == "field30_0x7c2c")
            name = "gameplay_seconds";
        if (name == "initial_store_pack_flags")
            name = "suit_flags";
        if (name == "field_0x7bf8")
            name = "shop_gold_brick_purchased_bits";
        if (name == "customizer.primary_name_unlocked")
            name = "customizer.primary_use_saved_name";
        if (name == "customizer.secondary_name_unlocked")
            name = "customizer.secondary_use_saved_name";
        Property field = {"", 0, 0, Kind::bytes};
        for (const Property &p : fields)
            if (p.name == name) {
                field = p;
                break;
            }
        // Every file byte can also be addressed directly; byte-array members
        // accept member[N] for convenient individual flags and character IDs.
        if (field.size == 0 && !name.empty() && name.back() == ']') {
            const size_t bracket = name.rfind('[');
            u64 index;
            if (bracket != std::string::npos && number(name.substr(bracket + 1, name.size() - bracket - 2), index)) {
                if (name.substr(0, bracket) == "byte" && index < save.bytes.size())
                    field = {name, static_cast<size_t>(index), 1, Kind::unsigned_integer};
                for (const Property &p : fields)
                    if (p.kind == Kind::bytes && p.name == name.substr(0, bracket) && index < p.size)
                        field = {name, p.offset + static_cast<size_t>(index), 1, Kind::unsigned_integer};
                    else if (p.kind == Kind::bitmask && p.name == name.substr(0, bracket) && index < p.size / 4)
                        field = {name, p.offset + static_cast<size_t>(index) * 4, 4, Kind::unsigned_integer};
            }
        }
        if (field.size == 0)
            return false;
        u8 *destination = &save.bytes[field.offset];
        if (field.kind == Kind::bitmask)
            return assign_mask(destination, field, value);
        if (field.kind == Kind::bytes) {
            if (value.compare(0, 5, "text:") == 0 && value.size() - 5 < field.size) {
                memset(destination, 0, field.size);
                memcpy(destination, value.data() + 5, value.size() - 5);
                return true;
            }
            if (value.compare(0, 4, "hex:") != 0 || value.size() - 4 != field.size * 2)
                return false;
            for (size_t i = 0; i < field.size; ++i) {
                const std::string pair = value.substr(4 + i * 2, 2);
                if (pair.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos)
                    return false;
                destination[i] = static_cast<u8>(strtoul(pair.c_str(), nullptr, 16));
            }
            return true;
        }
        if (field.kind == Kind::floating) {
            char *end;
            errno = 0;
            const float parsed = strtof(value.c_str(), &end);
            if (errno || end == value.c_str() || *end || !std::isfinite(parsed))
                return false;
            memcpy(destination, &parsed, sizeof(parsed));
            return true;
        }
        const EnumValues values = enum_values(name);
        if (!values.values.empty()) {
            u64 combined = 0;
            size_t start = 0;
            bool symbolic = false;
            for (;;) {
                const size_t end = value.find('|', start);
                if (end != std::string::npos && !values.flags)
                    return false;
                const std::string token = value.substr(start, end == std::string::npos ? end : end - start);
                u64 part = 0;
                bool found = false;
                for (const auto &entry : values.values)
                    if (token == entry.name) {
                        part = entry.value;
                        found = true;
                        symbolic = true;
                        break;
                    }
                if (!found && !number(token, part))
                    return false;
                combined |= part;
                if (end == std::string::npos)
                    break;
                start = end + 1;
            }
            if (symbolic || value.find('|') != std::string::npos)
                value = std::to_string(combined);
        }
        u64 parsed;
        const bool negative = !value.empty() && value[0] == '-';
        if (!number(negative ? value.substr(1) : value, parsed))
            return false;
        const u64 limit =
            field.kind == Kind::signed_integer ? (u64(1) << (field.size * 8 - 1)) : (u64(1) << (field.size * 8));
        if (negative) {
            if (field.kind != Kind::signed_integer || parsed > limit)
                return false;
            parsed = 0 - parsed;
        } else if (parsed >= limit)
            return false;
        for (size_t i = 0; i < field.size; ++i)
            destination[i] = static_cast<u8>(parsed >> (i * 8));
        return true;
    }

    void list(const Save &save, const std::vector<Property> &fields, const std::string &filter) {
        printf("# payload_size=%zu checksum_valid=%s\n", save.payload_size,
               read_word(save, save.payload + save.payload_size) == checksum(save) ? "true" : "false");
        for (const Property &p : fields) {
            if (p.name.find(filter) == std::string::npos)
                continue;
            const u8 *data = &save.bytes[p.offset];
            if (p.kind == Kind::bitmask) {
                const auto names = bit_names(p);
                std::string value;
                printf("# %s: %s; set bits:", p.name.c_str(), mask_hex(data, p.size).c_str());
                for (size_t bit = 0; bit < names.size(); ++bit)
                    if (data[bit / 8] & (u8(1) << (bit % 8))) {
                        printf(" %zu=%s", bit, names[bit].c_str());
                        if (!value.empty())
                            value += '|';
                        value += names[bit];
                    }
                printf("%s\n%s=%s\n", value.empty() ? " none" : "", p.name.c_str(),
                       value.empty() ? "NONE" : value.c_str());
                continue;
            }
            const EnumValues enumeration = enum_values(p.name);
            if (!enumeration.values.empty()) {
                printf("# %s: %s", p.name.c_str(), mask_hex(data, p.size).c_str());
                if (enumeration.flags) {
                    printf("; set bits:");
                    bool any = false;
                    for (size_t bit = 0; bit < p.size * 8; ++bit)
                        if (data[bit / 8] & (u8(1) << (bit % 8))) {
                            const std::string label = enum_text(enumeration, u32(1) << bit);
                            printf(" %zu=%s", bit, label.c_str());
                            any = true;
                        }
                    if (!any)
                        printf(" none");
                }
                putchar('\n');
            }
            printf("%s=", p.name.c_str());
            if (p.kind == Kind::bytes) {
                printf("hex:");
                for (size_t i = 0; i < p.size; ++i)
                    printf("%02x", data[i]);
            } else if (p.kind == Kind::floating) {
                float value;
                memcpy(&value, data, sizeof(value));
                if (std::isfinite(value) && (value == 0 || std::isnormal(value)))
                    printf("%.9g", static_cast<double>(value));
                else {
                    // Raw aliases preserve NaN payloads and subnormals without
                    // relying on the host's decimal conversion/underflow rules.
                    printf("0\n");
                    for (size_t i = 0; i < p.size; ++i)
                        printf("byte[%zu]=%u%s", p.offset + i, data[i], i + 1 == p.size ? "" : "\n");
                }
            } else {
                u32 value = 0;
                memcpy(&value, data, p.size);
                if (p.kind == Kind::signed_integer) {
                    const i32 signed_value = p.size == 1   ? static_cast<i8>(value)
                                             : p.size == 2 ? static_cast<i16>(value)
                                                           : static_cast<i32>(value);
                    printf("%d", signed_value);
                } else {
                    const EnumValues values = enum_values(p.name);
                    printf("%s", enum_text(values, value).c_str());
                }
            }
            putchar('\n');
        }
    }

    bool write_save(const std::string &path, const Save &save, bool replace) {
        std::string temporary;
        int fd = -1;
        for (int attempt = 0; attempt < 100 && fd < 0; ++attempt) {
            temporary = path + ".incomplete." + std::to_string(getpid()) + "." + std::to_string(attempt);
            fd = open(temporary.c_str(),
                      O_WRONLY | O_CREAT | O_EXCL
#ifdef _WIN32
                          | O_BINARY
#endif
                      ,
                      0600);
            if (fd < 0 && errno != EEXIST)
                return false;
        }
        if (fd < 0)
            return false;
        FILE *file = fdopen(fd, "wb");
        if (!file) {
            close(fd);
            remove(temporary.c_str());
            return false;
        }
        bool ok = fwrite(save.bytes.data(), 1, save.bytes.size(), file) == save.bytes.size();
        if (fflush(file) != 0)
            ok = false;
#ifndef _WIN32
        struct stat original;
        if (replace && stat(path.c_str(), &original) == 0 && fchmod(fd, original.st_mode & 0777) != 0)
            ok = false;
        if (fsync(fd) != 0)
            ok = false;
#endif
        if (fclose(file) != 0)
            ok = false;
        if (ok) {
#ifdef _WIN32
            ok = MoveFileExA(temporary.c_str(), path.c_str(), replace ? MOVEFILE_REPLACE_EXISTING : 0) != 0;
#else
            ok = (replace ? rename(temporary.c_str(), path.c_str()) : link(temporary.c_str(), path.c_str())) == 0;
#endif
        }
        remove(temporary.c_str());
        return ok;
    }
} // namespace

bool host_read_game_fixture(const char *path, GAMESAVE_s &game) {
    Save save;
    if (!read_save(path, save) || !validate(save) || save.payload_size != sizeof(game) ||
        read_word(save, save.payload + save.payload_size) != checksum(save)) {
        fprintf(stderr, "smoke: invalid game save or checksum: %s\n", path);
        return false;
    }
    memcpy(&game, save.bytes.data() + save.payload, sizeof(game));
    return true;
}

int host_run_save(int argc, char **argv) {
    const u32 endian = 1;
    if (*reinterpret_cast<const u8 *>(&endian) != 1)
        return fail("requires a little-endian host");
    if (argc == 1 && strcmp(argv[0], "--help") == 0) {
        usage();
        return 0;
    }
    const std::string action = argc == 0 ? "list" : argv[0];
    if (action != "list" && action != "edit" && action != "create" && action != "schema")
        return fail("unknown action: " + action);
    int first_argument = 1;
    std::string path = std::string("res/SavedGames/") + slotname(0);
    bool explicit_path = false;
    if (argc > 1 && strcmp(argv[1], "--path") == 0) {
        if (argc < 3)
            return fail("missing value for --path");
        path = argv[2];
        explicit_path = true;
        first_argument = 3;
    } else if (argc > 1 && strncmp(argv[1], "--", 2) != 0 && strchr(argv[1], '=') == nullptr) {
        path = argv[1];
        explicit_path = true;
        first_argument = 2;
    }
    std::string output = path, from, filter;
    bool keep = false, options = false, tsv = false;
    std::vector<std::string> assignments;
    if (action == "list" || action == "schema") {
        for (int i = first_argument; i < argc; ++i) {
            const std::string argument = argv[i];
            if (argument == "--tsv" && action == "schema") {
                tsv = true;
            } else if (argument == "--filter" && filter.empty()) {
                if (++i == argc)
                    return fail("missing value for --filter");
                filter = argv[i];
            } else if (argument == "--options" && action == "schema" && !explicit_path) {
                options = true;
            } else if (action == "list" && explicit_path && filter.empty() && argument.compare(0, 2, "--") != 0) {
                filter = argument;
            } else
                return fail("unexpected option for " + action + ": " + argument);
        }
    } else
        for (int i = first_argument; i < argc; ++i) {
            const std::string argument = argv[i];
            if (argument == "--keep-derived")
                keep = true;
            else if (argument == "--options" && action == "create")
                options = true;
            else if (argument == "--output" || argument == "--from" || argument == "--params") {
                if (++i == argc)
                    return fail("missing value for " + argument);
                if (argument == "--output" && action == "edit")
                    output = argv[i];
                else if (argument == "--from" && action == "create")
                    from = argv[i];
                else if (argument == "--params") {
                    FILE *file = fopen(argv[i], "rb");
                    if (!file)
                        return fail("cannot open parameter file");
                    std::string line;
                    int ch;
                    while ((ch = fgetc(file)) != EOF) {
                        if (ch == '\n') {
                            if (!line.empty() && line.back() == '\r')
                                line.pop_back();
                            if (!line.empty() && line[0] != '#')
                                assignments.push_back(line);
                            line.clear();
                        } else
                            line += static_cast<char>(ch);
                    }
                    const bool error = ferror(file);
                    fclose(file);
                    if (error)
                        return fail("cannot read parameter file");
                    if (!line.empty() && line.back() == '\r')
                        line.pop_back();
                    if (!line.empty() && line[0] != '#')
                        assignments.push_back(line);
                } else
                    return fail("option not supported for " + action + ": " + argument);
            } else if (argument.find('=') != std::string::npos)
                assignments.push_back(argument);
            else
                return fail("unknown option: " + argument);
        }
    if (options && !from.empty())
        return fail("--options and --from are mutually exclusive");
    Save save;
    if (action == "schema") {
        if (explicit_path) {
            if (!read_save(path.c_str(), save))
                return fail("cannot read supported save: " + path);
        } else {
            save.payload_size = options ? sizeof(SUPEROPTIONS_s) : sizeof(GAMESAVE_s);
            save.bytes.resize(save.payload + save.payload_size + 8);
        }
        schema(save, properties(save), filter, tsv);
        return ferror(stdout) ? 1 : 0;
    }
    if (action != "create" || !from.empty()) {
        const std::string source = action == "create" ? from : path;
        if (!read_save(source.c_str(), save))
            return fail("cannot read supported save (header/length invalid or I/O failure): " + source);
    } else {
        SaveLoad header = {};
        header.field0_0x0 = 0x52474d48;
        header.field1_0x4 = 1;
        header.size = sizeof(header);
        save.payload_size = options ? sizeof(SUPEROPTIONS_s) : sizeof(Game);
        save.bytes.resize(sizeof(header) + save.payload_size + 8);
        memcpy(save.bytes.data(), &header, sizeof(header));
        if (!options) {
            NewGame();
            memcpy(&save.bytes[save.payload], &Game, sizeof(Game));
        }
        derive(save);
    }
    const std::vector<Property> fields = properties(save);
    if (action == "list") {
        list(save, fields, filter);
        return ferror(stdout) ? 1 : 0;
    }
    for (const std::string &assignment : assignments) {
        if (!assign(save, fields, assignment))
            return fail("invalid property/value: " + assignment.substr(0, 160));
    }
    const size_t original_payload = save.payload;
    if (!validate(save) || save.payload != original_payload)
        return fail("edits would change or invalidate the save layout");
    if (!keep)
        derive(save);
    if (!write_save(output, save, action == "edit" && output == path))
        return fail("cannot publish output (existing destination or I/O failure): " + output);
    fprintf(stderr, "save: wrote %zu bytes to %s\n", save.bytes.size(), output.c_str());
    return 0;
}
