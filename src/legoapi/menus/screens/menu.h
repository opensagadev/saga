#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct MENU_s;
void MenuDrawExtras(MENU_s *menu);
void MenuDrawNewGame(MENU_s *menu);
void MenuDrawOptions(MENU_s *menu);
void MenuDrawRestoreNewGame(MENU_s *menu);
void MenuDrawSelectControls(MENU_s *menu);
void MenuEnterOptions(MENU_s *menu);
void MenuExitOptions(MENU_s *menu);
void MenuUpdateExtras(MENU_s *menu);
void MenuUpdateNewGame(MENU_s *menu);
void MenuUpdateOptions(MENU_s *menu);
void MenuUpdateRestoreNewGame(MENU_s *menu);
void MenuUpdateSelectControls(MENU_s *menu);
