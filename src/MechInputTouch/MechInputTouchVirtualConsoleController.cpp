#include "decomp.h"
#include "MechInputTouch_types.h"
#include "globals.h"
#include "nu2api/nu3d/nutex.h"
i16 MechInputTouchVirtualConsoleController::s_textures[9];
extern "C" {
    i32 hasDoneLoadPerm;
}

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
    struct TextureLoad {
        i32 index;
        const char *name;
    };
    const TextureLoad textures[] = {
        {2, "STUFF/UIBUTTONS/UIBUTTONS_INTERACTBUTTON"},
        {3, "STUFF/UIBUTTONS/UIBUTTONS_TAGBUTTON"},
        {0, "STUFF/UIBUTTONS/UIBUTTONS_JUMPBUTTON"},
        {1, "STUFF/UIBUTTONS/UIBUTTONS_FIGHTBUTTON"},
        {4, "STUFF/UIBUTTONS/UIBUTTONS_MOVEMENTWHEEL"},
        {5, "STUFF/UIBUTTONS/UIBUTTONS_MOVEARROW"},
        {6, "STUFF/UIBUTTONS/UIBUTTONS_LOCK_LOCKED"},
        {7, "STUFF/UIBUTTONS/UIBUTTONS_LOCK_UNLOCKED"},
        {8, "STUFF/UIBUTTONS/UIBUTTONS_CROSSHAIR"},
    };
    for (u32 i = 0; i < sizeof(textures) / sizeof(textures[0]); ++i) {
        s_textures[textures[i].index] = static_cast<i16>(
            NuTexRead(const_cast<char *>(textures[i].name), &permbuffer_ptr, permbuffer_end));
    }
    hasDoneLoadPerm = 1;
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
