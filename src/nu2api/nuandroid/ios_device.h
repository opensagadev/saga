#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
extern i32 g_bgloadAsFastAsPossibleHack;
extern "C" {
    extern i32 g_languageIndex;
    i32 NuIOS_GetDeviceLanguage(void);
    i32 NuIOS_HardwareSupportsRetina(void);
    i32 NuIOS_IsLowestEndDevice(void);
    i32 NuIOS_IsMidRangeDevice(void);
    i32 NuIOS_IsSmallScreen(void);
    i64 getCurrentTime(void);
    void NuIOS_DeallocateSystemRenderbuffer(GLuint renderbuffer);
}
