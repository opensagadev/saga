#pragma once

#include "nu2api/nucore/nugcutscene.h"

extern "C" {
    struct CutSceneCleanUpEntry {
        void *handle;
        NUGCUTSCENE_s *cutscene;
        f32 accumulated_duration;
        u8 flags;
        u8 padding[3];
    };

    extern CutSceneCleanUpEntry *DefragCutSceneList;
    extern CutSceneCleanUpEntry *DefragCutSceneListBase;
    extern void *DefragCutSceneBaseMem;
    extern void *DefragCutSceneEndMem;
    extern void *DefragInstBaseMem;
    extern void *DefragInstEndMem;
    extern instNUGCUTSCENE_s *(*DefragGetInstFn)(void *);
    extern void *(*DefragCreateInstFn)(void *, NUGCUTSCENE_s *, VARIPTR *);
    extern void (*DefragInstDestroyedFn)(void *);
    extern i32 DefragCutSceneListSize;
}

DECOMP_ASSERT(sizeof(CutSceneCleanUpEntry) == 0x10, "cutscene cleanup entry size");
