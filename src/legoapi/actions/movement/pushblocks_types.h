#pragma once

// Types shared by legoapi/actions/movement/pushblocks.cpp and legoapi/actions/movement/climbing.cpp.

enum PushBlockCompletionFlags {
    PUSH_BLOCK_FIRST_OUTPUT_FLAG = 1 << 3,
    PUSH_BLOCK_ANY_OUTPUT_MASK = 0x7f8,
};
