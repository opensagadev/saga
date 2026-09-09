#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edfile.h"

#include "decomp.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nugscn.h"

// Editor subsystem stubs — all symbols with the "ed" prefix belong here.
// These are stubbed because the editor is not being decompiled.

extern "C" {

    void edbitsRegisterBaseScene(NUGSCN *scene) {
        (void)scene;
    }

    void edanimRegisterBaseScene(NUGSCN *scene) {
        (void)scene;
    }

    void edbitsRegisterBaseTerrain(void *terrain) {
        (void)terrain;
    }

    void edppStartPage(i32 page) {
        (void)page;
    }

    void edbriStartPage(i32 page) {
        (void)page;
    }

    void edgraStartPage(i8 page) {
        (void)page;
    }

    i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end) {
        (void)path;
        (void)gscn;
        (void)terrain;
        (void)buf;
        (void)buf_end;
        return -1;
    }

    i32 edbriLoadPage(char *path, void *gscn) {
        (void)path;
        (void)gscn;
        return -1;
    }

} // extern "C"
