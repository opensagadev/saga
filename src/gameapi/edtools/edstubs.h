#pragma once

#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nucore/common.h"

// Editor subsystem stubs — all symbols with the "ed" prefix.
// Definitions are in edstubs.cpp.

#ifdef __cplusplus
extern "C" {
#endif

    extern i32 edbits_editmode;
    extern i32 *edbits_editor_enabled;
    void edbitsRegisterEditMode(i32 mode);
    void edbitsRegisterEditorEnabledFlag(i32 *enabled);
    void edbitsRegisterBaseScene(NUGSCN *scene);
    void edanimRegisterBaseScene(NUGSCN *scene);
    i32 edanimLoadPage(char *path, NUGSCN *scene);
    i32 edanimLookupSpecial(char *name, NUGSCN *scene);
    NUVEC *edmainQueryLocVec(void);
    void edmainRegisterLocVec(NUVEC *position);
    void edbitsRegisterBaseTerrain(void *terrain);
    void edppStartPage(i32 page);
    void edppStopPage(i32 page);
    void edppClearPage(i8 page);
    void edpartClearPage(i8 page);
    void edgraClearPage(i8 page);
    void edbriClearPage(i8 page);
    void edanimClearPage(i32 page);
    void edanimStartPage(i32 page);
    void edanimStopPage(i32 page);
    void edpartStartPage(i8 page);
    void edpartStopPage(i8 page);
    void edbriStartPage(i32 page);
    void edgraStartPage(i8 page);
    i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
    i32 edbriLoadPage(char *path, void *gscn);
    void edpartSetParticlePage(i32 page);
    i32 edpartLoadPage(char *path, i32 param, void *gscn);
    void edbitsRegisterThingsScene(NUGSCN *scene);
    void edgraClumpsReset(void);
    void edanimParamReset(void);

#ifdef __cplusplus
}

struct nugspline_s *edSpline_SplineFind(NUGSCN *scene, char *name);
#endif
