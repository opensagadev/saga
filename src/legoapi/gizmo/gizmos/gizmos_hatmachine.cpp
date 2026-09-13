#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"

void HatMachine_MoveCode(WORLDINFO_s *, GameObject_s *, i32) {
    STUBBED();
}

i32 HatMachine_BeingUsed(HATMACHINE_s *hat_machine) {
    return hat_machine->state_bit0;
}

void HatMachine_FindNearest(WORLDINFO_s *, nuvec_s *, GameObject_s *, float *) {
    STUBBED();
}

void HatMachines_InitTerrain(WORLDINFO_s *) {
    STUBBED();
}

void HATMACHINE_s::ClearMechObjectInterface() {
    delete mech_object_interface;
}

MechObjectInterface *HATMACHINE_s::GetMechObjectInterface() {
    if (mech_object_interface == NULL) {
        new HatMachineObjectInterface(*this);
    }
    return mech_object_interface;
}
