#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/items/base/apiobject.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Existing production declaration; no test-owned implementation or CInfo provider.
i32 CannotKill(GameObject_s *object);

namespace {
    void require(bool condition, const char *message) {
        if (!condition) {
            std::fprintf(stderr, "CannotKill regression: %s\n", message);
            std::exit(1);
        }
    }
} // namespace

int main() {
    static_assert(sizeof(void *) == 4, "Canonical Linux i686 diagnostic only");
    static_assert(sizeof(CHARACTER_CONTEXT_INFO_s) == 16, "Context record stride");
    static_assert(offsetof(GAMECHARACTERDATA, field_0x94) == 0x94, "Runtime flags offset");
    static_assert(offsetof(GameObject_s, character_context) == 0x7a5, "Signed context offset");

    // BigJump is real row32 in move.cpp; CInfo points at row1 (NoContext is -1).
    const i8 contexts[] = {CHARACTER_CONTEXT_NONE, CHARACTER_CONTEXT_JUMP, 31};
    const char *names[] = {"NoContext", "Jump", "BigJump"};
    const u32 context_flags[] = {0x00001000, 0x01000000, 0x02400111};
    require(CInfo != NULL, "actual context provider must be linked");
    for (int c = 0; c < 3; ++c) {
        require(std::strcmp(CInfo[contexts[c]].name, names[c]) == 0, "actual context row identity");
        require(CInfo[contexts[c]].flags == context_flags[c], "actual context row flags");
    }

    const u32 runtime_flags[] = {0, 0x800, 1, 0xfffff7ffu, 0xffffffffu};
    const int runtime_classes[] = {0, 1, 0, 0, 1};
    const i8 slots[] = {-128, -1, 0, 1, 127};
    // Independent finite outcome table: context row x runtime class x slot.
    const i32 expected[3][2][5] = {
        {{0, 0, 0, 0, 0}, {0, 1, 0, 0, 0}},
        {{0, 0, 0, 0, 0}, {0, 1, 0, 0, 0}},
        {{1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}},
    };

    GameObject_s object = {};
    characterdata_s character = {};
    GAMECHARACTERDATA runtime = {};
    object.apiobj.character_data = &character;
    character.game_character = &runtime;
    int cases = 0;
    for (int c = 0; c < 3; ++c) {
        object.character_context = contexts[c];
        for (int r = 0; r < 5; ++r) {
            runtime.field_0x94 = runtime_flags[r];
            for (int s = 0; s < 5; ++s) {
                object.apiobj.field_0x27c = slots[s];
                require(CannotKill(&object) == expected[c][runtime_classes[r]][s], "finite truth-table result");
                require(object.character_context == contexts[c] && object.apiobj.field_0x27c == slots[s] &&
                            object.apiobj.character_data == &character && character.game_character == &runtime &&
                            runtime.field_0x94 == runtime_flags[r] && CInfo[contexts[c]].flags == context_flags[c],
                        "predicate must preserve its actual inputs");
                ++cases;
            }
        }
    }

    // Deliberately admitted test-only nulls: real BigJump context must short-circuit.
    // This is not evidence that shipped character assets publish null data.
    object.character_context = 31;
    object.apiobj.character_data = NULL;
    require(CannotKill(&object) == 1 && object.apiobj.character_data == NULL, "context gate before character load");
    ++cases;
    object.apiobj.character_data = &character;
    character.game_character = NULL;
    require(CannotKill(&object) == 1 && character.game_character == NULL, "context gate before runtime load");
    ++cases;

    require(cases == 77, "bounded case inventory");
    std::printf("CannotKill regression: PASS (%d cases)\n", cases);
    return 0;
}
