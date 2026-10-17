#pragma once

// Types shared by legoapi/menus/core/panel.cpp and legoapi/gizmos/fx/gizmopickups.cpp.

namespace {
    enum PANEL_BLOCKING_MENU {
        PANEL_MENU_EPISODE_I = 20,
        PANEL_MENU_EPISODE_II = 21,
        PANEL_MENU_EPISODE_III = 22,
        PANEL_MENU_EPISODE_IV = 23,
        PANEL_MENU_SAVE = 25,
        PANEL_MENU_LOAD = 26,
    };
}

enum COIN_TOTAL_SOURCE { COIN_TOTAL_SAVED_GAME, COIN_TOTAL_SUPER_STORY, COIN_TOTAL_BONUS };
