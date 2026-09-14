#pragma once

#include "gameapi/gui/apimenu.h"
#include "nu2api/nu3d/numtl.h"

enum {
    TOTAL_MENUS_COUNT = 100,
    RESERVED_MENUS_COUNT = 25,
};

extern i32 MenuLanguages;
extern i32 MenuDisableHeaders;
extern void (*drawslotsfn)(MENU *, f32);
extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern NUMTL *MenuFadeMtl;
extern i32 MenuDrawDropShadows;
