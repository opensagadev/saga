#pragma once

// Types shared by legoapi/gizmo/gizmos/gizmos_gizactions.cpp and legoapi/ai/core/aisysall.cpp.

namespace {
    struct GizmosAIRegistryCallbacks {
        GizmosAIRegistryCallbacks() {
            lego_aiactiondefs[LEGO_AI_ACTION_RELEASE_LOCATOR].eval_fn = Action_ReleaseLocator;
            lego_aiactiondefs[LEGO_AI_ACTION_ASSIGN_LOCATOR].eval_fn = Action_AssignLocatorInSet;
            lego_aiactiondefs[LEGO_AI_ACTION_GET_LOCATOR_FROM_SET].eval_fn = Action_GetLocatorFromSet;
        }
    };
}
