#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct MENU_s;
struct MISSIONSYS_s;
void EndMissionsMenu();
void InitMission(MISSIONSYS_s *, i32);
void MenuDrawEndMission(MENU_s *menu);
void MenuDrawMissions(MENU_s *menu);
void MenuInitMissions(MENU_s *menu);
void MenuUpdateEndMission(MENU_s *menu);
void MenuUpdateMissions(MENU_s *menu);
