#pragma once

#include "editor/aieditor_settings.h"
#include "editor/aieditor_state.h"
#include "editor/edpath_types.h"
#include "gameapi/ai/aisys/aisys.h"

struct nupad_s;
struct eduimenu_s;

struct EDAIPATHCHECK_s {
    i32 on_path;
    EDAIPATH_s *path;
    EDAIPATHNODE_s *first;
    EDAIPATHNODE_s *second;
    f32 fraction;
    f32 width;
    i32 angle;
};
DECOMP_ASSERT(sizeof(EDAIPATHCHECK_s) == 0x1c, "editor path check size");
DECOMP_ASSERT(offsetof(EDAIPATHCHECK_s, fraction) == 0x10, "editor path check fraction offset");
DECOMP_ASSERT(offsetof(EDAIPATHCHECK_s, angle) == 0x18, "editor path check angle offset");

typedef void AIEDITORPATHNODECALLBACK(EDAIPATHNODE_s *node);
extern "C" {
    extern AIEDITORPATHNODECALLBACK *AIPathNodeDeletedFn;
    void InitFn_AIPathNodeDeleted(AIEDITORPATHNODECALLBACK *function);
    void pathEditor_OnPathCheck(nuvec_s *point, EDAIPATHCHECK_s *result, EDAIPATH_s *path, f32 tolerance);
    void pathEditor_QuickOnPathCheck(nuvec_s *point, EDAIPATHCHECK_s *previous, EDAIPATHCHECK_s *result);
    void pathEditorSaveData(AIPATHSYS_s *system);
    AIPATHSYS_s *pathEditorCreateData(VARIPTR *cursor, VARIPTR *end, void **scratch_cursor, void **scratch_end);
}

eduimenu_s *pathEditor_Process(nupad_s *pad);
void pathEditorDrawConnectionInfo(nuvec_s *position, float scale, nuvec_s *direction, u32 flags, i32 mode);
