#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edfile.h"

#include "decomp.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nugscn.h"

// Unimplemented editor entry points not yet placed in their original owners.

extern "C" {

    void edbitsRegisterBaseScene(NUGSCN *scene) {
        STUBBED();
        (void)scene;
    }

    void edanimRegisterBaseScene(NUGSCN *scene) {
        STUBBED();
        (void)scene;
    }

    void edppStartPage(i32 page) {
        STUBBED();
        (void)page;
    }

    i32 edbriLoadPage(char *path, void *gscn) {
        STUBBED();
        (void)path;
        (void)gscn;
        return -1;
    }

} // extern "C"
