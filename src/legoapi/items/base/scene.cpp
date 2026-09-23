#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nuspecial.h"

#if defined(__SSE__) && !defined(__EMSCRIPTEN__)
#include <xmmintrin.h>
#endif

inline void SceneInstance::operator delete(void *pointer) {
    theMemoryManager.FreePool(pointer, sizeof(SceneInstance));
}

char const *SceneInstance::GetName() const {
    return name.data != NULL ? name.data + 1 : NULL;
}

void SceneInstance::SetName(char const *value) {
    name.Set(value);
}

VuVec const *SceneInstance::GetCurrentPosition() const {
    return reinterpret_cast<VuVec const *>(&current_transform.m30);
}

VuMtx const *SceneInstance::GetCurrentTransform() const {
    return reinterpret_cast<VuMtx const *>(&current_transform);
}

VuVec const *SceneInstance::GetInitialPosition() const {
    return reinterpret_cast<VuVec const *>(&initial_transform.m30);
}

VuMtx const *SceneInstance::GetInitialTransform() const {
    return reinterpret_cast<VuMtx const *>(&initial_transform);
}

int SceneInstance::GetVisibility() const {
    return visibility;
}

void SceneInstance::Render(VuMtx const *) const {
    NuSpecialDrawAt(const_cast<nuhspecial_s *>(&special), const_cast<NUMTX *>(&current_transform));
}

SceneInstance::SceneInstance() : next(NULL), previous(NULL) {
    editor_owned = 1;
    scene_id = static_cast<i16>(theSceneObjectHelper.scene_id);
    NuMtxSetIdentity(&initial_transform);
    NuMtxSetIdentity(&current_transform);
    visibility = 1;
}

void SceneInstance::SetCurrentPosition(VuVec const *position) {
#if defined(__SSE__) && !defined(__EMSCRIPTEN__)
    __m128 value = _mm_setzero_ps();
    value = _mm_loadl_pi(value, reinterpret_cast<__m64 const *>(position));
    value = _mm_loadh_pi(value, reinterpret_cast<__m64 const *>(reinterpret_cast<u8 const *>(position) + 8));
    _mm_storel_pi(reinterpret_cast<__m64 *>(&current_transform.m30), value);
    _mm_storeh_pi(reinterpret_cast<__m64 *>(&current_transform.m32), value);
#else
    *reinterpret_cast<VuVec *>(&current_transform.m30) = *position;
#endif
}

void SceneInstance::SetCurrentTransform(VuMtx const *transform) {
    current_transform = *reinterpret_cast<NUMTX const *>(transform);
}

void SceneInstance::SetInitialPosition(VuVec const *position) {
#if defined(__SSE__) && !defined(__EMSCRIPTEN__)
    __m128 value = _mm_setzero_ps();
    value = _mm_loadl_pi(value, reinterpret_cast<__m64 const *>(position));
    value = _mm_loadh_pi(value, reinterpret_cast<__m64 const *>(reinterpret_cast<u8 const *>(position) + 8));
    _mm_storel_pi(reinterpret_cast<__m64 *>(&current_transform.m30), value);
    _mm_storeh_pi(reinterpret_cast<__m64 *>(&current_transform.m32), value);
#else
    *reinterpret_cast<VuVec *>(&current_transform.m30) = *position;
#endif
}

void SceneInstance::SetInitialTransform(VuMtx const *transform) {
    initial_transform = *reinterpret_cast<NUMTX const *>(transform);
}

void SceneInstance::SetVisibility(i32 visible) {
    visibility = visible;
}
