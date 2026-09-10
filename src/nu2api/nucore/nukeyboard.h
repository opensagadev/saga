#pragma once
#include "nu2api/nucore/common.h"

#ifdef __cplusplus
extern "C" {
#endif
    void NuKeyboardRead(void);
    i32 NuKeyboard(i32 key);
    i32 NuKeyboard_db(i32 key);
    i32 NuKey_last(void);
    i32 NuKey_current(void);
    i32 NuKeyGet(u32 *modifiers);
    i32 NuKeyToAscii(u32 key, i32 shifted);
    void NuKeyFlush(void);
    void NuKey_simple(void);
#ifdef __cplusplus
}
#endif
