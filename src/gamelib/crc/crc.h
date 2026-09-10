#pragma once

#include "nu2api/nucore/common.h"

#ifdef __cplusplus
extern "C" {
#endif

    void CRC_Init(VARIPTR *buffer_start);
    u32 CRC_Process(const void *data, u32 size);
    u32 CRC_ProcessString(const char *str);
    u32 CRC_ProcessStringN(const char *str, u32 size);
    u32 CRC_ProcessStringIgnoreCase(const char *str);
    u32 CRC_ProcessStringNIgnoreCase(const char *str, u32 size);

#ifdef __cplusplus
}
#endif
