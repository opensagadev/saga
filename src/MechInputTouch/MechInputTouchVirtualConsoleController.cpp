#include "decomp.h"
#include "MechInputTouch_types.h"
i16 MechInputTouchVirtualConsoleController::s_textures[9];

float MechInputTouchVirtualConsoleController::s_defaultDPadPosX = -0.72f;
float MechInputTouchVirtualConsoleController::s_defaultDPadPosY = -0.65f;
float MechInputTouchVirtualConsoleController::s_defaultButtonsPosX = 0.77f;
float MechInputTouchVirtualConsoleController::s_defaultButtonsPosY = -0.62f;
float MechInputTouchVirtualConsoleController::s_defaultDPadPosX_SmallScreen = -0.62f;
float MechInputTouchVirtualConsoleController::s_defaultDPadPosY_SmallScreen = -0.49f;
float MechInputTouchVirtualConsoleController::s_defaultButtonsPosX_SmallScreen = 0.62f;
float MechInputTouchVirtualConsoleController::s_defaultButtonsPosY_SmallScreen = -0.49f;

void MechInputTouchVirtualConsoleController::Activate() {
    STUBBED();
}

void MechInputTouchVirtualConsoleController::Deactivate() {
    STUBBED();
}

void MechInputTouchVirtualConsoleController::LoadPerm() {
    STUBBED();
}

MechInputTouchVirtualConsoleController::MechInputTouchVirtualConsoleController(i32) {
    STUBBED();
}

void MechInputTouchVirtualConsoleController::OnDown(GameObject_s &, TouchHolder &) {
    STUBBED();
}

void MechInputTouchVirtualConsoleController::OnRelease(GameObject_s &, TouchHolder &) {
    STUBBED();
}

void MechInputTouchVirtualConsoleController::ProcessDragMovement(GameObject_s &) {
    STUBBED();
}

void MechInputTouchVirtualConsoleController::ResetButtonPositionsToDefault() {
    STUBBED();
}

void MechInputTouchVirtualConsoleController::ShouldBeActive() {
    STUBBED();
}

void MechInputTouchVirtualConsoleController::Update(NuInputTouchData const *) {
    STUBBED();
}

void MechInputTouchVirtualConsoleController::UpdateButtonPositions() {
    STUBBED();
}

void MechInputTouchVirtualConsoleController::UpdateDPadPos() {
    STUBBED();
}

MechInputTouchVirtualConsoleController::~MechInputTouchVirtualConsoleController() {
}
