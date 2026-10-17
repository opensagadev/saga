#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct ARCADE_LEVEL_s;
struct LEVELDATA_s;
struct MENU_s;
struct nugspline_s;
bool HubCustomiserUnlocked();
bool HubMinikitViewerUnlocked();
bool HubShopUnlocked();
extern ARCADE_LEVEL_s ArcadeLevel[12];
extern f32 hub_jabbaawake;
extern f32 selectmodetime;
extern i32 freeplay_selected[2];
extern i32 freeplaymode;
extern i32 hub_forceshopsave;
extern i32 hub_freeplay_area;
extern i32 hub_new_level;
extern i32 hub_selectmode;
extern i32 selectmodemode;
extern nugspline_s *hub_minikitviewer_camspl;
extern u8 hub_custodians_finished_loading;
i32 Hub_InMenu();
i32 Hub_PanelBusy();
void Hub_ActivateDoorMenu(LEVELDATA_s **level);
void Hub_ClearStats();
void Hub_DrawAreaStats(f32, i32, i32);
void Hub_DrawFreePlaySelect();
void Hub_InitFreePlaySelect(i32, i32, i32);
void Hub_ResetPanel();
void Hub_UpdateFreePlaySelect();
void Hub_UpdateKit(void);
void MenuDrawClips(MENU_s *menu);
void MenuDrawFreePlay(MENU_s *menu);
void MenuInitClips(MENU_s *menu);
void MenuInitFreePlay(MENU_s *menu);
void MenuUpdateClips(MENU_s *menu);
void MenuUpdateFreePlay(MENU_s *menu);
void WipeBackToHub();
