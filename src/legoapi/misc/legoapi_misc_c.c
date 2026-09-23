#include "decomp.h"

// The text-picker commit callbacks live in this original C translation unit.
// Keep their ABI layouts here instead of pulling C++ editor structs into C.
typedef struct eduimenu_s eduimenu_s;
typedef struct eduiitem_s eduiitem_s;
extern int NuStrCpy(char *destination, const char *source);
extern int eduiMenuDetach(eduimenu_s *menu);
extern void eduiMenuDestroy(eduimenu_s *menu);
extern void eduiSetActiveMenu(eduimenu_s *menu);

__attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                        unsigned int buttons) {
    void *original = *(void **)((char *)item + 0x0c);
    void (*callback)(void *, void *, unsigned int) =
        *(void (**)(void *, void *, unsigned int))((char *)original + 0x15c);
    void *parent = *(void **)((char *)menu + 0x40);

    NuStrCpy((char *)original + 0x4c, (char *)item + 0x4c);
    if (callback)
        callback(parent, original, buttons);
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}

__attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons) {
    void *original = *(void **)((char *)item + 0x0c);
    void (*callback)(void *, void *, unsigned int) =
        *(void (**)(void *, void *, unsigned int))((char *)original + 0x78);
    void *parent = *(void **)((char *)menu + 0x40);

    NuStrCpy(*(char **)((char *)original + 0x64), (char *)item + 0x4c);
    if (callback)
        callback(parent, original, buttons);
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    if (parent)
        eduiSetActiveMenu(parent);
}

// getCurrentTime is transcribed in nu2api/nuandroid/ios_graphics.cpp (original 0xe3450).
