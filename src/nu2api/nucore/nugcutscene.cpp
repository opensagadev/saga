#include "nu2api/nucore/nugcutscene.h"

// Callback storage is declared in reverse linked-BSS order because the
// original GCC 4.7 -O3 object emits these definitions in reverse source order.
extern "C" {
    NUGCUTSCENEGETHGOBJFN NuCutSceneGetHGObj = NULL;
    NUGCUTSCENECHARACTERPROCESSFN NuCutSceneCharacterProcess = NULL;
    NUGCUTSCENECHARACTERRENDERFN NuCutSceneCharacterRender = NULL;
    NUGCUTSCENECHARACTERRELEASEFN NuCutSceneCharacterRelease = NULL;
    NUGCUTSCENECHARACTEREVALFN NuCutSceneCharacterEval = NULL;
    NUGCUTSCENEFINDCHARACTERSFN NuCutSceneFindCharacters = NULL;
    void (*NuCutSceneDestroyCharacters)(NUGCUTSCENE_s *) = NULL;
    NUGCUTSCENERESETCHARACTERSFN NuCutSceneResetCharactersFn = NULL;
    NUGCUTSCENECHARACTERCREATEDATAFN NuCutSceneCharacterCreateData = NULL;
    NUGCUTSCENECHARACTERDESTROYDATAFN NuCutSceneCharacterDestroyData = NULL;
    NUGCUTSCENERIGIDCOLLISIONCHECKFN NuCutSceneRigidCollisionCheck = NULL;
    NUGCUTSCENERIGIDPOSTRENDERFN NuCutSceneRigidPostRender = NULL;
    NUGCUTSCENESFXFIXUPFN NuCutSceneSFXFixUp = NULL;
    NUGCUTSCENESFXUPDATEFN NuCutSceneSFXUpdate = NULL;
    NUGCUTSCENEREQUESTSFXFN NuCutSceneRequestSFX = NULL;

    void NuSetGetHGObjFromIndxFn(NUGCUTSCENEGETHGOBJFN function) {
        NuCutSceneGetHGObj = function;
    }

    void NuSetCutSceneCharacterProcessFn(NUGCUTSCENECHARACTERPROCESSFN function) {
        NuCutSceneCharacterProcess = function;
    }

    void NuSetCutSceneCharacterRenderFn(NUGCUTSCENECHARACTERRENDERFN function) {
        NuCutSceneCharacterRender = function;
    }

    void NuSetCutSceneCharacterEvalFn(NUGCUTSCENECHARACTEREVALFN function) {
        NuCutSceneCharacterEval = function;
    }

    void NuSetCutSceneCharacterReleaseFn(NUGCUTSCENECHARACTERRELEASEFN function) {
        NuCutSceneCharacterRelease = function;
    }

    void NuSetCutSceneFindCharactersFn(NUGCUTSCENEFINDCHARACTERSFN function) {
        NuCutSceneFindCharacters = function;
    }

    void NuSetCutSceneDestroyCharactersFn(void (*callback)(NUGCUTSCENE_s *)) {
        NuCutSceneDestroyCharacters = callback;
    }

    void NuSetCutSceneResetCharactersFn(NUGCUTSCENERESETCHARACTERSFN function) {
        NuCutSceneResetCharactersFn = function;
    }

    void NuSetCutSceneCharacterCreateDataFn(NUGCUTSCENECHARACTERCREATEDATAFN function) {
        NuCutSceneCharacterCreateData = function;
    }

    void NuSetCutSceneCharacterDestroyDataFn(NUGCUTSCENECHARACTERDESTROYDATAFN function) {
        NuCutSceneCharacterDestroyData = function;
    }

    void NuSetCutSceneRigidCollisionCheckFn(NUGCUTSCENERIGIDCOLLISIONCHECKFN callback) {
        NuCutSceneRigidCollisionCheck = callback;
    }

    void NuSetCutSceneSFXFixUpFn(NUGCUTSCENESFXFIXUPFN function) {
        NuCutSceneSFXFixUp = function;
    }

    void NuSetCutSceneSFXUpdateFn(NUGCUTSCENESFXUPDATEFN function) {
        NuCutSceneSFXUpdate = function;
    }

    void NuSetCutSceneRequestSFXFn(NUGCUTSCENEREQUESTSFXFN function) {
        NuCutSceneRequestSFX = function;
    }

    void NuSetCutSceneRigidPostRenderFn(NUGCUTSCENERIGIDPOSTRENDERFN function) {
        NuCutSceneRigidPostRender = function;
    }
} // extern "C"
