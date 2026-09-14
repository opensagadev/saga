#pragma once

#include "decomp.h"

extern "C" {
    void aieditor_ClearAllPathCnxTypes(void);
    void aieditor_RegisterPathCnxType(const char *name, u32 connection_flag, void *context, u32 flags);
    void aieditor_RegisterDefaultPathCnxTypes(void);
}
