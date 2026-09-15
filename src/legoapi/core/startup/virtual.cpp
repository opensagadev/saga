#include "decomp.h"
#include "legoapi/legoapi_types.h"

void VirtualControlDPad::Process(float) {
    STUBBED();
}

void VirtualControlDPad::Render() {
    STUBBED();
}

VirtualControlDPad::VirtualControlDPad(NuVec2 const &, float, MechInputTouchVirtualConsoleController &) {
    STUBBED();
}

VirtualControlDPad::~VirtualControlDPad() {
}

void VirtualControlButton::Process(float) {
    STUBBED();
}

void VirtualControlButton::Render() {
    STUBBED();
}

VirtualControlButton::VirtualControlButton(NuVec2 const &, float, MechInputTouchMainController::eButtonTypes) {
    STUBBED();
}

void VirtualControlButtonMover::Process(float) {
    STUBBED();
}

VirtualControlButtonMover::VirtualControlButtonMover(MechInputTouchVirtualConsoleController &) {
    STUBBED();
}

void VirtualControlDPad_LockButton::Process(float) {
    STUBBED();
}

void VirtualControlDPad_LockButton::Render() {
    STUBBED();
}

VirtualControlDPad_LockButton::VirtualControlDPad_LockButton(VirtualControlDPad &) {
    STUBBED();
}
