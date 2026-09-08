#include "host/platform/runtime.hpp"
#include "gameapi/gui/apimenu.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "legoapi/legoapi_types.h"
#include "decomp.h"

#include <SDL3/SDL.h>

extern "C" i32 __real_MenuInMemoryCard();

extern "C" {
    void __real_edppDeleteEffect(i32);
    extern i32 edpp_nearest;
    extern edpp_particle_s edpp_ptls[512];
    extern debinftype **debtab;
    extern debkeydatatype_s *debkeydata;
    extern i32 maxdebkeys;
    extern i32 edpp_types_used;
    void DebFreeOrphansInstantly(debinftype *);
    i32 LookupDebrisEffectPageIgnore(char *, i32, i32);
    void DebFreeInstantly(i32 *);
}
void edppDetermineNearest(float);
void edppPtlDestroy(i32);

extern "C" void __wrap_edppDeleteEffect(i32 index) {
    if (edpp_nearest != -1) {
        __real_edppDeleteEffect(index);
        return;
    }

    // Original 0x368ac6 reads zero padding at edpp_ptls - 76 with selection -1.
    // Either comparison outcome leaves selection at -1. Warn instead of making
    // that invalid host read, then perform the original remaining cleanup.
    LOG_WARN("edppDeleteEffect(%d): selection is -1; skipped original out-of-bounds selection read", index);
    DebFreeOrphansInstantly(debtab[index]);
    i32 replacement = LookupDebrisEffectPageIgnore(debtab[index]->name, 1, index);
    if (replacement != -1) {
        for (i32 i = 0; i < 512; ++i) {
            if (edpp_ptls[i].effect_index == index) {
                i32 handle = edpp_ptls[i].instance_id;
                if (handle != 99999 && handle != -1)
                    debkeydata[handle].effect_index = replacement;
                edpp_ptls[i].effect_index = replacement;
            }
        }
        for (i32 i = 0; i < maxdebkeys; ++i)
            if (debkeydata[i].effect_index == index)
                debkeydata[i].effect_index = replacement;
    } else {
        for (i32 i = 0; i < 512; ++i)
            if (edpp_ptls[i].effect_index == index)
                edppPtlDestroy(i);
        for (i32 i = 0; i < maxdebkeys; ++i) {
            if (debkeydata[i].effect_index == index) {
                i32 handle = i;
                DebFreeInstantly(&handle);
            }
        }
    }
    debtab[index] = NULL;
    --edpp_types_used;
    edppDetermineNearest(1.0f);
}

extern "C" i32 __wrap_MenuInMemoryCard() {
    // After MenuReset, the reference reads MenuInfo[-1].id from zero padding
    // at 0x665a24. Reproduce its false result without an invalid host read.
    if (GameMenuLevel != -1 && GameMenu[GameMenuLevel].menu == -1) {
        return 0;
    }
    return __real_MenuInMemoryCard();
}

const char *HostPlatformVideoDriver() {
    return "x11";
}

ANativeWindow *HostPlatformNativeWindow(SDL_Window *window) {
    const SDL_PropertiesID properties = SDL_GetWindowProperties(window);
    const i32 handle = static_cast<i32>(SDL_GetNumberProperty(properties, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0));
    return reinterpret_cast<ANativeWindow *>(handle);
}
