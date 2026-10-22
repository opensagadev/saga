// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include <new>
#include <string.h>

#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nucore/nuvec.hpp"
#include "nu2api/nucore/nuthread.h"
#include "legoapi/legoapi_types.h"

#include <GLES2/gl2.h>

#include "decomp.h"
#include "nu2api/nu3d/android/nupostfilter.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/NuMainFilter.h"
#include "nu2api/nucore/NuDeferredFilter.h"
#include "nu2api/nucore/NuMotionFilter.h"
#include "nu2api/nucore/NuMotionAccumFilter.h"
#include "nu2api/nucore/NuSpeedBlurFilter.h"
#include "nu2api/nucore/NuCopyFilter.h"
#include "nu2api/nucore/NuPostFilter.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "gamelib/util/gamelib_util_types.h"
#include <float.h>
#include <math.h>
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nurendercontext_inline.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/nupostshaderparams.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include <stdlib.h>
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/NuMainFilterGen.h"
#include "nu2api/nucore/NuMotionFilterGen.h"
#include "nu2api/nucore/NuMotionAccumFilterGen.h"
#include "nu2api/nucore/NuSpeedBlurFilterGen.h"
#include "nu2api/nucore/NuDeferredFilterGen.h"
#include "nu2api/nucore/nuvuvectype.hpp"
#include "nu2api/nu3d/nupostdraw.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nurand.h"
#include <stddef.h>
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDataPortManager.h"
#include "nu2api/nucore/NuPlatform.h"
#include "nu2api/nucore/NuPostFilterGen.h"
#include "nu2api/nu3d/android/nupostshaders.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/numtx.h"
#include <stdio.h>
#include "globals.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nucore/numem.h"
#include <cfloat>
#include "nu2api/nu3d/nulgtlaser.h"
#include <stdarg.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "java/java.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nucore/nutime.h"
#include <cstddef>
#include <cstring>
#include "nu2api/nu3d/android/nurenderthread.h"
#include <pthread.h>
#include <time.h>
#include "nu2api/numath/nuvec.h"
#include "legoapi/render/fx/game_deb.h"
#include <ctype.h>
#include "gameapi/edtools/edfile.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/world/world.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/levels/podrace.h"
#include "nu2api/nu3d/nuspline.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/characters/motion/action_info.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/level.h"
#include "nu2api/numath/nuang.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/numusic/sfx.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "nu2api/nucore/numechptr.hpp"
#include "gameapi/edtools/edui.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/actions/character/speederchase.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "gameframework/saveload.h"
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "editor/edpath.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath_types.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/world/areas.h"
#include "nu2api/numusic/numusic.h"
#include <limits.h>
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "nu2api/nucore/nuvideo.h"
#include "legoapi/misc/androidbatman.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "nu2api/nu3d/nushader_plain.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include "nu2api/nusound/nusound.h"
#include "legoapi/render/fx/particles.h"
#include "nu2api/nuplatform/nuplatform.h"
#include "nu2api/nu3d/nurndr_plain_types.h"
#include "nu2api/nucore/nuhtml.h"
#include "nu2api/nuplatform/nudevicespecs.hpp"
#include "gamelib/nuwind/nuwind.h"
#include "nu2api/nu3d/android/nutex_android.h"
#include "nu2api/nu3d/android/nudlist_callbacks.h"
#include "nu2api/nucore/nulst.h"
#include "nu2api/nufile/nu2api_nufile_types.h"
#include "nu2api/nu3d/numtl_types.h"
#include "nu2api/nu3d/android/nudlist_android.h"
#include "nu2api/nu3d/nu3d_includes.h"
#include "nu2api/nucore/android/stubs_android.h"
#include "nu2api/nucore/nucore_includes.h"
#include "nu2api/numath/numath_includes.h"

// Bitmask of which post-effects were successfully initialised.  Tested in
// renderThread_processRenderScenes (nurenderthread.cpp) against the per-slot
// masks — a disabled effect has its scene slot cleared before the frame is
// dispatched:
//
//   0x04  -> scn.unknown_58   (Bloom / main filter path)
//   0x08  -> scn.unknown_ac
//   0x10  -> scn.unknown_e4   (camera motion blur)
//   0x20  -> scn.unknown_48   (deferred shading)
//   0x40  -> scn.unknown_178
//   0x80  -> scn.unknown_188  (motion blur)
//
// The enum below names the bits as they appear in the original binary.

static i32 g_effectFlags; // bss - see masks above
static i32 g_effectsRan = 0;
static u8 g_lastFrameEffect = 0;

NuDeferredFilter *deferredFilter;
static NuMainFilter *mainFilter;
static NuMotionFilter *motionFilter;
static NuMotionAccumFilter *motionAccumFilter;
static NuSpeedBlurFilter *speedBlurFilter;
static NuCopyFilter *copyFilter;
static u8 filterMem[0x800];
static nueffecttex_s *g_backBufferCopy;
u32 g_posteffect_shaderEngineFlag;

// Original proxy descriptor is 12 bytes, including two one-byte flags.

static NuDataPort<nuframebuffer_s *> portOutFramebuffer;
static NuProxyBuffer proxyColorBuffer;
static NuProxyBuffer proxyNormalBuffer;
static NuProxyBuffer proxyVelocityBuffer;
static NuProxyBuffer proxyDepthRTBuffer;
static NuProxyBuffer proxyDepthBuffer;
static NuDataPort<NuProxyAttachment *> portMainNormalBuffer;
static NuDataPort<NuProxyAttachment *> portMainDepthBuffer;
static NuProxyBuffer proxyMainNormalBuffer;
static NuProxyBuffer proxyMainDepthBuffer;
static NuDataPort<NuProxyAttachment *> portMotionVelocityBuffer;
static NuProxyBuffer proxyMotionVelocityBuffer;
static VirtualStackAllocator filterAllocator(filterMem, sizeof(filterMem));

template <typename T> static void DestroyPostFilter(T *&slot) {
    NuPostFilterGen *filter = slot;
    if (filter != NULL) {
        filter->destroyTextureResources();
        filter->destroyResources();
        slot = NULL;
    }
}

template <typename T> static T *AllocatePostFilter() {
    T *filter = reinterpret_cast<T *>(filterAllocator.cursor);
    filterAllocator.cursor += sizeof(T);
    return new (filter) T();
}

static constexpr i32 kProxyKind_Color = 0;
static constexpr i32 kProxyKind_Normal = 1;
static constexpr i32 kProxyKind_Velocity = 2;
static constexpr i32 kProxyKind_Depth = 4;

static inline void ResetProxyBuffer(NuProxyBuffer *proxy, i32 kind) {
    proxy->texture = NULL;
    proxy->kind = kind;
    proxy->enabled = true;
    proxy->resolved = false;
}

static inline void FilterEnd(NuPostFilterGen *filter) {
    if (filter == nullptr) {
        return;
    }
    filter->resetAll();
}

// ── Post-effect API ─────────────────────────────────────────────────────────

NuDataPortManager NuPostFilterGen::resourceManager;
NuDataPort<nuframebuffer_s *> NuPostFilterGen::portOutFramebuffer;
NuDataPort<NuProxyAttachment *> NuPostFilterGen::portColorBuffer;
NuDataPort<NuProxyAttachment *> NuPostFilterGen::portNormalBuffer;
NuDataPort<NuProxyAttachment *> NuPostFilterGen::portVelocityBuffer;
NuDataPort<NuProxyAttachment *> NuPostFilterGen::portDepthRTBuffer;
NuDataPort<NuProxyAttachment *> NuPostFilterGen::portDepthBuffer;

OcclusionManager g_OcclusionManager;

static const VuVec unitCube[8] = {
    {-1, -1, -1, 1}, {-1, 1, -1, 1}, {1, 1, -1, 1}, {1, -1, -1, 1},
    {-1, -1, 1, 1},  {-1, 1, 1, 1},  {1, 1, 1, 1},  {1, -1, 1, 1},
};
static const VuVec halfUnitCube[8] = {
    {-1, -1, 0, 1}, {-1, 1, 0, 1}, {1, 1, 0, 1}, {1, -1, 0, 1},
    {-1, -1, 1, 1}, {-1, 1, 1, 1}, {1, 1, 1, 1}, {1, -1, 1, 1},
};

template <typename T> struct LightObjectPool {
    struct __attribute__((aligned(16))) Slot {
        // Construction is explicit; reserving the pool must not construct every light.
        u8 storage[sizeof(T)];
    } slots[8];
    i8 occupied;
    i32 next;

    LightObjectPool() : occupied(0), next(0) {
    }

    T *allocate() {
        i32 i;
        for (i = next; i < 8; ++i) {
            if ((occupied & (1 << (i & 7))) == 0) {
                T *result = reinterpret_cast<T *>(slots[i].storage);
                occupied |= 1 << (i & 7);
                next = (i + 1) % 8;
                return result;
            }
        }
        for (i = 0; i < next; ++i) {
            if ((occupied & (1 << (i & 7))) == 0) {
                T *result = reinterpret_cast<T *>(slots[i].storage);
                occupied |= 1 << (i & 7);
                next = (i + 1) % 8;
                return result;
            }
        }
        return NULL;
    }
};
static LightObjectPool<NuDynamicLight> dynamicLightPool;
static LightObjectPool<NURNDRSTATE> rndrStatePool;
static LightObjectPool<NUDISPLAYLISTITEM> dlistItemPool;
DECOMP_ASSERT(sizeof(dynamicLightPool) == 16144, "Dynamic light pool size");
DECOMP_ASSERT(sizeof(dlistItemPool) == 144, "Dynamic light list item pool size");
DECOMP_ASSERT(sizeof(rndrStatePool) == 528, "Dynamic light render state pool size");

const NuVec NuVec::vnull(0, 0, 0);
const NuVec NuVec::vx(1, 0, 0);
const NuVec NuVec::vy(0, 1, 0);
const NuVec NuVec::vz(0, 0, 1);

NuCriticalSection criticalSection("Texture");

static inline void PostBlurSetVertexParam(nushaderprogram_s *, u32, const f32 *, i32);

static inline VuVec LightClipTransform(const VuVec &point, const NUMTX &matrix) {
    VuVec result;
    result.x = point.x * matrix.m00;
    result.x += point.y * matrix.m10;
    result.x += point.z * matrix.m20;
    result.x += matrix.m30;
    result.y = point.x * matrix.m01;
    result.y += point.y * matrix.m11;
    result.y += point.z * matrix.m21;
    result.y += matrix.m31;
    result.z = point.x * matrix.m02;
    result.z += point.y * matrix.m12;
    result.z += point.z * matrix.m22;
    result.z += matrix.m32;
    result.w = point.x * matrix.m03;
    result.w += point.y * matrix.m13;
    result.w += point.z * matrix.m23;
    result.w += matrix.m33;
    const f32 reciprocal = 1.0f / result.w;
    result.x *= reciprocal;
    result.y *= reciprocal;
    result.z *= reciprocal;
    return result;
}
static inline NUVEC LightClipEdge(const VuVec &point, const VuVec &origin) {
    NUVEC edge = {point.x - origin.x, point.y - origin.y, point.z - origin.z};
    return edge;
}
static inline void LightClipPlane(VuVec &plane, const VuVec &origin, const NUVEC &first, const NUVEC &second) {
    plane.x = first.y * second.z - first.z * second.y;
    plane.y = first.z * second.x - first.x * second.z;
    plane.z = first.x * second.y - first.y * second.x;
    NuVecNorm((NUVEC *)&plane, (NUVEC *)&plane);
    plane.w = -(plane.x * origin.x + plane.y * origin.y + plane.z * origin.z);
}

static inline NUVEC ShadowCross(const VuVec &a, const VuVec &b, const VuVec &origin) {
    NUVEC first = {b.x - origin.x, b.y - origin.y, b.z - origin.z};
    NUVEC second = {a.x - origin.x, a.y - origin.y, a.z - origin.z};
    NUVEC normal;
    normal.y = first.x * second.z - first.z * second.x;
    normal.z = second.x * first.y - first.x * second.y;
    normal.x = second.y * first.z - second.z * first.y;
    return normal;
}

static inline VuVec WarpTransformPoint(const VuVec &point, const NUMTX &matrix) {
    NUMTX copy;
    copy.m00 = matrix.m00;
    copy.m01 = matrix.m01;
    copy.m02 = matrix.m02;
    copy.m03 = matrix.m03;
    copy.m10 = matrix.m10;
    copy.m11 = matrix.m11;
    copy.m12 = matrix.m12;
    copy.m13 = matrix.m13;
    copy.m20 = matrix.m20;
    copy.m21 = matrix.m21;
    copy.m22 = matrix.m22;
    copy.m23 = matrix.m23;
    copy.m30 = matrix.m30;
    copy.m31 = matrix.m31;
    copy.m32 = matrix.m32;
    copy.m33 = matrix.m33;
    VuVec output;
    NuVecMtxTransformH(&output.xyz, const_cast<NUVEC *>(&point.xyz), &copy);
    return output;
}
static inline void WarpAccumulatePoint(const VuVec &point, const NUMTX &matrix, VuVec &output, f32 &min_z, f32 &min_y,
                                       f32 &max_z, f32 &max_y) {
    VuVec result = WarpTransformPoint(point, matrix);
    output.x = result.x;
    output.y = result.y;
    output.z = result.z;
    output.w = result.w;
    min_z = result.z < min_z ? result.z : min_z;
    if (result.y < min_y) {
        min_y = result.y;
    }
    max_z = max_z < result.z ? result.z : max_z;
    max_y = max_y < result.y ? result.y : max_y;
}
static inline void WarpProjectBounds(VuVec &point, NUVEC &translation, f32 near_plane, f32 &left, f32 &bottom,
                                     f32 &right, f32 &top) {
    NUVEC shifted;
    NuVecAdd(&shifted, &point.xyz, &translation);
    f32 x = shifted.x * near_plane / shifted.z;
    f32 y = shifted.y * near_plane / shifted.z;
    if (left > x)
        left = x;
    else if (x > right)
        right = x;
    if (bottom > y)
        bottom = y;
    else if (y > top)
        top = y;
}

static inline void SetLightCameraVector(NUVEC4 &destination, f32 x, f32 y, f32 z, f32 w) {
    destination.x = x;
    destination.y = y;
    destination.z = z;
    destination.w = w;
}

// Generic post-filter implementation included by the original nu3d owner.
// Android deferred render/reset and scalar Gauss sampling retain their nucore owner.

static f32 motionFactorPan = 0.01f;
static f32 motionFactorPull = -0.024f;
static f32 motionFactorPanClamp = 0.02f;

nuframebuffer_s *NuPostFilterGen::blurFbo, *NuPostFilterGen::copyFbo;
nueffecttex_s *NuPostFilterGen::workTex;
nushaderprogram_s *NuPostFilterGen::copyTexProgram, *NuPostFilterGen::copyTexLodProgram;
nushaderprogram_s *NuPostFilterGen::copyTexColorDepthProgram, *NuPostFilterGen::blendTexProgram;
nushaderprogram_s *NuPostFilterGen::blur5x5Program, *NuPostFilterGen::blur7x7Program;
nushaderprogram_s *NuPostFilterGen::blurGuardProgram;

static inline void PostRegisterPort(NuPostDataPort &port, NuDataPortManager &manager, const char *name) {
    if (port.index >= 0)
        --port.manager->getEntry(port.index).references;
    port.manager = &manager;
    port.index = manager.registerPort(name, NULL);
}

__attribute__((weak)) void NuPostFilterGen::reset() {
    enabled = false;
}

__attribute__((weak)) void NuPostFilterGen::resetAll() {
}

static inline void CloneLightVector(NUVEC4 &destination, const NUVEC4 &source) {
    destination.x = source.x;
    destination.y = source.y;
    destination.z = source.z;
    destination.w = source.w;
}

NUMTX NuDynamicLight::cacheCameraView;
NUMTX NuDynamicLight::cacheCameraProj;

struct nuvisiboxtree_s;
struct nuvisiboxtreenode_s;
NUMTL *OccluderSet::ms_pZOnlyMtl3D;
NUMTL *OccluderSet::ms_pZOnlyMtl2D;
NUMTL *OccluderSet::ms_pAlphaMtl2D;

namespace {
struct OccluderVertex {
    f32 x, y, z, w;
    OccluderVertex(f32 vx, f32 vy, f32 vz, f32 vw) : x(vx), y(vy), z(vz), w(vw) {
    }
};
} // namespace

// The material setup is split out of Init by GCC (retail keeps it as Init.part.100).
static __attribute__((noinline, noclone)) void OccluderSetInitMaterials() __asm__("_ZN11OccluderSet4InitEjP9variptr_uS0_.part.100");

struct nuvisiboxtreenode_s {
    NUVEC minimum;
    NUVEC maximum;
    u16 children[2];
    u16 instance_count;
    u16 first_instance;
};

struct nuvisiboxtree_s {
    u32 reserved_00;
    u32 root_count;
    i32 *roots;
    f32 *far_clip;
    u16 *instances;
    u32 reserved_14;
    nuvisiboxtreenode_s *nodes;
};

struct NuBoxTreeVisibility {
    i32 instance_count;
    u32 reserved_04;
    nuvisiboxtree_s *tree;
    u32 reserved_0c[5];
    u8 *clip_results;
    u32 reserved_24;
    u8 state;
};

DECOMP_ASSERT(sizeof(nuvisiboxtreenode_s) == 0x20, "visibility box node size");
DECOMP_ASSERT(offsetof(nuvisiboxtree_s, nodes) == 0x18, "visibility box nodes offset");
DECOMP_ASSERT(offsetof(NuBoxTreeVisibility, state) == 0x28, "box visibility state offset");

extern "C" {
    i32 do_boxtree;
}

static u32 nta_sig_old;
static u32 nta_sig_off;
static u32 nta_sig_on;
static i32 nta_iframetime;
static u16 nta_script_mask;
static nutexanimprog_s *sys_progs;
static nutexanimprog_s *parprog;
static char labtab[64][21];
static i32 labtabcnt;
static char xdeflabtab[256][21];
static i32 xdeflabtabcnt;
i32 nta_labels[64];
nutexanimlist_s ntalsysbuff[64];
nutexanimlist_s *ntal_first;
nutexanimlist_s *ntal_free;
i32 g_texAnimCriticalSection;
i32 nutexanim_usepakfile = 1;

static i32 NuTexAnimLabelIndex(char *name, char (*table)[21], i32 *count) {
    if (strlen(name) > 20)
        name[20] = '\0';
    char *labels = &table[0][0];
    for (i32 i = 0; i < *count; ++i) {
        if (NuStrICmp(labels + i * 21, name) == 0)
            return i;
    }
    NuStrCpy(table[(*count)++], name);
    return *count - 1;
}

static void pftaRepeat(nufpar_s *parser);
static void pftaRepend(nufpar_s *parser);
static void pftaTexAdj(nufpar_s *parser);
static void pftaTexAdjR(nufpar_s *parser);
static void pftaUntiltex(nufpar_s *parser);
static void pftaScriptMask(nufpar_s *parser);
static void pftaScriptname(nufpar_s *parser);
static void pftaOn(nufpar_s *parser);
static void pftaEnd(nufpar_s *parser);
static void pftaOff(nufpar_s *parser);
static void pftaRet(nufpar_s *parser);
static void pftaTex(nufpar_s *parser);
static void pftaBtex(nufpar_s *parser);
static void pftaGoto(nufpar_s *parser);
static void pftaGosub(nufpar_s *parser);
static void pftaLabel(nufpar_s *parser);
static void pftaRate(nufpar_s *parser);
static void pftaTexR(nufpar_s *parser);
static void pftaWait(nufpar_s *parser);
static void pftaXDef(nufpar_s *parser);
static void pftaXRef(nufpar_s *parser);

static NUFPCOMJMP nutexanimcomtab[] = {
    {const_cast<char *>("repeat"), pftaRepeat},
    {const_cast<char *>("repend"), pftaRepend},
    {const_cast<char *>("texadj"), pftaTexAdj},
    {const_cast<char *>("texadjr"), pftaTexAdjR},
    {const_cast<char *>("untiltex"), pftaUntiltex},
    {const_cast<char *>("scriptmask"), pftaScriptMask},
    {const_cast<char *>("scriptname"), pftaScriptname},
    {const_cast<char *>("on"), pftaOn},
    {const_cast<char *>("end"), pftaEnd},
    {const_cast<char *>("off"), pftaOff},
    {const_cast<char *>("ret"), pftaRet},
    {const_cast<char *>("tex"), pftaTex},
    {const_cast<char *>("btex"), pftaBtex},
    {const_cast<char *>("goto"), pftaGoto},
    {const_cast<char *>("gosub"), pftaGosub},
    {const_cast<char *>("label"), pftaLabel},
    {const_cast<char *>("rate"), pftaRate},
    {const_cast<char *>("texr"), pftaTexR},
    {const_cast<char *>("wait"), pftaWait},
    {const_cast<char *>("xdef"), pftaXDef},
    {const_cast<char *>("xref"), pftaXRef},
    {NULL, NULL},
};

NURAND texanim_rand;

static bool TextureCondition(i32 condition, i32 texture, i32 value) {
    switch (condition) {
        case 0:
            return texture == value;
        case 1:
            return texture < value;
        case 2:
            return texture > value;
        case 3:
            return texture <= value;
        case 4:
            return texture >= value;
        case 5:
            return texture != value;
        default:
            return false;
    }
}

static VARIPTR fstack;
static i8 fstackmem[12288];
static VARIPTR fstack_limit;
static VARIPTR fstack_top;
static NUCAMERA *cam;
static NUMTX local_inv_view_mtx;
static NUVEC world_campos;
static i16 camera_roomid;
static NUFRUSTRUM **frustra;
static i32 *nfrustra;
static i32 draw_portals;

NUPLANE cam_plane;
i16 rooms_visited[18];

static NUFRUSTRUM *allocateFrustrum(i16 plane_count, i16 room_id) {
    NUFRUSTRUM *frustum = reinterpret_cast<NUFRUSTRUM *>(ALIGN(fstack_top.addr, 16));
    frustum->planes = reinterpret_cast<NUPLANE *>(reinterpret_cast<u8 *>(frustum) + sizeof(NUFRUSTRUM));
    fstack_top.addr = reinterpret_cast<usize>(frustum->planes + plane_count);
    frustum->room_id = room_id;
    frustum->plane_count = plane_count;
    frustra[*nfrustra] = frustum;
    ++*nfrustra;
    return frustum;
}

static void transposeClipPlanes(NUFRUSTRUM *frustum);

static void transformFrustrumPlane(NUPLANE *plane) {
    NuVec4MtxTransformH(reinterpret_cast<NUVEC4 *>(plane), reinterpret_cast<NUVEC4 *>(plane), &local_inv_view_mtx);
    plane->d =
        -(local_inv_view_mtx.m30 * plane->a + local_inv_view_mtx.m31 * plane->b + local_inv_view_mtx.m32 * plane->c);
}

static __used__ NUFRUSTRUM *buildFrustrum(NUVEC *minimum, NUVEC *maximum, i16 room_id);

// IPA-SRA passes the portal's vertex pointer and count fields by reference.
static __attribute__((noinline, noclone)) void computePortalExtents(NUVEC **vertices, i16 *vertex_count,
                                                                    NUVEC *maximum, NUVEC *minimum)
    __asm__("_ZL20computePortalExtentsP10nuportal_sP7nuvec_sS2_.isra.89");

static f32 planeDistance(const NUPLANE &plane, const NUVEC &point) {
    return plane.a * point.x + plane.b * point.y + plane.c * point.z + plane.d;
}

struct NuPortalLineVertex2D {
    f32 x, y;
    f32 unused[2];
    i32 colour;
    f32 u, v;
};

// Debug view of a clipped portal: its screen rectangle, the rectangle cast onto
// the portal plane, and rays from there to the next room's walls.  Our
// roomRecursive's estimated block frequencies make this call site look never
// executed, so hot/.text keep the speed-optimised body retail has.
static __attribute__((noinline, noclone, hot, section(".text"))) void drawClippedPortal(NUPORTAL *portal, NUPLANE **planes, u8 *plane_count,
                                                                 NUVEC *minimum, NUVEC *maximum)
    __asm__("_ZL17drawClippedPortalP10nuportal_sP8nuroom_sP7nuvec_sS4_i.isra.95");

static NUFRUSTRUM *buildPortalFrustrum(NUPORTAL *portal, i16 room_id) {
    NUFRUSTRUM *frustum = allocateFrustrum(portal->vertex_count, room_id);
    const f32 camera_side = planeDistance(portal->plane, world_campos);
    NUVEC camera_position = {
        local_inv_view_mtx.m30,
        local_inv_view_mtx.m31,
        local_inv_view_mtx.m32,
    };
    for (i32 i = 0; i < portal->vertex_count; ++i) {
        NUVEC first;
        NUVEC second;
        NUVEC normal;
        NuVecSub(&first, &camera_position, &portal->vertices[i]);
        NuVecSub(&second, &camera_position, &portal->vertices[(i + 1) % portal->vertex_count]);
        NuVecCross(&normal, &first, &second);
        NuVecNorm(&normal, &normal);
        if (camera_side >= 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        frustum->planes[i].a = normal.x;
        frustum->planes[i].b = normal.y;
        frustum->planes[i].c = normal.z;
        frustum->planes[i].d =
            -(normal.x * camera_position.x + normal.y * camera_position.y + normal.z * camera_position.z);
    }
    computePortalExtents(&portal->vertices, &portal->vertex_count, &frustum->maximum, &frustum->minimum);
    transposeClipPlanes(frustum);
    return frustum;
}

static NUFRUSTRUM *copyFrustrum(NUFRUSTRUM *source, i16 room_id) {
    NUFRUSTRUM *copy = allocateFrustrum(source->plane_count, room_id);
    *copy = *source;
    copy->room_id = room_id;
    return copy;
}

static void roomRecursive(NUGSCN *scene, NUFRUSTRUM *frustum, i16 room_id, i16 previous_room, i32 depth);
static NUVEC *override_campos;
static NUPLANE near_clip_plane;

// Constants

// ──────────────────────────────────────────────────────────────────────────────
// Constants
// ──────────────────────────────────────────────────────────────────────────────

static constexpr usize kItemSize = 0x10;
static constexpr u8 kItemId_Cnt = 0;
static constexpr u8 kItemId_Next = 1;
static constexpr u8 kItemId_Call = 3;
static constexpr u8 kItemId_Ret = 4;
static constexpr u8 kItemType_Mtl = 0x80;
static constexpr u8 kItemType_Terminator = 0x84;
static constexpr u8 kItemType_Nop = 0x87;
static constexpr u8 kItemType_Link = 0x8d;

extern "C" {
    VARIPTR *display_list_buffer = nullptr;
    VARIPTR *display_list_buffer_end = nullptr;

    // Manager lives in original bss @0x11a0080 (0x604 bytes).
    NUDLIST_MANAGER global_dlist_manager = {0};
}

// Capture debug state (bss @0x11a0070 / @0x11a0068).
extern "C" {
    i32 do_capture;
    i32 capture_fh;
    i32 nudlist_debug_level = 2;
}

// The original retains reads of this local BSS control despite having no
// program-side setter. Preserve its externally observable debug accesses.
static volatile i32 capture_dlist;

// Per-material reset

static void ResetRenderStateCache(nurndrstate_s *st) {
    st->mtl = nullptr;
    st->tex_id = -1;
    st->global_id = -1;
    st->lights_id = -1;
    st->camera_id = -1;
    st->fog_id = -1;
    st->konst_id = -1;
    st->reflection_id = -1;
}

// Clip bitsets hold two bits per clip object, followed by a sentinel block.
static i32 ClipUsedBlockCount(i32 nclip) {
    return (((nclip + 7) / 8) << 1) / 16 + 1;
}

static inline __attribute__((always_inline)) void ResetSceneBeforeFrame(nudisplayscene_s *scene, bool gated) {
    if (gated && (scene->flags & 6) == 0) {
        return;
    }

    if (scene->nmtls != 0) {
        NuMemSet128(scene->mtl_used[NuDisplaySceneCurrentBuffer(scene)], 0, ((scene->nmtls + 7) >> 7) + 1);
    }

    if (gated ? __builtin_expect(scene->nclip_objects != 0, 0) : scene->nclip_objects != 0) {
        i32 blocks = ClipUsedBlockCount(scene->nclip_objects);
        NuMemSet128(scene->clip_used[NuDisplaySceneCurrentBuffer(scene)], 0, blocks);
        if (!gated) {
            // Ungated variant re-initialises the back buffer to all-ones.
            NuMemSet128(scene->clip_used[NuDisplaySceneCurrentBuffer(scene) ^ 1], 0xff, blocks);
        }
    }

    for (u32 i = 0; i < scene->nmtls; ++i) {
        NUDISPLAYLIST *dl = scene->dlist_mtls[i];
        dl->mtl_last = dl->first;
        if (!gated) {
            dl->scene_buffer = 0;
        } else {
            u32 new_buf = (dl->scene_buffer == 0) ? 1 : 0;
            dl->scene_buffer = new_buf;
        }
        dl->scene_next = dl->scene_first[dl->scene_buffer];
        ResetRenderStateCache(dl->state);
    }
    scene->flags &= 0xf1;
}

// Retail keeps DisplayListBeforeFrame only as its two constant-propagated
// clones: .173 resets every buffer, .174 is the gated per-frame flip.
#if defined(__i386__)
#define DLIST_BEFORE_FRAME_ATTR __attribute__((noinline, noclone, regparm(1)))
#else
#define DLIST_BEFORE_FRAME_ATTR __attribute__((noinline, noclone))
#endif
static DLIST_BEFORE_FRAME_ATTR void DisplayListBeforeFrameReset(nudisplayscene_s *scene)
    __asm__("_ZL22DisplayListBeforeFrameP16nudisplayscene_si.constprop.173");
static DLIST_BEFORE_FRAME_ATTR void DisplayListBeforeFrameFlip(nudisplayscene_s *scene)
    __asm__("_ZL22DisplayListBeforeFrameP16nudisplayscene_si.constprop.174");

template <typename T> static T *CloneSceneAllocate(VARIPTR *buffer, usize count, usize alignment = 4) {
    buffer->addr = (buffer->addr + alignment - 1) & ~(alignment - 1);
    T *result = static_cast<T *>(buffer->void_ptr);
    buffer->addr += sizeof(T) * count;
    return result;
}

// NUMTL layout helpers — these fields are at fixed offsets in the original
// binary (verified against NuDisplayListSwapBuffersEndFrame visibility test).
#define MTL_BLEND_FLAG(mtl) (*(const u32 *)((const u8 *)(mtl) + 0xb0))
#define MTL_BLEND_OP2(mtl) (*(const u8 *)((const u8 *)(mtl) + 0xf8))
#define MTL_ATTRIB_DWORD1(mtl) (*(const u32 *)((const u8 *)(mtl) + 0x44))

// Dynamic material link / unlink

// ──────────────────────────────────────────────────────────────────────────────
// Dynamic material link / unlink
// ──────────────────────────────────────────────────────────────────────────────

static i32 MtlSortKey(const NUMTL *mtl) {
    // Original: ((char)(u16)mtl[0] >> 4) * 0x20000 + sort_pri
    return mtl->renderplane * 0x20000 + mtl->sort_pri;
}

static inline i32 DynMtlFind(const NUMTL *mtl) {
    NUDLIST_MANAGER *mgr = &global_dlist_manager;
    i32 idx = -1;
    for (i32 i = 0; i < static_cast<i32>(mgr->dyn_mtl_dlist.nmtls); ++i) {
        if (mgr->dyn_mtl_dlist.mtls[i] == mtl) {
            idx = i;
            break;
        }
    }
    return idx;
}

// Temporary C-linkage compatibility surface for unresolved engine owners.
extern "C" {
    NULGTARCMATERIAL NuLgtArcMtl[4] = {};
    i32 NuLgtLaserOldCnt;
}
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

struct ani3_animheader_s;
struct nuanimdatachunk_s;

extern "C" {
    u8 CutSceneBoundingBoxTrackRoot = 0;
    NUANIMBUFFEVALUATECB AnimBuffEvalCB = NULL;
    void **AnimBuffEvalData = NULL;
    i32 *AnimBuffEvalJoint = NULL;
}

static i32 clip_special_objects = 1;

using NUHGOBJVIDEOMEMFN = void (*)(nuhgobj_s *);

NUHGOBJVIDEOMEMFN hgobj_to_video_mem;
NUHGOBJVIDEOMEMFN video_mem_to_hgobj;

i32 NuCameraClipHGobj(nugscn_s *, NUMTX *, NUMTX *); // nu2api_nucore_misc.cpp
i32 findrange(nugscn_s *, i32);                      // utilities.cpp

extern "C" {
    i32 prev_lock;
    NUCAMERA locked_camera;
    NUCAMERA cam_copy;
    i32 camfx;
    NuCameraReflect global_reflect;

    extern nurenderscene_s currentScene;
    i32 NuRndrDoingScreenGrab;
    i32 motionBlurAccumActiveThisFrame;

    i32 PS2_SREZ_W = 4096;
    i32 PS2_SREZ_H = 4096;
    static i32 current_clip_scissor_to_viewport;
} // extern "C"

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    extern "C++" void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

    extern nurenderscene_s currentScene;
    i32 speedblur_enabled = 1;
    f32 NuLightsx, NuLightsy;
    NULGTARCLASER NuLgtArcLaserData[16];
    i32 NuLgtLaserCnt;
    i32 NuLgtArcLaserCnt;
    i32 NuLgtArcLaserFrame;
    NULGTLASER NuLgtLaserData[64];

    static inline u16 NuLgtHalf(f32 value) {
        union {
            f32 value;
            u32 bits;
        } conversion = {value};
        i32 sign = conversion.bits >> 31;
        i32 exponent = (conversion.bits >> 23) & 0xff;
        i32 mantissa = conversion.bits & 0x7fffff;
        // Preserve the original truncation and exponent clamp, including subnormals.
        exponent -= 112;
        if (exponent < 0)
            exponent = 0;
        if (exponent > 31)
            exponent = 31;
        return (sign << 15) | ((exponent & 31) << 10) | (mantissa >> 13);
    }
    static inline void NuLgtVertex(NUVEC4 *position, i32 colour, f32 u, f32 v) {
        g_NuPrim_StreamBufferPtr->u32_ptr[3] =
            g_NuPrim_NeedsOverbrightening ? colour : ((colour >> 1) & 0x7f7f7f) | (colour & 0xff000000);
        if (g_NuPrim_NeedsHalfUVs) {
            ((u16 *)g_NuPrim_StreamBufferPtr->u8_ptr)[8] = NuLgtHalf(u);
            ((u16 *)g_NuPrim_StreamBufferPtr->u8_ptr)[9] = NuLgtHalf(v);
        } else {
            g_NuPrim_StreamBufferPtr->f32_ptr[4] = u;
            g_NuPrim_StreamBufferPtr->f32_ptr[5] = v;
        }
        NuPrim2DAddXYZ(position->x, -position->y, position->z * 0.91f);
    }

    i32 ddmaxjoints;

#include "nu2api/nucore/nucore_plain_types.h"
    static inline NuLegacyDwaChunk *NuLegacyDwaGetChunk(void *animation, i32 index) {
        NuLegacyDwaChunk **chunks = *reinterpret_cast<NuLegacyDwaChunk ***>(static_cast<u8 *>(animation) + 0xc);
        return chunks[index];
    }
#undef NUGCUT_CURVE_VALUE

    i32 VisiSysCameraLock;
    i32 LoadedOcclusionData;
    i32 UsingOcclusionData;
    i32 CurrentViewBox = -1;
    u8 OcclusionBitArray[0x320];
    i32 do_InstTree = 1;
    i32 do_occlusion = 1;
    i32 do_visibility = 1;
    i32 do_octree;

    NuVisibilityResult *myvisi;
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
    }

    extern "C++" void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled);

#ifndef ANDROID
    void NuGetCurrentThreadId(void) {
        STUBBED();
    }
#endif
#ifndef ANDROID
    void NuThreadCreate(void) {
        STUBBED();
    }
#endif
    static f32 nu2api_paused;
} // extern "C"

struct nuframebuffer_s;
union variptr_u;

struct TimeBarSet {
    i32 uses_buffer;
    i32 slot_count;
    i32 *accumulators[2];
    NUTIME *start_times;
    i32 *field_14;
    i32 *field_18;
    i32 *colours;
    i32 *toggle_flags;
    const char **slot_names;
    i32 field_28;
};

DECOMP_ASSERT(sizeof(TimeBarSet) == 0x2c, "TimeBarSet size");
DECOMP_ASSERT(offsetof(TimeBarSet, slot_count) == 0x04, "TimeBarSet slot count offset");
DECOMP_ASSERT(offsetof(TimeBarSet, accumulators) == 0x08, "TimeBarSet accumulator offset");
DECOMP_ASSERT(offsetof(TimeBarSet, start_times) == 0x10, "TimeBarSet start-time offset");
DECOMP_ASSERT(offsetof(TimeBarSet, toggle_flags) == 0x20, "TimeBarSet toggle offset");
DECOMP_ASSERT(offsetof(TimeBarSet, slot_names) == 0x24, "TimeBarSet name offset");

static constexpr i32 kMaxTimeBarSets = 16;
static TimeBarSet *NuTimeBar_SetList[kMaxTimeBarSets];
static NUMTL *NuTimeBar_FrameOutMtl;
static i32 NuTimeBar_Initialised;
static i32 NuTimeBar_PeakReset;
static i32 NuTimeBar_GpuFrameOutEnabled;
static i32 NuTimeBar_EngineEnabled;
static i32 NuTimeBar_RenderPeakReset = 30;
static u32 NuTimeBar_DefaultColours[kMaxTimeBarSets] = {
    0x00000040, 0x22222240, 0x00004440, 0x00008840, 0x44000040, 0x88000040, 0x44004440, 0x88008840,
    0x00440040, 0x00880040, 0x00444440, 0x00888840, 0x44440040, 0x88880040, 0x44444440, 0x88888840,
};

static void *timebar_allocate(VARIPTR *buffer, usize size, usize alignment) {
    if (buffer == NULL) {
        return NU_ALLOC(size, alignment, 1, "Main", 0);
    }

    usize address = ALIGN(buffer->addr, alignment);
    buffer->addr = address + size;
    return reinterpret_cast<void *>(address);
}

template <typename T> static T *timebar_allocate_array(VARIPTR *buffer, i32 count) {
    return static_cast<T *>(timebar_allocate(buffer, static_cast<usize>(count) * sizeof(T), alignof(T)));
}

// Retail only has the IPA-SRA clone (the unused end-of-buffer argument dropped).
static __attribute__((noinline, noclone)) i32
CreateTimeBar(VARIPTR *buffer, i32 *colours, i32 slot_count) __asm__("_ZL13CreateTimeBarP9variptr_uS_Pii.isra.40");

extern "C" __attribute__((optimize("no-optimize-sibling-calls"))) i32 NuTimeBarCreateSetEx(VARIPTR *buffer,
                                                                                           VARIPTR unused_buffer,
                                                                                           i32 *colours);

// ---------------------------------------------------------------------------
// Public API — original addresses noted per function
// ---------------------------------------------------------------------------

static f32 timebar_unsigned_float(u32 value) {
    // Preserve the retail conversion for values whose sign bit is set.
    return static_cast<f32>(value);
}

// Game-thread scene-parameter queue (defined in nurndr_plain.cpp).

static const i32 cubeEdgeIndices[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7},
};

typedef f32 RawClipVec4 __attribute__((vector_size(16), aligned(4)));

i32 findrange(nugscn_s *scene, i32 first_joint);

extern i32 LevFlag[4];

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

struct nuhspecial_s;
struct nuvec_s;

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

struct eduimenu_s;
struct eduiitem_s;

struct EDCREATURE_s;

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

struct VuVec;

#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

struct numtx_s;

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern EdInputContext * Input;

inline EdMatrixControl::~EdMatrixControl() {
    Destroy();
}

inline void EdMatrixControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdMatrixControl));
}

inline void EdStringControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdStringControl));
}

template <> inline void EdValueControl<f32>::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdValueControl<f32>));
}

inline void EdFloatControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdFloatControl));
}

inline EdVectorControl::~EdVectorControl() {
    Destroy();
}

inline void EdVectorControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdVectorControl));
}

inline EdClassObjectNameControl::~EdClassObjectNameControl() {
}

inline void EdClassObjectNameControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdClassObjectNameControl));
}

inline EdSubSystem::~EdSubSystem() {
}

inline void EdControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdControl));
}

// Match each control's allocation type, rather than inheriting the base size.
inline void EdEnumControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdEnumControl));
}

inline void EdBitControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdBitControl));
}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

#if defined(__i386__) && defined(__SSE__)
#include <xmmintrin.h>
#endif

inline SplineObject::SplineObject()
    : next(NULL), previous(NULL), knots{}, points{}, led_file(Placeable::CurrentLedFile) {
}

inline SplineObject::~SplineObject() {
}

inline void SplineKnot::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(SplineKnot));
}

inline void SplineObject::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(SplineObject));
}

inline void SplinePointBlock::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(SplinePointBlock));
}

extern WORLDINFO WorldInfo[2];

void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

u8 BitCountTable[256] = {};

extern "C" {
    static i32 isBitCountTable;

    static inline void *NuLegacyRelocatePointer(void *pointer, isize delta) {
        return pointer == NULL ? NULL : reinterpret_cast<void *>(reinterpret_cast<usize>(pointer) + delta);
    }

}

extern "C" {
    NUJOINTPROCANIMFN JointProcAnimFn;
} // extern "C"

extern i32 LevFlag[4]; // shared per-level 16-byte state scratch

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

typedef isize __attribute__((may_alias)) NuGCutWord;

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

static const u8 KeyStructSizes[16] = {3, 4, 4, 3, 4, 3, 4, 8, 4, 8, 4, 0, 0, 0, 0, 0};
static const u8 CurveGroupMasks[3] = {2, 1, 8};

static inline f32 WrapAni4BlendRotation(f32 delta) {
    if (delta > 3.1415927f || delta < -3.1415927f) {
        i32 fixed_turn = static_cast<i32>(delta * 65536.0f / 6.2831855f) & 0xffff;
        if (fixed_turn >= 0x8000) {
            fixed_turn -= 0x10000;
        }
        delta = static_cast<f32>(fixed_turn) * 9.58738e-5f;
    }
    return delta;
}

static inline void SkipAni4V4Curve(u16 type, u8 *&keys, ani3_scalemin_s *&scale_min) {
    if (type < 16) {
        keys += KeyStructSizes[type];
        ++scale_min;
    }
}

static inline f32 DecodeAni4Quat3Scalar(const ani3_animheader_s *anim, i32 key_stride, const u16 *constants, u16 type,
                                        u32 quarter, f32 fraction, u8 *&keys, ani3_scalemin_s *&scale_min) {
    if (type != 6) {
        const u32 constant = constants[type - 16];
        return static_cast<f32>(constant) * anim->scale + anim->minimum;
    }

    const u32 first_word = *reinterpret_cast<const u32 *>(keys);
    const u32 next_word = *reinterpret_cast<const u32 *>(keys + key_stride);
    const f32 first_value = static_cast<f32>(first_word & 0xff);
    const f32 next_value = static_cast<f32>(next_word & 0xff);
    const u32 tangents = first_word >> 8;
    const f32 tangent_scale = 0.01587302f;
    const f32 tangent0 = static_cast<f32>((tangents >> (quarter * 6)) & 0x3f) * tangent_scale;

    f32 packed_value;
    if (quarter == 3) {
        const f32 interpolated = (next_value - first_value) * tangent0 + first_value;
        const f32 tangent1 = static_cast<f32>((next_word >> 8) & 0x3f) * tangent_scale;
        const i32 after_value = keys[key_stride * 2];
        const f32 next_interpolated = (after_value - next_value) * tangent1 + next_value;
        packed_value = (next_interpolated - interpolated) * fraction + interpolated;
    } else {
        const f32 tangent1 = static_cast<f32>((tangents >> (((quarter * 3 + 3) * 2) & 0x1f)) & 0x3f) * tangent_scale;
        packed_value = (next_value - first_value) * ((tangent1 - tangent0) * fraction + tangent0) + first_value;
    }

    const f32 value = packed_value * scale_min->scale + scale_min->minimum;
    keys += 4;
    ++scale_min;
    return value;
}

static void DecodeAni4Quat3Pair(const ani3_animheader_s *anim, i32 key_stride, const u16 *constants, u16 type,
                                u32 quarter, u8 *&keys, ani3_scalemin_s *&scale_min, f32 &first, f32 &second) {
    if (type == 7) {
        CalcValue1648Get2Values(reinterpret_cast<char *>(keys), quarter, key_stride, scale_min, &first, &second);
        keys += 8;
        ++scale_min;
        return;
    }
    if (type == 9) {
        const u16 *samples = reinterpret_cast<const u16 *>(keys);
        first = static_cast<f32>(samples[quarter]) * scale_min->scale + scale_min->minimum;
        const u16 next = quarter == 3 ? *reinterpret_cast<const u16 *>(keys + key_stride) : samples[quarter + 1];
        second = static_cast<f32>(static_cast<i32>(next)) * scale_min->scale + scale_min->minimum;
        keys += 8;
        ++scale_min;
        return;
    }
    if (type == 6) {
        const u32 first_word = *reinterpret_cast<const u32 *>(keys);
        const u32 next_word = *reinterpret_cast<const u32 *>(keys + key_stride);
        const f32 start = static_cast<f32>(first_word & 0xff);
        const f32 next = static_cast<f32>(next_word & 0xff);
        const u32 tangents = first_word >> 8;
        const f32 tangent0 = static_cast<f32>((tangents >> (quarter * 6)) & 0x3f) * 0.01587302f;
        first = ((next - start) * tangent0 + start) * scale_min->scale + scale_min->minimum;
        if (quarter == 3) {
            const f32 tangent1 = static_cast<f32>((next_word >> 8) & 0x3f) * 0.01587302f;
            const f32 after = static_cast<f32>(keys[key_stride * 2]);
            second = ((after - next) * tangent1 + next) * scale_min->scale + scale_min->minimum;
        } else {
            const f32 tangent1 = static_cast<f32>((tangents >> (((quarter * 3 + 3) * 2) & 0x1f)) & 0x3f) * 0.01587302f;
            second = ((next - start) * tangent1 + start) * scale_min->scale + scale_min->minimum;
        }
        keys += 4;
        ++scale_min;
        return;
    }

    first = second = static_cast<f32>(static_cast<u32>(constants[type - 16])) * anim->scale + anim->minimum;
}

static void DecodeAni4BlendQuat3Pair(const ani3_animheader_s *anim, i32 key_stride, const u16 *constants, u16 type,
                                     u32 quarter, u8 *&keys, ani3_scalemin_s *&scale_min, f32 &first, f32 &second) {
    if (type == 6) {
        const u32 first_word = *reinterpret_cast<const u32 *>(keys);
        const u32 next_word = *reinterpret_cast<const u32 *>(keys + key_stride);
        const f32 start = static_cast<f32>(first_word & 0xff);
        const f32 next = static_cast<f32>(next_word & 0xff);
        const u32 tangents = first_word >> 8;
        const f32 tangent0 = static_cast<f32>((tangents >> (quarter * 6)) & 0x3f) * 0.01587302f;
        first = ((next - start) * tangent0 + start) * scale_min->scale + scale_min->minimum;
        if (quarter == 3) {
            const f32 tangent1 = static_cast<f32>((next_word >> 8) & 0x3f) * 0.01587302f;
            const f32 after = static_cast<f32>(keys[key_stride * 2]);
            second = ((after - next) * tangent1 + next) * scale_min->scale + scale_min->minimum;
        } else {
            const f32 tangent1 = static_cast<f32>((tangents >> (((quarter * 3 + 3) * 2) & 0x1f)) & 0x3f) * 0.01587302f;
            second = ((next - start) * tangent1 + start) * scale_min->scale + scale_min->minimum;
        }
        keys += 4;
        ++scale_min;
        return;
    }

    if (type == 7) {
        CalcValue1648Get2Values(reinterpret_cast<char *>(keys), quarter, key_stride, scale_min, &first, &second);
        keys += 8;
        ++scale_min;
        return;
    }
    first = second = static_cast<f32>(static_cast<u32>(constants[type - 16])) * anim->scale + anim->minimum;
}

static void DecodeAni4BlendQuat3WPair(const ani3_animheader_s *anim, i32 key_stride, const u16 *constants, u16 type,
                                      u32 quarter, u8 *&keys, ani3_scalemin_s *&scale_min, f32 &first, f32 &second) {
    if (type == 6) {
        const u32 first_word = *reinterpret_cast<const u32 *>(keys);
        const u32 next_word = *reinterpret_cast<const u32 *>(keys + key_stride);
        const f32 start = static_cast<f32>(first_word & 0xff);
        const f32 next = static_cast<f32>(next_word & 0xff);
        const u32 tangents = first_word >> 8;
        const f32 tangent0 = static_cast<f32>((tangents >> (quarter * 6)) & 0x3f) * 0.01587302f;
        first = ((next - start) * tangent0 + start) * scale_min->scale + scale_min->minimum;
        if (quarter == 3) {
            const f32 tangent1 = static_cast<f32>((next_word >> 8) & 0x3f) * 0.01587302f;
            const f32 after = static_cast<f32>(keys[key_stride * 2]);
            second = ((after - next) * tangent1 + next) * scale_min->scale + scale_min->minimum;
        } else {
            const f32 tangent1 = static_cast<f32>((tangents >> (((quarter * 3 + 3) * 2) & 0x1f)) & 0x3f) * 0.01587302f;
            second = ((next - start) * tangent1 + start) * scale_min->scale + scale_min->minimum;
        }
        keys += 4;
        ++scale_min;
        return;
    }

    if (type == 7) {
        CalcValue1648Get2Values(reinterpret_cast<char *>(keys), quarter, key_stride, scale_min, &first, &second);
        keys += 8;
        ++scale_min;
        return;
    }
    first = second = static_cast<f32>(static_cast<u32>(constants[type - 16])) * anim->scale + anim->minimum;
}

// Euler curves interpolate quaternion endpoints, not interpolated Euler angles.
static void DecodeAni4EulerPair(const ani3_animheader_s *anim, i32 key_stride, const u16 *constants, u16 type,
                                u32 quarter, u8 *&keys, ani3_scalemin_s *&scale_min, f32 &first, f32 &second) {
    if (type == 6) {
        const u32 first_word = *reinterpret_cast<const u32 *>(keys);
        const u32 next_word = *reinterpret_cast<const u32 *>(keys + key_stride);
        const f32 start = static_cast<f32>(first_word & 0xff);
        const f32 next = static_cast<f32>(next_word & 0xff);
        const u32 tangents = first_word >> 8;
        const f32 tangent0 = static_cast<f32>((tangents >> (quarter * 6)) & 0x3f) * 0.01587302f;
        first = (tangent0 * (next - start) + start) * scale_min->scale + scale_min->minimum;
        if (quarter == 3) {
            const f32 tangent1 = static_cast<f32>((next_word >> 8) & 0x3f) * 0.01587302f;
            const i32 after = keys[key_stride * 2];
            second = ((after - next) * tangent1 + next) * scale_min->scale + scale_min->minimum;
        } else {
            const f32 tangent1 = static_cast<f32>((tangents >> (((quarter * 3 + 3) * 2) & 0x1f)) & 0x3f) * 0.01587302f;
            second = (tangent1 * (next - start) + start) * scale_min->scale + scale_min->minimum;
        }
        keys += 4;
        ++scale_min;
    } else {
        first = second = static_cast<f32>(static_cast<u32>(constants[type - 16])) * anim->scale + anim->minimum;
    }
}

extern "C" {
    f32 nuspecial_const_alpha = 1.0f;
    NUCOLOUR3 nuspecial_const_tint = {1.0f, 1.0f, 1.0f};
    i32 nuspecial_clip_state = -1;

    i32 special_debug;
    i32 nuspecial_const_alpha_enabled;
    i32 nuspecial_const_tint_enabled;
    volatile i32 nuspecial_draw_state;
    i32 nuspecial_vertex_noffsets;
    VARIPTR nuspecial_vertex_offsets;
    NUSPECIALVERTEXSTATES *nuspecial_vertex_states;
    i32 nuspecial_reflection;
    i32 nuspecial_shadowLightCount;
    void *nuspecial_shadowLight[4];
    i32 nuspecial_shadowLightClipOverride[4];
    i32 nuspecial_shadowLightHaveClipOverrides;
}

static i32 nuqfnt_init;

u8 sysfont[] = {
    0x88, 0x1e, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x78, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x7b, 0x00, 0x00, 0x00, 0x7b, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x90, 0x41, 0x00, 0x00, 0x50, 0x41, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8,
    0xff, 0xff, 0xff, 0xc4, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xff, 0x1c, 0x00, 0x00, 0x00, 0xe8, 0x05, 0x00, 0x00,
    0xb4, 0xff, 0xff, 0xff, 0xb0, 0xff, 0xff, 0xff, 0xac, 0xff, 0xff, 0xff, 0xa8, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xca, 0x42, 0x00, 0x00,
    0x90, 0x42, 0x00, 0x00, 0xa0, 0x40, 0x00, 0x00, 0xda, 0x42, 0x00, 0x00, 0x90, 0x42, 0x00, 0x00, 0xc0, 0x40, 0x00,
    0x00, 0x40, 0x41, 0x00, 0x00, 0xb4, 0x42, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0xb0, 0x41, 0x00, 0x00, 0xb4, 0x42,
    0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x04, 0x42, 0x00, 0x00, 0xb4, 0x42, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x28,
    0x42, 0x00, 0x00, 0xb4, 0x42, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x4c, 0x42, 0x00, 0x00, 0xb4, 0x42, 0x00, 0x00,
    0x00, 0x41, 0x00, 0x00, 0xa2, 0x42, 0x00, 0x00, 0xb4, 0x42, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0xb8, 0x42, 0x00,
    0x00, 0xb4, 0x42, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0xd8, 0x42, 0x00, 0x00, 0x00, 0x41,
    0x00, 0x00, 0x40, 0x41, 0x00, 0x00, 0xd8, 0x42, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0xc8, 0x41, 0x00, 0x00, 0xd8,
    0x42, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0xf0, 0x41, 0x00, 0x00, 0xd8, 0x42, 0x00, 0x00, 0x10, 0x41, 0x00, 0x00,
    0x20, 0x42, 0x00, 0x00, 0xd8, 0x42, 0x00, 0x00, 0x10, 0x41, 0x00, 0x00, 0x4c, 0x42, 0x00, 0x00, 0xd8, 0x42, 0x00,
    0x00, 0x00, 0x41, 0x00, 0x00, 0x74, 0x42, 0x00, 0x00, 0xd8, 0x42, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x8e, 0x42,
    0x00, 0x00, 0xd8, 0x42, 0x00, 0x00, 0xc0, 0x40, 0x00, 0x00, 0xa2, 0x42, 0x00, 0x00, 0xd8, 0x42, 0x00, 0x00, 0xc0,
    0x40, 0x00, 0x00, 0xb2, 0x42, 0x00, 0x00, 0xd8, 0x42, 0x00, 0x00, 0x10, 0x41, 0x00, 0x00, 0xc6, 0x42, 0x00, 0x00,
    0xd8, 0x42, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0xe2, 0x42, 0x00, 0x00, 0xd8, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00,
    0xf4, 0x42, 0x00, 0x00, 0x90, 0x42, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0xb4, 0x42, 0x00, 0x00, 0x10,
    0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xca,
    0x42, 0x00, 0x00, 0xb4, 0x42, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xde, 0x42, 0x00, 0x00, 0xb4, 0x42, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x40,
    0x00, 0x00, 0x50, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x00, 0x00, 0xb0, 0x41, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0xf8, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00,
    0x1c, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x41, 0x00, 0x00, 0x4c, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xe0, 0x40, 0x00, 0x00, 0x74, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x8c, 0x42,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0xa0, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
    0x40, 0x00, 0x00, 0xb4, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xec,
    0x42, 0x00, 0x00, 0xb4, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xc8, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0xda, 0x42, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0xee, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x40,
    0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x90, 0x41, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x40, 0x41, 0x00, 0x00, 0x90,
    0x41, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0xb0, 0x41, 0x00, 0x00, 0x90, 0x41, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00,
    0xf8, 0x41, 0x00, 0x00, 0x90, 0x41, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x24, 0x42, 0x00, 0x00, 0x90, 0x41, 0x00,
    0x00, 0xe0, 0x40, 0x00, 0x00, 0x4c, 0x42, 0x00, 0x00, 0x90, 0x41, 0x00, 0x00, 0xc0, 0x40, 0x00, 0x00, 0x74, 0x42,
    0x00, 0x00, 0x90, 0x41, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x8c, 0x42, 0x00, 0x00, 0x90, 0x41, 0x00, 0x00, 0xe0,
    0x40, 0x00, 0x00, 0xa0, 0x42, 0x00, 0x00, 0x90, 0x41, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0xb4, 0x42, 0x00, 0x00,
    0x90, 0x41, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0xc8, 0x42, 0x00, 0x00, 0x90, 0x41, 0x00, 0x00, 0x00, 0x41, 0x00,
    0x00, 0xda, 0x42, 0x00, 0x00, 0x90, 0x41, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0xee, 0x42, 0x00, 0x00, 0x90, 0x41,
    0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x10, 0x42, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x40,
    0x41, 0x00, 0x00, 0x10, 0x42, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0xb0, 0x41, 0x00, 0x00, 0x10, 0x42, 0x00, 0x00,
    0xe0, 0x40, 0x00, 0x00, 0xf0, 0x41, 0x00, 0x00, 0x10, 0x42, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x24, 0x42, 0x00,
    0x00, 0x10, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x48, 0x42, 0x00, 0x00, 0x10, 0x42, 0x00, 0x00, 0x00, 0x41,
    0x00, 0x00, 0x6c, 0x42, 0x00, 0x00, 0x10, 0x42, 0x00, 0x00, 0x20, 0x41, 0x00, 0x00, 0x8c, 0x42, 0x00, 0x00, 0x10,
    0x42, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x9e, 0x42, 0x00, 0x00, 0x10, 0x42, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00,
    0xb4, 0x42, 0x00, 0x00, 0x10, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x7c, 0x42, 0x00, 0x00, 0xb4, 0x42, 0x00,
    0x00, 0x80, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8e, 0x42,
    0x00, 0x00, 0xb4, 0x42, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x42, 0x00, 0x00, 0x10, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00,
    0x00, 0xda, 0x42, 0x00, 0x00, 0x10, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0xee, 0x42, 0x00, 0x00, 0x10, 0x42,
    0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x58, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x40,
    0x41, 0x00, 0x00, 0x58, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0xb0, 0x41, 0x00, 0x00, 0x58, 0x42, 0x00, 0x00,
    0xe0, 0x40, 0x00, 0x00, 0xf8, 0x41, 0x00, 0x00, 0x58, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x24, 0x42, 0x00,
    0x00, 0x58, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x4c, 0x42, 0x00, 0x00, 0x58, 0x42, 0x00, 0x00, 0x00, 0x41,
    0x00, 0x00, 0x74, 0x42, 0x00, 0x00, 0x58, 0x42, 0x00, 0x00, 0xa0, 0x40, 0x00, 0x00, 0x8c, 0x42, 0x00, 0x00, 0x58,
    0x42, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0xa0, 0x42, 0x00, 0x00, 0x58, 0x42, 0x00, 0x00, 0xc0, 0x40, 0x00, 0x00,
    0xb4, 0x42, 0x00, 0x00, 0x58, 0x42, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0xc8, 0x42, 0x00, 0x00, 0x58, 0x42, 0x00,
    0x00, 0xe0, 0x40, 0x00, 0x00, 0xda, 0x42, 0x00, 0x00, 0x58, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0xee, 0x42,
    0x00, 0x00, 0x58, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x90, 0x42, 0x00, 0x00, 0xe0,
    0x40, 0x00, 0x00, 0x50, 0x41, 0x00, 0x00, 0x90, 0x42, 0x00, 0x00, 0xc0, 0x40, 0x00, 0x00, 0xb0, 0x41, 0x00, 0x00,
    0x90, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0xf8, 0x41, 0x00, 0x00, 0x90, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00,
    0x00, 0x24, 0x42, 0x00, 0x00, 0x90, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x4c, 0x42, 0x00, 0x00, 0x90, 0x42,
    0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x70, 0x42, 0x00, 0x00, 0x90, 0x42, 0x00, 0x00, 0x10, 0x41, 0x00, 0x00, 0x8c,
    0x42, 0x00, 0x00, 0x90, 0x42, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0xa0, 0x42, 0x00, 0x00, 0x90, 0x42, 0x00, 0x00,
    0xe0, 0x40, 0x00, 0x00, 0xb4, 0x42, 0x00, 0x00, 0x90, 0x42, 0x00, 0x00, 0xe0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x20, 0x00, 0x21, 0x00, 0x21, 0x00, 0x22, 0x00, 0x15, 0x00,
    0x23, 0x00, 0x0e, 0x00, 0x25, 0x00, 0x25, 0x00, 0x26, 0x00, 0x03, 0x00, 0x27, 0x00, 0x0c, 0x00, 0x28, 0x00, 0x05,
    0x00, 0x29, 0x00, 0x06, 0x00, 0x2a, 0x00, 0x04, 0x00, 0x2b, 0x00, 0x07, 0x00, 0x2c, 0x00, 0x2c, 0x00, 0x2d, 0x00,
    0x01, 0x00, 0x2e, 0x00, 0x2e, 0x00, 0x2f, 0x00, 0x0f, 0x00, 0x30, 0x00, 0x30, 0x00, 0x31, 0x00, 0x31, 0x00, 0x32,
    0x00, 0x32, 0x00, 0x33, 0x00, 0x33, 0x00, 0x34, 0x00, 0x34, 0x00, 0x35, 0x00, 0x35, 0x00, 0x36, 0x00, 0x36, 0x00,
    0x37, 0x00, 0x37, 0x00, 0x38, 0x00, 0x38, 0x00, 0x39, 0x00, 0x39, 0x00, 0x3a, 0x00, 0x09, 0x00, 0x3b, 0x00, 0x08,
    0x00, 0x3c, 0x00, 0x0a, 0x00, 0x3d, 0x00, 0x02, 0x00, 0x3e, 0x00, 0x0b, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x40, 0x00,
    0x0d, 0x00, 0x41, 0x00, 0x41, 0x00, 0x42, 0x00, 0x42, 0x00, 0x43, 0x00, 0x43, 0x00, 0x44, 0x00, 0x44, 0x00, 0x45,
    0x00, 0x45, 0x00, 0x46, 0x00, 0x46, 0x00, 0x47, 0x00, 0x47, 0x00, 0x48, 0x00, 0x48, 0x00, 0x49, 0x00, 0x49, 0x00,
    0x4a, 0x00, 0x4a, 0x00, 0x4b, 0x00, 0x4b, 0x00, 0x4c, 0x00, 0x4c, 0x00, 0x4d, 0x00, 0x4d, 0x00, 0x4e, 0x00, 0x4e,
    0x00, 0x4f, 0x00, 0x4f, 0x00, 0x50, 0x00, 0x50, 0x00, 0x51, 0x00, 0x51, 0x00, 0x52, 0x00, 0x52, 0x00, 0x53, 0x00,
    0x53, 0x00, 0x54, 0x00, 0x54, 0x00, 0x55, 0x00, 0x55, 0x00, 0x56, 0x00, 0x56, 0x00, 0x57, 0x00, 0x57, 0x00, 0x58,
    0x00, 0x58, 0x00, 0x59, 0x00, 0x59, 0x00, 0x5a, 0x00, 0x5a, 0x00, 0x5b, 0x00, 0x5b, 0x00, 0x5c, 0x00, 0x10, 0x00,
    0x5d, 0x00, 0x5d, 0x00, 0x5e, 0x00, 0x14, 0x00, 0x5f, 0x00, 0x13, 0x00, 0x61, 0x00, 0x61, 0x00, 0x62, 0x00, 0x62,
    0x00, 0x63, 0x00, 0x63, 0x00, 0x64, 0x00, 0x64, 0x00, 0x65, 0x00, 0x65, 0x00, 0x66, 0x00, 0x66, 0x00, 0x67, 0x00,
    0x67, 0x00, 0x68, 0x00, 0x68, 0x00, 0x69, 0x00, 0x69, 0x00, 0x6a, 0x00, 0x6a, 0x00, 0x6b, 0x00, 0x6b, 0x00, 0x6c,
    0x00, 0x6c, 0x00, 0x6d, 0x00, 0x6d, 0x00, 0x6e, 0x00, 0x6e, 0x00, 0x6f, 0x00, 0x6f, 0x00, 0x70, 0x00, 0x70, 0x00,
    0x71, 0x00, 0x71, 0x00, 0x72, 0x00, 0x72, 0x00, 0x73, 0x00, 0x73, 0x00, 0x74, 0x00, 0x74, 0x00, 0x75, 0x00, 0x75,
    0x00, 0x76, 0x00, 0x76, 0x00, 0x77, 0x00, 0x77, 0x00, 0x78, 0x00, 0x78, 0x00, 0x79, 0x00, 0x79, 0x00, 0x7a, 0x00,
    0x7a, 0x00, 0x7b, 0x00, 0x11, 0x00, 0x7d, 0x00, 0x12, 0x00, 0x91, 0x00, 0x0c, 0x00, 0x92, 0x00, 0x0c, 0x00, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x56, 0x52, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0xff, 0xff, 0xd8, 0x8c, 0xcc, 0xec, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0xff, 0xff, 0xe0, 0x81, 0x81, 0x81, 0x00, 0x00, 0xff, 0xff, 0xbc,
    0x9c, 0x8c, 0x8c, 0x00, 0x00, 0xff, 0xff, 0xd8, 0x70, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81,
    0x00, 0x00, 0xff, 0xff, 0x80, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0xff,
    0xff, 0xc1, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0xff, 0xff, 0x98, 0x18,
    0x18, 0x18, 0x00, 0x00, 0xff, 0xff, 0x01, 0x01, 0x01, 0x81, 0x00, 0x00, 0xff, 0xff, 0xc1, 0xc7, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x1c, 0x0e, 0x07, 0x03, 0x00, 0x00, 0xff, 0xff, 0x81, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x7c, 0xcc, 0x8c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xf0, 0x30, 0x31, 0x00, 0x00, 0xff, 0xff, 0x8c, 0x8c, 0x8c, 0x8c, 0x00, 0x00,
    0xff, 0xff, 0x8c, 0x8c, 0xcc, 0x7c, 0x00, 0x00, 0xff, 0xff, 0x31, 0x31, 0xf1, 0x31, 0x00, 0x00, 0xff, 0xff, 0x31,
    0x31, 0x30, 0xf0, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xc7, 0xc0, 0xc0,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x1f, 0x00, 0x80, 0x00, 0x00, 0xff,
    0xff, 0xc0, 0xc0, 0xc3, 0xc0, 0x00, 0x00, 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc7, 0x00, 0x00, 0xff, 0xff, 0x80, 0x80,
    0x8f, 0x80, 0x00, 0x00, 0xff, 0xff, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x1f, 0x00,
    0x00, 0xff, 0xff, 0x31, 0x30, 0x38, 0x0e, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0xff, 0xff,
    0x70, 0x78, 0x6c, 0x66, 0x00, 0x00, 0xff, 0xff, 0x18, 0xb0, 0xb0, 0x30, 0x00, 0x00, 0xff, 0xff, 0x39, 0x1f, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0x63, 0x61, 0xff, 0x60, 0x00, 0x00, 0xff, 0xff, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xff, 0xff, 0x18, 0x18, 0x18, 0xf8, 0x00, 0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x83, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x60, 0x60, 0xe0, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x98, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x61, 0x63, 0x63, 0x63, 0x00, 0x00, 0xff,
    0xff, 0x61, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x1e,
    0x33, 0x01, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xc6, 0xc6, 0xc6, 0x00,
    0x00, 0xff, 0xff, 0x01, 0x01, 0x01, 0x39, 0x00, 0x00, 0xff, 0xff, 0x31, 0x31, 0x33, 0x3e, 0x00, 0x00, 0xff, 0xff,
    0xc6, 0xc6, 0xfe, 0xc6, 0x00, 0x00, 0xff, 0xff, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xf8, 0x60, 0x60, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0xff, 0xff, 0x60,
    0x60, 0x60, 0xf8, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x60, 0xc1,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0xff,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0xff, 0xff, 0xcc, 0x86,
    0x86, 0x86, 0x00, 0x00, 0xff, 0xff, 0x86, 0x86, 0x86, 0x86, 0x00, 0x00, 0xff, 0xff, 0x30, 0x31, 0x31, 0x31, 0x00,
    0x00, 0xff, 0xff, 0x31, 0xf1, 0xb1, 0x31, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x83, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
    0xcf, 0x00, 0x00, 0xff, 0xff, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0xff, 0xff, 0x83, 0x01, 0x01, 0x03, 0x00, 0x00,
    0xff, 0xff, 0x19, 0x00, 0x00, 0x03, 0x00, 0x00, 0xff, 0xff, 0x0f, 0x1e, 0x18, 0x18, 0x00, 0x00, 0xff, 0xff, 0xcc,
    0xf8, 0xc0, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x30, 0x30, 0x01, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0xff,
    0xff, 0xf8, 0xdc, 0x8c, 0x8c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0xff, 0xe1, 0x31,
    0x31, 0xf1, 0x00, 0x00, 0xff, 0xff, 0xc6, 0x8c, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x1c, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc3, 0x06, 0x06, 0x07, 0x00, 0x00, 0xff, 0xff, 0x00, 0x1e, 0x07,
    0x03, 0x00, 0x00, 0xff, 0xff, 0x1f, 0x03, 0x83, 0x83, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0xc6, 0x00, 0x00, 0xff, 0xff, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0xff, 0xff, 0x06, 0x06, 0x06, 0x06,
    0x00, 0x00, 0xff, 0xff, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0xff, 0xff, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0xff,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x9c, 0x98, 0x98, 0x00,
    0x00, 0xff, 0xff, 0x98, 0x98, 0xf8, 0xf0, 0x00, 0x00, 0xff, 0xff, 0x1b, 0x1b, 0x19, 0x99, 0x00, 0x00, 0xff, 0xff,
    0xd9, 0xd9, 0xf9, 0x70, 0x00, 0x00, 0xff, 0xff, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0xee, 0x7c, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x3e, 0x3b, 0x31, 0x31, 0x00, 0x00, 0xff, 0xff, 0x00,
    0x06, 0x06, 0x06, 0x00, 0x00, 0xff, 0xff, 0x7e, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0xff, 0xff, 0xf0, 0x60, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf0, 0xc0,
    0xc0, 0xc0, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0xc7, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x00, 0x00, 0x07, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0xff, 0xff, 0x18, 0x18, 0x1c, 0x0c, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x0c, 0x0c,
    0x0c, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x06, 0x06, 0x07, 0x00, 0x00,
    0xff, 0xff, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0xff, 0xff, 0x63,
    0x63, 0x36, 0x1c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xff, 0xff, 0x8c, 0x8c, 0x8c, 0x8c,
    0x00, 0x00, 0xff, 0xff, 0x36, 0x63, 0x63, 0x63, 0x00, 0x00, 0xff, 0xff, 0x77, 0x3e, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0x8c, 0xf8, 0x80, 0x80, 0x00, 0x00, 0xff, 0xff, 0xcc, 0x78, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xcf, 0xcc, 0xcc, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00, 0x18, 0x1c, 0x0c, 0x00, 0x00, 0xff, 0xff, 0xcc, 0xcc, 0xcc, 0xcc, 0x00, 0x00, 0xff, 0xff,
    0xcc, 0xcc, 0xcc, 0xc7, 0x00, 0x00, 0xff, 0xff, 0x0e, 0x07, 0x07, 0x0f, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x1c, 0x18,
    0x18, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x03, 0x03, 0x03, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x0c, 0x0c, 0x9c, 0x00, 0x00, 0xff, 0xff, 0x03,
    0x03, 0x03, 0x03, 0x00, 0x00, 0xff, 0xff, 0x03, 0x03, 0x03, 0x7f, 0x00, 0x00, 0xff, 0xff, 0x9c, 0xfc, 0x6c, 0x0c,
    0x00, 0x00, 0xff, 0xff, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xff,
    0x7f, 0xc1, 0xe1, 0x61, 0x61, 0x00, 0x00, 0xff, 0x7f, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xff, 0xff, 0x61, 0x63,
    0x63, 0x63, 0x00, 0x00, 0xff, 0xff, 0x61, 0x71, 0xf1, 0x31, 0x00, 0x00, 0xff, 0x6f, 0x30, 0x30, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0xe3, 0x67, 0x67, 0x66, 0x00, 0x00, 0xff, 0xff, 0x66, 0xe6, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0xff, 0x6f, 0x0e, 0x8c, 0x8c, 0x86, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x1e, 0x00, 0x00, 0xff, 0xff, 0x33, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0xff, 0x83, 0x8e, 0x8c, 0x8c, 0x00, 0x00,
    0xff, 0x7f, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0xff, 0x33,
    0x1e, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x33, 0x33, 0x73,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x8c, 0xcc, 0x6c, 0x00, 0x00, 0xff,
    0xff, 0xf3, 0xf3, 0xb3, 0x33, 0x00, 0x00, 0xff, 0xff, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, 0xff, 0xff, 0x6c, 0x6d,
    0x6f, 0x6f, 0x00, 0x00, 0xff, 0xff, 0x6e, 0x6c, 0xcc, 0x8c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00, 0x83, 0x86, 0x8c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x0f, 0x19, 0x31, 0x00, 0x00, 0xff, 0xff, 0x8c, 0x8c, 0x8c, 0x8c, 0x00, 0x00, 0xff, 0xff, 0x8c, 0x8c, 0x86,
    0x83, 0x00, 0x00, 0xff, 0xff, 0x31, 0x31, 0x19, 0x0f, 0x00, 0x00, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0xff, 0xff, 0x98, 0x98, 0x18, 0x19,
    0x00, 0x00, 0xff, 0xff, 0x1b, 0x1b, 0x1f, 0x8e, 0x00, 0x00, 0xff, 0xff, 0x99, 0x19, 0x0f, 0x0f, 0x00, 0x00, 0xff,
    0xff, 0x06, 0x0f, 0x0f, 0x19, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x61, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0xfc, 0x00,
    0x00, 0xff, 0xff, 0x61, 0x33, 0x33, 0x1e, 0x00, 0x00, 0xff, 0xff, 0x1e, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0xff, 0xff,
    0x80, 0xc0, 0xe0, 0x60, 0x00, 0x00, 0xff, 0xff, 0x70, 0x30, 0x18, 0x1c, 0x00, 0x00, 0xff, 0xff, 0x8c, 0xcc, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x19, 0x30, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x03, 0xc3, 0xc3, 0xc0, 0x00, 0x00, 0xff, 0xff, 0xc3,
    0xc3, 0xc3, 0xc3, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x06, 0x03, 0x03,
    0x00, 0x00, 0xff, 0xff, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0x0c, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
    0x0c, 0x0c, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x98, 0xfc, 0x6c, 0x6c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x60, 0x00, 0x00, 0xff, 0xff, 0x01, 0x01, 0xc0, 0x60, 0x00, 0x00, 0xff, 0xff, 0x00, 0xe0, 0x30, 0x30, 0x00, 0x00,
    0xff, 0xff, 0x60, 0x60, 0xe3, 0xe6, 0x00, 0x00, 0xff, 0xff, 0x66, 0x67, 0x66, 0x66, 0x00, 0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x07, 0x0e, 0x00, 0x00, 0xff,
    0xff, 0x8c, 0x8c, 0x8c, 0x8c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x1e, 0x33, 0x00, 0x00, 0xff, 0xff, 0x01, 0x01,
    0x01, 0x01, 0x00, 0x00, 0xff, 0xff, 0x30, 0xe1, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xe7, 0xe7, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf1, 0x33, 0x33, 0x33, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0x83, 0xc6, 0x66, 0x66, 0x00, 0x00, 0xff, 0xff, 0x0e, 0x07, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x1e, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00,
    0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x83, 0x86, 0x8c, 0x8c,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x1f, 0x3b, 0x31, 0x31, 0x00, 0x00, 0xff,
    0xff, 0x8c, 0x8c, 0xdc, 0xf8, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x31, 0x31,
    0x61, 0xc1, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xdc, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xe1, 0xe1, 0x00, 0x00, 0xff, 0xff, 0x00, 0x06, 0x06, 0x03, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0x83, 0x83, 0x03, 0x03, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0xc0, 0x00, 0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x8f, 0x19, 0x00, 0x00, 0xff, 0xff, 0x8c, 0x8c, 0x8c, 0x8c,
    0x00, 0x00, 0xff, 0xff, 0xdc, 0xf8, 0x80, 0x80, 0x00, 0x00, 0xff, 0xff, 0x61, 0x61, 0x61, 0x61, 0x00, 0x00, 0xff,
    0xff, 0x61, 0x61, 0x01, 0x01, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x1c,
    0x36, 0x22, 0x00, 0x00, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xc0, 0x60, 0x60, 0x00,
    0x00, 0xff, 0xff, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x07, 0x1f, 0x18, 0x00, 0x00, 0xff, 0xff, 0x1c, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x01, 0x03, 0x03, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0xff, 0xff, 0x31, 0x31, 0x3b, 0x3e, 0x00, 0x00, 0xff, 0xff, 0x30,
    0x33, 0x1e, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x06, 0x06, 0x3f, 0x06, 0x00, 0x00, 0xff,
    0xff, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0x00, 0x00, 0xff, 0xff, 0xc0, 0xc0,
    0xc0, 0xf8, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x07, 0x00,
    0x00, 0xff, 0xff, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33,
    0x33, 0x00, 0x00, 0xff, 0xff, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0xff, 0xff, 0x0e, 0x3c, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0xff, 0xff, 0xc6, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x18, 0x0c, 0x0c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x0c, 0x18, 0x18, 0x00, 0x00, 0xff, 0xff, 0xb8, 0xb0, 0xb0, 0xf0, 0x00, 0x00, 0xff,
    0xff, 0xe0, 0x40, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x33, 0xb1, 0xf1, 0xe1, 0x00, 0x00, 0xff, 0xff, 0x60, 0x60,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00,
    0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0xff, 0xff,
    0x36, 0xdc, 0x70, 0xd8, 0x00, 0x00, 0xff, 0xff, 0x6c, 0x20, 0x60, 0xc0, 0x00, 0x00, 0xff, 0xff, 0x43, 0xe1, 0xe0,
    0x31, 0x00, 0x00, 0xff, 0xff, 0x33, 0x32, 0x73, 0xe1, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc1, 0x81, 0x01, 0x83, 0x00, 0x00, 0xff, 0xff, 0xc3, 0x03, 0x07, 0x0d,
    0x00, 0x00, 0xff, 0xff, 0x1a, 0x0f, 0x07, 0x0f, 0x00, 0x00, 0xff, 0xff, 0x1a, 0x02, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xc0, 0xf0, 0x00,
    0x00, 0xff, 0xff, 0x1c, 0x1c, 0xf0, 0xc0, 0x00, 0x00, 0xff, 0xff, 0x00, 0x33, 0xf3, 0xc0, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x00, 0xc0, 0xf3, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0xff, 0xff, 0x0e, 0x0e, 0x03, 0x00, 0x00, 0x00, 0xff, 0xff, 0x0e,
    0x06, 0x86, 0xc0, 0x00, 0x00, 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0xff,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0xff, 0xff, 0x06, 0x06,
    0x06, 0x0c, 0x00, 0x00, 0xff, 0xff, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0xff, 0xff, 0x30, 0x30, 0x30, 0x18, 0x00,
    0x00, 0xff, 0xff, 0x0c, 0x18, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0x18, 0x0c, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0xff, 0xff, 0xc0, 0xc0, 0xf8,
    0xf8, 0x00, 0x00, 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x80, 0x87, 0x87, 0x00, 0x00,
    0xff, 0xff, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x66,
    0x00, 0x00, 0xff, 0xff, 0x1e, 0x33, 0x71, 0x7c, 0x00, 0x00, 0xff, 0xff, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0xff,
    0xff, 0xd8, 0xd8, 0xfe, 0x6c, 0x00, 0x00, 0xff, 0xff, 0x6c, 0x6c, 0xff, 0x36, 0x00, 0x00, 0xff, 0xff, 0x66, 0x3c,
    0x01, 0x33, 0x00, 0x00, 0xff, 0xff, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x36, 0x36, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x81, 0x80, 0x00, 0x00, 0xff, 0xff,
    0xc0, 0xc0, 0x60, 0x60, 0x00, 0x00, 0xff, 0xff, 0xc3, 0xc3, 0x81, 0x81, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3,
    0xc3, 0xc3, 0xc3, 0x00, 0x00, 0xff, 0xff, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0x06, 0x06, 0x0c, 0x1c,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xd8, 0x98, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x30, 0x19, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x0c, 0x1c, 0x38, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x6c, 0x6c, 0x6c, 0x6c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x80, 0x00, 0x00, 0xff, 0xff, 0x19, 0x1b, 0x1f, 0x0e, 0x00, 0x00,
    0xff, 0xff, 0x8c, 0xcc, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x0f, 0x06, 0x06, 0x0f, 0x00, 0x00, 0xff, 0xff, 0x19,
    0x30, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x87, 0x01, 0x01,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x07, 0x06, 0x06, 0x00, 0x00, 0xff,
    0xff, 0x36, 0x36, 0x3c, 0x1c, 0x00, 0x00, 0xff, 0xff, 0x18, 0x18, 0x1c, 0x0c, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x60,
    0x30, 0x18, 0x00, 0x00, 0xff, 0xff, 0x0c, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x0e, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x00,
    0xf0, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0x66, 0x66, 0xc6, 0x86, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0xff, 0xff, 0x00,
    0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0x00, 0x00, 0xff, 0xff, 0x8c, 0x8c, 0x86, 0x83,
    0x00, 0x00, 0xff, 0xff, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0x31, 0x31, 0x3b, 0x1f, 0x00, 0x00, 0xff,
    0xff, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0x07, 0x07, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x0c, 0x0c, 0x0c, 0x00,
    0x00, 0xff, 0xff, 0x00, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x03, 0xe3, 0xe0, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff,
    0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00,
    0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x00, 0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x0c, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x80, 0xc0, 0xc0, 0x00, 0x00, 0xff,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x0f, 0x1d, 0x18, 0x00, 0x00, 0xff, 0xff, 0x01, 0x01,
    0x01, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0xff, 0x06, 0x06, 0x06, 0x06, 0x00,
    0x00, 0xff, 0xff, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0xff, 0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x1c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0xff, 0xff, 0x0c,
    0x06, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0xff,
    0xff, 0x03, 0x03, 0x86, 0x06, 0x00, 0x00, 0xff, 0xff, 0x0e, 0x06, 0x06, 0x06, 0x00, 0x00, 0xff, 0xff, 0x06, 0x06,
    0x03, 0x06, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x0c, 0x18, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x06, 0x06, 0x06, 0x0e, 0x00, 0x00, 0xff, 0xff, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0x1c, 0x18, 0x18, 0x18, 0x00, 0x00, 0xff, 0xff, 0x18, 0x18, 0x70, 0x18, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x18, 0x18, 0x18, 0x1c, 0x00, 0x00,
    0xff, 0xff, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00,
    0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xc0, 0xc0,
    0x00, 0x00, 0xff, 0xff, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0xff,
    0xff, 0xc0, 0x60, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0xff, 0xff, 0x18, 0x1c, 0x0e, 0x07, 0x00, 0x00, 0xff, 0xff,
    0x03, 0x00, 0x03, 0x03, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
    0xcc, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xe0, 0x30, 0x18, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x03, 0x06, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, 0x00, 0x00, 0x00,
    0x00, 0xf0, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0xcc, 0x66, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x1f, 0x1f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0x8a,
    0x9a, 0x22, 0x12, 0xaa, 0x5a, 0x9e, 0x9a, 0x8c, 0x00, 0x00, 0x00, 0x88, 0x48, 0x00, 0x30, 0x69, 0x41, 0x00, 0x00,
    0x66, 0x36, 0x61, 0x11, 0x19, 0x00, 0x22, 0x12, 0xee, 0x7e, 0x00, 0x00, 0x00, 0x4a, 0x12, 0x01, 0xbb, 0x5b, 0x5a,
    0xda, 0x5a, 0x4a, 0x00, 0x00, 0xbb, 0x6b, 0x00, 0x00, 0x51, 0x0c, 0x01, 0x00, 0xcc, 0x6c, 0x08, 0x39, 0x08, 0x08,
    0x00, 0x00, 0xee, 0x7e, 0x00, 0x87, 0xc5, 0xa6, 0x10, 0x01, 0xcc, 0x6c, 0x94, 0xf4, 0x87, 0x00, 0x00, 0x00, 0xbb,
    0x6b, 0x00, 0x1c, 0x84, 0x84, 0x00, 0x00, 0xcc, 0x6c, 0x90, 0x90, 0x94, 0x00, 0x10, 0x01, 0xff, 0xff, 0x00, 0x00,
    0x00, 0xa5, 0x12, 0x11, 0xcc, 0x6c, 0xa1, 0xe1, 0xa5, 0xa5, 0x22, 0x12, 0xdd, 0x7d, 0x01, 0x00, 0x14, 0x0c, 0x01,
    0x00, 0xee, 0x7e, 0x08, 0x08, 0x08, 0x88, 0x00, 0x00, 0xff, 0xff, 0x01, 0x00, 0x00, 0x40, 0x01, 0x00, 0xff, 0xff,
    0x4a, 0x5b, 0x5b, 0x5b, 0x10, 0x01, 0x88, 0x48, 0x01, 0x00, 0x00, 0x50, 0x01, 0x00, 0x66, 0x36, 0x5a, 0x0b, 0x31,
    0x41, 0x22, 0x12, 0xff, 0xff, 0x4a, 0x18, 0x00, 0x00, 0x00, 0x00, 0xdd, 0x6d, 0x00, 0x18, 0xde, 0x5a, 0x00, 0x00,
    0xaa, 0x5a, 0x6a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0x69, 0x00, 0x30, 0x79, 0x12, 0x10, 0x01, 0xde, 0xfb, 0x01,
    0x00, 0x00, 0x01, 0x01, 0x00, 0xee, 0x7e, 0xa2, 0xa2, 0xa2, 0xa2, 0x12, 0x11, 0xcc, 0x6c, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x52, 0x54, 0xd4, 0xdc, 0x10, 0x01, 0xff, 0xff, 0xa2, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0x00, 0x20, 0xe6, 0xa5, 0x12, 0x11, 0xee, 0x7e, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x08, 0x08,
    0x8c, 0x08, 0x00, 0x00, 0xff, 0xff, 0x00, 0x7b, 0x42, 0x63, 0x00, 0x00, 0x88, 0x48, 0x22, 0x22, 0x12, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00, 0xc6, 0x29, 0x26, 0x10, 0x01, 0xcc, 0x6c, 0xef, 0x09, 0xef, 0x00, 0x00, 0x00, 0x99, 0x59,
    0x00, 0x00, 0x00, 0x6a, 0x12, 0x11, 0xee, 0x7e, 0x2a, 0x3a, 0x2a, 0x4a, 0x10, 0x01, 0xff, 0xff, 0x01, 0x00, 0x11,
    0x30, 0x01, 0x00, 0xee, 0x7e, 0x61, 0xa1, 0x21, 0x21, 0x12, 0x11, 0xbb, 0x6b, 0x00, 0x80, 0x99, 0x99, 0x12, 0x01,
    0xdd, 0x6d, 0xb9, 0xbd, 0xa4, 0x00, 0x10, 0x01, 0xcc, 0x6c, 0x00, 0x63, 0x12, 0x12, 0x12, 0x11, 0xcc, 0x6c, 0x13,
    0x12, 0x62, 0x00, 0x00, 0x00, 0xee, 0x7e, 0x00, 0x00, 0x00, 0xad, 0x12, 0x11, 0xa9, 0x5a, 0xbd, 0xb5, 0xa5, 0xa5,
    0x44, 0x24, 0xcc, 0x6c, 0x00, 0x00, 0x00, 0x52, 0x22, 0x12, 0xee, 0x7e, 0xd2, 0x52, 0x12, 0x13, 0x10, 0x01, 0xdd,
    0x7d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x54, 0x34, 0x25, 0x37, 0x12, 0x01, 0xde, 0xfb, 0x01, 0x00,
    0x00, 0x40, 0x01, 0x00, 0xff, 0xff, 0x09, 0x85, 0xc6, 0x42, 0x10, 0x01, 0xdd, 0x7d, 0x52, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xee, 0x7e, 0x01, 0x09, 0x29, 0x19, 0x00, 0x00, 0xff, 0xff, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbb, 0x5b,
    0x00, 0x02, 0x42, 0xa2, 0x10, 0x01, 0xcc, 0x6c, 0x01, 0x00, 0x00, 0x40, 0x01, 0x00, 0x77, 0x37, 0x81, 0x98, 0xa0,
    0xa4, 0x22, 0x12, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x63, 0x12, 0x16, 0x00, 0x00,
    0xbb, 0x6b, 0xb4, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x5a, 0x00, 0x00, 0x1d, 0xa5, 0x22, 0x12, 0xee, 0x7e, 0xf3,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x36, 0x00, 0x00, 0x71, 0xd2, 0x00, 0x00, 0xdd, 0x6d, 0x5a, 0xde, 0x00, 0x00,
    0x00, 0x00, 0x77, 0x47, 0x01, 0x00, 0x00, 0xce, 0x01, 0x00, 0xbb, 0x6b, 0x10, 0x13, 0x00, 0x00, 0x00, 0x00, 0xdd,
    0x6d, 0x00, 0x00, 0x00, 0x33, 0x10, 0x01, 0xdd, 0x7d, 0xda, 0xda, 0xde, 0x18, 0x00, 0x00, 0x88, 0x48, 0x18, 0x00,
    0x00, 0x87, 0x00, 0x00, 0x88, 0x48, 0x08, 0x70, 0x68, 0x00, 0x10, 0x01, 0xdd, 0x6d, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0xde, 0xfb, 0xa5, 0xa5, 0x04, 0x06, 0x00, 0x00, 0xee, 0x7e, 0x00, 0x00, 0x02, 0xa7, 0x00, 0x00, 0xcc, 0x6c,
    0x08, 0x08, 0x00, 0x80, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0xde, 0xfb, 0xa2, 0xa2, 0xa2,
    0x00, 0x12, 0x11, 0xdd, 0x7d, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0xaa, 0x5a, 0x54, 0xd4, 0x88, 0x00, 0x10, 0x01,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbb, 0x6b, 0x97, 0x8c, 0x5a, 0x50, 0x44, 0x24, 0xee, 0x7e, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x49, 0x31, 0x79, 0x03, 0x00, 0x00, 0x99, 0x59, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0xff, 0xff, 0x00, 0xd8, 0x8e, 0x8e, 0x00, 0x00, 0x88, 0x48, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc,
    0x6c, 0x20, 0x20, 0x83, 0x83, 0x10, 0x01, 0xbb, 0x5b, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x88, 0x58, 0xc1, 0x03,
    0xd1, 0x03, 0x01, 0x00, 0xff, 0xff, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x48, 0x08, 0x3c, 0x08, 0x08, 0x22,
    0x12, 0xee, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc6, 0xed, 0xae, 0xfa, 0x22, 0x12, 0xdd, 0x7d,
    0x6a, 0x67, 0x07, 0x00, 0x00, 0x00, 0x99, 0x49, 0x90, 0x91, 0x18, 0x08, 0x00, 0x00, 0xaa, 0x5a, 0x0c, 0x04, 0x00,
    0x00, 0x00, 0x00, 0x66, 0x36, 0x29, 0x29, 0x01, 0x01, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x4c, 0x00, 0x00,
    0xcc, 0x7c, 0xa2, 0xa6, 0x00, 0x00, 0x00, 0x00, 0xdd, 0x7d, 0x00, 0x00, 0x00, 0xe9, 0x00, 0x00, 0xcc, 0x6c, 0x35,
    0x27, 0x52, 0x00, 0x10, 0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0xff, 0xff, 0x86, 0x46, 0x24, 0x06,
    0x12, 0x11, 0xbb, 0x6b, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xa5, 0xa5, 0x00, 0x00, 0x10, 0x01, 0xcc,
    0x6c, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0xff, 0xff, 0xd2, 0xf2, 0x10, 0x10, 0x00, 0x00, 0xdd, 0x7d, 0x00, 0x00,
    0x60, 0x63, 0x00, 0x00, 0x88, 0x48, 0x00, 0x98, 0x00, 0x00, 0x10, 0x01, 0xff, 0xff, 0x00, 0x00, 0x10, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x60, 0x63, 0x60, 0x00, 0x00, 0x00, 0x99, 0x49, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0xcc, 0x6c,
    0x21, 0x21, 0x21, 0x21, 0x00, 0x00, 0xff, 0xff, 0x21, 0x00, 0x00, 0x00, 0x12, 0x01, 0xff, 0xff, 0x00, 0x42, 0x00,
    0x42, 0x10, 0x01, 0x55, 0x35, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x6c, 0x20, 0x21, 0x21, 0x33, 0x00, 0x00,
    0xbb, 0x6b, 0x22, 0x26, 0x60, 0x00, 0x00, 0x00, 0xcc, 0x6c, 0x06, 0x04, 0x04, 0x0c, 0x00, 0x00, 0xff, 0xff, 0x04,
    0x04, 0x06, 0xe0, 0x00, 0x00, 0x99, 0x59, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00, 0xee, 0x6e, 0x08, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x66, 0x36, 0x40, 0x60, 0x30, 0x31, 0x00, 0x00, 0x88, 0x48, 0x11, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x18, 0x24, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xf0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x0a,
    0x00, 0x00, 0x00, 0x00, 0xff, 0x7f, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x9b, 0x57, 0x52, 0x22,
    0x12, 0x65, 0x36, 0x00, 0x0b, 0x6d, 0x2f, 0x00, 0x00, 0x77, 0x47, 0x00, 0x2a, 0x4e, 0x23, 0x00, 0x00, 0xcc, 0x6c,
    0x00, 0x23, 0x0d, 0x07, 0x10, 0x01, 0x44, 0x24, 0x00, 0x00, 0x5b, 0x5d, 0x22, 0x12, 0x77, 0x47, 0x1b, 0x00, 0x00,
    0xda, 0x12, 0x11, 0xbb, 0x6b, 0x00, 0x00, 0xc0, 0xe0, 0x22, 0x12, 0xff, 0xff, 0xa0, 0x00, 0x00, 0x2b, 0x00, 0x00,
    0xdd, 0x7d, 0x00, 0xb8, 0x10, 0x24, 0x34, 0x13, 0x88, 0x48, 0x00, 0x18, 0x94, 0x98, 0x00, 0x00, 0xaa, 0x5a, 0x90,
    0x75, 0x75, 0x90, 0x22, 0x12, 0x88, 0x48, 0x00, 0xdb, 0xff, 0x7b, 0x00, 0x00, 0x66, 0x36, 0x00, 0x00, 0x1e, 0xa3,
    0x12, 0x01, 0xcc, 0x7c, 0x8c, 0x00, 0x01, 0x81, 0x10, 0x01, 0xdd, 0x6d, 0x00, 0x00, 0x11, 0x78, 0x00, 0x00, 0x76,
    0x37, 0xbe, 0x00, 0x00, 0xd7, 0x10, 0x01, 0x89, 0x48, 0x49, 0x00, 0x00, 0x00, 0x10, 0x01, 0x66, 0x36, 0x2f, 0xef,
    0x06, 0x00, 0x00, 0x00, 0x66, 0x36, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xe5, 0x25, 0x00, 0x00, 0x10,
    0x01, 0x66, 0x36, 0x1f, 0x5f, 0x18, 0x00, 0x22, 0x12, 0x88, 0x48, 0x00, 0x1e, 0x9e, 0x00, 0x00, 0x00, 0x56, 0x35,
    0x08, 0x69, 0x08, 0x00, 0x00, 0x00, 0xcc, 0x6c, 0x08, 0x4b, 0x2f, 0x23, 0x22, 0x12, 0x88, 0x48, 0x01, 0x01, 0x00,
    0x00, 0x12, 0x01, 0xff, 0xff, 0x27, 0xac, 0x20, 0x00, 0x00, 0x00, 0xaa, 0x5a, 0x7b, 0x40, 0x00, 0x80, 0x00, 0x00,
    0x77, 0x47, 0x91, 0x90, 0x00, 0x00, 0x00, 0x00, 0x55, 0x25, 0x01, 0x01, 0x01, 0x00, 0x12, 0x11, 0xff, 0xff, 0x00,
    0x21, 0x20, 0x20, 0x10, 0x01, 0xdd, 0x7d, 0xc0, 0xc0, 0x14, 0x00, 0x00, 0x00, 0x44, 0x24, 0x64, 0x62, 0x00, 0x00,
    0x00, 0x00, 0x33, 0x13, 0x11, 0xbd, 0x10, 0x37, 0x10, 0x01, 0x33, 0x23, 0x01, 0xda, 0x06, 0x00, 0x13, 0x11, 0xba,
    0x5b, 0xb0, 0xf6, 0x00, 0xf6, 0x12, 0x11, 0x88, 0x48, 0x00, 0x56, 0xe2, 0x01, 0x00, 0x00, 0x88, 0x48, 0x1a, 0x00,
    0x80, 0x00, 0x22, 0x12, 0xbb, 0x5b, 0x16, 0x06, 0x00, 0xa6, 0x01, 0x00, 0xaa, 0x6a, 0xd1, 0x00, 0x13, 0x00, 0x10,
    0x01, 0x88, 0x48, 0x01, 0x01, 0x20, 0x09, 0x12, 0x11, 0x22, 0x12, 0x45, 0x50, 0x00, 0x41, 0x11, 0x11, 0xcc, 0x6c,
    0xb9, 0x00, 0x31, 0x00, 0x13, 0x01, 0xbb, 0x6b, 0x45, 0x50, 0x00, 0x41, 0x13, 0x11, 0xcc, 0x6c, 0xb9, 0x10, 0x31,
    0x00, 0x11, 0x01, 0x77, 0x47, 0x00, 0x00, 0x66, 0x44, 0x32, 0x13, 0x77, 0x47, 0x00, 0x00, 0x66, 0x44, 0x22, 0x12,
    0x88, 0x48, 0x00, 0x00, 0x66, 0x44, 0x22, 0x12, 0x88, 0x48, 0x00, 0x00, 0x66, 0x44, 0x32, 0x13, 0x77, 0x47, 0x00,
    0xff, 0x00, 0xff, 0x22, 0x12, 0x33, 0x13, 0x00, 0xff, 0x00, 0xff, 0x12, 0x01, 0x44, 0x24, 0x00, 0xff, 0x00, 0xff,
    0x12, 0x01, 0x44, 0x24, 0x00, 0xff, 0x00, 0xff, 0x22, 0x12, 0x33, 0x13, 0xff, 0xff, 0xff, 0xff, 0x32, 0x13, 0x33,
    0x13, 0xff, 0xff, 0xff, 0xff, 0x32, 0x13, 0x33, 0x13, 0xff, 0xff, 0xff, 0xff, 0x32, 0x13, 0x33, 0x13, 0xff, 0xff,
    0xff, 0xff, 0x32, 0x13, 0x33, 0x13, 0x00, 0xff, 0x00, 0xff, 0x22, 0x12, 0x33, 0x13, 0xff, 0xff, 0xff, 0xff, 0x32,
    0x13, 0x33, 0x13, 0xff, 0xff, 0xff, 0xff, 0x0b, 0x00, 0x00, 0x00, 0x78, 0xe1, 0xff, 0xff, 0x78, 0xe1, 0xff, 0xff,
    0xa4, 0xe1, 0xff, 0xff, 0xa4, 0xe1, 0xff, 0xff, 0xa4, 0xe1, 0xff, 0xff, 0xa4, 0xe1, 0xff, 0xff, 0xa4, 0xe1, 0xff,
    0xff, 0xa4, 0xe1, 0xff, 0xff, 0xa4, 0xe1, 0xff, 0xff, 0xa4, 0xe1, 0xff, 0xff, 0xa4, 0xe1, 0xff, 0xff, 0x4d, 0x45};

NUQFNT *system_qfont;

NUQFNT_CSMODE NuQFntCSMode;
i32 NuQFntCSModeStackIndex;
NUQFNT_CSMODE NuQFntCSModeStack[16];

f32 qfnt_rezscale_w = 1.0f;
f32 qfnt_rezscale_h = 1.0f;

f32 qfnt_offscale_x;
f32 qfnt_offscale_y;

f32 qfnt_len_scale = 1.0f;
f32 qfnt_height_scale = 1.0f;

static f32 justify_stretch = 1.0f;
static f32 justify_squash = 1.0f;
f32 nuqfnt_space_width;
u32 NuQFntMode;

static i32 g_buttonsFont;

DECOMP_ASSERT(sizeof(VUFNT) == 0x4c, "VUFNT size");
DECOMP_ASSERT(offsetof(VUFNT, glyphs) == 0x34, "VUFNT glyphs offset");
DECOMP_ASSERT(offsetof(VUFNT, unicode_map) == 0x38, "VUFNT Unicode map offset");
DECOMP_ASSERT(offsetof(VUFNT, platform_data) == 0x48, "VUFNT platform data offset");
DECOMP_ASSERT(sizeof(VUFNT_ANDROID) == 0x58, "VUFNT_ANDROID size");
DECOMP_ASSERT(offsetof(VUFNT_ANDROID, x_scale) == 0x50, "VUFNT_ANDROID x scale offset");
DECOMP_ASSERT(offsetof(VUFNT_ANDROID, y_scale) == 0x54, "VUFNT_ANDROID y scale offset");

u32 NuQFntModeStack[16];
i32 NuQFntModeStackIndex;

struct NuBloomParameters;
struct _vuv_s;

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

// ---------------------------------------------------------------------------
// Scene state — mirrors original BSS layout
// ---------------------------------------------------------------------------

static constexpr i32 kSceneRingCapacity = 16;

extern "C" {
    i32 PS2_REZ_W = 1280;
    i32 PS2_REZ_H = 720;
    // Scene currently being built.  Size is 0x218 bytes; original lives in BSS
    // and is referenced as a plain object by all render/present code.
    struct nurenderscene_s currentScene = {0};

    // Deferred ring: NuRndrEndScene copies the completed scene here; the render
    // thread drains it.  Stride is 0x218, 16 slots (0x2180 bytes total) in the
    // original.
    i32 sceneParametersCount = 0;
    struct nurenderscene_s sceneParameters[kSceneRingCapacity] = {0};
} // extern "C"

// Scene / GScn
using NUGSCNVIDEOMEMFN = void (*)(NUGSCN *);

NUGSCNVIDEOMEMFN gscene_to_video_mem;
NUGSCNVIDEOMEMFN video_mem_to_gscene;

class NuDynamicLight;
extern "C" {
    static void DisplaySceneSetClipResult(NUDLDLISTSCENE *scene, i32 clip_index, i32 clip_result) {
        for (i32 i = 0; i < scene->clip_objects[clip_index].nmaterials; ++i) {
            const u32 material = scene->clip_objects[clip_index].material_ids[i];
            scene->mtl_used[NuDisplaySceneCurrentBuffer(scene)][material >> 3] |= static_cast<u8>(1U << (material & 7));
        }
        scene->clip_used[NuDisplaySceneCurrentBuffer(scene)][clip_index >> 2] |=
            static_cast<u8>(clip_result << ((clip_index & 3) * 2));
    }

    static f32 DisplaySceneDistanceSqrToCamera(const NUCLIPBOUNDS &bounds) {
        const f32 x = global_camera.mtx.m30 - bounds.center.x;
        const f32 y = global_camera.mtx.m31 - bounds.center.y;
        const f32 z = global_camera.mtx.m32 - bounds.center.z;
        return x * x + y * y + z * z;
    }

    // A scene has one bound/visibility record per instance, but one or more
    // clip-object records per instance.  lod_ranges terminates every instance
    // group with zero; preceding values select progressively nearer LODs.
    static i32 DisplaySceneSelectLod(const NUDLDLISTSCENE *scene, i32 first_clip, f32 distance_sqr) {
        i32 selected_clip = first_clip;
        if (scene->lod_ranges[selected_clip] != 0.0f && distance_sqr < scene->lod_ranges[selected_clip]) {
            do {
                ++selected_clip;
            } while (distance_sqr < scene->lod_ranges[selected_clip]);
        }
        return selected_clip;
    }

    static i32 DisplaySceneNextInstance(const NUDLDLISTSCENE *scene, i32 clip_index) {
        while (scene->lod_ranges[clip_index] != 0.0f) {
            ++clip_index;
        }
        return clip_index + 1;
    }

    static f32 DisplaySceneFarClip(const NUDLDLISTSCENE *scene, i32 instance_index, i32 clip_index) {
        f32 far_clip = scene->far_clip_ranges[clip_index];
        if (scene->fade_ranges == NULL ||
            (scene->visibility_flags[instance_index] & NUDL_INSTANCE_FLAG_DISTANCE_FADE) == 0) {
            return far_clip;
        }

        const f32 fade_start = scene->fade_ranges[instance_index * 2];
        const f32 fade_end = scene->fade_ranges[instance_index * 2 + 1];
        if (fade_end > fade_start) {
            return fade_end;
        }
        return far_clip;
    }

    static f32 DisplaySceneDistanceFade(const NUDLDLISTSCENE *scene, i32 instance_index, f32 distance_sqr) {
        if (scene->fade_ranges == NULL ||
            (scene->visibility_flags[instance_index] & NUDL_INSTANCE_FLAG_DISTANCE_FADE) == 0) {
            return 1.0f;
        }

        const f32 fade_start = scene->fade_ranges[instance_index * 2];
        const f32 fade_end = scene->fade_ranges[instance_index * 2 + 1];
        if (fade_end <= fade_start) {
            if (fade_end * fade_end < distance_sqr) {
                const f32 alpha = (NuFsqrt(distance_sqr) - fade_end) / (fade_start - fade_end);
                return alpha < 1.0f ? alpha : 1.0f;
            }
            return 0.0f;
        }

        if (fade_start * fade_start < distance_sqr) {
            const f32 alpha = (fade_end - NuFsqrt(distance_sqr)) / (fade_end - fade_start);
            return alpha > 0.0f ? alpha : 0.0f;
        }
        return 1.0f;
    }

    extern nurenderscene_s currentScene;

    static __attribute__((noinline, noclone)) void NuDisplaySceneRndrBody(NUDLDLISTSCENE *scene)
        __asm__("NuDisplaySceneRndr.part.101");

}

// Material
static u32 mtl_animation_mask = 0x00ffffff;

static inline NUMTL *NuMtlAllocateBuff(VARIPTR *buffer) {
    NUMTL *material = reinterpret_cast<NUMTL *>(ALIGN(buffer->addr, 16));
    buffer->addr = reinterpret_cast<usize>(material + 1);
    memset(material, 0, sizeof(*material));
    DefaultMtl(material);
    material->unknown_0_4 = false;
    return material;
}

// Debug / visualisation geometry
static inline void NuRndrPrimSetColour(i32 colour);
static inline void NuRndrPrimUV(f32 u, f32 v);

f32 circle_scale_radius = 0.5f;

static i32 dump_state = -1;

extern "C" {
    i32 NuRndrStopUpdate;
    NUVEC NuRndrDebBase;
    NUVEC NuRndrDebRange;
    NUVEC NuRndrDebRangeInv;
}

extern "C" {
    i32 g_minmiplevel = 13;
    f32 g_mipmapbias;
}

f32 global_windspeed = 1.0f;
f32 global_windscale = 1.0f;

i32 global_GobjWasDrawnUnreflected;

struct nuvec4_s;

i32 nurndr_pixel_width;
i32 nurndr_pixel_height;
i32 nurndr_nforced_mtls;
NUMTL **nurndr_forced_mtl_table;
NUMTL *nurndr_forced_mtl;
i32 nurndr_force_lod;

i32 global_GobjIsReflectedGeometry;
i32 global_GobjIsShadowReceive;

u32 rndrstream_nbuffers = 2;
u32 rndrstream_last_used;
i32 nurndr_maxstreamsize;
VARIPTR rndrstream_end;
VARIPTR rndrstream_free;
static VARIPTR rndrstream[NURNDR_STREAM_MAX_BUFFERS];
static i32 rndrstream_buffid;
static u32 rndrstream_used;     // _ZL15rndrstream_used @0x11b8380
static u32 rndrstream_max_used; // _ZL19rndrstream_max_used @0x11b8390

static NUMTL *pZClearMaterial;
static NUMTL *pAlphaMask;
static const NUPRIMSCALEMODE kPRIM_COORD_LOOKUP[3] = {NUPRIM_SCALEMODE_PS2, NUPRIM_SCALEMODE_NORMALISED,
                                                      NUPRIM_SCALEMODE_ABSOLUTE};

NUCAMERA global_camera;
i32 cam_state_count;
NUCAMERASTATE cam_state[16];
NUVEC cam_axes = {1.0f, 1.0f, 1.0f};
i32 force_camera_farclip;
NUMTX cmtx = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};
NUMTX psmtx = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};
NUMTX vpsmtx = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};
f32 nucamera_farclip_hack = 0.1f;
NUMTX pc_vport_mtx = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

NUMTX clip_planes = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

NUMTX pmtx = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};
NUMTX smtx = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};
NUMTX vmtx = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

NUMTX vpmtx = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

NUMTX vpc_sci_mtx = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

NUMTX vpc_vport_mtx = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

NUCLIPPLANES ClipPlanes;

f32 zx;
f32 zy;

f32 zxs;
f32 zys;

extern "C" {
    i32 TreeInitialised;
    NUMTX CamSpaceFrustrumPlanes;
    NUMTX CamSpaceScissorPlanes;
    NUMTX FrustrumPlanes;
    NUMTX ScissorPlanes;
    NUVEC4 NearPlane;
    NUVEC4 AbsNearPlane;
    NUMTX AbsFrustrumPlanes;
    NUMTX AbsScissorPlanes;
}

static char buffer[0x1000]; // flush threshold leaves room for formatted output
static char *txt;           // original @0xb9d720-rel
static char *flush;         // original @0xb9d730-rel
extern "C" {
    NUFILE hfh;
    char unknown[8] = "unknown";
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

static f32 curx;
static f32 cury;
static f32 nextx;
static f32 dx;
i32 size;

extern "C" {
    u8 ForceScenePlayBack;
} // extern "C"

namespace {
    struct LegacyInstanceAnimationLink {
        NUMTX matrix;
        u8 pad_40[8];
        nuinstanim_s *animation;
    };

    DECOMP_ASSERT(sizeof(LegacyInstanceAnimationLink) == 0x4c, "legacy instance animation link ABI");
    DECOMP_ASSERT(offsetof(LegacyInstanceAnimationLink, animation) == 0x48, "legacy instance animation pointer offset");
} // namespace

#define EDBRI_USED_ITEM(name) static void name(eduimenu_s *, eduiitem_s *, u32) __attribute__((used))
#define EDBRI_USED_MENU(name) static void name(eduimenu_s *, eduimenu_s *) __attribute__((used))
EDBRI_USED_ITEM(edbricbFileLoad);
EDBRI_USED_ITEM(edbricbFileSave);
EDBRI_USED_ITEM(edbricbDpadModeMenu);
EDBRI_USED_ITEM(edbricbPlankCountMenu);
EDBRI_USED_ITEM(edbricbPostInstanceMenu);
EDBRI_USED_ITEM(edbricbPlankInstanceMenu);
EDBRI_USED_ITEM(edbricbBridgePropertiesMenu);
EDBRI_USED_MENU(edbricbCancelOptMenu);

// The original editor kept these local callback symbols in one translation unit.
// The current split leaves some menu entry paths in another unit, so retain the
// callbacks during -O3 reconstruction until that call graph is reconnected.
#define EDGRA_RETAIN_ITEM(name) static void name(eduimenu_s *, eduiitem_s *, u32) __attribute__((used))
#define EDGRA_RETAIN_MENU(name) static void name(eduimenu_s *, eduimenu_s *) __attribute__((used))
EDGRA_RETAIN_ITEM(edgracbFileLoad);
EDGRA_RETAIN_ITEM(edgracbFileSave);
EDGRA_RETAIN_ITEM(edgracbCopyClump);
EDGRA_RETAIN_ITEM(edgraToggleFilter);
EDGRA_RETAIN_ITEM(edgracbSScaleMenu);
EDGRA_RETAIN_ITEM(edgracbDiscardCopy);
EDGRA_RETAIN_ITEM(edgracbGlobalsMenu);
EDGRA_RETAIN_ITEM(edgracbSetDpadMode);
EDGRA_RETAIN_ITEM(edgracbChangeSScale);
EDGRA_RETAIN_ITEM(edgracbDpadModeMenu);
EDGRA_RETAIN_ITEM(edgracbInstanceMenu);
EDGRA_RETAIN_ITEM(edgracbSetClumpArea);
EDGRA_RETAIN_ITEM(edgracbSetClumpDist);
EDGRA_RETAIN_ITEM(edgracbSetClumpMode);
EDGRA_RETAIN_ITEM(edgracbSetClumpWind);
EDGRA_RETAIN_ITEM(edgracbClumpAreaMenu);
EDGRA_RETAIN_ITEM(edgracbClumpDistMenu);
EDGRA_RETAIN_ITEM(edgracbClumpFadeMenu);
EDGRA_RETAIN_ITEM(edgracbClumpModeMenu);
EDGRA_RETAIN_ITEM(edgracbClumpSizesMenu);
EDGRA_RETAIN_ITEM(edgracbSetClumpFadeIn);
EDGRA_RETAIN_ITEM(edgracbSetClumpHeight);
EDGRA_RETAIN_ITEM(edgracbApplyGlobalFade);
EDGRA_RETAIN_ITEM(edgracbSetClumpFadeOut);
EDGRA_RETAIN_ITEM(edgracbSetGlobalFadeIn);
EDGRA_RETAIN_ITEM(edgracbSetInstanceType);
EDGRA_RETAIN_ITEM(edgracbToggleClumpTilt);
EDGRA_RETAIN_ITEM(edgraChangeFilterName);
EDGRA_RETAIN_ITEM(edgracbClumpTerrainMenu);
EDGRA_RETAIN_ITEM(edgracbSetGlobalFadeOut);
EDGRA_RETAIN_ITEM(edgracbSetClumpMaxHeight);
EDGRA_RETAIN_ITEM(edgracbSetClumpMinHeight);
EDGRA_RETAIN_ITEM(edgracbChangeInstanceMenu);
EDGRA_RETAIN_ITEM(edgracbChangeInstanceType);
EDGRA_RETAIN_ITEM(edgracbToggleClumpTerrain);
EDGRA_RETAIN_ITEM(edgracbClumpPropertiesMenu);
EDGRA_RETAIN_ITEM(edgracbToggleClumpReactive);
EDGRA_RETAIN_ITEM(edgracbSetClumpTerrainOffset);
EDGRA_RETAIN_MENU(edgracbCancelOptMenu);
EDGRA_RETAIN_MENU(edgracbCancelSScaleMenu);
EDGRA_RETAIN_MENU(edgracbCancelGlobalsMenu);
EDGRA_RETAIN_MENU(edgracbCancelDpadModeMenu);
EDGRA_RETAIN_MENU(edgracbCancelInstanceMenu);
EDGRA_RETAIN_MENU(edgracbCancelClumpAreaMenu);
EDGRA_RETAIN_MENU(edgracbCancelClumpDistMenu);
EDGRA_RETAIN_MENU(edgracbCancelClumpFadeMenu);
EDGRA_RETAIN_MENU(edgracbCancelClumpModeMenu);
EDGRA_RETAIN_MENU(edgracbCancelClumpSizesMenu);
EDGRA_RETAIN_MENU(edgracbCancelClumpTerrainMenu);
EDGRA_RETAIN_MENU(edgracbCancelChangeInstanceMenu);
EDGRA_RETAIN_MENU(edgracbCancelClumpPropertiesMenu);

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

// Original 0x378fb0: the optimized rotating scan shares the terrain engine state.

// Graphics loading flags
i32 RemoveDirectionalMaps = 0;
i32 RemoveNormalMaps = 0;

NUVIDEORESHEADER g_VideoResHeader;

extern "C" {
    f32 aiEditor_LocatorWidth = 0.25f;
}

extern const u8 HintRGB[5][3] = {
    {255, 0, 0}, {0, 255, 0}, {0, 127, 255}, {127, 0, 255}, {255, 255, 0},
};

static u8 cacheValues[256];

static inline u16 AlphaImageFloatToHalf(f32 value) {
    u32 bits;
    memcpy(&bits, &value, sizeof(bits));
    i32 mantissa = bits & 0x7fffff;
    i32 exponent = static_cast<i32>((bits >> 23) & 0xff) - 0x70;
    i32 sign = bits >> 31;
    i32 half_exponent = 0;
    if (exponent >= 0) {
        half_exponent = 0x7c00;
        if (exponent < 0x20) {
            half_exponent = exponent * 0x400;
        }
    }
    return static_cast<u16>((mantissa >> 13) | (sign << 15) | half_exponent);
}

static inline void AlphaImagePrimUV(f32 u, f32 v) {
    PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);
    if (!g_NuPrim_NeedsHalfUVs) {
        vertex->float_uv[0] = u;
        vertex->float_uv[1] = v;
    } else {
        vertex->half_uv[0] = AlphaImageFloatToHalf(u);
        vertex->half_uv[1] = AlphaImageFloatToHalf(v);
    }
}

f32 PANEL_REDBRICKSCALE = 0.4f;
i32 DRAWBGLOAD = 0;
u16 PowerUp_PanelYRot[2] = {0, 0};
f32 PowerUp_PanelPosMul[2] = {0.0f, 0.0f};
f32 POWERUPOBJSIZE = 0.0f;
f32 REDBRICKPOSX = 0.0f;
f32 REDBRICKPOSY = -0.5f;
f32 REDBRICKPOS2X = 1.25f;
f32 REDBRICKPOS2Y = 0.0f;
i32 Arcade_Points[2] = {0};
f32 BOSSICONY = 0.0f;
i32 FPSDISPLAY = 0;
i32 ShowPlayerCoordinate = 0;

extern "C" {
    extern nurenderscene_s currentScene;
}

struct nutexanimprog_s;

nutexmanager_s *g_texman;
i32 streamOff;

i32 max_textures;
static NUNATIVETEX **texture_list;
static i32 *texture_order;
static i32 gTextureLoadCount;
static constexpr f32 kVirtualWidth = 640.0f;
static constexpr f32 kVirtualHeight = 224.0f;

NUVIEWPORT vpCurrent = {0};
NUVIEWPORT vpDevice = {0};
i32 vport_inval = 0;
NUMTX vp_smtx = {0}; // scaling matrix
NUMTX vp_cmtx = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};
NUVPREGION g_NuVpRegion = {
    0.0f, 0.0f, kVirtualWidth, kVirtualHeight, 0.0f, 0.0f, kVirtualWidth, kVirtualHeight,
    1.0f, 1.0f, 0.0f,          0.0f,           1.0f, 1.0f, 0.0f,          0.0f,
};

i32 draw_octree = 1;

void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled);

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

static i32 max_materials;
static NUMTL *material_list;
static NULSTHDR *g_overrideList;
DECOMP_ASSERT(offsetof(NUMTL, shader_desc) + offsetof(NUSHADERMTLDESC, vtx_desc) == 0x1f0,
              "Material override vertex descriptor offset");

static NUMTL *FindOverrideMaterial(NUMTL *original, const NUVERTEXDESCRIPTOR &descriptor, NUMTL *fallback = NULL) {
    for (NULNKHDR *node = NuLstGetNext(g_overrideList, NULL); node != NULL; node = NuLstGetNext(g_overrideList, node)) {
        MTL_OVERRIDE_RECORD *entry = reinterpret_cast<MTL_OVERRIDE_RECORD *>(node);
        if (entry->original == original && entry->material->shader_desc.vtx_desc.flags == descriptor.flags) {
            return entry->material;
        }
    }
    return fallback;
}

i32 numtl_renderplane;

NUMTL *numtl_defaultmtl2d;
NUMTL *numtl_defaultmtl3d;

void NuMainFilterGen::destroyTextureResources() {
    STUBBED();
}

void NuMainFilterGen::reset() {
    enabled = false;
    dof_enabled = false;
    bloom_enabled = false;
    motion_blur_enabled = false;
    active_filter_count = 0;
}

void NuCopyFilter::reset() {
    input_fbo = copy_fbo;
    copy_texture = NULL;
}

void NuMotionAccumFilterGen::destroyTextureResources() {
    STUBBED();
}

void NuSpeedBlurFilterGen::destroyTextureResources() {
    STUBBED();
}

i32 OccluderSet::SortByDepth(void const *left, void const *right) {
    i32 a = *static_cast<const i32 *>(left);
    i32 b = *static_cast<const i32 *>(right);
    if (a == -1)
        return 1;
    if (b == -1)
        return -1;
    OccluderRecord *records = g_OcclusionManager.current_set->occluders;
    return records[b].depth > records[a].depth ? -1 : 1;
}

void NuDeferredFilterGen::destroyTextureResources() {
    for (i32 i = 0; i < 4; ++i)
        textures[i] = NULL;
}

static void pftaRet(nufpar_s *parser) {
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 12;
    parprog->instruction_count = index + 1;
}

static void pftaRepend(nufpar_s *parser) {
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 14;
    parprog->instruction_count = index + 1;
}

static void pftaEnd(nufpar_s *parser) {
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 16;
    parprog->instruction_count = index + 1;
}

static void transposeClipPlanes(NUFRUSTRUM *frustum) {
    f32 cam_a = cam_plane.a;
    frustum->transposed_planes[3] = cam_a;
    f32 cam_d = cam_plane.d;
    f32 cam_c = cam_plane.c;
    f32 cam_b = cam_plane.b;
    NUPLANE *planes = frustum->planes;
    frustum->transposed_planes[7] = cam_b;
    frustum->transposed_planes[11] = cam_c;
    frustum->transposed_planes[15] = cam_d;
    frustum->transposed_planes[16] = cam_a;
    frustum->transposed_planes[20] = cam_b;
    frustum->transposed_planes[24] = cam_c;
    frustum->transposed_planes[28] = cam_d;
    frustum->transposed_planes[17] = cam_a;
    frustum->transposed_planes[21] = cam_b;
    frustum->transposed_planes[25] = cam_c;
    frustum->transposed_planes[29] = cam_d;
    frustum->transposed_planes[18] = cam_a;
    frustum->transposed_planes[22] = cam_b;
    frustum->transposed_planes[26] = cam_c;
    frustum->transposed_planes[30] = cam_d;
    frustum->transposed_planes[19] = cam_a;
    frustum->transposed_planes[23] = cam_b;
    frustum->transposed_planes[27] = cam_c;
    frustum->transposed_planes[31] = cam_d;

    frustum->transposed_planes[0] = planes[0].a;
    frustum->transposed_planes[4] = planes[0].b;
    frustum->transposed_planes[8] = planes[0].c;
    frustum->transposed_planes[12] = planes[0].d;
    frustum->transposed_planes[1] = planes[1].a;
    frustum->transposed_planes[5] = planes[1].b;
    frustum->transposed_planes[9] = planes[1].c;
    frustum->transposed_planes[13] = planes[1].d;
    frustum->transposed_planes[2] = planes[2].a;
    frustum->transposed_planes[6] = planes[2].b;
    frustum->transposed_planes[10] = planes[2].c;
    frustum->transposed_planes[14] = planes[2].d;
    if (frustum->plane_count <= 3) {
        return;
    }
    frustum->transposed_planes[3] = planes[3].a;
    frustum->transposed_planes[7] = planes[3].b;
    frustum->transposed_planes[11] = planes[3].c;
    frustum->transposed_planes[15] = planes[3].d;
    if (frustum->plane_count == 4) {
        return;
    }
    frustum->transposed_planes[16] = planes[4].a;
    frustum->transposed_planes[20] = planes[4].b;
    frustum->transposed_planes[24] = planes[4].c;
    frustum->transposed_planes[28] = planes[4].d;
    if (frustum->plane_count == 5) {
        return;
    }
    frustum->transposed_planes[17] = planes[5].a;
    frustum->transposed_planes[21] = planes[5].b;
    frustum->transposed_planes[25] = planes[5].c;
    frustum->transposed_planes[29] = planes[5].d;
    if (frustum->plane_count == 6) {
        return;
    }
    frustum->transposed_planes[18] = planes[6].a;
    frustum->transposed_planes[22] = planes[6].b;
    frustum->transposed_planes[26] = planes[6].c;
    frustum->transposed_planes[30] = planes[6].d;
    if (frustum->plane_count == 7) {
        return;
    }
    frustum->transposed_planes[19] = planes[7].a;
    frustum->transposed_planes[23] = planes[7].b;
    frustum->transposed_planes[27] = planes[7].c;
    frustum->transposed_planes[31] = planes[7].d;
}

static void DisplayListMaterialClipUpdate(nudisplayscene_s *scene) {
    if (scene == NULL || scene->mtls == NULL || scene->mtls[0] == NULL)
        return;
    const u8 buffer = NuDisplaySceneCurrentBuffer(scene);
    const u8 *current = scene->mtl_used[buffer];
    const u8 *previous = scene->mtl_used[buffer ^ 1];
    const i32 count = (scene->nmtls + 7) >> 3;
    NUDISPLAYLIST **lists = scene->dlist_mtls;
    for (i32 byte_index = 0, material_index = 0; byte_index < count; ++byte_index, material_index += 8) {
        const u8 bits = current[byte_index];
        if (previous[byte_index] == bits)
            continue;
        for (i32 bit = 0; bit < 8; ++bit) {
            const i32 index = material_index + bit;
            i32 enabled = (bits >> bit) & 1;
            NUDISPLAYLIST *list = lists[index];
            NUMTL *material = scene->mtls[list->mtl_id];
            if (enabled && material != NULL && MTL_BLEND_FLAG(material) != 0)
                enabled = MTL_BLEND_OP2(material) != 0xff;
            if (enabled)
                list->mtl_item->id = 0;
            else
                list->mtl_item->id = 1;
            if (static_cast<i32>(scene->nmtls) <= index + 1)
                return;
        }
    }
}

void NuDeferredFilterGen::initTextureResources(i32 width, i32 height) {
    textures[2] = NuEffectTexCreate2D(768, 1344, 1, 1, 4);
    textures[3] = NuEffectTexCreate2D(640, 2048, 1, 1, 4);
    textures[4] = textures[5] = NULL;
    textures[0] = NuEffectTexCreate2D(width / 2, height, sample_count, 1, 2);
    textures[1] = NuEffectTexCreate2D(1, 1, 1, 0, 2);
}

void NuSpeedBlurFilterGen::initTextureResources(i32 width, i32 height) {
    texture = NuEffectTexCreate2D(width / 2, height / 2, 1, 1, 2);
}

void NuMotionAccumFilterGen::initTextureResources(i32 width, i32 height) {
    accumulation_texture = NuEffectTexCreate2D(width, height, 1, 1, 2);
}

static void BoxTreeRndrRec(nuvisiboxtree_s *tree, unsigned char *results, nuvisiboxtreenode_s *node, int instance_count,
                           float far_clip, nugscn_s *scene) {
    if (node->children[0] == 0xffff) {
        i32 clip = NuCameraClipTestExtents(&node->minimum, &node->maximum, &numtx_identity, far_clip, 0);
        i32 instance = tree->instances[node->first_instance];
        results[instance >> 2] |= clip << ((instance & 3) * 2);
    } else {
        i32 clip = NuCameraClipTestExtents(&node->minimum, &node->maximum, &numtx_identity, far_clip, 1);
        if (clip == 0)
            return;
        if (clip == 1) {
            for (i32 i = 0; i < node->instance_count; ++i) {
                i32 instance = tree->instances[node->first_instance + i];
                results[instance >> 2] |= 1 << ((instance & 3) * 2);
            }
        } else {
            BoxTreeRndrRec(tree, results, &tree->nodes[node->children[0]], instance_count, far_clip, scene);
            BoxTreeRndrRec(tree, results, &tree->nodes[node->children[1]], instance_count, far_clip, scene);
        }
    }
}

static void pftaXDef(nufpar_s *parser) {
    NuFParGetWord(parser);
    i32 label = NuTexAnimLabelIndex(parser->word_buf, xdeflabtab, &xdeflabtabcnt);
    i32 index = parprog->label_count;
    parprog->label_ids[index] = static_cast<i16>(label);
    parprog->label_offsets[index] = parprog->instruction_count;
    parprog->label_count = index + 1;
}

static void pftaScriptname(nufpar_s *parser) {
    NuFParGetWord(parser);
    parser->word_buf[32] = '\0';
    strcpy(parprog->name, parser->word_buf);
}

static void pftaRepeat(nufpar_s *parser) {
    i32 value0 = NuFParGetInt(parser);
    if (value0 == 0)
        value0 = -1;
    i32 value1 = NuFParGetInt(parser);
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 13;
    parprog->instructions[static_cast<i16>(index + 1)] = static_cast<i16>(value0);
    parprog->instructions[static_cast<i16>(index + 2)] = static_cast<i16>(value1);
    parprog->instruction_count = index + 3;
}

static void pftaOff(nufpar_s *parser) {
    i32 signal = NuFParGetInt(parser);
    parprog->off_signal[signal] = parprog->instruction_count;
    parprog->off_mask |= 1u << signal;
}

static void pftaOn(nufpar_s *parser) {
    i32 signal = NuFParGetInt(parser);
    parprog->on_signal[signal] = parprog->instruction_count;
    parprog->on_mask |= 1u << signal;
}

static void pftaRate(nufpar_s *parser) {
    i32 value0 = NuFParGetInt(parser);
    i32 value1 = NuFParGetInt(parser);
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 7;
    parprog->instructions[static_cast<i16>(index + 1)] =
        static_cast<i16>(static_cast<i32>(value0 * (1.0f / 60.0f) * 4096.0f));
    parprog->instructions[static_cast<i16>(index + 2)] =
        static_cast<i16>(static_cast<i32>(value1 * (1.0f / 60.0f) * 4096.0f));
    parprog->instruction_count = index + 3;
}

static void pftaWait(nufpar_s *parser) {
    i32 value0 = NuFParGetInt(parser);
    i32 value1 = NuFParGetInt(parser);
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 4;
    parprog->instructions[static_cast<i16>(index + 1)] =
        static_cast<i16>(static_cast<i32>(value0 * (1.0f / 60.0f) * 4096.0f));
    parprog->instructions[static_cast<i16>(index + 2)] =
        static_cast<i16>(static_cast<i32>(value1 * (1.0f / 60.0f) * 4096.0f));
    parprog->instruction_count = index + 3;
}

static void pftaTexAdjR(nufpar_s *parser) {
    i32 value0 = NuFParGetInt(parser);
    i32 value1 = NuFParGetInt(parser);
    i32 value2 = NuFParGetInt(parser);
    i32 value3 = NuFParGetInt(parser);
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 3;
    parprog->instructions[static_cast<i16>(index + 1)] = static_cast<i16>(value0);
    parprog->instructions[static_cast<i16>(index + 2)] = static_cast<i16>(value1);
    parprog->instructions[static_cast<i16>(index + 3)] = static_cast<i16>(value2);
    parprog->instructions[static_cast<i16>(index + 4)] = static_cast<i16>(value3);
    parprog->instruction_count = index + 5;
}

static void pftaTexAdj(nufpar_s *parser) {
    i32 value0 = NuFParGetInt(parser);
    i32 value1 = NuFParGetInt(parser);
    i32 value2 = NuFParGetInt(parser);
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 2;
    parprog->instructions[static_cast<i16>(index + 1)] = static_cast<i16>(value0);
    parprog->instructions[static_cast<i16>(index + 2)] = static_cast<i16>(value1);
    parprog->instructions[static_cast<i16>(index + 3)] = static_cast<i16>(value2);
    parprog->instruction_count = index + 4;
}

static void pftaTexR(nufpar_s *parser) {
    i32 value0 = NuFParGetInt(parser);
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 1;
    parprog->instructions[static_cast<i16>(index + 1)] = static_cast<i16>(value0);
    parprog->instruction_count = index + 2;
}

static void pftaTex(nufpar_s *parser) {
    i32 value0 = NuFParGetInt(parser);
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 0;
    parprog->instructions[static_cast<i16>(index + 1)] = static_cast<i16>(value0);
    parprog->instruction_count = index + 2;
}

static void pftaScriptMask(nufpar_s *parser) {
    parprog->mask = static_cast<u16>(NuFParGetInt(parser));
}

void NuPostFilterGen::destroyResources() {
    NuFramebufferDestroy(input_fbo);
    input_fbo = NULL;
}

void NuDeferredFilterGen::destroyResources() {
    for (i32 i = 0; i < 6; ++i)
        NuFramebufferDestroy(shadow_fbos[i]);
    NuFramebufferDestroy(light_fbo);
    NuPostFilterGen::destroyResources();
}

void NuMotionAccumFilterGen::destroyResources() {
    NuFramebufferDestroy(accumulation_fbo);
    accumulation_fbo = NULL;
    NuPostFilterGen::destroyResources();
}

void NuCopyFilter::destroyResources() {
    NuFramebufferDestroy(copy_fbo);
    NuPostFilterGen::destroyResources();
}

void NuMainFilterGen::destroyResources() {
    NuFramebufferDestroy(blur_fbo);
    blur_fbo = NULL;
    NuPostFilterGen::destroyResources();
}

void NuPostFilterGen::initResources() {
    input_fbo = NuFramebufferCreate();
}

void NuDeferredFilterGen::initResources() {
    NuPostFilterGen::initResources();
    for (i32 i = 0; i < 6; ++i)
        shadow_fbos[i] = NuFramebufferCreate();
    light_fbo = NuFramebufferCreate();
    NuFramebufferAttachTex2D(light_fbo, 0, textures[0], 0);
    dynamic_light_count = deferred_geometry_count = 0;
    resetAll();
}

void NuMotionAccumFilterGen::initResources() {
    NuPostFilterGen::initResources();
    accumulation_fbo = NuFramebufferCreate();
    NuFramebufferAttachTex2D(accumulation_fbo, 0, accumulation_texture, 0);
}

void NuCopyFilter::initResources() {
    NuPostFilterGen::initResources();
    copy_fbo = NuFramebufferCreate();
    NuFramebufferAttachTex2D(copy_fbo, 0, workTex, 0);
    input_fbo = copy_fbo;
}

void NuMainFilterGen::initResources() {
    NuPostFilterGen::initResources();
    blur_fbo = NuFramebufferCreate();
    dof_enabled = bloom_enabled = motion_blur_enabled = false;
    active_filter_count = 0;
}

void NuMainFilterGen::initTextureResources(i32 width, i32 height) {
    blur_texture = NuEffectTexCreate2D(width / 2, height / 2, 2, 1, 2);
    downsample_lod = 0;
    i32 w = width, h = height;
    for (i32 i = 0; i < 3; ++i) {
        if (w < 128 || h < 128)
            break;
        ++downsample_lod;
        w >>= 1;
        h >>= 1;
    }
    downsample_texture = NuEffectTexCreate2D(w, h, 1, 1, 2);
    if (height < 704) {
        dof_blur -= 1.0f;
        blur_radius -= 0.85f;
        blur_gain -= 0.5f;
    }
}

extern "C" {
extern "C++" {
        static void DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                                  NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix,
                                                  void **transform_packet, f32 alpha) {
            display_list_buffer->addr = ALIGN(display_list_buffer->addr, 0x10);
            VARIPTR *buffer = NuDisplayListLinkItems(list, 3);

            void *geometry_packet = geometry->next;
            void *lighting_packet = geometry[-2].next;
            list->items->type = geometry[-2].type;
            list->items->next = lighting_packet;
            list->items->id = 3;
            first_and_last[0] = list->items++;
            *transform_packet = DisplayListCreateGeomTransformPS(buffer, world_matrix, list->dlist->mtls[list->mtl_id],
                                                                 geometry_packet, *transform_packet);
            list->items->type = 0x8c;
            list->items->id = 3;
            list->items->next = *transform_packet;
            first_and_last[1] = list->items++;

            list->items->type = 0x82;
            list->items->id = 3;
            list->items->next = geometry_packet;
            first_and_last[2] = list->items++;

            DisplayListSetAlphaPS(first_and_last[1], first_and_last[2], alpha);
        }

        static void DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                           NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix,
                                           void **transform_packet, NUMTX *skin_matrices,
                                           DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster) {
            display_list_buffer->addr = ALIGN(display_list_buffer->addr, 0x10);
            VARIPTR *buffer = NuDisplayListLinkItems(list, 3);

            void *geometry_packet = geometry->next;
            *transform_packet = DisplayListCreateGeomTransformPS(buffer, world_matrix, list->dlist->mtls[list->mtl_id],
                                                                 geometry_packet, *transform_packet);
            list->items->type = 0x8c;
            list->items->id = 3;
            list->items->next = *transform_packet;
            first_and_last[0] = list->items++;

            NUDISPLAYLISTGEOM *render_geometry;
            void *skin_packet = DisplayListCreateSkinTransformPS(buffer, skin_matrices, deformer_weights,
                                                                 static_cast<NUDISPLAYLISTGEOM *>(geometry_packet),
                                                                 &render_geometry, &first_and_last[2]);
            list->items->type = 0x99;
            list->items->id = 3;
            list->items->next = skin_packet;
            first_and_last[1] = list->items++;

            list->items->id = 3;
            list->items->type = 0x98;
            list->items->next = render_geometry;
            first_and_last[1] = list->items++;

            DisplayListSetAlphaPS(first_and_last[0], first_and_last[1], 1.0f);
            DisplayListSetShadowCasterFlagPS(first_and_last[0], first_and_last[1], shadow_caster);
        }

}
}

static i32 CreateTimeBar(VARIPTR *buffer, i32 *colours, i32 slot_count) {

    TimeBarSet *timebar = timebar_allocate_array<TimeBarSet>(buffer, 1);
    std::memset(timebar, 0, sizeof(*timebar));
    timebar->uses_buffer = buffer != NULL;
    timebar->accumulators[0] = timebar_allocate_array<i32>(buffer, slot_count);
    timebar->accumulators[1] = timebar_allocate_array<i32>(buffer, slot_count);
    timebar->start_times = timebar_allocate_array<NUTIME>(buffer, slot_count);
    timebar->field_14 = timebar_allocate_array<i32>(buffer, slot_count);
    timebar->field_18 = timebar_allocate_array<i32>(buffer, slot_count);
    timebar->colours = timebar_allocate_array<i32>(buffer, slot_count);
    timebar->toggle_flags = timebar_allocate_array<i32>(buffer, slot_count);
    timebar->slot_names = timebar_allocate_array<const char *>(buffer, slot_count);

    std::memset(timebar->accumulators[0], 0, static_cast<usize>(slot_count) * sizeof(i32));
    std::memset(timebar->accumulators[1], 0, static_cast<usize>(slot_count) * sizeof(i32));
    std::memset(timebar->start_times, 0, static_cast<usize>(slot_count) * sizeof(NUTIME));
    std::memset(timebar->field_14, 0, static_cast<usize>(slot_count) * sizeof(i32));
    std::memset(timebar->field_18, 0, static_cast<usize>(slot_count) * sizeof(i32));
    std::memset(timebar->colours, 0, static_cast<usize>(slot_count) * sizeof(i32));
    std::memset(timebar->toggle_flags, 0, static_cast<usize>(slot_count) * sizeof(i32));
    std::memset(timebar->slot_names, 0, static_cast<usize>(slot_count) * sizeof(const char *));

    i32 list_index;
    for (list_index = 0; list_index < kMaxTimeBarSets; ++list_index) {
        if (NuTimeBar_SetList[list_index] == NULL) {
            NuTimeBar_SetList[list_index] = timebar;
            break;
        }
    }
    if (list_index == kMaxTimeBarSets) {
        list_index = kMaxTimeBarSets - 1;
        NuTimeBar_SetList[list_index] = timebar;
    }

    if (__builtin_expect(colours != NULL, 0)) {
        std::memcpy(timebar->colours, colours, static_cast<usize>(slot_count) * sizeof(i32));
    } else {
        std::memcpy(timebar->colours, NuTimeBar_DefaultColours, static_cast<usize>(slot_count) * sizeof(i32));
    }
    timebar->slot_count = slot_count;

    i32 set = list_index - 1;
    timebar->field_28 = set > 0;
    return set;
}

static void DisplayListBeforeFrameReset(nudisplayscene_s *scene) {
    ResetSceneBeforeFrame(scene, false);
}

static void DisplayListBeforeFrameFlip(nudisplayscene_s *scene) {
    ResetSceneBeforeFrame(scene, true);
}

static void pftaUntiltex(nufpar_s *parser) {
    NuFParGetWord(parser);
    char *comparison = parser->word_buf;
    i32 condition = 0;
    switch (comparison[0]) {
        case '<':
            switch (comparison[1]) {
                case '=':
                    condition = 3;
                    break;
                case '>':
                    condition = 5;
                    break;
                default:
                    condition = 1;
                    break;
            }
            break;
        case '>':
            condition = comparison[1] == '=' ? 4 : 2;
            break;
        case '!':
            condition = 5;
            break;
        default:
            break;
    }
    i32 value = NuFParGetInt(parser);
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 15;
    parprog->instructions[static_cast<i16>(index + 1)] = condition;
    parprog->instructions[static_cast<i16>(index + 2)] = static_cast<i16>(value);
    parprog->instruction_count = index + 3;
}

static __used__ NUFRUSTRUM *buildFrustrum(NUVEC *minimum, NUVEC *maximum, i16 room_id) {
    NUFRUSTRUM *frustum = allocateFrustrum(4, room_id);
    frustum->minimum = *minimum;
    frustum->maximum = *maximum;

    const f32 tangent = NU_TAN_LUT(cam->fov * 0.5f * 10430.378f);
    const f32 horizontal_tangent = tangent / cam->aspect;

    f32 edge = minimum->y * tangent;
    f32 inverse_length = 1.0f / NuFsqrt(edge * edge + 1.0f);
    frustum->planes[2] = {0.0f, inverse_length, -edge * inverse_length, 0.0f};

    edge = maximum->y * tangent;
    inverse_length = 1.0f / NuFsqrt(edge * edge + 1.0f);
    frustum->planes[3] = {0.0f, -inverse_length, edge * inverse_length, 0.0f};

    edge = maximum->x * horizontal_tangent;
    inverse_length = 1.0f / NuFsqrt(edge * edge + 1.0f);
    frustum->planes[0] = {-inverse_length, 0.0f, edge * inverse_length, 0.0f};

    edge = minimum->x * horizontal_tangent;
    inverse_length = 1.0f / NuFsqrt(edge * edge + 1.0f);
    frustum->planes[1] = {inverse_length, 0.0f, -edge * inverse_length, 0.0f};

    for (i32 i = 0; i < 4; ++i) {
        transformFrustrumPlane(&frustum->planes[i]);
    }
    transposeClipPlanes(frustum);
    return frustum;
}

void NuMotionFilterGen::render() {
    nuframebuffer_s *output = static_cast<nuframebuffer_s *>(portOutFramebuffer.get());
    NuProxyBuffer *color = static_cast<NuProxyBuffer *>(portColorBuffer.get());
    NuProxyBuffer *velocity = static_cast<NuProxyBuffer *>(portVelocityBuffer.get());
    NuPostResolve(color);
    NuPostResolve(velocity);
    nueffecttex_s *destination = NuFramebufferGetAttachedTex(output, 0, NULL, NULL);
    PostBindProgram(programs[0]);
    f32 ratio = maximum / scale;
    f32 params[4] = {ratio, 0.5f, 0, 0};
    f32 weights[7][4];
#define MOTION_SAMPLE(index)                                                                                           \
    weights[index][0] = (NuPow(static_cast<f32>(index + 1) / 7.0f, falloff) - 0.5f) * ratio;                           \
    weights[index][1] = weights[index][0];                                                                             \
    weights[index][2] = weights[index][3] = 0.0f;
    MOTION_SAMPLE(0)
    MOTION_SAMPLE(1)
    MOTION_SAMPLE(2)
    MOTION_SAMPLE(3)
    MOTION_SAMPLE(4)
    MOTION_SAMPLE(5)
    MOTION_SAMPLE(6)
#undef MOTION_SAMPLE
    PostBlurSetVertexParam(programs[0], 0x808a, params, 4);
    PostBlurSetVertexParam(programs[0], 0x808b, &weights[0][0], 28);
    NuFramebufferBind(output);
    PostDrawQuad();
    color->kind = 4;
    color->texture = destination;
    color->enabled = true;
    color->resolved = false;
    portColorBuffer.set(color);
}

void NuMotionAccumFilterGen::render() {
    nuframebuffer_s *output = static_cast<nuframebuffer_s *>(portOutFramebuffer.get());
    NuProxyBuffer *color = static_cast<NuProxyBuffer *>(portColorBuffer.get());
    NuPostResolve(color);
    nueffecttex_s *destination = NuFramebufferGetAttachedTex(output, 0, NULL, NULL);
    f32 weights[256], sum = 0.0f;
    for (i32 i = 0; i < frames; ++i) {
        weights[i] = NuPow(static_cast<f32>(i + 1) / frames, mode);
        sum += weights[i];
    }
    if (frames > 0) {
        f32 reciprocal_sum = 1.0f / sum;
        for (i32 i = 0; i < frames; ++i)
            weights[i] *= reciprocal_sum;
        sum = 0.0f;
        for (i32 i = 0; i < frames; ++i) {
            sum += weights[i];
            weights[i] /= sum;
        }
    }
    if (current_frame + 1 == frames) {
        current_frame = 0;
    } else {
        current_frame = current_frame + 1;
    }
    PostBindProgram(program);
    f32 params[4] = {weights[current_frame], 0, 0, 0};
    PostBlurSetVertexParam(program, 0x808a, params, 4);
    i32 width, height;
    NuEffectTexGetDimension(accumulation_texture, 0, &width, &height);
    NuFramebufferBind(accumulation_fbo);
    NuRenderContextSetViewport(0, 0, width, height);
    PostDrawQuad();
    NuFramebufferResolve(0, true);
    PostBindProgram(program);
    PostBlurSetVertexParam(program, 0x808a, params, 4);
    NuFramebufferBind(output);
    PostDrawQuad();
    color->texture = destination;
    color->kind = 4;
    color->enabled = true;
    color->resolved = false;
    portColorBuffer.set(color);
}

static void pftaGosub(nufpar_s *parser) {
    NuFParGetWord(parser);
    i32 label = NuTexAnimLabelIndex(parser->word_buf, labtab, &labtabcnt);
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 10;
    parprog->instructions[static_cast<i16>(index + 1)] = static_cast<i16>(label);
    parprog->instruction_count = index + 2;
}

static void pftaGoto(nufpar_s *parser) {
    NuFParGetWord(parser);
    i32 label = NuTexAnimLabelIndex(parser->word_buf, labtab, &labtabcnt);
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 9;
    parprog->instructions[static_cast<i16>(index + 1)] = static_cast<i16>(label);
    parprog->instruction_count = index + 2;
}

static void pftaLabel(nufpar_s *parser) {
    NuFParGetWord(parser);
    i32 label = NuTexAnimLabelIndex(parser->word_buf, labtab, &labtabcnt);
    nta_labels[label] = parprog->instruction_count;
}

static void pftaBtex(nufpar_s *parser) {
    NuFParGetWord(parser);
    char *comparison = parser->word_buf;
    i32 condition = 0;
    switch (comparison[0]) {
        case '<':
            switch (comparison[1]) {
                case '=':
                    condition = 3;
                    break;
                case '>':
                    condition = 5;
                    break;
                default:
                    condition = 1;
                    break;
            }
            break;
        case '>':
            condition = comparison[1] == '=' ? 4 : 2;
            break;
        case '!':
            condition = 5;
            break;
        default:
            break;
    }
    i32 value = NuFParGetInt(parser);
    NuFParGetWord(parser);
    i32 label = NuTexAnimLabelIndex(parser->word_buf, labtab, &labtabcnt);
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 11;
    parprog->instructions[static_cast<i16>(index + 1)] = condition;
    parprog->instructions[static_cast<i16>(index + 2)] = static_cast<i16>(value);
    parprog->instructions[static_cast<i16>(index + 3)] = static_cast<i16>(label);
    parprog->instruction_count = index + 4;
}

static void pftaXRef(nufpar_s *parser) {
    NuFParGetWord(parser);
    i32 label = NuTexAnimLabelIndex(parser->word_buf, xdeflabtab, &xdeflabtabcnt);
    i16 index = parprog->instruction_count;
    parprog->instructions[index] = 17;
    parprog->instructions[static_cast<i16>(index + 1)] = static_cast<i16>(label);
    parprog->instruction_count = index + 2;
}

NuMainFilterGen::NuMainFilterGen() {
    dof_strength = 1.0f;
    dof_near = 1.0f;
    dof_far = 1.0f;
    dof_bias = 0;
    dof_mode = 3;
    bloom = NULL;
    dof_blur = 3.0f;
    blur_radius = 5.0f;
    blur_gain = 2.1f;
    downsample_lod = 0;
    motion_scale = 0.0f;
    motion_maximum = 0.0f;
    motion_falloff = 1.0f;
}

NuMotionFilterGen::NuMotionFilterGen() {
    scale = maximum = 0.0f;
    falloff = 1.0f;
}

extern "C" {
// original 0x2ab8b0 — per-frame reset called at the top of
// renderThread_processRenderScenes before the safe scene list is walked.
extern "C" void NuPostEffectReset(void) {
    g_effectsRan = 0;
    g_lastFrameEffect = 0;
}

// original 0x2ab8d0 — resetAll (vtable+0x20) for each allocated filter,
// followed by the four original proxy resets. Speed blur and the depth-RT
// proxy are deliberately absent from this reset sequence.
extern "C" void NuPostEffectEnd(void) {
    FilterEnd(deferredFilter);
    FilterEnd(mainFilter);
    FilterEnd(motionFilter);
    FilterEnd(motionAccumFilter);

    ResetProxyBuffer(&proxyColorBuffer, kProxyKind_Color);
    ResetProxyBuffer(&proxyNormalBuffer, kProxyKind_Normal);
    ResetProxyBuffer(&proxyVelocityBuffer, kProxyKind_Velocity);
    ResetProxyBuffer(&proxyDepthBuffer, kProxyKind_Depth);
}

// original 0x2ab9c0
extern "C" i32 NuPostEffectIsInitialised(u32 mask) {
    return (g_effectFlags & static_cast<i32>(mask)) != 0;
}

extern "C" void NuPostEffectEnable(i32 mask) {
    if ((g_effectFlags & mask) == 0)
        return;
    switch (mask) {
        case 4:
            if (!mainFilter->bloom_enabled) {
                ++mainFilter->active_filter_count;
                mainFilter->bloom_enabled = true;
            }
            break;
        case 8:
            if (!mainFilter->dof_enabled) {
                ++mainFilter->active_filter_count;
                mainFilter->dof_enabled = true;
            }
            break;
        case 0x10:
            motionFilter->enabled = true;
            break;
        case 0x20:
            deferredFilter->enabled = true;
            break;
        case 0x40:
            motionAccumFilter->enabled = true;
            break;
        case 0x80:
            speedBlurFilter->enabled = true;
            break;
    }
}

extern "C" void NuPostEffectDisable(i32 mask) {
    switch (mask) {
        case 4:
            if (mainFilter->bloom_enabled) {
                --mainFilter->active_filter_count;
                mainFilter->bloom_enabled = false;
            }
            break;
        case 8:
            if (mainFilter->dof_enabled) {
                --mainFilter->active_filter_count;
                mainFilter->dof_enabled = false;
            }
            break;
        case 0x10:
            motionFilter->enabled = false;
            break;
        case 0x20:
            deferredFilter->enabled = false;
            break;
        // The original disables motionFilter here as well (0x2abaf8).
        case 0x40:
            motionFilter->enabled = false;
            break;
        case 0x80:
            speedBlurFilter->enabled = false;
            break;
    }
}

extern "C" i32 NuPostEffectIsEnabled(i32 mask) {
    if ((g_effectFlags & mask) == 0)
        return false;
    switch (mask) {
        case 4:
            return mainFilter->bloom_enabled;
        case 8:
            return mainFilter->dof_enabled;
        case 0x10:
            return motionFilter->isEnabled();
        case 0x20:
            return deferredFilter->isEnabled();
        case 0x40:
            return motionAccumFilter->isEnabled();
        case 0x80:
            return speedBlurFilter->isEnabled();
    }
    return false;
}

// original 0x2abc40 — feeds a dynamic light handle into the deferred filter's
// light list.  The handle is an opaque light pointer; the original checks
// *(i32*)(light+0x7bc) != 0 before calling (see nurenderthread.cpp).
extern "C" void NuPostEffectAddDynamicLight(void *light) {
    if (deferredFilter == nullptr) {
        return;
    }
    i32 count = deferredFilter->dynamic_light_count;
    deferredFilter->dynamic_lights[count] = static_cast<NuDynamicLight *>(light);
    deferredFilter->dynamic_light_count = count + 1;
}

extern "C" i32 NuPostEffectGetActiveDynamicLightCount() {
    return deferredFilter != NULL ? deferredFilter->dynamic_light_count : 0;
}

extern "C" void NuPostEffectDepthOfField(const NuDepthOfFieldParameters *parameters) {
    if (mainFilter == NULL)
        return;
    mainFilter->dof_strength = parameters->strength;
    mainFilter->dof_near = parameters->near_distance;
    mainFilter->dof_far = parameters->far_distance;
    mainFilter->dof_bias = parameters->bias;
    mainFilter->dof_mode = parameters->mode;
}

extern "C" void NuPostEffectDeferredShading(const f32 *parameters) {
    // The four shading parameters are copied as one block.
    struct DeferredShadingParameters {
        f32 values[4];
    };
    if (deferredFilter == NULL)
        return;
    *reinterpret_cast<DeferredShadingParameters *>(deferredFilter->parameters) =
        *reinterpret_cast<const DeferredShadingParameters *>(parameters);
}

extern "C" void NuPostEffectBloom(const NuBloomParameters *parameters) {
    if (mainFilter != NULL)
        mainFilter->bloom = parameters;
}

extern "C" void NuPostEffectMotionBlur(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum,
                                       f32 falloff) {
    if (mainFilter == NULL)
        return;
    mainFilter->motion_previous = *previous;
    mainFilter->motion_current = *current;
    mainFilter->motion_scale = scale;
    mainFilter->motion_maximum = maximum;
    mainFilter->motion_falloff = falloff;
    motionFilter->previous = *previous;
    motionFilter->current = *current;
    motionFilter->scale = scale;
    motionFilter->maximum = maximum;
    motionFilter->falloff = falloff;
}

extern "C" void NuPostEffectAccumulationMotionBlur(i32 frames, f32 blend, f32 mode) {
    if (mainFilter != NULL) {
        mainFilter->accumulation_frames = frames;
        mainFilter->accumulation_blend = blend;
        mainFilter->accumulation_mode = mode;
    }
    if (motionAccumFilter != NULL) {
        motionAccumFilter->frames = frames;
        motionAccumFilter->mode = mode;
        motionAccumFilter->blend = blend;
    }
}

extern "C" void NuPostEffectSpeedBlur(const NuSpeedBlurParameters *parameters) {
    if (speedBlurFilter != NULL)
        speedBlurFilter->parameters = parameters;
}

extern "C" nueffecttex_s *NuPostEffectGetBackBuffer(i32 frame) {
    static i32 backFrameId = -1;
    static nueffecttex_s *backBufferCopy;
    if (backFrameId != frame) {
        backFrameId = frame;
        nuframebuffer_s *bound = NuFramebufferGetBound();
        backBufferCopy = g_backBufferCopy;
        NuFramebufferEnableGuards(bound, true);
        NuFramebufferCopyTex2D(0, backBufferCopy, 0, 0, 0, backBufferCopy->width, backBufferCopy->height);
        NuFramebufferEnableGuards(bound, false);
    }
    return backBufferCopy;
}

extern "C" nueffecttex_s *NuPostEffectGetDepthBuffer(i32 frame) {
    static i32 depthFrameId = -1;
    static nueffecttex_s *depthBufferCopy;
    if (depthFrameId != frame) {
        depthFrameId = frame;
        nueffecttex_s *texture;
        if (proxyDepthBuffer.texture == NULL) {
            NuFramebufferResolve(4, false);
            texture = NuFramebufferGetAttachedTex(NuFramebufferGetBound(), 4, NULL, NULL);
        } else {
            if (!proxyDepthBuffer.resolved) {
                NuFramebufferResolve(proxyDepthBuffer.kind, proxyDepthBuffer.enabled);
                proxyDepthBuffer.resolved = true;
            }
            texture = proxyDepthBuffer.texture;
        }
        depthBufferCopy = texture;
    }
    return depthBufferCopy;
}

extern "C" void NuPostEffectRender(nuframebuffer_s *output) {
    nuframebuffer_s *bound = NuFramebufferGetBound();
    if (output == NULL)
        output = NuFramebufferGetObject(1);
    NuFramebufferGetObject(0);
    nueffecttex_s *color = NuFramebufferGetAttachedTex(bound, 0, NULL, NULL);
    nueffecttex_s *normal = NuFramebufferGetAttachedTex(bound, 1, NULL, NULL);
    nueffecttex_s *velocity = NuFramebufferGetAttachedTex(bound, 2, NULL, NULL);
    nueffecttex_s *depth_rt = NuFramebufferGetAttachedTex(bound, 2, NULL, NULL);
    nueffecttex_s *depth = NuFramebufferGetAttachedTex(bound, 4, NULL, NULL);
    NuFramebufferResolveMultisample(0);
    proxyColorBuffer.texture = color;
    proxyColorBuffer.kind = 0;
    proxyColorBuffer.enabled = proxyColorBuffer.resolved = true;
    proxyNormalBuffer.texture = normal;
    proxyNormalBuffer.kind = 1;
    proxyNormalBuffer.enabled = proxyNormalBuffer.resolved = true;
    proxyVelocityBuffer.texture = velocity;
    proxyVelocityBuffer.kind = 2;
    proxyVelocityBuffer.enabled = proxyVelocityBuffer.resolved = true;
    proxyDepthRTBuffer.texture = depth_rt;
    proxyDepthRTBuffer.kind = 2;
    proxyDepthRTBuffer.enabled = proxyDepthRTBuffer.resolved = true;
    proxyDepthBuffer.texture = depth;
    proxyDepthBuffer.kind = 4;
    proxyDepthBuffer.enabled = proxyDepthBuffer.resolved = true;
    NuPostFilterGen::portColorBuffer.set(&proxyColorBuffer);
    NuPostFilterGen::portNormalBuffer.set(&proxyNormalBuffer);
    NuPostFilterGen::portVelocityBuffer.set(&proxyVelocityBuffer);
    NuPostFilterGen::portDepthRTBuffer.set(&proxyDepthRTBuffer);
    NuPostFilterGen::portDepthBuffer.set(&proxyDepthBuffer);
    NuPostFilterGen *filters[5];
    i32 count = 0;
    if (speedBlurFilter != NULL) {
        if (speedBlurFilter->isEnabled()) {
            filters[count++] = speedBlurFilter;
            NuFramebufferAttachTex2D(speedBlurFilter->getInputFbo(), 0, color, 0);
        }
    }
    if (deferredFilter != NULL) {
        if (deferredFilter->isEnabled()) {
            filters[count++] = deferredFilter;
            NuFramebufferAttachTex2D(deferredFilter->getInputFbo(), 0, color, 0);
        }
    }
    if (mainFilter != NULL) {
        if (mainFilter->isEnabled()) {
            filters[count++] = mainFilter;
            NuFramebufferAttachTex2D(mainFilter->getInputFbo(), 0, color, 0);
        }
    }
    if (motionFilter != NULL) {
        if (motionFilter->isEnabled()) {
            filters[count++] = motionFilter;
            NuFramebufferAttachTex2D(motionFilter->getInputFbo(), 0, color, 0);
        }
    }
    if (motionAccumFilter != NULL) {
        if (motionAccumFilter->isEnabled()) {
            filters[count++] = motionAccumFilter;
            NuFramebufferAttachTex2D(motionAccumFilter->getInputFbo(), 0, color, 0);
        }
    }
    g_boundShader = 0;
    glUseProgram(0);
    g_currentShaderProgram = NULL;
    for (i32 i = 0; i < count; ++i) {
        NuPostFilterGen::portOutFramebuffer.set(i + 1 < count ? filters[i + 1]->getInputFbo() : output);
        filters[i]->render();
        filters[i]->reset();
    }
    ++g_effectsRan;
}

}

void NuPostFilterGen::initSharedTextureResources(i32 width, i32 height) {
    workTex = NuEffectTexCreate2D(width, height, 1, 0x11, 2);
}

void NuPostFilterGen::destroySharedTextureResources() {
    STUBBED();
}

void NuPostFilterGen::destroySharedResources() {
    STUBBED();
}

extern "C" {
extern "C" void NuPostEffectDestroy() {
    DestroyPostFilter(mainFilter);
    DestroyPostFilter(motionFilter);
    DestroyPostFilter(motionAccumFilter);
    DestroyPostFilter(speedBlurFilter);
    DestroyPostFilter(deferredFilter);
    DestroyPostFilter(copyFilter);
    NuPostFilterGen::destroySharedTextureResources();
    NuPostFilterGen::destroySharedResources();
    filterAllocator.cursor = filterMem;
}

}

void NuPostFilterGen::copy(nueffecttex_s *, nuframebuffer_s *output) {
    NuFramebufferBind(output);
    PostBindProgram(copyTexProgram);
    PostDrawQuad();
}

void NuPostFilterGen::copyDepth(nueffecttex_s *, nuframebuffer_s *) {
    STUBBED();
}

void NuPostFilterGen::copy(nueffecttex_s *color, nueffecttex_s *, nuframebuffer_s *output) {
    NuFramebufferBind(output);
    PostBindProgram(copyTexColorDepthProgram);
    if (NuFramebufferGetWidth(output) == color->width)
        NuFramebufferGetHeight(output);
    PostDrawQuad();
}

void NuCopyFilter::render(nuframebuffer_s *output) {
    nueffecttex_s *color = NuFramebufferGetAttachedTex(input_fbo, 0, NULL, NULL);
    if (copy_texture != NULL)
        copy(color, copy_texture, output);
    else
        copy(color, output);
}

void NuPostFilterGen::copy(nueffecttex_s *destination, i32 lod, nueffecttex_s *, i32, nushaderprogram_s *program,
                           nueffecttex_s *) {
    i32 width, height;
    NuEffectTexGetDimension(destination, lod, &width, &height);
    NuFramebufferAttachTex2D(copyFbo, 0, destination, lod);
    NuFramebufferBind(copyFbo);
    NuRenderContextSetViewport(0, 0, width, height);
    PostBindProgram(program);
    PostDrawQuad();
    NuFramebufferResolve(0, true);
}

void NuPostFilterGen::blend(nueffecttex_s *, nueffecttex_s *, nuframebuffer_s *output) {
    NuFramebufferBind(output);
    PostBindProgram(blendTexProgram);
    PostDrawQuad();
}

void NuPostFilterGen::blur7x7Separate(nueffecttex_s *source, i32 source_lod, nueffecttex_s *textures, i32 first_lod,
                                      i32 iterations, i32 levels, bool mip_chain, float radius,
                                      nushaderprogram_s *program) {
    i32 guard = static_cast<i32>(ceil(radius)) * 3;
    i32 work_width = workTex->width;
    i32 work_height = workTex->height;
    {
        nushaderprogram_s *bound = program;
        g_boundShader = bound != NULL ? bound->program : 0;
        glUseProgram(g_boundShader);
        g_currentShaderProgram = bound;
    }
    for (i32 lod = 0; lod < levels; ++lod) {
        i32 level = first_lod + lod;
        nueffecttex_s *input = level == first_lod ? source : (mip_chain ? textures : textures + level - 1);
        i32 input_lod = level == first_lod ? source_lod : (mip_chain ? level - 1 : 0);
        nueffecttex_s *output = mip_chain ? textures : textures + level;
        i32 output_lod = mip_chain ? level : 0;
        for (i32 pass = 0; pass < iterations; ++pass) {
            i32 width, height, out_width, out_height;
            NuEffectTexGetDimension(input, input_lod, &width, &height);
            NuEffectTexGetDimension(output, output_lod, &out_width, &out_height);
            f32 dx = radius / width;
            f32 dy = radius / height;
            VuVec horizontal[7] = {
                {dx * 0.0f, 0.0f, .34f, 0.0f},  {dx * 1.0f, 0.0f, .18f, 0.0f},  {dx * 2.0f, 0.0f, .10f, 0.0f},
                {dx * 3.0f, 0.0f, .05f, 0.0f},  {dx * -1.0f, 0.0f, .18f, 0.0f}, {dx * -2.0f, 0.0f, .10f, 0.0f},
                {dx * -3.0f, 0.0f, .05f, 0.0f},
            };
            VuVec vertical[7] = {
                {0.0f, dy * 0.0f, .34f, 0.0f},  {0.0f, dy * 1.0f, .18f, 0.0f},  {0.0f, dy * 2.0f, .10f, 0.0f},
                {0.0f, dy * 3.0f, .05f, 0.0f},  {0.0f, dy * -1.0f, .18f, 0.0f}, {0.0f, dy * -2.0f, .10f, 0.0f},
                {0.0f, dy * -3.0f, .05f, 0.0f},
            };
            VuVec bias(1, 1, 0.0f / width, 0.0f / height);
            VuVec output_bias;
            nueffecttex_s *work = NuPostFilterGen::workTex;
            NuFramebufferAttachTex2D(NuPostFilterGen::blurFbo, 0, work, 0);
            NuFramebufferBind(NuPostFilterGen::blurFbo);
            NuRenderContextSetViewport(0, 0, out_width, out_height);
            PostBlurSetVertexParam(program, 0xa0, &horizontal[0].x, 28);
            PostBlurSetVertexParam(program, 0xad, &bias.x, 4);
            PostBlurDrawQuad();
            {
                {
                    nushaderprogram_s *bound = NuPostFilterGen::blurGuardProgram;
                    g_boundShader = bound != NULL ? bound->program : 0;
                    glUseProgram(g_boundShader);
                    g_currentShaderProgram = bound;
                }
                if (out_height < work_height) {
                    const VuVec edge(1, 0, 0, 1);
                    PostBlurSetVertexParam(NuPostFilterGen::blurGuardProgram, 0x80, &edge.x, 4);
                    NuRenderContextSetViewport(0, out_height, out_width, guard);
                    PostBlurDrawQuad();
                }
                if (out_width < work_width) {
                    const VuVec edge(0, 1, 1, 0);
                    NuRenderContextSetViewport(out_width, 0, guard, out_height);
                    PostBlurSetVertexParam(NuPostFilterGen::blurGuardProgram, 0x80, &edge.x, 4);
                    PostBlurDrawQuad();
                }
            }
            NuFramebufferResolve(0, true);
            NuFramebufferAttachTex2D(NuPostFilterGen::blurFbo, 0, output, output_lod);
            NuFramebufferBind(NuPostFilterGen::blurFbo);
            NuRenderContextSetViewport(0, 0, out_width, out_height);
            {
                nushaderprogram_s *bound = program;
                g_boundShader = bound != NULL ? bound->program : 0;
                glUseProgram(g_boundShader);
                g_currentShaderProgram = bound;
            }
            output_bias.x = static_cast<f32>(out_width) / work_width;
            output_bias.y = static_cast<f32>(out_height) / work_height;
            output_bias.z = 0.0f / work_width;
            output_bias.w = 0.0f / work_height;
            PostBlurSetVertexParam(program, 0xa0, &vertical[0].x, 28);
            PostBlurSetVertexParam(program, 0xad, &output_bias.x, 4);
            PostBlurDrawQuad();
            NuFramebufferResolve(0, true);
            input = output;
            input_lod = output_lod;
        }
    }
}

void NuPostFilterGen::blur7x7Loopback(nueffecttex_s *source, i32 source_lod, nueffecttex_s *textures, i32 first_lod,
                                      i32 iterations, i32 levels, bool mip_chain, float radius,
                                      nushaderprogram_s *program) {
    {
        nushaderprogram_s *bound = program;
        g_boundShader = bound != NULL ? bound->program : 0;
        glUseProgram(g_boundShader);
        g_currentShaderProgram = bound;
    }
    for (i32 lod = 0; lod < levels; ++lod) {
        i32 level = first_lod + lod;
        i32 input_lod = level == first_lod ? source_lod : (mip_chain ? level - 1 : 0);
        nueffecttex_s *input = level == first_lod ? source : (mip_chain ? textures : textures + level - 1);
        nueffecttex_s *output = mip_chain ? textures : textures + level;
        i32 output_lod = mip_chain ? level : 0;
        for (i32 pass = 0; pass < iterations; ++pass) {
            i32 width, height, out_width, out_height;
            NuEffectTexGetDimension(input, input_lod, &width, &height);
            NuEffectTexGetDimension(output, output_lod, &out_width, &out_height);
            f32 dx = radius / width;
            f32 dy = radius / height;
            VuVec horizontal[7], vertical[7];
            horizontal[0] = VuVec(dx * 0.0f, 0.0f, .34f, 0.0f);
            vertical[0] = VuVec(0.0f, dy * 0.0f, .34f, 0.0f);
            horizontal[1] = VuVec(dx * 1.0f, 0.0f, .18f, 0.0f);
            vertical[1] = VuVec(0.0f, dy * 1.0f, .18f, 0.0f);
            horizontal[2] = VuVec(dx * 2.0f, 0.0f, .10f, 0.0f);
            vertical[2] = VuVec(0.0f, dy * 2.0f, .10f, 0.0f);
            horizontal[3] = VuVec(dx * 3.0f, 0.0f, .05f, 0.0f);
            vertical[3] = VuVec(0.0f, dy * 3.0f, .05f, 0.0f);
            horizontal[4] = VuVec(dx * -1.0f, 0.0f, .18f, 0.0f);
            vertical[4] = VuVec(0.0f, dy * -1.0f, .18f, 0.0f);
            horizontal[5] = VuVec(dx * -2.0f, 0.0f, .10f, 0.0f);
            vertical[5] = VuVec(0.0f, dy * -2.0f, .10f, 0.0f);
            horizontal[6] = VuVec(dx * -3.0f, 0.0f, .05f, 0.0f);
            vertical[6] = VuVec(0.0f, dy * -3.0f, .05f, 0.0f);
            VuVec bias(1, 1, 0.0f / width, 0.0f / height);
            VuVec output_bias(1, 1, 0.0f / out_width, 0.0f / out_height);
            NuFramebufferAttachTex2D(NuPostFilterGen::blurFbo, 0, output, output_lod);
            NuFramebufferBind(NuPostFilterGen::blurFbo);
            NuRenderContextSetViewport(0, 0, out_width, out_height);
            PostBlurSetVertexParam(program, 0xa0, &horizontal[0].x, 28);
            PostBlurSetVertexParam(program, 0xad, &bias.x, 4);
            PostBlurDrawQuad();
            NuFramebufferResolve(0, true);
            NuFramebufferAttachTex2D(NuPostFilterGen::blurFbo, 0, output, output_lod);
            NuFramebufferBind(NuPostFilterGen::blurFbo);
            NuRenderContextSetViewport(0, 0, out_width, out_height);
            PostBlurSetVertexParam(program, 0xa0, &vertical[0].x, 28);
            PostBlurSetVertexParam(program, 0xad, &output_bias.x, 4);
            PostBlurDrawQuad();
            NuFramebufferResolve(0, true);
            input = output;
            input_lod = output_lod;
        }
    }
}

void NuMainFilterGen::preprocessDofMotionBlur(nueffecttex_s *) {
    i32 selection = 1;
    if (dof_enabled) {
        selection = dof_bias == 1 ? (motion_blur_enabled ? 4 : 3) : (motion_blur_enabled ? 2 : 0);
    }
    nushaderprogram_s *program = programs[selection];
    PostBindProgram(program);
    if (dof_enabled) {
        f32 fov, aspect, near_z, far_z;
        NuMtxGetPerspectiveD3D(reinterpret_cast<NUMTX *>(g_renderContext_projection), &fov, &aspect, &near_z, &far_z);
        f32 strength_near = dof_strength * dof_near;
        f32 projection_scale = far_z / (far_z - near_z);
        f32 product = strength_near * dof_far;
        f32 span = dof_far - dof_near;
        f32 denominator = near_z * projection_scale * span;
        f32 values[4] = {product / denominator, strength_near / span - (projection_scale * product) / denominator,
                         dof_blur, 0};
        PostBlurSetVertexParam(program, 0x83, values, 4);
    }
    if (motion_blur_enabled) {
        NUMTX bias = {0.5f, 0, 0, 0, 0, -0.5f, 0, 0, 0, 0, 1, 0, 0.5f, 0.5f, 0, 1};
        NUMTX inverse_bias, inverse_previous, transform;
        NuMtxInvH(&inverse_bias, &bias);
        NuMtxInvH(&inverse_previous, &motion_previous);
        NuMtxMulH(&transform, &inverse_bias, &inverse_previous);
        NuMtxMulH(&transform, &transform, &motion_current);
        NuMtxMulH(&transform, &transform, &bias);
        NuMtxTranspose(&transform, &transform);
        PostBlurSetVertexParam(program, 0x8084, reinterpret_cast<f32 *>(&transform), 16);
    }
    if (dof_enabled || motion_blur_enabled) {
        i32 width, height;
        NuEffectTexGetDimension(blur_texture, 0, &width, &height);
        NuFramebufferAttachTex2D(blur_fbo, 0, blur_texture, 0);
        NuFramebufferBind(blur_fbo);
        NuRenderContextSetViewport(0, 0, width, height);
        PostDrawQuad();
        NuFramebufferResolveAll(true);
        if (dof_mode > 0)
            blur7x7Loopback(blur_texture, 0, blur_texture, 1, dof_mode, 1, true, 1.5f, programs[15]);
    }
}

void NuMainFilterGen::preprocessBlurTextures(nueffecttex_s *color, nueffecttex_s *normal) {
    i16 width = color->width;
    i16 height = color->height;
    if (!dof_enabled && !bloom_enabled)
        return;
    if (normal != NULL)
        copy(color, 1, color, 0, programs[16], normal);
    else
        copy(color, 1, color, 0, copyTexProgram, NULL);
    i32 levels;
    if (dof_enabled)
        levels = static_cast<i32>(NuLog2(static_cast<f32>(width < height ? width : height))) - 6;
    else
        levels = downsample_lod;
    blur7x7Loopback(color, 1, color, 2, 1, levels, true, 1.0f, blur7x7Program);
    if (bloom_enabled) {
        f32 threshold[4] = {bloom->threshold < 0 ? 0 : (bloom->threshold > 1 ? 1 : bloom->threshold), 0, 0, 0};
        static f32 scaleBias[4] = {1, 1, 0, 0};
        PostBindProgram(programs[17]);
        PostBlurSetVertexParam(programs[17], 0xae, threshold, 4);
        PostBlurSetVertexParam(programs[17], 0xaf, scaleBias, 4);
        copy(downsample_texture, 0, color, downsample_lod, programs[17], NULL);
        i32 passes = static_cast<i32>(bloom->blur_iterations);
        if (passes > 0)
            blur7x7Loopback(downsample_texture, 0, downsample_texture, 0, passes, 1, true, 1.0f, blur7x7Program);
        f32 remainder = bloom->blur_iterations - passes;
        if (remainder > 0)
            blur7x7Loopback(downsample_texture, 0, downsample_texture, 0, 1, 1, true, remainder, blur7x7Program);
    }
}

void NuMainFilterGen::render() {
    nuframebuffer_s *output = static_cast<nuframebuffer_s *>(portOutFramebuffer.get());
    NuProxyBuffer *color = static_cast<NuProxyBuffer *>(portColorBuffer.get());
    NuProxyBuffer *normal = static_cast<NuProxyBuffer *>(portNormalBuffer.get());
    NuProxyBuffer *depth = static_cast<NuProxyBuffer *>(portDepthBuffer.get());
    NuPostResolve(color);
    if (normal->texture != NULL)
        NuPostResolve(normal);
    nueffecttex_s *normal_texture = normal->texture;
    nueffecttex_s *color_texture = color->texture;
    nueffecttex_s *depth_texture = depth->texture;
    nueffecttex_s *destination = NuFramebufferGetAttachedTex(output, 0, NULL, NULL);
    nueffecttex_s *depth_destination = NuFramebufferGetAttachedTex(output, 4, NULL, NULL);
    if (bloom_enabled || dof_enabled)
        preprocessBlurTextures(color_texture, normal_texture);
    if (dof_enabled || motion_blur_enabled)
        preprocessDofMotionBlur(depth_texture);
    i32 selection = (bloom_enabled ? (bloom->directional ? 1 : 2) : 0) | (dof_enabled ? 4 : 0);
    nushaderprogram_s *program = NULL;
    switch (selection) {
        case 1:
            program = programs[8];
            break;
        case 2:
            program = programs[6];
            break;
        case 4:
            program = programs[5];
            break;
        case 5:
            program = programs[9];
            break;
        case 6:
            program = programs[7];
            break;
    }
    g_boundShader = program != NULL ? program->program : 0;
    glUseProgram(g_boundShader);
    g_currentShaderProgram = program;
    if (dof_enabled) {
        f32 params[4] = {dof_blur, 1, 0, 0};
        f32 jitter[4] = {static_cast<f32>(lrand48()) * 4.656613e-10f, static_cast<f32>(lrand48()) * 4.656613e-10f, 0,
                         0};
        PostBlurSetVertexParam(program, 0x80, params, 4);
        PostBlurSetVertexParam(program, 0x81, jitter, 4);
    }
    if (bloom_enabled) {
        f32 params[4] = {blur_gain * bloom->intensity, bloom->blend, 0, 0};
        PostBlurSetVertexParam(program, 0x82, params, 4);
        f32 angle[4] = {bloom->intensity * bloom->near_scale, (bloom->far_scale - bloom->near_scale) * bloom->intensity,
                        bloom->near_angle / 180.0f, 180.0f / (bloom->far_angle - bloom->near_angle)};
        PostBlurSetVertexParam(program, 0x88, angle, 4);
        if (bloom->directional) {
            const f32 pi = 3.1415927f;
            f32 directional[4] = {bloom->direction_near_scale * bloom->intensity,
                                  (bloom->direction_far_scale - bloom->direction_near_scale) * bloom->intensity,
                                  (bloom->direction_near_angle * pi) / 180.0f,
                                  180.0f / ((bloom->direction_far_angle - bloom->direction_near_angle) * pi)};
            PostBlurSetVertexParam(program, 0x89, directional, 4);
            NUVEC4 direction = {bloom->direction.x, bloom->direction.y, bloom->direction.z, 1};
            NuVecNorm(reinterpret_cast<NUVEC *>(&direction), reinterpret_cast<NUVEC *>(&direction));
            direction.w = bloom->direction_bias;
            PostBlurSetVertexParam(program, 0x8a, &direction.x, 4);
        }
    }
    NuFramebufferBind(output);
    NuRenderContextSetViewport(0, 0, destination->width, destination->height);
    PostMainDrawGrid();
    color->texture = destination;
    color->kind = 0;
    color->enabled = true;
    color->resolved = false;
    portColorBuffer.set(color);
    if (depth_destination != NULL) {
        depth->texture = depth_destination;
        depth->kind = 4;
        depth->enabled = true;
        depth->resolved = false;
        portDepthBuffer.set(depth);
    }
}

__attribute__((optimize("O2"))) void NuPostFilterGen::GetSampleOffsets_GaussBlur5x5(i32 width, i32 height, VuVec *samples, float scale) {
    f32 dx = 1.0f / width, dy = 1.0f / height;
    // Retail assigns the thirteen diamond taps before its weight-scale loop.
    // Preserve zero products and the positive two-step additions as written.
    samples[0].x = -2.0f * dx;
    samples[0].y = dy * 0.0f;
    samples[0].z = 0.053990968f;
    samples[0].w = 0.0f;
    samples[1].x = -dx;
    samples[1].y = -dy;
    samples[1].z = 0.14676267f;
    samples[1].w = 0.0f;
    samples[2].x = -dx;
    samples[2].y = dy * 0.0f;
    samples[2].z = 0.24197073f;
    samples[2].w = 0.0f;
    samples[3].x = -dx;
    samples[3].y = dy;
    samples[3].z = 0.14676267f;
    samples[3].w = 0.0f;
    samples[4].x = dx * 0.0f;
    samples[4].y = -2.0f * dy;
    samples[4].z = 0.053990968f;
    samples[4].w = 0.0f;
    samples[5].x = dx * 0.0f;
    samples[5].y = -dy;
    samples[5].z = 0.24197073f;
    samples[5].w = 0.0f;
    samples[6].x = dx * 0.0f;
    samples[6].y = dy * 0.0f;
    samples[6].z = 0.3989423f;
    samples[6].w = 0.0f;
    samples[7].x = dx * 0.0f;
    samples[7].y = dy;
    samples[7].z = 0.24197073f;
    samples[7].w = 0.0f;
    samples[8].x = dx * 0.0f;
    samples[8].y = dy + dy;
    samples[8].z = 0.053990968f;
    samples[8].w = 0.0f;
    samples[9].x = dx;
    samples[9].y = -dy;
    samples[9].z = 0.14676267f;
    samples[9].w = 0.0f;
    samples[10].x = dx;
    samples[10].y = dy * 0.0f;
    samples[10].z = 0.24197073f;
    samples[10].w = 0.0f;
    samples[11].x = dx;
    samples[11].y = dy;
    samples[11].z = 0.14676267f;
    samples[11].w = 0.0f;
    samples[12].x = dx + dx;
    samples[12].y = dy * 0.0f;
    samples[12].z = 0.053990968f;
    samples[12].w = 0.0f;
    for (i32 i = 0; i < 13; ++i) {
        samples[i].z = (samples[i].z / 2.1698399f) * scale;
    }
}

void NuPostFilterGen::blur5x5(nueffecttex_s *source, i32 source_lod, nueffecttex_s *textures, i32 first_lod,
                              i32 iterations, i32 levels, bool mip_chain) {
    PostBindProgram(blur5x5Program);
    for (i32 lod = 0; lod < levels; ++lod) {
        i32 level = first_lod + lod;
        nueffecttex_s *input = level == first_lod ? source : (mip_chain ? textures : textures + level - 1);
        i32 input_lod = level == first_lod ? source_lod : (mip_chain ? level - 1 : 0);
        nueffecttex_s *output = mip_chain ? textures : textures + level;
        i32 output_lod = mip_chain ? level : 0;
        for (i32 pass = 0; pass < iterations; ++pass) {
            i32 width, height, out_width, out_height;
            NuEffectTexGetDimension(input, input_lod, &width, &height);
            NuEffectTexGetDimension(output, output_lod, &out_width, &out_height);
            VuVec samples[13];
            GetSampleOffsets_GaussBlur5x5(width, height, samples, 1.0f);
            VuVec scale_bias(1, 1, 0.0f / width, 0.0f / height);
            NuFramebufferAttachTex2D(blurFbo, 0, output, output_lod);
            NuFramebufferBind(blurFbo);
            NuRenderContextSetViewport(0, 0, out_width, out_height);
            PostBlurSetVertexParam(blur5x5Program, 0xa0, &samples[0].x, 52);
            PostBlurSetVertexParam(blur5x5Program, 0xad, &scale_bias.x, 4);
            PostDrawQuad();
            NuFramebufferResolve(0, true);
            input = output;
            input_lod = output_lod;
        }
    }
}

void NuPostFilterGen::renderQuad() {
    STUBBED();
}

void NuPostFilterGen::renderQuadGrid() {
    STUBBED();
}

void NuPostFilterGen::renderFrustum(numtx_s *) {
    STUBBED();
}

NuMotionAccumFilterGen::NuMotionAccumFilterGen() {
    blend = 1.0f;
    frames = mode = 0;
}

f32 NuMotionAccumFilterGen::GetTiming(i32 *last_frame) {
    const f32 frame_time = nuapi.forced_frame_time;
    if (frames != 0 && frame_time != 0.0f) {
        if (blend > 1.0f) {
            blend = 1.0f;
        } else if (blend < 0.0f) {
            blend = 0.0f;
        }
        *last_frame = current_frame == frames - 1;
        return frame_time / frames;
    }
    *last_frame = 1;
    return 0.0f;
}

extern "C" {
extern "C" f32 NuPostEffectTiming(i32 *last_frame) {
    if (motionAccumFilter != NULL && motionBlurAccumActiveThisFrame != 0) {
        motionBlurAccumActiveThisFrame = 0;
        return motionAccumFilter->GetTiming(last_frame);
    }
    *last_frame = 1;
    return 0.0f;
}

}

NuSpeedBlurFilterGen::NuSpeedBlurFilterGen() {
}

void NuSpeedBlurFilterGen::computeSpeedBlur(VuVec &result) {
    NUMTX previous_inverse, current_inverse;
    NuMtxInvH(&previous_inverse, const_cast<NUMTX *>(&parameters->previous));
    NuMtxInvH(&current_inverse, const_cast<NUMTX *>(&parameters->current));
    NUVEC4 motion;
    NuVec4MtxTransformH(&motion, reinterpret_cast<NUVEC4 *>(&previous_inverse) + 3,
                        const_cast<NUMTX *>(&parameters->current));
    if (motion.z < 0.0f) {
        NuVec4MtxTransformH(&motion, reinterpret_cast<NUVEC4 *>(&current_inverse) + 3,
                            const_cast<NUMTX *>(&parameters->previous));
        motion.x = -motion.x;
        motion.y = -motion.y;
        motion.z = -motion.z;
    }
    NuVec4Scale(&motion, &motion, 1.0f / motion.w);
    motion.x *= motionFactorPan;
    motion.y *= motionFactorPan;
    motion.z *= motionFactorPull;
    NuVec4Scale(&motion, &motion, parameters->scale);
    motion.x = motion.x < -motionFactorPanClamp ? -motionFactorPanClamp
                                                : (motion.x > motionFactorPanClamp ? motionFactorPanClamp : motion.x);
    motion.y = motion.y < -motionFactorPanClamp ? -motionFactorPanClamp
                                                : (motion.y > motionFactorPanClamp ? motionFactorPanClamp : motion.y);
    memcpy(&result.xyz, &motion, sizeof(result.xyz));
    result.w = 0.0f;
}

void NuSpeedBlurFilterGen::render() {
    nuframebuffer_s *output = static_cast<nuframebuffer_s *>(portOutFramebuffer.get());
    NuProxyBuffer *color = static_cast<NuProxyBuffer *>(portColorBuffer.get());
    NuProxyBuffer *depth = static_cast<NuProxyBuffer *>(portDepthBuffer.get());
    NuPostResolve(color);
    NuPostResolve(depth);
    struct {
        f32 depth[4];
        VuVec motion;
    } values;
    // The original supplies two vec4 registers; only the first two depth
    // components and the motion vector are assigned.
    values.depth[0] = 0.0f;
    values.depth[1] = parameters->scale * 100000.0f;
    computeSpeedBlur(values.motion);
    nueffecttex_s *color_texture = color->texture;
    nueffecttex_s *destination = NuFramebufferGetAttachedTex(output, 0, NULL, NULL);
    if (texture != NULL)
        copy(texture, 0, color_texture, 0, copyTexProgram, NULL);
    PostBindProgram(programs[0]);
    PostBlurSetVertexParam(programs[0], 0x80, values.depth, 8);
    NuFramebufferBind(output);
    PostDrawQuad(true);
    color->kind = 0;
    color->texture = destination;
    color->enabled = true;
    color->resolved = false;
    portColorBuffer.set(color);
}

extern "C" {
extern "C" void NuSpeedBlurSetMotionFactors(f32 pan, f32 pull, f32 clamp) {
    motionFactorPan = pan;
    motionFactorPull = pull;
    motionFactorPanClamp = clamp;
}

}

i32 NuDataPortManager::registerPort(char const *name, void *data) {
    for (Entry *entry = entries; entry < entries + 256; ++entry) {
        if (NuStrCmp(entry->name, name) == 0) {
            entry->data = data;
            return entry - entries;
        }
    }
    for (Entry *entry = entries; entry < entries + 256; ++entry) {
        if (entry->references == 0) {
            memmove(entry->name, name, NuStrLen(name) + 1);
            entry->data = data;
            ++entry->references;
            return entry - entries;
        }
    }
    return -1;
}

void NuPostFilterGen::initSharedResources() {
    PostRegisterPort(portOutFramebuffer, resourceManager, "postEffect.outFramebuffer");
    PostRegisterPort(portColorBuffer, resourceManager, "postEffect.colorBuffer");
    PostRegisterPort(portNormalBuffer, resourceManager, "postEffect.normalBuffer");
    PostRegisterPort(portVelocityBuffer, resourceManager, "postEffect.velocityBuffer");
    PostRegisterPort(portDepthRTBuffer, resourceManager, "postEffect.depthRTBuffer");
    PostRegisterPort(portDepthBuffer, resourceManager, "postEffect.depthBuffer");
    blurFbo = NuFramebufferCreate();
    copyFbo = NuFramebufferCreate();
}

void OcclusionManager::SetEnabled(bool value) {
    enabled = value;
}

void OcclusionManager::EndFrame() {
}

OccluderSet::OccluderSet() : occluders(NULL), indices(NULL), capacity(0), count(0) {
}

OcclusionManager::OcclusionManager()
    : initialized(false), enabled(true), building_set(NULL), current_set(NULL), unknown_158(0.3f), unknown_15c(-1.0f) {
}

OccluderSet::~OccluderSet() {
}

OcclusionManager::~OcclusionManager() {
}

void OccluderSet::Clear() {
    count = 0;
    queries_prepared = false;
}

void OcclusionManager::BeginFrame() {
    if (initialized && enabled) {
        OccluderSet *completed = building_set;
        building_set = current_set;
        current_set = completed;
        building_set->Clear();
        unknown_160 = 0;
        unknown_164 = 0;
    }
}

void OccluderSet::AddOccluder(const NUVEC *a, const NUVEC *b, const NUVEC *c, const NUVEC *d) {
    if (count < capacity) {
        queries_prepared = false;
        const OccluderVertex va(a->x, a->y, a->z, 1.0f);
        memcpy(&occluders[count].vertices[0], &va, sizeof(va));
        const OccluderVertex vb(b->x, b->y, b->z, 1.0f);
        memcpy(&occluders[count].vertices[1], &vb, sizeof(vb));
        const OccluderVertex vc(c->x, c->y, c->z, 1.0f);
        memcpy(&occluders[count].vertices[2], &vc, sizeof(vc));
        const OccluderVertex vd(d->x, d->y, d->z, 1.0f);
        memcpy(&occluders[count].vertices[3], &vd, sizeof(vd));
        ++count;
    }
}

void OccluderSet::PrepareForQueries(numtx_s const *query, numtx_s const *projection) {
    projection_matrix = *projection;
    query_matrix = *query;
    for (u32 index = 0; index < count; ++index) {
        OccluderRecord &record = occluders[index];
        record.min_x = record.min_y = record.min_depth = 1000000.0f;
        record.max_x = record.max_y = record.depth = -1000000.0f;
        bool valid = true;
        for (u32 vertex = 0; vertex < 4; ++vertex) {
            NUVEC4 &v = record.transformed[vertex];
            NuVec4MtxTransform(&v, reinterpret_cast<NUVEC *>(&record.vertices[vertex]), &projection_matrix);
            if (!(v.w > 0.0f)) {
                valid = false;
                break;
            }
            v.x /= v.w;
            v.y /= v.w;
            v.z /= v.w;
            record.max_x = v.x > record.max_x ? v.x : record.max_x;
            record.min_x = v.x < record.min_x ? v.x : record.min_x;
            record.min_y = v.y < record.min_y ? v.y : record.min_y;
            record.max_y = v.y > record.max_y ? v.y : record.max_y;
            record.min_depth = v.w < record.min_depth ? v.w : record.min_depth;
            record.depth = v.w > record.depth ? v.w : record.depth;
        }
        if (valid && (record.depth < 0.0f || record.min_depth < 0.0f || record.max_x < -1.0f || record.min_x > 1.0f ||
                      record.max_y < -1.0f || record.min_y > 1.0f)) {
            valid = false;
        }
        if (!valid) {
            indices[index] = 0xffffffffu;
        } else {
            indices[index] = index;
        }
    }
    qsort(indices, count, sizeof(*indices), SortByDepth);
    queries_prepared = true;
}

bool OccluderSet::IsOccludedSphere(nuvec_s const *center, float radius) {
    if (!queries_prepared)
        return false;
    NUVEC4 projected;
    NuVec4MtxTransform(&projected, const_cast<NUVEC *>(center), &projection_matrix);
    if (radius > projected.w)
        return false;
    projected.x /= projected.w;
    i32 limit = static_cast<i32>(count) < 101 ? static_cast<i32>(count) : 100;
    projected.y /= projected.w;
    projected.z /= projected.w;
    float screen_radius = radius / projected.w;
    NUVEC4 normal;
    for (i32 i = 0; i != limit; ++i) {
        if (indices[i] == 0xffffffffu)
            continue;
        OccluderRecord &record = occluders[indices[i]];
        if (projected.w - radius - 2.0f < record.depth)
            return false;
        if (projected.x - screen_radius > record.max_x || projected.y - screen_radius > record.max_y ||
            record.min_x > projected.x + screen_radius || record.min_y > projected.y + screen_radius)
            continue;
        NUVEC4 *v = record.transformed;
        float winding = (v[1].y - v[0].y) * (v[2].x - v[0].x) - (v[1].x - v[0].x) * (v[2].y - v[0].y);
        NUVEC4 *start = &v[(!(winding > 0.0f)) * 3];
        NUVEC4 *end = &v[(winding <= 0.0f) + 1];
        normal.y = -(end->x - start->x);
        normal.x = end->y - start->y;
        normal.z = normal.w = 0.0f;
        NuVecNorm(reinterpret_cast<NUVEC *>(&normal), reinterpret_cast<NUVEC *>(&normal));
        if (__builtin_expect(
                (projected.x - start->x) * normal.x + (projected.y - start->y) * normal.y + 0.0f - screen_radius < 0.0f,
                0))
            continue;
        start = &v[(!(winding > 0.0f)) + 1];
        end = &v[(winding > 0.0f) + 1];
        normal.y = -(end->x - start->x);
        normal.x = end->y - start->y;
        normal.z = normal.w = 0.0f;
        NuVecNorm(reinterpret_cast<NUVEC *>(&normal), reinterpret_cast<NUVEC *>(&normal));
        if ((projected.x - start->x) * normal.x + (projected.y - start->y) * normal.y + 0.0f - screen_radius < 0.0f)
            continue;
        start = &v[(winding > 0.0f) + 1];
        end = &v[(winding > 0.0f) * 3];
        normal.x = end->y - start->y;
        normal.y = -(end->x - start->x);
        normal.z = normal.w = 0.0f;
        NuVecNorm(reinterpret_cast<NUVEC *>(&normal), reinterpret_cast<NUVEC *>(&normal));
        if ((projected.x - start->x) * normal.x + (projected.y - start->y) * normal.y + 0.0f - screen_radius < 0.0f)
            continue;
        start = &v[(winding > 0.0f) * 3];
        end = &v[(!(winding > 0.0f)) * 3];
        normal.x = end->y - start->y;
        normal.y = -(end->x - start->x);
        normal.z = normal.w = 0.0f;
        NuVecNorm(reinterpret_cast<NUVEC *>(&normal), reinterpret_cast<NUVEC *>(&normal));
        if ((projected.x - start->x) * normal.x + (projected.y - start->y) * normal.y + 0.0f - screen_radius < 0.0f)
            continue;
        return true;
    }
    return false;
}

bool OccluderSet::IsOccludedOBB(nuvec_s const *minimum, nuvec_s const *maximum, numtx_s const *matrix) {
    if (!queries_prepared)
        return false;
    NUVEC4 corners[8] = {{minimum->x, maximum->y, minimum->z, 1.0f}, {minimum->x, maximum->y, maximum->z, 1.0f},
                         {maximum->x, maximum->y, maximum->z, 1.0f}, {maximum->x, maximum->y, minimum->z, 1.0f},
                         {minimum->x, minimum->y, minimum->z, 1.0f}, {minimum->x, minimum->y, maximum->z, 1.0f},
                         {maximum->x, minimum->y, maximum->z, 1.0f}, {maximum->x, minimum->y, minimum->z, 1.0f}};
    NUMTX transform;
    if (matrix)
        NuMtxMulH(&transform, const_cast<NUMTX *>(matrix), &projection_matrix);
    else
        transform = projection_matrix;
    float min_x = 1000000.0f, min_y = 1000000.0f, min_depth = 1000000.0f;
    float max_x = -1000000.0f, max_y = -1000000.0f;
    for (u32 i = 0; i < 8; ++i) {
        NUVEC4 &v = corners[i];
        NuVec4MtxTransform(&v, reinterpret_cast<NUVEC *>(&v), &transform);
        float w = v.w;
        if (w <= 0.0f)
            return false;
        v.x /= w;
        v.y /= w;
        v.z /= w;
        min_x = v.x < min_x ? v.x : min_x;
        min_y = v.y < min_y ? v.y : min_y;
        max_x = v.x > max_x ? v.x : max_x;
        min_depth = v.w < min_depth ? v.w : min_depth;
        max_y = v.y > max_y ? v.y : max_y;
    }
    i32 limit = static_cast<i32>(count) < 100 ? static_cast<i32>(count) : 100;
    for (i32 i = 0; i != limit; ++i) {
        if (__builtin_expect(indices[i] == 0xffffffffu, 0))
            continue;
        OccluderRecord &record = occluders[indices[i]];
        if (record.depth > min_depth - 2.0f)
            return false;
        if (min_x > record.max_x || min_y > record.max_y || record.min_x > max_x || record.min_y > max_y)
            continue;
        NUVEC4 *v = record.transformed;
        float winding = (v[2].x - v[0].x) * (v[1].y - v[0].y) - (v[1].x - v[0].x) * (v[2].y - v[0].y);
        bool inside = true;
        for (i32 edge = 0; edge < 4 && inside; ++edge) {
            i32 start = !(winding <= 0.0f) ? edge : 3 - edge;
            i32 end = winding > 0.0f ? (edge + 1) & 3 : (6 - edge) & 3;
            NUVEC4 normal;
            normal.y = -(v[end].x - v[start].x);
            normal.x = v[end].y - v[start].y;
            normal.z = normal.w = 0.0f;
            NuVecNorm(reinterpret_cast<NUVEC *>(&normal), reinterpret_cast<NUVEC *>(&normal));
            for (u32 corner = 0; corner < 8; ++corner) {
                float distance =
                    (corners[corner].x - v[start].x) * normal.x + (corners[corner].y - v[start].y) * normal.y + 0.0f;
                if (__builtin_expect(distance < 0.01f, 0)) {
                    inside = false;
                    break;
                }
            }
        }
        if (__builtin_expect(!!(inside), 1))
            return true;
    }
    return false;
}

void OccluderSet::RenderOccluders(bool depth_only) const {
    NUMTL *material = NULL;
    if (depth_only)
        material = queries_prepared ? ms_pZOnlyMtl2D : ms_pZOnlyMtl3D;
    if (queries_prepared) {
        ++NuPrimCSPos;
        NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_NORMALISED);
        NuPrim2DBegin(0, 5, material);
        for (u32 i = 0; i != count; ++i) {
            if (indices[i] == 0xffffffffu)
                continue;
            const OccluderRecord &record = occluders[indices[i]];
            if (record.min_depth < 0.0f || record.depth < 0.0f)
                continue;
            i32 colour = static_cast<u32>(record.vertices[0].x * record.vertices[0].z) | 0xff000000u;
            if (!g_NuPrim_NeedsOverbrightening)
                *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) =
                    ((colour >> 1) & 0x007f7f7f) | 0xff000000u;
            else
                *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) = colour;
            NuPrim2DAddXYZ(record.transformed[0].x, -record.transformed[0].y, record.transformed[0].z);
            if (!g_NuPrim_NeedsOverbrightening)
                *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) =
                    ((colour >> 1) & 0x007f7f7f) | 0xff000000u;
            else
                *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) = colour;
            NuPrim2DAddXYZ(record.transformed[1].x, -record.transformed[1].y, record.transformed[1].z);
            if (!g_NuPrim_NeedsOverbrightening)
                *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) =
                    ((colour >> 1) & 0x007f7f7f) | 0xff000000u;
            else
                *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) = colour;
            NuPrim2DAddXYZ(record.transformed[2].x, -record.transformed[2].y, record.transformed[2].z);
            if (!g_NuPrim_NeedsOverbrightening)
                *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) =
                    ((colour >> 1) & 0x007f7f7f) | 0xff000000u;
            else
                *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) = colour;
            NuPrim2DAddXYZ(record.transformed[2].x, -record.transformed[2].y, record.transformed[2].z);
            if (!g_NuPrim_NeedsOverbrightening)
                *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) =
                    ((colour >> 1) & 0x007f7f7f) | 0xff000000u;
            else
                *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) = colour;
            NuPrim2DAddXYZ(record.transformed[3].x, -record.transformed[3].y, record.transformed[3].z);
            if (!g_NuPrim_NeedsOverbrightening)
                *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) =
                    ((colour >> 1) & 0x007f7f7f) | 0xff000000u;
            else
                *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) = colour;
            NuPrim2DAddXYZ(record.transformed[0].x, -record.transformed[0].y, record.transformed[0].z);
        }
        NuPrim2DEnd();
        --NuPrimCSPos;
        NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    } else {
        NuPrim3DBegin(0, 5, material, NULL);
        u32 total = count;
        if (total != 0) {
            i32 previous_vertices = g_NuPrim_VertexCount;
            const OccluderRecord *record = occluders;
            for (u32 i = 0; i != total; ++i, ++record) {
                i32 colour = static_cast<u32>(record->vertices[0].x * record->vertices[0].z) | 0xff000000u;
                if (!g_NuPrim_NeedsOverbrightening)
                    *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) =
                        ((colour >> 1) & 0x007f7f7f) | 0xff000000u;
                else
                    *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) = colour;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->x = record->vertices[0].x;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->y = record->vertices[0].y;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->z = record->vertices[0].z;
                g_NuPrim_StreamBufferPtr->addr += 24;
                if (!g_NuPrim_NeedsOverbrightening)
                    *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) =
                        ((colour >> 1) & 0x007f7f7f) | 0xff000000u;
                else
                    *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) = colour;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->x = record->vertices[1].x;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->y = record->vertices[1].y;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->z = record->vertices[1].z;
                g_NuPrim_StreamBufferPtr->addr += 24;
                if (!g_NuPrim_NeedsOverbrightening)
                    *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) =
                        ((colour >> 1) & 0x007f7f7f) | 0xff000000u;
                else
                    *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) = colour;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->x = record->vertices[2].x;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->y = record->vertices[2].y;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->z = record->vertices[2].z;
                g_NuPrim_StreamBufferPtr->addr += 24;
                if (!g_NuPrim_NeedsOverbrightening)
                    *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) =
                        ((colour >> 1) & 0x007f7f7f) | 0xff000000u;
                else
                    *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) = colour;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->x = record->vertices[2].x;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->y = record->vertices[2].y;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->z = record->vertices[2].z;
                g_NuPrim_StreamBufferPtr->addr += 24;
                if (!g_NuPrim_NeedsOverbrightening)
                    *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) =
                        ((colour >> 1) & 0x007f7f7f) | 0xff000000u;
                else
                    *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) = colour;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->x = record->vertices[3].x;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->y = record->vertices[3].y;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->z = record->vertices[3].z;
                g_NuPrim_StreamBufferPtr->addr += 24;
                if (g_NuPrim_NeedsOverbrightening)
                    *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) = colour;
                else
                    *reinterpret_cast<i32 *>(g_NuPrim_StreamBufferPtr->addr + 12) =
                        ((colour >> 1) & 0x007f7f7f) | 0xff000000u;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->x = record->vertices[0].x;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->y = record->vertices[0].y;
                reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr)->z = record->vertices[0].z;
                g_NuPrim_StreamBufferPtr->addr += 24;
            }
            g_NuPrim_VertexCount = previous_vertices + total * 6;
        }
        NuPrim3DEnd();
    }
}

void OcclusionManager::RenderZPass() const {
    if (initialized && enabled)
        current_set->RenderOccluders(true);
}

void OccluderSet::OnCameraSet() {
    queries_prepared = false;
}

void OcclusionManager::OnCameraSet() {
    if (initialized && enabled)
        current_set->OnCameraSet();
}

extern "C" {
    bool NuOcclusionManagerIsInitialised(void) {
        return g_OcclusionManager.initialized;
    }

    bool NuOcclusionManagerIsEnabled(void) {
        return g_OcclusionManager.initialized && g_OcclusionManager.enabled;
    }

    void NuOcclusionManagerSetEnabled(i32 enabled) {
        g_OcclusionManager.SetEnabled(enabled != 0);
    }

    void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold) {
        g_OcclusionManager.unknown_158 = threshold;
    }

    void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold) {
        g_OcclusionManager.unknown_15c = threshold;
    }

}

void NuOcclusionManagerBeginFrame(void) {
    g_OcclusionManager.BeginFrame();
}

extern "C" {
    void NuOcclusionManagerEndFrame(void) {
        g_OcclusionManager.EndFrame();
    }

    void NuOcclusionManagerOnCameraSet(void) {
        g_OcclusionManager.OnCameraSet();
    }

    void NuOcclusionManagerRenderZPass(void) {
        g_OcclusionManager.RenderZPass();
    }

    void NuDynamicLightSetDirectional(NuDynamicLight *light, f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane) {
        light->parameter_4 = 0;
        NuMtxSetOrthoBlend(&light->projection, left, right, bottom, top, near_plane, far_plane);
    }

extern "C" i32 NuDynamicLightIsEnabled(void *light) {
    return light != NULL;
}

    void NuDynamicLightSetEnabled(NuDynamicLight *light, i32 enabled) {
        if (__builtin_expect(enabled != 0 && light->enabled == 0 && light->render_set_count > 0, 0)) {
            NuDynamicLight::RenderSet *set = light->render_sets;
            NuDynamicLight::RenderSet *end = set + light->render_set_count;
            do {
                set->field_2a0 = set->field_29c;
                set->field_2e4 = set->field_2e0;
                ++set;
            } while (set != end);
        }
        light->enabled = enabled;
    }

    i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light) {
        return light->active_render_set_count;
    }

    void NuDynamicLightBeginCapture(NuDynamicLight *light, i32 render_set) {
        currentScene.unknown_3c = light;
        currentScene.unknown_38 = 1;
        currentScene.unknown_40 = render_set;
    }

    void NuDynamicLightEndCapture(void) {
        // Retail no-op.
    }

    void NuDynamicLightLookAt(NuDynamicLight *light, NUVEC *eye, NUVEC *target, NUVEC *up) {
        NUVEC direction;
        direction.z = target->z - eye->z;
        direction.y = target->y - eye->y;
        direction.x = target->x - eye->x;
        light->direction_w = 0.0f;
        light->direction = direction;
        NuVecNorm(&light->direction, &light->direction);
        NUVEC position;
        position.z = eye->z;
        position.y = eye->y;
        position.x = eye->x;
        light->position_w = 1.0f;
        light->position = position;
        NuMtxLookAtD3D(&light->view, eye, target, up);
    }

    NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material) {
        if (material->shader_desc.unknown_1b4 != 0) {
            return &light->render_sets[render_set].display_lists[1];
        }
        return &light->render_sets[render_set].display_lists[0];
    }

    NUMTX *NuDynamicLightGetView(NuDynamicLight *light) {
        return &light->view;
    }

    NUMTX *NuDynamicLightGetProjection(NuDynamicLight *light) {
        return &light->projection;
    }

    void NuDynamicLightSetParameterf(NuDynamicLight *light, i32 parameter, f32 value) {
        switch (parameter) {
            case 0:
                light->parameter_7c0 = value;
                break;
            case 1:
                light->parameter_7c4 = value;
                break;
            case 2:
                light->parameter_7c8 = value;
                break;
            case 3:
                light->parameter_7cc = value;
                break;
            case 6:
            case 7:
                light->render_sets[0].parameter_110 = value;
                break;
            case 8:
                light->render_sets[1].parameter_110 = value;
                break;
            case 9:
            case 10:
            case 11:
                light->render_sets[0].parameter_104 = value;
                break;
            case 12:
            case 13:
            case 14:
                light->render_sets[0].parameter_100 = value;
                break;
            case 15:
            case 16:
                light->render_sets[1].parameter_104 = value;
                break;
            case 17:
            case 18:
                light->render_sets[1].parameter_100 = value;
                break;
            case 19:
                light->parameter_7d0 = value;
                break;
            case 20:
                light->parameter_7d4 = value;
                break;
        }
    }

    f32 NuDynamicLightGetParameterf(NuDynamicLight *light, i32 parameter) {
        switch (parameter) {
            case 0:
                return light->parameter_7c0;
            case 1:
                return light->parameter_7c4;
            case 2:
                return light->parameter_7c8;
            case 3:
                return light->parameter_7cc;
            case 6:
            case 7:
                return light->render_sets[0].parameter_110;
            case 8:
                return light->render_sets[1].parameter_110;
            case 9:
            case 10:
            case 11:
            case 15:
            case 16:
                return light->render_sets[1].parameter_104;
            case 12:
            case 13:
            case 14:
            case 17:
            case 18:
                return light->render_sets[0].parameter_100;
            case 19:
                return light->parameter_7d0;
            case 20:
                return light->parameter_7d4;
            default:
                return 0.0f;
        }
    }

    void NuDynamicLightSetParameteri(NuDynamicLight *light, i32 parameter, i32 value) {
        if (parameter == 4) {
            light->parameter_4 = value;
        } else if (parameter == 5) {
            light->parameter_5 = value;
        }
    }

    i32 NuDynamicLightGetParameteri(NuDynamicLight *light, i32 parameter) {
        switch (parameter) {
            case 4:
                return light->parameter_4;
            case 5:
                return light->parameter_5;
            default:
                return 0;
        }
    }

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

    void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id) {
        NuDynamicLight::RenderSet &set = light->render_sets[index];
        set.reserved_33c[set.geometry_count++] = scene_id;
    }

    i32 NuDynamicLightIsUsedOnSpecials(NuDynamicLight *light) {
        return light->used_on_specials;
    }

    void NuDynamicLightSetUsedOnSpecials(NuDynamicLight *light, i32 enabled) {
        light->used_on_specials = enabled;
    }

}

void NuDynamicLight::destroy(NuDynamicLight *light) {
    i32 index = (reinterpret_cast<u8 *>(light) - reinterpret_cast<u8 *>(dynamicLightPool.slots)) /
                i32(sizeof(dynamicLightPool.slots[0]));
    dynamicLightPool.next = index;
    dynamicLightPool.occupied &= ~(1 << (index & 7));
}

extern "C" {
    void NuDynamicLightDestroy(NuDynamicLight *light) {
        NuDynamicLight::destroy(light);
    }

}

void NuDynamicLight::setCameraViewProj(NUMTX *view, NUMTX *projection) {
    cacheCameraView = *view;
    cacheCameraProj = *projection;
}

bool NuDynamicLight::testShadowExtrusion(const VuVec &minimum, const VuVec &maximum, i32 set_index) {
    const RenderSet &set = render_sets[set_index];
    const f32 min_x = minimum.x, min_y = minimum.y, min_z = minimum.z;
    const f32 max_y = maximum.y, max_x = maximum.x, max_z = maximum.z;
    const i32 count = set.shadow_plane_count;
    for (i32 i = 0; i < count; ++i) {
        const NUVEC4 &plane = set.shadow_planes[i];
        f32 x0 = min_x * plane.x;
        f32 y0 = min_y * plane.y;
        f32 z0 = min_z * plane.z;
        f32 xy00 = x0 + y0;
        if ((xy00 + z0) + plane.w > 0.0f)
            continue;
        f32 y1 = max_y * plane.y;
        f32 xy01 = x0 + y1;
        if ((xy01 + z0) + plane.w > 0.0f)
            continue;
        f32 x1 = max_x * plane.x;
        f32 xy11 = y1 + x1;
        if ((z0 + xy11) + plane.w > 0.0f)
            continue;
        f32 xy10 = y0 + x1;
        if ((z0 + xy10) + plane.w > 0.0f)
            continue;
        f32 z1 = max_z * plane.z;
        if ((xy00 + z1) + plane.w > 0.0f)
            continue;
        if ((xy01 + z1) + plane.w > 0.0f)
            continue;
        if ((xy11 + z1) + plane.w > 0.0f)
            continue;
        if ((xy10 + z1) + plane.w > 0.0f)
            continue;
        return false;
    }
    return true;
}

extern "C" {
    i32 NuDynamicLightTestShadowExtrusionExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent,
                                                i32 render_set) {
        VuVec minimum;
        VuVec maximum;
        minimum.x = center->x - extent->x;
        minimum.y = center->y - extent->y;
        minimum.z = center->z - extent->z;
        maximum.x = center->x + extent->x;
        maximum.y = center->y + extent->y;
        maximum.z = center->z + extent->z;
        return light->testShadowExtrusion(minimum, maximum, render_set);
    }

}

i32 NuDynamicLight::testShadowExtrusions(const VuVec &minimum, const VuVec &maximum) {
    i32 result = 0;
    for (i32 i = 0; i < active_render_set_count; ++i) {
        if (testShadowExtrusion(minimum, maximum, i))
            result |= 1 << i;
    }
    return result;
}

struct nudynamiclight_s;

void NuDynamicLightTestShadowExtrusions(nudynamiclight_s *light, _vuv_s const *first, _vuv_s const *second, i32 *) {
    reinterpret_cast<NuDynamicLight *>(light)->testShadowExtrusions(*reinterpret_cast<const VuVec *>(first),
                                                                    *reinterpret_cast<const VuVec *>(second));
}

extern "C" {
    void NuDynamicLightTestShadowExtrusionsExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent) {
        VuVec minimum;
        VuVec maximum;
        const f32 extent_x = extent->x;
        const f32 extent_y = extent->y;
        const f32 extent_z = extent->z;
        const f32 center_x = center->x;
        const f32 center_y = center->y;
        const f32 center_z = center->z;
        minimum.x = center_x - extent_x;
        minimum.y = center_y - extent_y;
        minimum.z = center_z - extent_z;
        maximum.x = extent_x + center_x;
        maximum.y = extent_y + center_y;
        maximum.z = extent_z + center_z;
        light->testShadowExtrusions(minimum, maximum);
    }

    void NuDynamicLightTestShadowExtrusions(NuDynamicLight *light, const VuVec *first, const VuVec *second) {
        VuVec first_copy;
        VuVec second_copy;
        second_copy.x = second->x;
        second_copy.y = second->y;
        second_copy.z = second->z;
        second_copy.w = second->w;
        first_copy.x = first->x;
        first_copy.y = first->y;
        first_copy.z = first->z;
        first_copy.w = first->w;
        light->testShadowExtrusions(first_copy, second_copy);
    }

}

void NuDynamicLight::bindShaderResources(nushaderprogram_s *program) {
    NUMTX inverse;
    NuMtxInv(&inverse, &view);
    NUVEC light;
    if (parameter_4 == 0) {
        light.x = 0.0f;
        light.y = 0.0f;
        light.z = -1.0f;
        NuVecMtxRotate(&light, &light, &inverse);
        NuVecMtxRotate(&light, &light, reinterpret_cast<NUMTX *>(g_renderContext_view));
        NuVecNorm(&light, &light);
    } else {
        light.x = inverse.m30;
        light.y = inverse.m31;
        light.z = inverse.m32;
        NuVecMtxTransformH(&light, &light, reinterpret_cast<NUMTX *>(g_renderContext_view));
    }
    PostBlurSetVertexParam(program, 0x8085, &light.x, 4);
    NUVEC4 parameters;
    parameters.x = reserved_7c0[0];
    parameters.y = reserved_7c0[1] > 0.0f ? 1.0f / reserved_7c0[1] : 0.0f;
    parameters.z = 1.0f - reserved_7c8.x;
    parameters.w = reserved_7c8.y;
    PostBlurSetVertexParam(program, 0x8086, &parameters.x, 4);
    NUVEC4 split = {active_render_set_count == 1 ? 1000.0f : split_distances[1], 0, 0, 0};
    PostBlurSetVertexParam(program, 0x8088, &split.x, 4);
    NUMTX bias __attribute__((aligned(16))) = {0.5f, 0, 0, 0, 0, -0.5f, 0, 0, 0, 0, 0.5f, 0, 0.5f, 0.5f, 0.5f, 1};
    i32 semantic = 0x8e;
    for (i32 i = 0; i < active_render_set_count; ++i, semantic += 7) {
        RenderSet &set = render_sets[i];
        NUMTX shadow;
        NuMtxTranspose(&shadow, &set.shadow_transform);
        PostBlurSetVertexParam(program, (semantic - 5) | 0x8000, &shadow.m00, 16);
        NUMTX transform;
        NuMtxMulH(&transform, &inverse, &set.warp);
        NuMtxMulH(&transform, &transform, &bias);
        NuMtxInvH(&transform, &transform);
        NUVEC4 row_z = {transform.m02, transform.m12, transform.m22, transform.m32};
        NUVEC4 row_w = {transform.m03, transform.m13, transform.m23, transform.m33};
        PostBlurSetVertexParam(program, (semantic - 1) | 0x8000, &row_z.x, 4);
        PostBlurSetVertexParam(program, semantic | 0x8000, &row_w.x, 4);
        NUVEC4 shadow_parameters;
        shadow_parameters.x = set.parameter_100;
        shadow_parameters.y = 1.0f - set.parameter_104;
        shadow_parameters.z = reserved_7c8.z;
        shadow_parameters.w = reserved_7c8.w - reserved_7c8.z;
        PostBlurSetVertexParam(program, (semantic + 1) | 0x8000, &shadow_parameters.x, 4);
    }
}

i32 rawClip(VuVec const *input, VuVec *output, i32, VuVec const &plane) {
    i32 count = 0;
    for (i32 edge = 0; edge < 12; ++edge) {
        VuVec const &a = input[cubeEdgeIndices[edge][0]];
        VuVec const &b = input[cubeEdgeIndices[edge][1]];
        f32 da = plane.x * a.x + plane.y * a.y + plane.z * a.z + plane.w;
        f32 db = plane.x * b.x + plane.y * b.y + plane.z * b.z + plane.w;
        if (da > 0.0f) {
            output[count].x = a.x;
            output[count].y = a.y;
            output[count].z = a.z;
            output[count].w = a.w;
            if (db > 0.0f) {
                *reinterpret_cast<RawClipVec4 *>(&output[count + 1]) = *reinterpret_cast<const RawClipVec4 *>(&b);
                count += 2;
            } else {
                count += 2;
                f32 t = da / (da - db);
                output[count - 1].w = 0.0f;
                output[count - 1].y = a.y + (b.y - a.y) * t;
                output[count - 1].z = a.z + (b.z - a.z) * t;
                output[count - 1].x = a.x + (b.x - a.x) * t;
            }
        } else if (db > 0.0f) {
            output[count].x = b.x;
            output[count].y = b.y;
            output[count].z = b.z;
            output[count].w = b.w;
            count += 2;
            f32 t = -da / (db - da);
            output[count - 1].w = 0.0f;
            output[count - 1].y = a.y + (b.y - a.y) * t;
            output[count - 1].z = a.z + (b.z - a.z) * t;
            output[count - 1].x = a.x + (b.x - a.x) * t;
        }
    }
    return count;
}

void NuDynamicLight::computeLightSpace(NUVEC *direction, NUVEC *camera, NUMTX *view, NUMTX *inverse) {
    NuMtxSetIdentity(inverse);
    NUVEC *up = reinterpret_cast<NUVEC *>(&inverse->m10);
    up->z = -direction->z;
    up->y = -direction->y;
    up->x = -direction->x;
    NUVEC cross;
    cross.z = up->y * camera->x - up->x * camera->y;
    cross.y = up->x * camera->z - up->z * camera->x;
    cross.x = up->z * camera->y - up->y * camera->z;
    NUVEC forward;
    forward.z = up->x * cross.y - up->y * cross.x;
    forward.y = up->z * cross.x - up->x * cross.z;
    forward.x = up->y * cross.z - up->z * cross.y;
    f32 length = NuFsqrt(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
    forward.z /= length;
    forward.y /= length;
    forward.x /= length;
    NUVEC *axis = reinterpret_cast<NUVEC *>(&inverse->m20);
    axis->z = forward.z;
    axis->y = forward.y;
    axis->x = forward.x;
    NUVEC *right = reinterpret_cast<NUVEC *>(&inverse->m00);
    right->x = up->y * axis->z - up->z * axis->y;
    right->y = up->z * axis->x - up->x * axis->z;
    right->z = up->x * axis->y - up->y * axis->x;
    NuMtxInv(view, inverse);
}

void NuDynamicLight::computeWarpEffect(NuDynamicLight::RenderSet &set) {
    NUMTX render_view = set.view;
    NUMTX camera_view = cacheCameraView;
    NUMTX render_view_projection, inverse_view_projection, inverse_render_view, inverse_camera_view;
    NuMtxMulH(&render_view_projection, &render_view, &set.projection);
    NuMtxInvH(&inverse_view_projection, &render_view_projection);
    NuMtxInv(&inverse_render_view, &render_view);
    NuMtxInv(&inverse_camera_view, &camera_view);
    NUVEC light_forward = *NUMTX_GET_ROW_VEC(&inverse_render_view, 2);
    NUVEC camera_forward = *NUMTX_GET_ROW_VEC(&inverse_camera_view, 2);
    NUMTX light_space, inverse_light_space;
    computeLightSpace(&light_forward, &camera_forward, &light_space, &inverse_light_space);
    VuVec points[10];
    const NUVEC4 *source = set.corners;
    VuVec *destination = points;
    const NUVEC4 *end = source + 8;
    while (source < end) {
        *destination++ = VuVec(source->x, source->y, source->z, source->w);
        ++source;
    }
    points[8].x = direction.x * 200.0f + points[0].x;
    points[8].y = direction.y * 200.0f + points[0].y;
    points[8].z = direction.z * 200.0f + points[0].z;
    points[8].w = 0.0f;
    points[9].x = points[0].x - direction.x * 200.0f;
    points[9].y = points[0].y - direction.y * 200.0f;
    points[9].z = points[0].z - direction.z * 200.0f;
    points[9].w = 0.0f;
    VuVec transformed[10];
    f32 min_z = FLT_MAX, min_y = FLT_MAX, max_z = -FLT_MAX, max_y = -FLT_MAX;
    WarpAccumulatePoint(points[0], light_space, transformed[0], min_z, min_y, max_z, max_y);
    WarpAccumulatePoint(points[1], light_space, transformed[1], min_z, min_y, max_z, max_y);
    WarpAccumulatePoint(points[2], light_space, transformed[2], min_z, min_y, max_z, max_y);
    WarpAccumulatePoint(points[3], light_space, transformed[3], min_z, min_y, max_z, max_y);
    WarpAccumulatePoint(points[4], light_space, transformed[4], min_z, min_y, max_z, max_y);
    WarpAccumulatePoint(points[5], light_space, transformed[5], min_z, min_y, max_z, max_y);
    WarpAccumulatePoint(points[6], light_space, transformed[6], min_z, min_y, max_z, max_y);
    WarpAccumulatePoint(points[7], light_space, transformed[7], min_z, min_y, max_z, max_y);
    WarpAccumulatePoint(points[8], light_space, transformed[8], min_z, min_y, max_z, max_y);
    WarpAccumulatePoint(points[9], light_space, transformed[9], min_z, min_y, max_z, max_y);
    f32 fovy, aspect, camera_near, camera_far;
    NuMtxGetPerspectiveD3D(&cacheCameraProj, &fovy, &aspect, &camera_near, &camera_far);
    f32 depth = max_z - min_z;
    camera_far = depth;
    f32 weight = 1.0f - fabsf(camera_forward.x * light_forward.x + camera_forward.y * light_forward.y +
                              camera_forward.z * light_forward.z);
    f32 near_plane = (NuFsqrt(depth * camera_near) + camera_near) * set.warp_factor / weight;
    NUVEC camera_position = {inverse_camera_view.m30, inverse_camera_view.m31, inverse_camera_view.m32};
    NUVEC camera_light;
    NuVecMtxTransformH(&camera_light, &camera_position, &light_space);
    NUVEC translation = {-camera_light.x, -((max_y + min_y) * 0.5f), -(min_z - near_plane)};
    f32 left = FLT_MAX, bottom = FLT_MAX, right = -FLT_MAX, top = -FLT_MAX;
    WarpProjectBounds(transformed[0], translation, near_plane, left, bottom, right, top);
    WarpProjectBounds(transformed[1], translation, near_plane, left, bottom, right, top);
    WarpProjectBounds(transformed[2], translation, near_plane, left, bottom, right, top);
    WarpProjectBounds(transformed[3], translation, near_plane, left, bottom, right, top);
    WarpProjectBounds(transformed[4], translation, near_plane, left, bottom, right, top);
    WarpProjectBounds(transformed[5], translation, near_plane, left, bottom, right, top);
    WarpProjectBounds(transformed[6], translation, near_plane, left, bottom, right, top);
    WarpProjectBounds(transformed[7], translation, near_plane, left, bottom, right, top);
    WarpProjectBounds(transformed[8], translation, near_plane, left, bottom, right, top);
    WarpProjectBounds(transformed[9], translation, near_plane, left, bottom, right, top);
    NUMTX offset, frustum;
    NuMtxSetTranslation(&offset, &translation);
    NuMtxSetFrustumBlend(&frustum, left, right, bottom, top, near_plane, depth + near_plane);
    NUMTX axes = {1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1};
    NUMTX shifted_light, projected_light, result;
    NuMtxMulH(&shifted_light, &light_space, &offset);
    NuMtxMulH(&projected_light, &shifted_light, &frustum);
    NuMtxMulH(&result, &projected_light, &axes);
    set.warp = result;
}

void NuDynamicLight::refreshShadowTransform(RenderSet &set) {
    NUMTX inverse_camera;
    NuMtxInvH(&inverse_camera, &cacheCameraView);
    computeWarpEffect(set);
    NUMTX bias = {0.5f, 0, 0, 0, 0, -0.5f, 0, 0, 0, 0, 0.5f, 0, 0.5f, 0.5f, 0.5f, 1};
    NuMtxMulH(&set.shadow_transform, &inverse_camera, &set.warp);
    NuMtxMulH(&set.shadow_transform, &set.shadow_transform, &bias);
}

void NuDynamicLight::computeBoundingSpace(const VuVec *points, VuMtx *matrix) {
    NUVEC minimum = {FLT_MAX, FLT_MAX, FLT_MAX};
    NUVEC maximum = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
    for (i32 i = 0; i < 8; i++) {
        const VuVec &point = points[i];
        if (point.x < minimum.x)
            minimum.x = point.x;
        else if (point.x > maximum.x)
            maximum.x = point.x;
        if (point.y < minimum.y)
            minimum.y = point.y;
        else if (point.y > maximum.y)
            maximum.y = point.y;
        if (point.z < minimum.z)
            minimum.z = point.z;
        else if (point.z > maximum.z)
            maximum.z = point.z;
    }
    NuMtxSetOrthoBlend(&matrix->matrix, minimum.x, maximum.x, minimum.y, maximum.y, minimum.z, maximum.z);
}

void NuDynamicLight::computeClippingPlanes(const VuMtx &matrix, bool zero_near, VuVec &left, VuVec &right,
                                           VuVec &bottom, VuVec &top, VuVec &near_plane, VuVec &far_plane) {
    const VuVec symmetric[8] = {{-1, -1, -1, 1}, {-1, 1, -1, 1}, {1, 1, -1, 1}, {1, -1, -1, 1},
                                {-1, -1, 1, 1},  {-1, 1, 1, 1},  {1, 1, 1, 1},  {1, -1, 1, 1}};
    const VuVec positive[8] = {{-1, -1, 0, 1}, {-1, 1, 0, 1}, {1, 1, 0, 1}, {1, -1, 0, 1},
                               {-1, -1, 1, 1}, {-1, 1, 1, 1}, {1, 1, 1, 1}, {1, -1, 1, 1}};
    const VuVec *source = zero_near ? positive : symmetric;
    VuVec corners[8];
    VuVec *destination = corners;
    const VuVec *end = source + 8;
    do {
        *destination++ = LightClipTransform(*source++, matrix.matrix);
    } while (source != end);
    NUVEC near_up = LightClipEdge(corners[1], corners[0]);
    NUVEC depth_left = LightClipEdge(corners[4], corners[0]);
    NUVEC near_right = LightClipEdge(corners[3], corners[0]);
    NUVEC far_left = LightClipEdge(corners[5], corners[6]);
    NUVEC far_down = LightClipEdge(corners[7], corners[6]);
    NUVEC depth_right = LightClipEdge(corners[2], corners[6]);
    LightClipPlane(left, corners[0], near_up, depth_left);
    LightClipPlane(right, corners[6], depth_right, far_down);
    LightClipPlane(top, corners[6], far_left, depth_right);
    LightClipPlane(bottom, corners[0], depth_left, near_right);
    LightClipPlane(far_plane, corners[6], far_down, far_left);
    LightClipPlane(near_plane, corners[0], near_right, near_up);
}

void NuDynamicLight::computeFrustumCube(nucamera_s const *camera, VuVec *corners, VuVec *far_plane) {
    i32 angle = static_cast<i32>((camera->fov * 0.5f) * 10430.3779296875f);
    f32 near_z = camera->near_clip;
    f32 near_y = near_z * NU_SIN_LUT(angle) / NU_COS_LUT(angle);
    f32 near_x = near_y / camera->aspect;
    f32 ratio = camera->far_clip / near_z;
    corners[0].w = corners[1].w = corners[2].w = corners[3].w = 1.0f;
    corners[4].w = corners[5].w = corners[6].w = corners[7].w = 0.0f;
    corners[0].x = -near_x;
    corners[0].y = -near_y;
    corners[0].z = near_z;
    corners[1].x = -near_x;
    corners[1].y = near_y;
    corners[1].z = near_z;
    corners[2].x = near_x;
    corners[2].y = near_y;
    corners[2].z = near_z;
    corners[3].x = near_x;
    corners[3].y = -near_y;
    corners[3].z = near_z;
    corners[4].x = corners[0].x * ratio;
    corners[4].y = corners[0].y * ratio;
    corners[4].z = corners[0].z * ratio;
    corners[5].x = corners[1].x * ratio;
    corners[5].y = corners[1].y * ratio;
    corners[5].z = corners[1].z * ratio;
    corners[6].x = corners[2].x * ratio;
    corners[6].y = corners[2].y * ratio;
    corners[6].z = corners[2].z * ratio;
    corners[7].x = corners[3].x * ratio;
    corners[7].y = corners[3].y * ratio;
    corners[7].z = corners[3].z * ratio;
    NUMTX matrix;
    memcpy(&matrix, &camera->mtx, sizeof(matrix));
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[0]), reinterpret_cast<NUVEC *>(&corners[0]), &matrix);
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[1]), reinterpret_cast<NUVEC *>(&corners[1]), &matrix);
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[2]), reinterpret_cast<NUVEC *>(&corners[2]), &matrix);
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[3]), reinterpret_cast<NUVEC *>(&corners[3]), &matrix);
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[4]), reinterpret_cast<NUVEC *>(&corners[4]), &matrix);
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[5]), reinterpret_cast<NUVEC *>(&corners[5]), &matrix);
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[6]), reinterpret_cast<NUVEC *>(&corners[6]), &matrix);
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[7]), reinterpret_cast<NUVEC *>(&corners[7]), &matrix);
    NUVEC edge1 = {corners[5].x - corners[4].x, corners[5].y - corners[4].y, corners[5].z - corners[4].z};
    NUVEC edge2 = {corners[6].x - corners[4].x, corners[6].y - corners[4].y, corners[6].z - corners[4].z};
    far_plane->x = edge1.y * edge2.z - edge1.z * edge2.y;
    far_plane->y = edge1.z * edge2.x - edge1.x * edge2.z;
    far_plane->z = edge1.x * edge2.y - edge1.y * edge2.x;
    NuVecNorm(reinterpret_cast<NUVEC *>(far_plane), reinterpret_cast<NUVEC *>(far_plane));
    far_plane->w = -(far_plane->x * corners[4].x + far_plane->y * corners[4].y + far_plane->z * corners[4].z);
}

i32 NuDynamicLight::computeShadowClippingPlanes(const VuVec &direction, const VuVec *corners, VuVec *planes) {
    NUVEC extrusion = {direction.x * -120.0f, direction.y * -120.0f, direction.z * -120.0f};
    i32 count = 0;
    bool front[6];
    {
        NUVEC normal = ShadowCross(corners[1], corners[2], corners[0]);
        front[0] = extrusion.x * normal.x + extrusion.y * normal.y + extrusion.z * normal.z >= 0.0f;
        if (!front[0]) {
            VuVec &plane = planes[count++];
            plane.x = -normal.x;
            plane.y = -normal.y;
            plane.z = -normal.z;
            plane.w = 0.0f;
            NuVecNorm(&plane.xyz, &plane.xyz);
            plane.w = -(plane.x * corners[0].x + plane.y * corners[0].y + plane.z * corners[0].z);
        }
    }
    {
        NUVEC normal = ShadowCross(corners[2], corners[6], corners[3]);
        front[1] = extrusion.x * normal.x + extrusion.y * normal.y + extrusion.z * normal.z >= 0.0f;
        if (!front[1]) {
            VuVec &plane = planes[count++];
            plane.x = -normal.x;
            plane.y = -normal.y;
            plane.z = -normal.z;
            plane.w = 0.0f;
            NuVecNorm(&plane.xyz, &plane.xyz);
            plane.w = -(plane.x * corners[3].x + plane.y * corners[3].y + plane.z * corners[3].z);
        }
    }
    {
        NUVEC normal = ShadowCross(corners[6], corners[5], corners[7]);
        front[2] = extrusion.x * normal.x + extrusion.y * normal.y + extrusion.z * normal.z >= 0.0f;
        if (!front[2]) {
            VuVec &plane = planes[count++];
            plane.x = -normal.x;
            plane.y = -normal.y;
            plane.z = -normal.z;
            plane.w = 0.0f;
            NuVecNorm(&plane.xyz, &plane.xyz);
            plane.w = -(plane.x * corners[7].x + plane.y * corners[7].y + plane.z * corners[7].z);
        }
    }
    {
        NUVEC normal = ShadowCross(corners[5], corners[1], corners[4]);
        front[3] = extrusion.x * normal.x + extrusion.y * normal.y + extrusion.z * normal.z >= 0.0f;
        if (!front[3]) {
            VuVec &plane = planes[count++];
            plane.x = -normal.x;
            plane.y = -normal.y;
            plane.z = -normal.z;
            plane.w = 0.0f;
            NuVecNorm(&plane.xyz, &plane.xyz);
            plane.w = -(plane.x * corners[4].x + plane.y * corners[4].y + plane.z * corners[4].z);
        }
    }
    {
        NUVEC normal = ShadowCross(corners[5], corners[6], corners[1]);
        front[4] = extrusion.x * normal.x + extrusion.y * normal.y + extrusion.z * normal.z >= 0.0f;
        if (!front[4]) {
            VuVec &plane = planes[count++];
            plane.x = -normal.x;
            plane.y = -normal.y;
            plane.z = -normal.z;
            plane.w = 0.0f;
            NuVecNorm(&plane.xyz, &plane.xyz);
            plane.w = -(plane.x * corners[1].x + plane.y * corners[1].y + plane.z * corners[1].z);
        }
    }
    {
        NUVEC normal = ShadowCross(corners[0], corners[3], corners[4]);
        front[5] = extrusion.x * normal.x + extrusion.y * normal.y + extrusion.z * normal.z >= 0.0f;
        if (!front[5]) {
            VuVec &plane = planes[count++];
            plane.x = -normal.x;
            plane.y = -normal.y;
            plane.z = -normal.z;
            plane.w = 0.0f;
            NuVecNorm(&plane.xyz, &plane.xyz);
            plane.w = -(plane.x * corners[4].x + plane.y * corners[4].y + plane.z * corners[4].z);
        }
    }
    static const i32 edge_faces[12][2] = {{3, 0}, {4, 0}, {1, 0}, {5, 0}, {2, 3}, {2, 4},
                                          {2, 1}, {2, 5}, {5, 3}, {3, 4}, {4, 1}, {1, 5}};
    static const i32 edge_vertices[12][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6},
                                             {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};
    for (i32 edge = 0; edge < 12 && count <= 8; ++edge) {
        bool first_front = front[edge_faces[edge][0]];
        if (first_front == front[edge_faces[edge][1]])
            continue;
        const VuVec &first = corners[edge_vertices[edge][0]];
        const VuVec &second = corners[edge_vertices[edge][1]];
        VuVec extruded = {extrusion.x + first.x, extrusion.y + first.y, extrusion.z + first.z, 0.0f};
        VuVec &plane = planes[count++];
        if (first_front) {
            NUVEC normal = ShadowCross(second, extruded, first);
            plane.x = normal.x;
            plane.y = normal.y;
            plane.z = normal.z;
            NuVecNorm((NUVEC *)&plane, (NUVEC *)&plane);
            plane.w = -(plane.x * first.x + plane.y * first.y + plane.z * first.z);
        } else {
            NUVEC normal = ShadowCross(second, first, extruded);
            plane.x = normal.x;
            plane.y = normal.y;
            plane.z = normal.z;
            NuVecNorm((NUVEC *)&plane, (NUVEC *)&plane);
            plane.w = -(extruded.x * plane.x + extruded.y * plane.y + extruded.z * plane.z);
        }
    }
    return count;
}

void NuDynamicLight::computeShadowFrustrumCapsule(const VuVec &direction, const VuVec *corners, VuVec &center,
                                                  VuVec &end, float &radius) {
    center.x = corners[0].x;
    center.y = corners[0].y;
    center.z = corners[0].z;
    center.w = corners[0].w;
    for (i32 i = 1; i < 8; i++) {
        center.x += corners[i].x;
        center.y += corners[i].y;
        center.z += corners[i].z;
    }
    center.x *= 0.125f;
    center.y *= 0.125f;
    center.z *= 0.125f;
    radius = 0.0f;
    float distance;
    for (i32 i = 0; i < 8; i++) {
        float x = center.x - corners[i].x, y = center.y - corners[i].y, z = center.z - corners[i].z;
        distance = x * x + y * y + z * z;
        radius = distance > radius ? distance : radius;
    }
    // The original takes the final corner distance here, after the maximum stores.
    radius = NuFsqrt(distance);
    VuVec result = {center.x - direction.x * 100.0f, center.y - direction.y * 100.0f, center.z - direction.z * 100.0f,
                    0.0f};
    end.x = result.x;
    end.y = result.y;
    end.z = result.z;
    end.w = 0.0f;
}

void NuDynamicLight::setupCustomCameraFrustum(NUCAMERA *camera, const f32 *splits, i32 split_count) {
    NuMtxSetPerspectiveD3D(&cacheCameraProj, (180.0f * camera->fov) / 3.1415927410125732f, 1.0f / camera->aspect,
                           camera->near_clip, camera->far_clip);
    NuMtxInv(&cacheCameraView, &camera->mtx);
    SetLightCameraVector(camera_position, camera->mtx.m30, camera->mtx.m31, camera->mtx.m32, 1.0f);
    SetLightCameraVector(camera_forward, camera->mtx.m20, camera->mtx.m21, camera->mtx.m22, 0.0f);
    i32 interval_count = split_count - 1;
    active_render_set_count = interval_count;
    for (i32 i = 0; i < split_count; ++i) {
        split_distances[i] = splits[i];
    }
    f32 near_clip = camera->near_clip;
    f32 far_clip = camera->far_clip;
    for (i32 i = 0; i < interval_count; ++i) {
        RenderSet &set = render_sets[i];
        camera->near_clip = splits[i];
        camera->far_clip = splits[i + 1];
        computeFrustumCube(camera, reinterpret_cast<VuVec *>(set.corners), reinterpret_cast<VuVec *>(&set.far_plane));
        set.shadow_plane_count = computeShadowClippingPlanes(reinterpret_cast<const VuVec &>(direction),
                                                             reinterpret_cast<const VuVec *>(set.corners),
                                                             reinterpret_cast<VuVec *>(set.shadow_planes));
        set.view = view;
        set.projection = projection;
        computeShadowFrustrumCapsule(reinterpret_cast<const VuVec &>(direction),
                                     reinterpret_cast<const VuVec *>(set.corners),
                                     reinterpret_cast<VuVec &>(set.capsule_center),
                                     reinterpret_cast<VuVec &>(set.capsule_end), set.capsule_radius);
    }
    for (i32 i = 0; i < interval_count; ++i) {
        split_planes[i].x = 0.0f;
        split_planes[i].y = 0.0f;
        split_planes[i].z = 1.0f / splits[i + 1];
        split_planes[i].w = -1.0f;
    }
    camera->near_clip = near_clip;
    camera->far_clip = far_clip;
}

extern "C" {
    void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count) {
        light->setupCustomCameraFrustum(camera, splits, count);
    }

}

NuDeferredFilterGen::NuDeferredFilterGen() {
    memset(shadow_fbos, 0, sizeof(shadow_fbos));
    light_fbo = NULL;
    enabled = false;
    sample_count = 4;
    parameters[0] = 0.0f;
    parameters[1] = 0.5f;
}

extern "C" {
extern "C" void NuPostEffectInit(u32 flags, VARIPTR *buffer, VARIPTR buffer_end) {
    g_effectFlags = flags;
    if (flags & 0x0c)
        mainFilter = AllocatePostFilter<NuMainFilter>();
    if (flags & 0x10) {
        motionFilter = AllocatePostFilter<NuMotionFilter>();
        g_posteffect_shaderEngineFlag |= 0x20;
    }
    if (flags & 0x40)
        motionAccumFilter = AllocatePostFilter<NuMotionAccumFilter>();
    if (flags & 0x80)
        speedBlurFilter = AllocatePostFilter<NuSpeedBlurFilter>();
    if (flags & 0x20) {
        deferredFilter = AllocatePostFilter<NuDeferredFilter>();
        g_posteffect_shaderEngineFlag |= 0x10;
    }
    copyFilter = AllocatePostFilter<NuCopyFilter>();
    NuEffectTexLockVP(buffer, buffer_end.void_ptr);
    NuPostFilterGen::initSharedTextureResources(100, 100);
    if (flags & 0x0c)
        mainFilter->initTextureResources(100, 100);
    if (flags & 0x10)
        motionFilter->initTextureResources(100, 100);
    if (flags & 0x40)
        motionAccumFilter->initTextureResources(100, 100);
    if (flags & 0x80)
        speedBlurFilter->initTextureResources(100, 100);
    if (flags & 0x20)
        deferredFilter->initTextureResources(100, 100);
    if (flags & 1)
        g_backBufferCopy = NuEffectTexCreate2D(100, 100, 1, 1, 2);
    copyFilter->initTextureResources(100, 100);
    NuEffectTexUnlockVP();
    NuPostFilter::initSharedResources(100, 100);
    if (flags & 0x0c)
        mainFilter->initResources();
    if (flags & 0x10)
        motionFilter->initResources();
    if (flags & 0x40)
        motionAccumFilter->initResources();
    if (flags & 0x80)
        speedBlurFilter->initResources();
    if (flags & 0x20)
        deferredFilter->initResources();
    copyFilter->initResources();
    NuPostEffectReset();
}

}

void NuDeferredFilterGen::renderStencilMask(NuDynamicLight &) {
    STUBBED();
}

extern "C" {
    void NuPause(i32 paused) {
        nu2api_paused = (f32)paused;
    }

    void buildBitCountTable(void) {
        for (u32 value = 0; value < 256; ++value) {
            BitCountTable[value] = 0;
            for (u32 bit = 0; bit < 8; ++bit) {
                if ((value >> bit) & 1)
                    ++BitCountTable[value];
            }
        }
        isBitCountTable = 1;
    }

    void *NuAnimDataFixPtrs(void *animation, isize delta) {

        if (isBitCountTable == 0)
            buildBitCountTable();
        if (animation != NULL)
            animation = reinterpret_cast<void *>(reinterpret_cast<usize>(animation) + delta);
        nuanimdata_s *data = static_cast<nuanimdata_s *>(animation);
        data->node_name = static_cast<char *>(NuLegacyRelocatePointer(data->node_name, delta));
        data->chunks = static_cast<nuanimdatachunk_s **>(NuLegacyRelocatePointer(data->chunks, delta));
        if (data->chunks != NULL) {
            for (i32 i = 0; i < data->chunk_count; ++i) {
                data->chunks[i] = static_cast<nuanimdatachunk_s *>(NuLegacyRelocatePointer(data->chunks[i], delta));
                nuanimdatachunk_s *chunk = data->chunks[i];
                if (chunk != NULL) {
                    chunk->curve_sets =
                        static_cast<nuanimcurveset_s **>(NuLegacyRelocatePointer(chunk->curve_sets, delta));
                    if (chunk->curve_sets != NULL) {
                        for (i32 j = 0; j < chunk->node_count; ++j) {
                            chunk->curve_sets[j] =
                                static_cast<nuanimcurveset_s *>(NuLegacyRelocatePointer(chunk->curve_sets[j], delta));
                            nuanimcurveset_s *set = chunk->curve_sets[j];
                            if (set != NULL) {
                                set->constants = static_cast<f32 *>(NuLegacyRelocatePointer(set->constants, delta));
                                set->curves =
                                    static_cast<nuanimcurve_s **>(NuLegacyRelocatePointer(set->curves, delta));
                                if (set->curves != NULL) {
                                    for (i32 k = 0; k < set->curve_count; ++k) {
                                        void *relocated = NuLegacyRelocatePointer(set->curves[k], delta);
                                        set->curves[k] = static_cast<nuanimcurve_s *>(relocated);
                                        nuanimcurve_s *curve = set->curves[k];
                                        if (curve != NULL) {
                                            curve->keys =
                                                static_cast<nuanimkey_s *>(NuLegacyRelocatePointer(curve->keys, delta));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return data;
    }

    void *NuAnimDataLoadBuff(char *path, VARIPTR *buf, VARIPTR *buf_end) {
        buf->addr = ALIGN(buf->addr, 16);
        const i32 file_size = NuFileLoadBuffer(path, buf->void_ptr, static_cast<i32>(buf_end->addr - buf->addr));
        u8 *file_data = static_cast<u8 *>(buf->void_ptr);
        buf->addr = reinterpret_cast<usize>(file_data) + file_size;

        const isize delta = reinterpret_cast<isize>(file_data) - *reinterpret_cast<isize *>(file_data + 4);
        *reinterpret_cast<isize *>(file_data + 4) = delta;
        void *animation = NuAnimDataFixPtrs(*reinterpret_cast<void **>(file_data + 8), delta);
        *reinterpret_cast<void **>(file_data + 8) = animation;
        return animation;
    }

    f32 NuAnimCurveCalcVal2(nuanimcurve_s *curve, nuanimtime_s *time) {
        i32 key = 0;
        switch (time->time_byte) {
            case 0:
                key = BitCountTable[curve->key_mask[0] & time->time_mask];
                break;
            case 1:
                key = BitCountTable[curve->key_mask[0]] + BitCountTable[curve->key_mask[1] & time->time_mask];
                break;
            case 2:
                key = BitCountTable[curve->key_mask[0]];
                key += BitCountTable[curve->key_mask[1]];
                key += BitCountTable[curve->key_mask[2] & time->time_mask];
                break;
            case 3:
                key = BitCountTable[curve->key_mask[0]];
                key += BitCountTable[curve->key_mask[1]];
                key += BitCountTable[curve->key_mask[2]];
                key += BitCountTable[curve->key_mask[3] & time->time_mask];
                break;
        }
        nuanimkey_s *first = &curve->keys[key - 1];
        if (curve->flags & 1) {
            if ((curve->flags & 2) && key <= curve->key_count && time->time - first->time > first[1].time - time->time)
                return first[1].value;
            return first->value;
        }
        f32 value = first->value;
        f32 span = first[1].time - first->time;
        f32 delta = value - first[1].value;
        f32 t = (time->time - first->time) * first->reciprocal_span;
        f32 tangent0 = first->tangent * span;
        f32 tangent1 = first[1].tangent * span;
        return (((((delta + delta + tangent0 + tangent1) * t + delta * -3.0f) - (tangent0 + tangent0) - tangent1) * t +
                 tangent0) *
                t) +
               value;
    }

    void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix) {
        NUVEC rotation, translation, scale;
        NUANGVEC angles;
        if (set->flags & 1) {
            if (set->curves[3] != NULL) {
                rotation.x = NuAnimCurveCalcVal2(set->curves[3], time);
            } else {
                rotation.x = set->constants[3];
            }
            if (set->curves[4] != NULL) {
                rotation.y = NuAnimCurveCalcVal2(set->curves[4], time);
            } else {
                rotation.y = set->constants[4];
            }
            if (set->curves[5] != NULL) {
                rotation.z = NuAnimCurveCalcVal2(set->curves[5], time);
            } else {
                rotation.z = set->constants[5];
            }
            angles.x = static_cast<NUANG>(rotation.x * 10430.378f);
            angles.y = static_cast<NUANG>(rotation.y * 10430.378f);
            angles.z = static_cast<NUANG>(rotation.z * 10430.378f);
            NuMtxSetRotateXYZVU0(matrix, &angles);
        } else {
            NuMtxSetIdentity(matrix);
        }
        if (set->flags & 8) {
            if (set->curves[6] != NULL) {
                scale.x = NuAnimCurveCalcVal2(set->curves[6], time);
            } else {
                scale.x = set->constants[6];
            }
            if (set->curves[7] != NULL) {
                scale.y = NuAnimCurveCalcVal2(set->curves[7], time);
            } else {
                scale.y = set->constants[7];
            }
            if (set->curves[8] != NULL) {
                scale.z = NuAnimCurveCalcVal2(set->curves[8], time);
            } else {
                scale.z = set->constants[8];
            }
            NuMtxPreScaleVU0(matrix, &scale);
        }
        if (set->curves[0] != NULL) {
            translation.x = NuAnimCurveCalcVal2(set->curves[0], time);
        } else {
            translation.x = set->constants[0];
        }
        if (set->curves[1] != NULL) {
            translation.y = NuAnimCurveCalcVal2(set->curves[1], time);
        } else {
            translation.y = set->constants[1];
        }
        if (set->curves[2] != NULL) {
            translation.z = NuAnimCurveCalcVal2(set->curves[2], time);
        } else {
            translation.z = set->constants[2];
        }
        NuMtxTranslate(matrix, &translation);
        matrix->m02 = -matrix->m02;
        matrix->m12 = -matrix->m12;
        matrix->m20 = -matrix->m20;
        matrix->m21 = -matrix->m21;
        matrix->m23 = -matrix->m23;
        matrix->m32 = -matrix->m32;
    }

    f32 NuAnimCurve2CalcValEx(nuanimcurve2_s *curve, nuanimtime_s *time, u32 type) {
        nuanimcurvedata_s *data = curve->data.curvedata;
        u32 *key_mask = data->key_mask + time->chunk;
        if (type == 4) {
            i32 frame = static_cast<i32>(NuFloor(time->time_offset));
            return static_cast<f32>((*key_mask >> ((frame - 1) & 0x1f)) & 1);
        }

        i32 key = 0;
        switch (time->time_byte) {
            case 0:
                key = BitCountTable[reinterpret_cast<u8 *>(key_mask)[0] & time->time_mask];
                break;
            case 1:
                key = BitCountTable[reinterpret_cast<u8 *>(key_mask)[0]] +
                      BitCountTable[reinterpret_cast<u8 *>(key_mask)[1] & time->time_mask];
                break;
            case 2:
                key = BitCountTable[reinterpret_cast<u8 *>(key_mask)[0]] +
                      BitCountTable[reinterpret_cast<u8 *>(key_mask)[1]] +
                      BitCountTable[reinterpret_cast<u8 *>(key_mask)[2] & time->time_mask];
                break;
            case 3:
                key = (BitCountTable[reinterpret_cast<u8 *>(key_mask)[1]] +
                       BitCountTable[reinterpret_cast<u8 *>(key_mask)[0]]) +
                      (BitCountTable[reinterpret_cast<u8 *>(key_mask)[3] & time->time_mask] +
                       BitCountTable[reinterpret_cast<u8 *>(key_mask)[2]]);
                break;
        }
        f32 result = 0.0f;
        i32 key_offset = data->key_offsets[time->chunk];

        switch (type) {
            case 1: {
                u8 *key_data = static_cast<u8 *>(data->key_data);
                f32 *first = reinterpret_cast<f32 *>(key_data + (key + key_offset - 1) * 0x10);
                f32 span = first[4] - first[0];
                f32 value_delta = first[2] - first[6];
                f32 t = (time->time - first[0]) * first[1];
                f32 tangent0 = first[3] * span;
                f32 tangent1 = first[7] * span;
                result = (((((value_delta * 2.0f + tangent0 + tangent1) * t + value_delta * -3.0f) - tangent0 * 2.0f) -
                           tangent1) *
                              t +
                          tangent0) *
                             t +
                         first[2];
                break;
            }
            case 2: {
                u8 *key_data = static_cast<u8 *>(data->key_data);
                f32 *header = reinterpret_cast<f32 *>(key_data);
                u8 *first = key_data + (key + key_offset + 1) * 4;
                f32 first_time = static_cast<f32>(static_cast<u32>(first[3]));
                f32 span = static_cast<f32>(static_cast<u32>(first[7])) - first_time;
                f32 inverse_span = span == 0.0f ? 0.0f : 1.0f / span;
                f32 tangent1 = static_cast<f32>(*reinterpret_cast<i8 *>(first + 6)) * header[0] * span;
                f32 tangent0 = static_cast<f32>(*reinterpret_cast<i8 *>(first + 2)) * header[0] * span;
                f32 value0 = static_cast<f32>(*reinterpret_cast<i16 *>(first)) * header[1];
                f32 value_delta = value0 - static_cast<f32>(*reinterpret_cast<i16 *>(first + 4)) * header[1];
                f32 t = ((time->time - 1.0f) - first_time) * inverse_span;
                result = (((((value_delta * 2.0f + tangent0 + tangent1) * t - value_delta * 3.0f) - tangent0 * 2.0f) -
                           tangent1) *
                              t +
                          tangent0) *
                             t +
                         value0;
                break;
            }
            case 3:
                result = *reinterpret_cast<f32 *>(static_cast<u8 *>(data->key_data) + (key + key_offset - 1) * 8);
                break;
            case 6: {
                u8 *key_data = static_cast<u8 *>(data->key_data);
                f32 *header = reinterpret_cast<f32 *>(key_data);
                f32 header_3 = header[3];
                f32 header_2 = header[2];
                f32 header_1 = header[1];
                u8 *first = key_data + (key + key_offset + 3) * 4;
                f32 first_time = static_cast<f32>(static_cast<u32>(first[3])) * header_3;
                f32 header_0 = header[0];
                f32 next_time = static_cast<f32>(static_cast<u32>(first[7])) * header_3;
                if (next_time == first_time) {
                    next_time = first_time + 1.0f;
                }
                f32 span = next_time - first_time;
                f32 tangent0 = static_cast<f32>(*reinterpret_cast<i8 *>(first + 2)) * header_0 * span;
                f32 tangent1 = static_cast<f32>(*reinterpret_cast<i8 *>(first + 6)) * header_0 * span;
                f32 value0 = static_cast<f32>(*reinterpret_cast<i16 *>(first)) * header_1 + header_2;
                f32 value_delta =
                    value0 - (static_cast<f32>(*reinterpret_cast<i16 *>(first + 4)) * header_1 + header_2);
                f32 inverse_span = 1.0f / span;
                f32 t = ((time->time - 1.0f) - first_time) * inverse_span;
                result = (((((value_delta * 2.0f + tangent0 + tangent1) * t - value_delta * 3.0f) - tangent0 * 2.0f) -
                           tangent1) *
                              t +
                          tangent0) *
                             t +
                         value0;
                break;
            }
            case 5: {
                u8 *key_data = static_cast<u8 *>(data->key_data);
                f32 *header = reinterpret_cast<f32 *>(key_data);
                f32 header_0 = header[0];
                f32 header_1 = header[1];
                i16 *first = reinterpret_cast<i16 *>(key_data + (key + key_offset) * 6 + 0x0c);
                f32 header_2 = header[2];
                f32 first_time = static_cast<f32>(static_cast<u32>(static_cast<u16>(first[2])));
                f32 span = static_cast<f32>(static_cast<u32>(static_cast<u16>(first[5]))) - first_time;
                f32 tangent0 = static_cast<f32>(static_cast<i8>(first[1])) * header_0 * span;
                f32 value0 = static_cast<f32>(first[0]) * header_1 + header_2;
                f32 tangent1 = static_cast<f32>(static_cast<i8>(first[4])) * header_0 * span;
                f32 value_delta = value0 - (static_cast<f32>(first[3]) * header_1 + header_2);
                f32 inverse_span = 1.0f / span;
                f32 t = ((time->time - 1.0f) - first_time) * inverse_span;
                result = (((((value_delta * 2.0f + tangent0 + tangent1) * t - value_delta * 3.0f) - tangent0 * 2.0f) -
                           tangent1) *
                              t +
                          tangent0) *
                             t +
                         value0;
                break;
            }
            default:
                break;
        }
        return result;
    }

}

u32 NuAnimGetAnimDataSizeANI3(ani3_animheader_s *animation) {
    ani3_animheader_s *block = animation;
    while (block->next_block != 0) {
        block = reinterpret_cast<ani3_animheader_s *>(reinterpret_cast<u8 *>(block) + block->next_block);
    }
    if (block->node_flags == NULL) {
        return 0;
    }
    return reinterpret_cast<usize>(block->node_flags) + block->node_count - reinterpret_cast<usize>(animation);
}

void NuAnimRelocatePtrsANI3(ani3_animheader_s *animation, i32 offset) {
    ani3_animheader_s *block = animation;
    do {
        if (block->scale_min != NULL) {
            block->scale_min = reinterpret_cast<ani3_scalemin_s *>(reinterpret_cast<usize>(block->scale_min) + offset);
        }
        if (block->constants != NULL) {
            block->constants = reinterpret_cast<i16 *>(reinterpret_cast<usize>(block->constants) + offset);
        }
        if (block->curve_types != NULL) {
            block->curve_types = reinterpret_cast<u16 *>(reinterpret_cast<usize>(block->curve_types) + offset);
        }
        if (block->keys != NULL) {
            block->keys = reinterpret_cast<u8 *>(reinterpret_cast<usize>(block->keys) + offset);
        }
        if (block->node_flags != NULL) {
            block->node_flags = reinterpret_cast<u8 *>(reinterpret_cast<usize>(block->node_flags) + offset);
        }
        if (block->field_38 != NULL) {
            block->field_38 = reinterpret_cast<void *>(reinterpret_cast<usize>(block->field_38) + offset);
        }
        if (block->next_block == 0) {
            break;
        }
        block = reinterpret_cast<ani3_animheader_s *>(reinterpret_cast<u8 *>(block) + block->next_block);
    } while (true);
}

extern "C" {
    void NuAnimDataCalcTime(void *animation, f32 frame, nuanimtime_s *time) {
        u8 *data = static_cast<u8 *>(animation);
        const f32 duration = *reinterpret_cast<f32 *>(data);
        f32 clamped_frame;
        i32 chunk;
        if (duration <= frame) {
            if (duration == 1.0f) {
                time->time = 1.0f;
                time->chunk = 0;
                time->time_byte = 0;
                time->time_mask = 1;
                return;
            }
            clamped_frame = duration - 0.01f;
            time->time = clamped_frame;
            chunk = (static_cast<i32>(clamped_frame) - 1) >> 5;
        } else if (frame < 1.0f) {
            clamped_frame = 1.0f;
            time->time = 1.0f;
            chunk = 0;
        } else {
            clamped_frame = frame;
            time->time = frame;
            chunk = (static_cast<i32>(frame) - 1) >> 5;
        }

        time->chunk = chunk;
        const i32 chunk_count = *reinterpret_cast<i32 *>(data + 8);
        if (chunk_count <= chunk) {
            chunk = chunk_count - 1;
            time->chunk = chunk;
        }
        time->time_offset = clamped_frame - static_cast<f32>(chunk << 5);
        const i32 chunk_frame = static_cast<i32>(NuFloor(time->time_offset)) - 1;
        time->chunk_frame = static_cast<u32>(chunk_frame);
        time->time_byte = (chunk_frame / 8) & 0xff;
        time->time_mask = static_cast<u8>((1 << (chunk_frame % 8 + 1)) - 1);
    }

    void NuAnimData2CalcTime(nuanimdata2_s *anim, f32 frame, nuanimtime_s *time) {
        union {
            u32 magic;
            f32 duration;
        } header;
        header.magic = reinterpret_cast<ani3_animheader_s *>(anim)->magic;
        if (header.magic - 0x414e4934U <= 1) {
            ani3_animheader_s *ani3 = reinterpret_cast<ani3_animheader_s *>(anim);
            if (!(frame < 1.0f)) {
                if (frame >= ani3->frame_count) {
                    time->time = static_cast<f32>(static_cast<u32>(ani3->frame_count)) - 0.01f;
                } else {
                    time->time = frame;
                }
            } else {
                time->time = 1.0f;
            }
            return;
        }

        f32 clamped_frame;
        i32 chunk;
        if (header.duration <= frame) {
            if (header.duration == 1.0f) {
                time->time = 1.0f;
                time->chunk = 0;
                time->time_byte = 0;
                time->time_mask = 1;
                return;
            }
            clamped_frame = header.duration - 0.01f;
            time->time = clamped_frame;
            chunk = (static_cast<i32>(clamped_frame) - 1) >> 5;
        } else if (frame < 1.0f) {
            clamped_frame = 1.0f;
            time->time = 1.0f;
            chunk = 0;
        } else {
            clamped_frame = frame;
            time->time = frame;
            chunk = (static_cast<i32>(frame) - 1) >> 5;
        }
        time->chunk = chunk;
        if (chunk >= anim->chunk_count) {
            chunk = anim->chunk_count - 1;
            time->chunk = chunk;
        }
        time->time_offset = clamped_frame - static_cast<f32>(chunk << 5);
        i32 chunk_frame = static_cast<i32>(NuFloor(time->time_offset)) - 1;
        time->chunk_frame = chunk_frame;
        time->time_byte = (chunk_frame / 8) & 0xff;
        time->time_mask = static_cast<u8>((1 << (chunk_frame % 8 + 1)) - 1);
    }

    i32 NuAnimDataFindVersion(char *path) {
        u32 header[3] = {};
        NUFILE file = NuFileOpen(path, NUFILE_READ);
        if (file == 0) {
            return -1;
        }
        NuFileRead(file, header, sizeof(header));
        NuFileClose(file);
        return static_cast<i32>(header[0]);
    }

}

nuanimdatachunk_s *NuAnimDataChunkCreate(i32 curve_set_count) {
    nuanimdatachunk_s *chunk = static_cast<nuanimdatachunk_s *>(
        NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(nuanimdatachunk_s), 4, 1, "", 0));
    chunk->node_count = curve_set_count;
    chunk->valid_curve_set_count = 0;
    chunk->curve_sets = NULL;
    chunk->keys = NULL;
    chunk->curves = NULL;

    chunk->curve_sets = static_cast<nuanimcurveset_s **>(NuMemoryGet()->GetThreadMem()->_BlockAlloc(
        curve_set_count * sizeof(nuanimcurveset_s *), 4, 1, "", 0));
    memset(chunk->curve_sets, 0, curve_set_count * sizeof(nuanimcurveset_s *));
    return chunk;
}

extern "C" {
    void *NuAnimDataCreate(i32 chunk_count) {
        const u32 allocation_size = static_cast<u32>(chunk_count) * sizeof(void *) + 0x10;
        u8 *animation = static_cast<u8 *>(NU_ALLOC(allocation_size, 4, 1, "", 0));
        memset(animation, 0, allocation_size);
        *reinterpret_cast<i32 *>(animation + 8) = chunk_count;
        *reinterpret_cast<void ***>(animation + 0xc) = reinterpret_cast<void **>(animation + 0x10);
        return animation;
    }

    void *NuAnimCurveSetCreate(i32 curve_count) {
        if (curve_count == 0) {
            return NULL;
        }

        u8 *curve_set = static_cast<u8 *>(NU_ALLOC(0x10, 4, 1, "", 0));
        memset(curve_set, 0, 0x10);
        curve_set[0xc] = static_cast<u8>(curve_count);

        const u32 array_size = static_cast<u32>(curve_count) * sizeof(void *);
        void **curves = static_cast<void **>(NU_ALLOC(array_size, 4, 1, "", 0));
        *reinterpret_cast<void ***>(curve_set + 8) = curves;
        memset(curves, 0, array_size);

        void *curve_flags = NU_ALLOC(array_size, 4, 1, "", 0);
        *reinterpret_cast<void **>(curve_set + 4) = curve_flags;
        memset(curve_flags, 0, array_size);
        return curve_set;
    }

    void *NuAnimDataRead(NUFILE file) {

        if (isBitCountTable == 0) {
            buildBitCountTable();
        }

        char *name = NULL;
        const i32 name_size = NuFileReadInt(file);
        if (name_size != 0) {
            name = static_cast<char *>(NuMemoryGet()->GetThreadMem()->_BlockAlloc(name_size, 4, 1, "", 0));
            NuFileRead(file, name, name_size);
        }

        const f32 duration = NuFileReadFloat(file);
        const i32 chunk_count = NuFileReadInt(file);
        u8 *animation = static_cast<u8 *>(NuAnimDataCreate(chunk_count));
        *reinterpret_cast<f32 *>(animation) = duration;
        *reinterpret_cast<char **>(animation + 4) = name;

        for (i32 chunk_index = 0; chunk_index < *reinterpret_cast<i32 *>(animation + 8); ++chunk_index) {
            const i32 curve_set_count = NuFileReadInt(file);
            void **chunk_slot = *reinterpret_cast<void ***>(animation + 0xc) + chunk_index;
            u8 *chunk = reinterpret_cast<u8 *>(*chunk_slot = NuAnimDataChunkCreate(curve_set_count));
            *reinterpret_cast<i32 *>(chunk) = curve_set_count;

            u8 *curve_data = NULL;
            const i32 curve_data_count = NuFileReadInt(file);
            if (curve_data_count != 0) {
                const u32 curve_data_size = static_cast<u32>(curve_data_count) * 0x10;
                curve_data =
                    static_cast<u8 *>(NuMemoryGet()->GetThreadMem()->_BlockAlloc(curve_data_size, 4, 1, "", 0));
                *reinterpret_cast<void **>(chunk + 0xc) = curve_data;
                NuFileRead(file, curve_data, curve_data_size);
                curve_data = *reinterpret_cast<u8 **>(chunk + 0xc);
            } else {
                *reinterpret_cast<void **>(chunk + 0xc) = NULL;
            }

            u8 *shared_curves = NULL;
            const i32 shared_curve_count = NuFileReadInt(file);
            if (shared_curve_count != 0) {
                const u32 shared_curve_size = static_cast<u32>(shared_curve_count) * 0x10;
                shared_curves =
                    static_cast<u8 *>(NuMemoryGet()->GetThreadMem()->_BlockAlloc(shared_curve_size, 4, 1, "", 0));
                *reinterpret_cast<void **>(chunk + 0x10) = shared_curves;
                NuFileRead(file, shared_curves, shared_curve_size);
            } else {
                *reinterpret_cast<void **>(chunk + 0x10) = NULL;
            }

            for (i32 set_index = 0; set_index < curve_set_count; ++set_index) {
                const i32 curve_count = NuFileReadChar(file);
                if (curve_count == 0) {
                    continue;
                }

                void **set_slot = *reinterpret_cast<void ***>(chunk + 8) + set_index;
                *set_slot = NuAnimCurveSetCreate(curve_count);
                u8 *curve_set = static_cast<u8 *>((*reinterpret_cast<void ***>(chunk + 8))[set_index]);
                *reinterpret_cast<i32 *>(curve_set) = NuFileReadInt(file);
                for (i32 curve_index = 0;
                     curve_index < *reinterpret_cast<i8 *>(
                                       static_cast<u8 *>((*reinterpret_cast<void ***>(chunk + 8))[set_index]) + 0xc);
                     ++curve_index) {
                    curve_set = static_cast<u8 *>((*reinterpret_cast<void ***>(chunk + 8))[set_index]);
                    f32 *value = *reinterpret_cast<f32 **>(curve_set + 4) + curve_index;
                    *value = NuFileReadFloat(file);
                }
            }

            u8 *shared_cursor = *reinterpret_cast<u8 **>(chunk + 0x10);
            u8 *curve_data_cursor = curve_data;
            for (i32 set_index = 0; set_index < *reinterpret_cast<i32 *>(chunk); ++set_index) {
                u8 *curve_set = static_cast<u8 *>((*reinterpret_cast<void ***>(chunk + 8))[set_index]);
                if (curve_set == NULL) {
                    continue;
                }
                for (i32 curve_index = 0;
                     curve_index < *reinterpret_cast<i8 *>(
                                       static_cast<u8 *>((*reinterpret_cast<void ***>(chunk + 8))[set_index]) + 0xc);
                     ++curve_index) {
                    curve_set = static_cast<u8 *>((*reinterpret_cast<void ***>(chunk + 8))[set_index]);
                    if ((*reinterpret_cast<f32 **>(curve_set + 4))[curve_index] == FLT_MAX) {
                        (*reinterpret_cast<void ***>(curve_set + 8))[curve_index] = shared_cursor;
                        ++*reinterpret_cast<i32 *>(chunk + 4);
                        *reinterpret_cast<void **>(shared_cursor + 4) = curve_data_cursor;
                        curve_data_cursor += *reinterpret_cast<i32 *>(shared_cursor + 8) << 4;
                        shared_cursor += 0x10;
                    }
                }
            }
        }
        return animation;
    }

    void NuAnimCurveDestroy(void *curve) {
        void *keys = *reinterpret_cast<void **>(static_cast<u8 *>(curve) + 4);
        if (keys != NULL) {
            NU_FREE(keys);
        }
        NU_FREE(curve);
    }

    void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves) {
        if (curve_set == NULL) {
            return;
        }

        nuanimcurveset_s *set = static_cast<nuanimcurveset_s *>(curve_set);
        if (set->curves != NULL) {
            if (destroy_curves != 0) {
                for (i32 index = 0; index < set->curve_count; ++index) {
                    if (set->curves[index] != NULL) {
                        NuAnimCurveDestroy(set->curves[index]);
                    }
                }
            }
            NU_FREE(set->curves);
        }

        if (set->constants != NULL) {
            NU_FREE(set->constants);
        }
        NU_FREE(curve_set);
    }

}

void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk) {
    const i32 destroy_curves = chunk->curves == NULL;

    for (i32 index = 0; index < chunk->node_count; ++index) {
        if (chunk->curve_sets[index] != NULL) {
            NuAnimCurveSetDestroy(chunk->curve_sets[index], destroy_curves);
        }
    }

    if (chunk->keys != NULL) {
        NuMemoryGet()->GetThreadMem()->BlockFree(chunk->keys, 0);
    }
    if (chunk->curves != NULL) {
        NuMemoryGet()->GetThreadMem()->BlockFree(chunk->curves, 0);
    }
    if (chunk->curve_sets != NULL) {
        NuMemoryGet()->GetThreadMem()->BlockFree(chunk->curve_sets, 0);
    }
    NuMemoryGet()->GetThreadMem()->BlockFree(chunk, 0);
}

extern "C" {
    void NuAnimDataDestroy(void *animation) {
        nuanimdata_s *data = static_cast<nuanimdata_s *>(animation);
        for (i32 index = 0; index < data->chunk_count; ++index) {
            NuAnimDataChunkDestroy(data->chunks[index]);
        }

        if (data->node_name != NULL) {
            NU_FREE(data->node_name);
        }
        NU_FREE(animation);
    }

    void *NuAnimCurveCreate(void) {
        // Retail returns null.
        return NULL;
    }

    void SetProceduralAnimationFn(void *function) {
        JointProcAnimFn = reinterpret_cast<NUJOINTPROCANIMFN>(function);
    }

    // Original @0x2bd900. Apply the character's per-joint procedural offsets
    // to a decompressed ANI4/ANI5 pose before hierarchy evaluation.
    void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides) {
        if (buffer == NULL) {
            buffer = static_cast<nuanimbuff_s *>(globalbuffer);
        }

        constexpr f32 kRadiansToNuAngle = 10430.378f;
        constexpr f32 kNuAngleToRadians = 0.0000958738f;

        for (i32 override_index = 0; override_index < override_count; ++override_index) {
            NUJOINTANIM_s &joint_override = overrides[override_index];
            if (joint_override.joint_index >= object->joint_override_map_count) {
                continue;
            }

            const u8 joint_index = object->joint_override_map[joint_override.joint_index];
            if (joint_index == 0xff) {
                continue;
            }

            nuanimbuffjoint_s &joint = buffer->joints[joint_index];
            const u8 flags = joint_override.flags;
            if ((flags & NUJOINTANIM_ROTATION) != 0) {
                joint.rotation.x += joint_override.rotation.x;
                joint.rotation.y += joint_override.rotation.y;
                joint.rotation.z += joint_override.rotation.z;

                i32 angles[3] = {
                    static_cast<i32>(joint.rotation.x * kRadiansToNuAngle),
                    static_cast<i32>(joint.rotation.y * kRadiansToNuAngle),
                    static_cast<i32>(joint.rotation.z * kRadiansToNuAngle),
                };
                const u8 limit_flags[3] = {
                    NUJOINTANIM_LIMIT_ROTATION_X,
                    NUJOINTANIM_LIMIT_ROTATION_Y,
                    NUJOINTANIM_LIMIT_ROTATION_Z,
                };
                for (i32 axis = 0; axis < 3; ++axis) {
                    if ((flags & limit_flags[axis]) != 0) {
                        const u32 wrapped = static_cast<u32>(angles[axis]) & 0xffff;
                        if (wrapped >= 0x8000) {
                            angles[axis] = static_cast<i32>(wrapped) - 0x10000;
                        } else {
                            angles[axis] = static_cast<i32>(wrapped);
                        }
                        if (angles[axis] > joint_override.rotation_limit_start[axis]) {
                            angles[axis] = joint_override.rotation_limit_start[axis];
                        } else if (angles[axis] < joint_override.rotation_limit_end[axis]) {
                            angles[axis] = joint_override.rotation_limit_end[axis];
                        }
                    }
                }
                joint.rotation = {
                    static_cast<f32>(angles[0]) * kNuAngleToRadians,
                    static_cast<f32>(angles[1]) * kNuAngleToRadians,
                    static_cast<f32>(angles[2]) * kNuAngleToRadians,
                };
            }

            if ((flags & NUJOINTANIM_SCALE) != 0) {
                joint.scale.x += joint_override.scale.x;
                joint.scale.y += joint_override.scale.y;
                joint.scale.z += joint_override.scale.z;
            }
            if ((flags & NUJOINTANIM_TRANSLATION) != 0) {
                joint.translation.x += joint_override.translation.x;
                joint.translation.y += joint_override.translation.y;
                joint.translation.z += joint_override.translation.z;
            }
        }
    }

    void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR) {
        u32 *anim_buffer = reinterpret_cast<u32 *>(ALIGN(buf->addr, 0x10));
        buf->void_ptr = anim_buffer + 4;
        anim_buffer[2] = buf->addr;
        buf->addr = ALIGN(buf->addr, 0x10) + max_joints * 0x30;
        anim_buffer[3] = buf->addr;
        buf->addr += max_joints;
        reinterpret_cast<u16 *>(anim_buffer)[2] = static_cast<u16>(max_joints);
        anim_buffer[0] = 0;
        return anim_buffer;
    }

}

void NuAnimBuffInit(i32 max_joints, variptr_u *buf, variptr_u buf_end) {
    MaxAnimJoints = max_joints;
    globalbuffer = NuAnimBuffCreate(max_joints, buf, buf_end);
}

extern "C" {
    void NuAnimInit(i32 max_joints, VARIPTR *buf, VARIPTR buf_end) {

        buildBitCountTable();
        NuAnimBuffInit(max_joints, buf, buf_end);
    }

    // Original @0x2bbf90.  Scratch buffers are nested allocations from the
    // engine's LIFO scratch arena, so destruction deliberately releases them
    // in the reverse order.
    void NuAnimBuffCreateScratch(nuanimbuff_s *buffer) {
        if (buffer == NULL) {
            return;
        }

        nuanimbuff_s *global = static_cast<nuanimbuff_s *>(globalbuffer);
        buffer->joint_count = 0;
        buffer->max_joints = global->max_joints;
        buffer->joints = static_cast<nuanimbuffjoint_s *>(NuScratchAlloc32((buffer->max_joints * 3 + 3) * 0x10));
        buffer->joints = reinterpret_cast<nuanimbuffjoint_s *>(ALIGN(reinterpret_cast<usize>(buffer->joints), 0x10));
        buffer->joint_flags = static_cast<u8 *>(NuScratchAlloc32(buffer->max_joints));
    }

    // Original @0x2bc000.
    void NuAnimBuffDestroyScratch(nuanimbuff_s *buffer) {
        if (buffer == NULL) {
            return;
        }
        NuScratchRelease();
        NuScratchRelease();
        buffer->max_joints = 0;
        buffer->joints = NULL;
        buffer->joint_flags = NULL;
    }

    void ANI_FixUpAddrs(ani3_animheader_s *anim, isize delta, i32) {
        if (anim->magic != 0x414e4934) {
            return;
        }
        while (true) {
            if (anim->constants != NULL) {
                anim->constants = reinterpret_cast<i16 *>(reinterpret_cast<usize>(anim->constants) + (usize)delta);
            }
            if (anim->scale_min != NULL) {
                anim->scale_min =
                    reinterpret_cast<ani3_scalemin_s *>(reinterpret_cast<usize>(anim->scale_min) + (usize)delta);
            }
            if (anim->keys != NULL) {
                anim->keys = reinterpret_cast<u8 *>(reinterpret_cast<usize>(anim->keys) + (usize)delta);
            }
            if (anim->curve_types != NULL) {
                anim->curve_types = reinterpret_cast<u16 *>(reinterpret_cast<usize>(anim->curve_types) + (usize)delta);
            }
            if (anim->node_flags != NULL) {
                anim->node_flags = reinterpret_cast<u8 *>(reinterpret_cast<usize>(anim->node_flags) + (usize)delta);
            }
            if (anim->field_38 != NULL) {
                anim->field_38 = reinterpret_cast<void *>(reinterpret_cast<usize>(anim->field_38) + (usize)delta);
            }
            u16 next = anim->next_block;
            if (next == 0) {
                break;
            }
            delta += next;
            anim = reinterpret_cast<ani3_animheader_s *>(reinterpret_cast<usize>(anim) + next);
        }
    }

    void *NuAnimData2FixPtrs(void *data, isize delta, isize external_delta, i32 flags) {
        if (isBitCountTable == 0)
            buildBitCountTable();

        nuanimdata2_s *anim = static_cast<nuanimdata2_s *>(NuLegacyRelocatePointer(data, delta));
        if (anim != NULL) {
            if (*reinterpret_cast<u32 *>(&anim->duration) + 0xbeb1b6ccU < 2) {
                if (external_delta != 0) {
                    ANI_FixUpAddrs(reinterpret_cast<ani3_animheader_s *>(anim), delta, flags);
                } else {
                    ANI_FixUpAddrs(reinterpret_cast<ani3_animheader_s *>(anim),
                                   static_cast<isize>(reinterpret_cast<usize>(anim)), flags);
                }
            } else {
                anim->curves = static_cast<nuanimcurve2_s *>(NuLegacyRelocatePointer(anim->curves, delta));
                anim->curve_types = static_cast<u8 *>(NuLegacyRelocatePointer(anim->curve_types, delta));
                anim->node_flags = static_cast<u8 *>(NuLegacyRelocatePointer(anim->node_flags, delta));
                i32 curve_count = static_cast<i32>(anim->node_count) * static_cast<i32>(anim->curve_count);
                for (i32 i = 0; i < curve_count; ++i) {
                    if (anim->curve_types[i] != 0) {
                        nuanimcurvedata_s *curve = anim->curves[i].data.curvedata;
                        curve = static_cast<nuanimcurvedata_s *>(NuLegacyRelocatePointer(curve, delta));
                        anim->curves[i].data.curvedata = curve;
                        curve->key_mask = static_cast<u32 *>(NuLegacyRelocatePointer(curve->key_mask, delta));
                        curve->key_offsets = static_cast<u16 *>(NuLegacyRelocatePointer(curve->key_offsets, delta));
                        curve->key_data = NuLegacyRelocatePointer(curve->key_data, delta);
                    }
                }
            }
        }
        return anim;
    }

extern "C" void *NuAnimData2Relocate(void **data, VARIPTR *buf) {
    void *source = *data;
    u32 magic = *static_cast<u32 *>(source);
    if (magic == ANI3_MAGIC_VERSION_4 || magic == ANI3_MAGIC_VERSION_5) {
        u32 size = NuAnimGetAnimDataSizeANI3(static_cast<ani3_animheader_s *>(source));
        buf->addr = ALIGN(buf->addr, 16);
        void *destination = buf->void_ptr;
        buf->addr += size;
        *data = destination;
        memcpy(destination, source, size);
        NuAnimRelocatePtrsANI3(static_cast<ani3_animheader_s *>(destination),
                               reinterpret_cast<usize>(destination) - reinterpret_cast<usize>(source));
        return destination;
    }

    buf->addr = ALIGN(buf->addr, 16);
    memcpy(buf->void_ptr, *data, *static_cast<u32 *>(*data));
    *data = buf->void_ptr;
    buf->addr += *static_cast<u32 *>(*data);
    u32 *header = static_cast<u32 *>(*data);
    header[2] = reinterpret_cast<usize>(
        NuAnimData2FixPtrs(reinterpret_cast<void *>(static_cast<usize>(header[2])),
                           reinterpret_cast<usize>(header) - static_cast<usize>(header[1]), 0, 0));
    header = static_cast<u32 *>(*data);
    header[1] = reinterpret_cast<usize>(header);
    return reinterpret_cast<void *>(static_cast<usize>(header[2]));
}

extern "C" void *NuAnimData2Fixup(i32 file_size, void **data) {
    u32 *header = static_cast<u32 *>(*data);
    u32 magic = header[0];
    if (static_cast<i32>(header[1]) > static_cast<i32>(0x414e4934)) {
        return NuPtrBlockFix(header);
    }
    if (magic == 0x414e4933 || magic == 0x414e4934) {
        ANI_FixUpAddrs(reinterpret_cast<ani3_animheader_s *>(header),
                       static_cast<isize>(reinterpret_cast<usize>(header)), 0);
        return header;
    }
    header[0] = static_cast<u32>(file_size);
    const usize relocation_delta = reinterpret_cast<usize>(header) - static_cast<usize>(header[1]);
    header[2] = reinterpret_cast<usize>(
        NuAnimData2FixPtrs(reinterpret_cast<void *>(static_cast<usize>(header[2])), (isize)relocation_delta, 0, 0));
    header = static_cast<u32 *>(*data);
    header[1] = reinterpret_cast<usize>(header);
    return reinterpret_cast<void *>(static_cast<usize>(header[2]));
}

extern "C" void *NuAnimData2LoadBuffEx(char *path, VARIPTR *buf, VARIPTR *buf_end, void **result) {
    buf->addr = ALIGN(buf->addr, 0x10);
    const i32 file_size = NuFileLoadBuffer(path, buf->void_ptr, static_cast<i32>(buf_end->addr - buf->addr));
    if (file_size == 0) {
        if (NuFileGetLastError() == -1) {
            *buf = *buf_end;
        }
        *result = NULL;
        return NULL;
    }
    void *data = buf->void_ptr;
    if (static_cast<i32>(static_cast<u32 *>(data)[1]) > static_cast<i32>(0x414e4934)) {
        data = NuPtrBlockFix(data);
        buf->void_ptr = data;
        *result = data;
        buf->addr += static_cast<usize>(file_size);
        return *result;
    }
    *result = data;
    buf->addr += static_cast<usize>(file_size);
    return NuAnimData2Fixup(file_size, result);
}

extern "C" void *NuAnimData2LoadBuff(char *path, VARIPTR *buf, VARIPTR *buf_end) {
    void *result;
    return NuAnimData2LoadBuffEx(path, buf, buf_end, &result);
}

extern "C" void *NuAnimData2LoadBuffFromPAK(void *data, i32 file_size) {
    if (file_size == 0) {
        return NULL;
    }
    if (static_cast<i32>(static_cast<u32 *>(data)[1]) > static_cast<i32>(0x414e4934)) {
        return NuPtrBlockFix(data);
    }
    void *result = data;
    return NuAnimData2Fixup(file_size, &result);
}

    void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints) {
        AnimBuffEvalCB = callback;
        AnimBuffEvalData = data;
        AnimBuffEvalJoint = joints;
    }

    i32 NuAnimSetUseQuatsFlag(i32 enabled) {
        const i32 previous = ForceEulerToQuat;
        ForceEulerToQuat = static_cast<u8>(enabled);
        return previous;
    }

    i32 NuAnimGetUseQuatsFlag(void) {
        return ForceEulerToQuat;
    }

    i32 NuAnimPushSetUseQuatsFlag(i32 enabled) {
        i32 previous = ForceEulerToQuat;
        if (NumQuatPushes > 3) {
            return 0;
        }
        QuatPushes[NumQuatPushes++] = previous;
        ForceEulerToQuat = enabled;
        return previous;
    }

    i32 NuAnimPopUseQuatsFlag(void) {
        if (NumQuatPushes == 0) {
            return 0;
        }
        ForceEulerToQuat = QuatPushes[--NumQuatPushes];
        return ForceEulerToQuat;
    }

    void *NuAnimGetAnimLOD(void *animation, i32 lod) {
        if (lod < 0 || animation == NULL) {
            return NULL;
        }

        u8 *lod_animation = static_cast<u8 *>(animation);
        if (lod == 0) {
            return lod_animation;
        }
        do {
            const i32 next_lod = *reinterpret_cast<u16 *>(lod_animation + 0x14);
            if (next_lod == 0) {
                return NULL;
            }
            lod_animation += next_lod;
        } while (--lod != 0);
        return lod_animation;
    }

    i32 NuAnimNumNodes(void *animation) {
        const ani3_animheader_s *header = static_cast<const ani3_animheader_s *>(animation);
        if (header->magic == ANI3_MAGIC_VERSION_4 || header->magic == ANI3_MAGIC_VERSION_5) {
            return header->node_count;
        }
        return *reinterpret_cast<const i16 *>(static_cast<const u8 *>(animation) + 4);
    }

extern "C" f32 NuAnimEndFrame(void *animation_data) {
    ani3_animheader_s *animation = static_cast<ani3_animheader_s *>(animation_data);
    if (animation->magic != ANI3_MAGIC_VERSION_4 && animation->magic != ANI3_MAGIC_VERSION_5) {
        return *static_cast<f32 *>(animation_data);
    }

    return static_cast<f32>(static_cast<u32>(animation->frame_count)) +
           static_cast<f32>(static_cast<i32>(animation->first_frame));
}

extern "C" f32 NuAnimEndFrameOld(void *animation_data) {
    ani3_animheader_s *animation = static_cast<ani3_animheader_s *>(animation_data);
    if (animation->magic != ANI3_MAGIC_VERSION_4 && animation->magic != ANI3_MAGIC_VERSION_5) {
        return *static_cast<f32 *>(animation_data);
    }

    const u32 declared_end_frame = animation->declared_end_frame;
    if (declared_end_frame == 0) {
        return static_cast<f32>(animation->frame_count + animation->first_frame);
    }
    return static_cast<f32>(declared_end_frame);
}

    // Original @0x2c17d0. The non-quaternion ANI4 player uses a compact
    // four-samples-per-word curve stream. Only groups enabled by the node's
    // translation/rotation/scale flags occupy space in that stream.

    f32 GetInstAnimEndFrame(nugscn_s *scene, nuinstanim_s *instance_animation) {
        if (instance_animation == NULL) {
            return 0.0f;
        }

        void *animation = scene->instance_animation_data[instance_animation->anim_ix];
        if (animation != NULL) {
            return NuAnimEndFrameOld(animation);
        }
        if ((instance_animation->end_frame_lookup_bits & NUINSTANIM_END_FRAME_LOOKUP_MASK) == 0 ||
            scene->animation_end_frames == NULL) {
            return 0.0f;
        }

        return static_cast<f32>(scene->animation_end_frames[instance_animation->end_frame_lookup_index - 1].end_frame);
    }

}

i32 findrange(nugscn_s *scene, i32 first_joint) {
    // This helper's original C++ ABI names nugscn_s, but animation callers
    // pass the hierarchy object whose joint table starts at the same offsets.
    nuhgobj_s *object = reinterpret_cast<nuhgobj_s *>(scene);
    const i32 joint_count = object->joint_count;
    nuhgobjjoint_s *joints = object->joints;
    const i32 parent_index = joints[first_joint].parent_index;
    i32 end_joint = first_joint + 1;
    while (end_joint < joint_count && joints[end_joint].parent_index != parent_index) {
        ++end_joint;
    }
    return end_joint - 1;
}

void NuAnimBuffEvaluate_3_QuatB(numtx_s *base, nuanimbuff_s *buffer, nugscn_s *scene, numtx_s *matrices,
                                ani3_animheader_s *animation, NUHGOBJROOTFN root_fn, nuvec_s *root_translation,
                                void *root_data) {
    nuhgobj_s *object = reinterpret_cast<nuhgobj_s *>(scene);
    i32 count = animation->node_count < object->joint_count ? animation->node_count : object->joint_count;
    NUQUAT *quaternions = static_cast<NUQUAT *>(NuScratchAlloc32((count + 1) * 16));
    quaternions = reinterpret_cast<NUQUAT *>(ALIGN(reinterpret_cast<usize>(quaternions), 16));
    NUVEC *scales = static_cast<NUVEC *>(NuScratchAlloc32(count * sizeof(NUVEC)));
    if (!buffer)
        buffer = static_cast<nuanimbuff_s *>(globalbuffer);
    void *callback_data[256];
    if (AnimBuffEvalData && AnimBuffEvalJoint) {
        memset(callback_data, 0, object->joint_count * sizeof(void *));
        for (i32 index = 0; AnimBuffEvalData[index]; ++index) {
            i32 mapped = AnimBuffEvalJoint[index];
            if (mapped >= 0 && mapped < object->joint_override_map_count) {
                u8 joint_index = object->joint_override_map[mapped];
                if (joint_index != 255)
                    callback_data[joint_index] = AnimBuffEvalData[index];
            }
        }
    }
    // Slot255 and the root/nonroot scale selection follow the original evaluator.
    NUVEC root_scale;
    if (base) {
        root_scale = NuMtxGetScale(base);
        NuMtxToQuat(base, &quaternions[255]);
    } else {
        root_scale.x = root_scale.y = root_scale.z = 1.0f;
        quaternions[255] = NUQUAT{0, 0, 0, 1};
    }
    nuanimbuffjoint_s *joint = buffer->joints;
    NUVEC root_values = {0, 0, 0};
    NUQUAT *rotation = quaternions;
    NUMTX *matrix = matrices;
    NUVEC *scale = scales;
    const u8 *joint_flags = buffer->joint_flags;
    for (i32 index = 0; index < count; ++index, ++joint, ++rotation, ++matrix, ++scale) {
        u8 flags = *joint_flags++;
        u8 parent = object->joints[index].parent_index;
        if (flags & 1)
            *rotation = *reinterpret_cast<NUQUAT *>(&joint->rotation);
        else
            *rotation = NUQUAT{0, 0, 0, 1};
        NUVEC *parent_scale;
        if (parent != 255) {
            const NUQUAT q = *rotation, p = quaternions[parent];
            rotation->z = (p.w * q.z + q.w * p.z + p.x * q.y) - q.x * p.y;
            rotation->x = (p.w * q.x + q.w * p.x + p.y * q.z) - p.z * q.y;
            rotation->y = (p.w * q.y + q.w * p.y + p.z * q.x) - p.x * q.z;
            rotation->w = ((p.w * q.w - q.x * p.x) - q.y * p.y) - q.z * p.z;
            parent_scale = &root_scale;
        } else
            parent_scale = &scales[parent];
        NuQuatToMtx(rotation, matrix);
        if (flags & 8) {
            scale->x = joint->scale.x * parent_scale->x;
            scale->y = joint->scale.y * parent_scale->y;
            scale->z = joint->scale.z * parent_scale->z;
            NuMtxPreScaleVU0(matrix, scale);
        } else
            *scale = *parent_scale;
        if (flags & 16)
            scale->x = scale->y = scale->z = 1.0f;
        if (flags & 2) {
            root_values.x = joint->translation.x;
            root_values.y = joint->translation.y;
            root_values.z = -joint->translation.z;
            if (parent != 255)
                NuVecMtxTransform(reinterpret_cast<NUVEC *>(&matrix->m30), &joint->translation, &matrices[parent]);
            else if (base)
                NuVecMtxTransform(reinterpret_cast<NUVEC *>(&matrix->m30), &joint->translation, base);
            else {
                matrix->m30 = joint->translation.x;
                matrix->m31 = joint->translation.y;
                matrix->m32 = joint->translation.z;
            }
        } else if (parent != 255) {
            matrix->m30 = matrices[parent].m30;
            matrix->m31 = matrices[parent].m31;
            matrix->m32 = matrices[parent].m32;
            matrix->m33 = matrices[parent].m33;
        } else if (base) {
            matrix->m30 = base->m30;
            matrix->m31 = base->m31;
            matrix->m32 = base->m32;
            matrix->m33 = base->m33;
        }
        if (parent == 255) {
            if (root_fn)
                root_fn(matrix, root_data, &root_values, &root_values, root_translation, 0.0f);
            root_fn = NULL;
            if (flags & 64)
                scale->x = scale->y = scale->z = 1.0f;
        }
        if (AnimBuffEvalCB && callback_data[index])
            AnimBuffEvalCB(matrix, callback_data[index], rotation);
    }
    // Callbacks observe the engine coordinates; reflect the completed hierarchy afterward.
    for (i32 index = 0; index < count; ++index) {
        NUMTX *matrix = &matrices[index];
        matrix->m02 = -matrix->m02;
        matrix->m12 = -matrix->m12;
        matrix->m20 = -matrix->m20;
        matrix->m21 = -matrix->m21;
        matrix->m23 = -matrix->m23;
        matrix->m32 = -matrix->m32;
    }
    for (i32 index = count; index < object->joint_count; ++index)
        NuMtxSetIdentity(&matrices[index]);
    AnimBuffEvalData = NULL;
    AnimBuffEvalCB = NULL;
    AnimBuffEvalJoint = NULL;
    NuScratchRelease();
    NuScratchRelease();
}

extern "C" {
    // Original @0x2bd180.  Evaluate the decompressed per-joint samples into
    // hierarchy matrices while carrying scale through the parent chain.
    void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data) {
        if (__builtin_expect(buffer == NULL, 1)) {
            buffer = static_cast<nuanimbuff_s *>(globalbuffer);
        }

        if (buffer->use_quaternions != 0) {
            NuAnimBuffEvaluate_3_QuatB(NULL, buffer, reinterpret_cast<nugscn_s *>(object), matrices, animation, root_fn,
                                       root_translation, root_data);
            return;
        }

        static NUVEC scale_array[256];

        void *callback_data[256];
        if (AnimBuffEvalData != NULL && AnimBuffEvalJoint != NULL) {
            memset(callback_data, 0, object->joint_count * sizeof(void *));
            for (i32 callback_index = 0; AnimBuffEvalData[callback_index] != NULL; ++callback_index) {
                const i32 override_index = AnimBuffEvalJoint[callback_index];
                if (override_index >= 0 && override_index < object->joint_override_map_count) {
                    const u8 joint_index = object->joint_override_map[override_index];
                    if (joint_index != 0xff) {
                        callback_data[joint_index] = AnimBuffEvalData[callback_index];
                    }
                }
            }
        }

        const i32 evaluated_count =
            animation->node_count < object->joint_count ? animation->node_count : object->joint_count;
        scale_array[0xff] = {1.0f, 1.0f, 1.0f};
        NUVEC root_values = {0.0f, 0.0f, 0.0f};
        for (i32 joint_index = 0; joint_index < evaluated_count; ++joint_index) {
            const nuanimbuffjoint_s &joint = buffer->joints[joint_index];
            const u8 flags = buffer->joint_flags[joint_index];
            const u8 parent_index = object->joints[joint_index].parent_index;

            NUMTX &local_matrix = matrices[joint_index];
            if ((flags & NUANIMBUFF_JOINT_ROTATION) != 0) {
                NUANGVEC angles = {
                    static_cast<NUANG>(joint.rotation.x * 10430.378f),
                    static_cast<NUANG>(joint.rotation.y * 10430.378f),
                    static_cast<NUANG>(joint.rotation.z * 10430.378f),
                };
                NuMtxSetRotateXYZVU0(&local_matrix, &angles);
                if ((flags & NUANIMBUFF_JOINT_BIND_MATRIX) != 0) {
                    NuMtxMulRVU0(&local_matrix, &local_matrix, &object->joints[joint_index].animation_bind_matrix);
                }
            } else if ((flags & NUANIMBUFF_JOINT_BIND_MATRIX) != 0) {
                local_matrix = object->joints[joint_index].animation_bind_matrix;
            } else {
                NuMtxSetIdentity(&local_matrix);
            }

            if ((flags & NUANIMBUFF_JOINT_SCALE) != 0) {
                scale_array[joint_index] = joint.scale;
                NuMtxPreScaleVU0(&local_matrix, &scale_array[joint_index]);
                scale_array[joint_index].x *= scale_array[parent_index].x;
                scale_array[joint_index].y *= scale_array[parent_index].y;
                scale_array[joint_index].z *= scale_array[parent_index].z;
            } else {
                scale_array[joint_index] = scale_array[object->joints[joint_index].parent_index];
            }

            if ((flags & NUANIMBUFF_JOINT_CANCEL_PARENT_SCALE) != 0) {
                const NUVEC &parent_scale = scale_array[parent_index];
                NUVEC inverse_parent_scale;
                if (parent_scale.x != 0.0f && parent_scale.y != 0.0f && parent_scale.z != 0.0f) {
                    inverse_parent_scale = {1.0f / parent_scale.x, 1.0f / parent_scale.y, 1.0f / parent_scale.z};
                    scale_array[joint_index].x *= inverse_parent_scale.x;
                    scale_array[joint_index].y *= inverse_parent_scale.y;
                    scale_array[joint_index].z *= inverse_parent_scale.z;
                } else {
                    inverse_parent_scale = {0.0f, 0.0f, 0.0f};
                    scale_array[joint_index] = {0.0f, 0.0f, 0.0f};
                }
                NuMtxScaleVU0(&local_matrix, &inverse_parent_scale);
            }

            if ((flags & NUANIMBUFF_JOINT_TRANSLATION) != 0) {
                NUVEC translation = joint.translation;
                root_values = joint.translation;
                root_values.z = -root_values.z;
                NuMtxTranslate(&local_matrix, &translation);
            }

            local_matrix.m02 = -local_matrix.m02;
            local_matrix.m12 = -local_matrix.m12;
            local_matrix.m20 = -local_matrix.m20;
            local_matrix.m21 = -local_matrix.m21;
            local_matrix.m23 = -local_matrix.m23;
            local_matrix.m32 = -local_matrix.m32;

            if (root_fn != NULL) {
                root_fn(&local_matrix, root_data, &root_values, &root_values, root_translation, 0.0f);
            }

            if (parent_index == 0xff) {
                if ((flags & 0x40) != 0) {
                    scale_array[joint_index] = {1.0f, 1.0f, 1.0f};
                }
            } else {
                NuMtxMulVU0(&matrices[joint_index], &local_matrix, &matrices[parent_index]);
            }
            if (AnimBuffEvalCB != NULL && callback_data[joint_index] != NULL) {
                AnimBuffEvalCB(&matrices[joint_index], callback_data[joint_index], 0);
            }
            root_fn = NULL;
        }

        for (i32 joint_index = evaluated_count; joint_index < object->joint_count; ++joint_index) {
            NuMtxSetIdentity(&matrices[joint_index]);
        }

        AnimBuffEvalData = NULL;
        AnimBuffEvalCB = NULL;
        AnimBuffEvalJoint = NULL;
    }

}

void CalcValue1648Get2Values(char *data, i32 quarter, i32 stride, ani3_scalemin_s *scale_min, float *first,
                             float *second) {
    u16 *next = reinterpret_cast<u16 *>(data + stride);
    u16 *keys = reinterpret_cast<u16 *>(data);
    i32 start, delta, end, last;
    switch (quarter) {
        case 0: {
            start = keys[0];
            delta = static_cast<i32>(next[0]) - start;
            *first = (static_cast<f32>(keys[1] & 0xfff) * static_cast<f32>(delta) / 4095.0f + static_cast<f32>(start)) *
                         scale_min->scale +
                     scale_min->minimum;
            *second =
                (static_cast<f32>(keys[2] & 0xfff) * static_cast<f32>(delta) / 4095.0f + static_cast<f32>(start)) *
                    scale_min->scale +
                scale_min->minimum;
            break;
        }
        case 1: {
            start = keys[0];
            delta = static_cast<i32>(next[0]) - start;
            *first = (static_cast<f32>(keys[2] & 0xfff) * static_cast<f32>(delta) / 4095.0f + static_cast<f32>(start)) *
                         scale_min->scale +
                     scale_min->minimum;
            *second =
                (static_cast<f32>(keys[3] & 0xfff) * static_cast<f32>(delta) / 4095.0f + static_cast<f32>(start)) *
                    scale_min->scale +
                scale_min->minimum;
            break;
        }
        case 2: {
            start = keys[0];
            delta = static_cast<i32>(next[0]) - start;
            *first = (static_cast<f32>(keys[3] & 0xfff) * static_cast<f32>(delta) / 4095.0f + static_cast<f32>(start)) *
                         scale_min->scale +
                     scale_min->minimum;
            last = ((keys[2] & 0xf000) >> 8) | ((keys[3] & 0xf000) >> 4) | (keys[1] >> 12);
            *second = (static_cast<f32>(last) * static_cast<f32>(delta) / 4095.0f + static_cast<f32>(start)) *
                          scale_min->scale +
                      scale_min->minimum;
            break;
        }
        case 3: {
            last = ((keys[2] & 0xf000) >> 8) | ((keys[3] & 0xf000) >> 4) | (keys[1] >> 12);
            start = keys[0];
            end = next[0];
            *first = (static_cast<f32>(last) * static_cast<f32>(end - start) / 4095.0f + static_cast<f32>(start)) *
                         scale_min->scale +
                     scale_min->minimum;
            *second = (static_cast<f32>(static_cast<i32>(next[static_cast<u32>(stride) / sizeof(u16)]) - end) *
                           static_cast<f32>(next[1] & 0xfff) / 4095.0f +
                       static_cast<f32>(end)) *
                          scale_min->scale +
                      scale_min->minimum;
            break;
        }
    }
}

float CalcValue1648(char *data, i32 quarter, i32 stride, float fraction, ani3_scalemin_s *scale_min) {
    u16 *next = reinterpret_cast<u16 *>(data + stride);
    u16 *keys = reinterpret_cast<u16 *>(data);

    switch (quarter) {
        case 0: {
            float tangent = static_cast<float>((keys[2] & 0xfff) - (keys[1] & 0xfff)) * fraction +
                            static_cast<float>(keys[1] & 0xfff);
            float value =
                (tangent / 4095.0f) * static_cast<float>(static_cast<i32>(*next) - static_cast<i32>(keys[0])) +
                static_cast<float>(static_cast<i32>(keys[0]));
            return value * scale_min->scale + scale_min->minimum;
        }
        case 1: {
            float tangent = static_cast<float>((keys[3] & 0xfff) - (keys[2] & 0xfff)) * fraction +
                            static_cast<float>(keys[2] & 0xfff);
            float value =
                (tangent / 4095.0f) * static_cast<float>(static_cast<i32>(*next) - static_cast<i32>(keys[0])) +
                static_cast<float>(static_cast<i32>(keys[0]));
            return value * scale_min->scale + scale_min->minimum;
        }
        case 2: {
            i32 start = keys[3] & 0xfff;
            i32 end = ((keys[2] & 0xf000) >> 8) | ((keys[3] & 0xf000) >> 4) | (keys[1] >> 12);
            float tangent = static_cast<float>(end - start) * fraction + static_cast<float>(start);
            float value =
                (tangent / 4095.0f) * static_cast<float>(static_cast<i32>(*next) - static_cast<i32>(keys[0])) +
                static_cast<float>(static_cast<i32>(keys[0]));
            return value * scale_min->scale + scale_min->minimum;
        }
        case 3: {
            i32 next_value = *next;
            i32 tangent = ((keys[2] & 0xf000) >> 8) | ((keys[3] & 0xf000) >> 4) | (keys[1] >> 12);
            float a =
                static_cast<float>(tangent) * static_cast<float>(next_value - static_cast<i32>(keys[0])) / 4095.0f +
                static_cast<float>(static_cast<i32>(keys[0]));
            float b = static_cast<float>(static_cast<i32>(reinterpret_cast<u16 *>(data)[stride]) - next_value) *
                          static_cast<float>(next[1] & 0xfff) / 4095.0f +
                      static_cast<float>(next_value);
            float value = (b - a) * fraction + a;
            return value * scale_min->scale + scale_min->minimum;
        }
    }

    return 0.0f;
}

__attribute__((optimize("O2"))) i32 ANI_SimpleAni3PlayerV4Joint_Quat3(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, i32 joint_count,
                                      i32 first_joint) {
    const u8 *node_flags = anim->node_flags;
    buffer->use_quaternions = 1;
    const i32 key_stride = anim->key_stride;
    u32 quarter;
    f32 fraction;
    i32 key_offset;
    if (__builtin_expect(anim->key_count == 1, 0)) {
        quarter = 0;
        fraction = 0.0f;
        key_offset = 0;
    } else {
        const i32 last = anim->key_count - 1;
        f32 key = (frame - anim->first_frame) * static_cast<f32>(last) / static_cast<f32>(anim->frame_count - 1);
        key = key < 0.0f ? 0.0f : key;
        i32 whole;
        if (!(static_cast<f32>(last) <= key)) {
            whole = static_cast<i32>(key);
            fraction = key - static_cast<f32>(whole);
        } else {
            const f32 final_key = static_cast<f32>(last);
            whole = static_cast<i32>(final_key);
            fraction = final_key - static_cast<f32>(whole);
        }
        quarter = static_cast<u32>(whole) & 3;
        key_offset = (whole / 4) * key_stride;
    }
    const u16 *constants = reinterpret_cast<const u16 *>(anim->constants);
    u8 *keys = anim->keys + key_offset;
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u16 *curve_types = anim->curve_types;
    i32 count = joint_count < anim->node_count ? joint_count : anim->node_count;
    nuanimbuffjoint_s *joint = buffer->joints;
    u8 *joint_flags = buffer->joint_flags + first_joint;
    const u8 *first_flags = node_flags + first_joint;
    for (; node_flags < first_flags; ++node_flags, ++joint) {
        if (*node_flags & 2) {
            SkipAni4V4Curve(curve_types[0], keys, scale_min);
            SkipAni4V4Curve(curve_types[1], keys, scale_min);
            SkipAni4V4Curve(curve_types[2], keys, scale_min);
        }
        if (__builtin_expect(!!(*node_flags & 1), 1)) {
            SkipAni4V4Curve(curve_types[3], keys, scale_min);
            SkipAni4V4Curve(curve_types[4], keys, scale_min);
            SkipAni4V4Curve(curve_types[5], keys, scale_min);
        }
        if (*node_flags & 8) {
            SkipAni4V4Curve(curve_types[6], keys, scale_min);
            SkipAni4V4Curve(curve_types[7], keys, scale_min);
            SkipAni4V4Curve(curve_types[8], keys, scale_min);
        }
        curve_types += 9;
    }
    const u8 *end_flags = first_flags + count;
    for (; node_flags < end_flags; ++node_flags, ++joint) {
        const u8 flags = *node_flags;
        *joint_flags++ = flags;
        f32 *output = &joint->translation.x;
        for (i32 group = 0; group < 3; ++group, curve_types += 3, output += 4) {
            if (!(flags & CurveGroupMasks[group])) {
                if (group == 0) {
                    output[0] = output[1] = output[2] = 0.0f;
                } else if (group == 1) {
                    output[0] = output[1] = output[2] = 0.0f;
                    output[3] = 1.0f;
                } else {
                    output[0] = output[1] = output[2] = 1.0f;
                }
            } else if (group != 1) {
                output[0] = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[0], quarter, fraction, keys,
                                                  scale_min);
                output[1] = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[1], quarter, fraction, keys,
                                                  scale_min);
                output[2] = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[2], quarter, fraction, keys,
                                                  scale_min);
            } else {
                NUQUAT first, second;
                DecodeAni4Quat3Pair(anim, key_stride, constants, curve_types[0], quarter, keys, scale_min, first.x,
                                    second.x);
                DecodeAni4Quat3Pair(anim, key_stride, constants, curve_types[1], quarter, keys, scale_min, first.y,
                                    second.y);
                DecodeAni4Quat3Pair(anim, key_stride, constants, curve_types[2], quarter, keys, scale_min, first.z,
                                    second.z);
                first.w = NuFsqrt(1.0f - (first.x * first.x + first.y * first.y + first.z * first.z));
                second.w = NuFsqrt(1.0f - (second.x * second.x + second.y * second.y + second.z * second.z));
                if (first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w < 0.0f) {
                    NUQUAT negated = {-second.x, -second.y, -second.z, -second.w};
                    second = negated;
                }
                NUQUAT result;
                result.y = first.y * (1.0f - fraction) + second.y * fraction;
                result.x = first.x * (1.0f - fraction) + second.x * fraction;
                result.z = first.z * (1.0f - fraction) + second.z * fraction;
                result.w = first.w * (1.0f - fraction) + second.w * fraction;
                f32 length =
                    NuFsqrt(result.w * result.w + result.x * result.x + result.y * result.y + result.z * result.z);
                f32 inverse_length = NuFdiv(1.0f, length);
                output[0] = result.x * inverse_length;
                output[1] = result.y * inverse_length;
                output[2] = result.z * inverse_length;
                output[3] = result.w * inverse_length;
            }
        }
    }
    return 0;
}

__attribute__((optimize("O2"))) i32 ANI_SimpleAni3PlayerV4Joint_Quat3W(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, i32 joint_count,
                                       i32 first_joint) {

    const u8 *node_flags = anim->node_flags;
    buffer->use_quaternions = 1;
    const i32 key_stride = anim->key_stride;
    f32 fraction;
    u32 quarter;
    i32 key_offset;
    if (anim->key_count == 1) {
        quarter = 0;
        fraction = 0.0f;
        key_offset = 0;
    } else {
        const f32 last_key = static_cast<f32>(anim->key_count - 1);
        f32 key = (frame - anim->first_frame) * last_key / static_cast<f32>(anim->frame_count - 1);
        if (!(key >= 0.0f)) {
            key = 0.0f;
        }
        i32 whole_key;
        if (last_key <= key) {
            whole_key = static_cast<i32>(last_key);
            fraction = last_key - static_cast<f32>(whole_key);
        } else {
            whole_key = static_cast<i32>(key);
            fraction = key - static_cast<f32>(whole_key);
        }
        quarter = static_cast<u32>(whole_key) & 3;
        key_offset = (whole_key / 4) * anim->key_stride;
    }
    const u16 *constants = reinterpret_cast<const u16 *>(anim->constants);
    ani3_scalemin_s *scale_min = anim->scale_min;
    u8 *keys = anim->keys + key_offset;
    nuanimbuffjoint_s *joint = buffer->joints;
    const u16 *curve_types = anim->curve_types;
    // The original clamps the count before applying first_joint.
    i32 count = joint_count < anim->node_count ? joint_count : anim->node_count;
    u8 *joint_flags = buffer->joint_flags + first_joint;
    const u8 *first_flags = node_flags + first_joint;
    for (; node_flags < first_flags; ++node_flags, ++joint) {
        for (i32 group = 0; group < 3; ++group) {
            i32 components = group == 1 ? 4 : 3;
            if (*node_flags & CurveGroupMasks[group]) {
                for (i32 component = 0; component < components; ++component) {
                    SkipAni4V4Curve(*curve_types++, keys, scale_min);
                }
            } else
                curve_types += components;
        }
    }
    const u8 *end_flags = first_flags + count;
    for (; node_flags < end_flags; ++node_flags, ++joint) {
        const u8 flags = *node_flags;
        *joint_flags++ = flags;
        f32 *output = &joint->translation.x;
        for (i32 group = 0; group < 3; ++group, output += 4) {
            if (!(flags & CurveGroupMasks[group])) {
                if (group == 0) {
                    output[0] = output[1] = output[2] = 0.0f;
                } else if (group == 1) {
                    output[0] = output[1] = output[2] = 0.0f;
                    output[3] = 1.0f;
                } else {
                    output[0] = output[1] = output[2] = 1.0f;
                }
                curve_types += group == 1 ? 4 : 3;
            } else if (group != 1) {
                output[0] = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[0], quarter, fraction, keys,
                                                  scale_min);
                output[1] = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[1], quarter, fraction, keys,
                                                  scale_min);
                output[2] = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[2], quarter, fraction, keys,
                                                  scale_min);
                curve_types += 3;
            } else {
                // This format stores w explicitly and interpolates the stored signs.
                // The original keeps both decoded quaternions 16-byte aligned in a realigned frame.
                NUQUAT first __attribute__((aligned(16)));
                NUQUAT second __attribute__((aligned(16)));
                DecodeAni4Quat3Pair(anim, key_stride, constants, curve_types[0], quarter, keys, scale_min, first.x,
                                    second.x);
                DecodeAni4Quat3Pair(anim, key_stride, constants, curve_types[1], quarter, keys, scale_min, first.y,
                                    second.y);
                DecodeAni4Quat3Pair(anim, key_stride, constants, curve_types[2], quarter, keys, scale_min, first.z,
                                    second.z);
                DecodeAni4Quat3Pair(anim, key_stride, constants, curve_types[3], quarter, keys, scale_min, first.w,
                                    second.w);
                curve_types += 4;
                NUQUAT result;
                result.y = first.y * (1.0f - fraction) + second.y * fraction;
                result.x = first.x * (1.0f - fraction) + second.x * fraction;
                result.z = first.z * (1.0f - fraction) + second.z * fraction;
                result.w = first.w * (1.0f - fraction) + second.w * fraction;
                f32 length =
                    NuFsqrt(result.w * result.w + result.x * result.x + result.y * result.y + result.z * result.z);
                f32 inverse_length = NuFdiv(1.0f, length);
                output[0] = result.x * inverse_length;
                output[1] = result.y * inverse_length;
                output[2] = result.z * inverse_length;
                output[3] = result.w * inverse_length;
            }
        }
    }
    return 0;
}

extern "C" {
extern "C" i32 ANI_SimpleAni3PlayerV4Joint_EulerQuat(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer,
                                                     i32 joint_count, i32 first_joint) {
    const u8 *node_flags = anim->node_flags;
    buffer->use_quaternions = 1;
    u32 quarter;
    f32 fraction;
    i32 key_offset;
    if (anim->key_count == 1) {
        quarter = 0;
        fraction = 0.0f;
        key_offset = 0;
    } else {
        const i32 last = anim->key_count - 1;
        f32 key = (frame - anim->first_frame) * static_cast<f32>(last) / static_cast<f32>(anim->frame_count - 1);
        key = key < 0.0f ? 0.0f : key;
        i32 whole;
        if (__builtin_expect(static_cast<f32>(last) <= key, 0)) {
            whole = last;
            fraction = 0.0f;
        } else {
            whole = static_cast<i32>(key);
            fraction = key - static_cast<f32>(whole);
        }
        quarter = static_cast<u32>(whole) & 3;
        key_offset = (whole / 4) * anim->key_stride;
    }
    u8 *keys = anim->keys + key_offset;
    i32 key_stride = anim->key_stride;
    const u16 *constants = reinterpret_cast<const u16 *>(anim->constants);
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u16 *curve_types = anim->curve_types;
    i32 count = joint_count < anim->node_count ? joint_count : anim->node_count;
    const u8 *first_flags = node_flags + first_joint;
    for (const u8 *skip_flags = node_flags; skip_flags < first_flags; ++skip_flags) {
        if (*skip_flags & 2) {
            SkipAni4V4Curve(curve_types[0], keys, scale_min);
            SkipAni4V4Curve(curve_types[1], keys, scale_min);
            SkipAni4V4Curve(curve_types[2], keys, scale_min);
        }
        if (__builtin_expect(!!(*skip_flags & 1), 1)) {
            SkipAni4V4Curve(curve_types[3], keys, scale_min);
            SkipAni4V4Curve(curve_types[4], keys, scale_min);
            SkipAni4V4Curve(curve_types[5], keys, scale_min);
        }
        if (*skip_flags & 8) {
            SkipAni4V4Curve(curve_types[6], keys, scale_min);
            SkipAni4V4Curve(curve_types[7], keys, scale_min);
            SkipAni4V4Curve(curve_types[8], keys, scale_min);
        }
        curve_types += 9;
    }
    node_flags += first_joint;
    nuanimbuffjoint_s *joint = buffer->joints + first_joint;
    const u8 *end_flags = node_flags + count;
    u8 *joint_flags = buffer->joint_flags + first_joint;
    for (; node_flags < end_flags; ++node_flags, ++joint) {
        const u8 flags = *node_flags;
        *joint_flags++ = flags;
        f32 *output = &joint->translation.x;
        for (i32 group = 0; group < 3; ++group, curve_types += 3, output += 4) {
            if (!(flags & CurveGroupMasks[group])) {
                if (group == 0) {
                    output[0] = output[1] = output[2] = 0.0f;
                } else if (group == 1) {
                    output[0] = output[1] = output[2] = 0.0f;
                    output[3] = 1.0f;
                } else {
                    output[0] = output[1] = output[2] = 1.0f;
                }
            } else if (group != 1) {
                output[0] = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[0], quarter, fraction, keys,
                                                  scale_min);
                output[1] = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[1], quarter, fraction, keys,
                                                  scale_min);
                output[2] = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[2], quarter, fraction, keys,
                                                  scale_min);
            } else {
                NUVEC first_angles, second_angles;
                DecodeAni4EulerPair(anim, key_stride, constants, curve_types[0], quarter, keys, scale_min,
                                    first_angles.x, second_angles.x);
                DecodeAni4EulerPair(anim, key_stride, constants, curve_types[1], quarter, keys, scale_min,
                                    first_angles.y, second_angles.y);
                DecodeAni4EulerPair(anim, key_stride, constants, curve_types[2], quarter, keys, scale_min,
                                    first_angles.z, second_angles.z);
                if (curve_types[0] != 6 && curve_types[1] != 6 && curve_types[2] != 6) {
                    NuQuatFromEulerXYZ(
                        reinterpret_cast<NUQUAT *>(output), static_cast<i32>(first_angles.x * 10430.378f),
                        static_cast<i32>(first_angles.y * 10430.378f), static_cast<i32>(first_angles.z * 10430.378f));
                } else {
                    NUQUAT first, second;
                    NuQuatFromEulerXYZ(&first, static_cast<i32>(first_angles.x * 10430.378f),
                                       static_cast<i32>(first_angles.y * 10430.378f),
                                       static_cast<i32>(first_angles.z * 10430.378f));
                    NuQuatFromEulerXYZ(&second, static_cast<i32>(second_angles.x * 10430.378f),
                                       static_cast<i32>(second_angles.y * 10430.378f),
                                       static_cast<i32>(second_angles.z * 10430.378f));
                    if (__builtin_expect(first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w < 0.0f, 1)) {
                        second.x = -second.x;
                        second.y = -second.y;
                        second.z = -second.z;
                        second.w = -second.w;
                    }
                    NUQUAT result;
                    result.x = first.x * (1.0f - fraction) + second.x * fraction;
                    result.y = first.y * (1.0f - fraction) + second.y * fraction;
                    result.z = first.z * (1.0f - fraction) + second.z * fraction;
                    result.w = first.w * (1.0f - fraction) + second.w * fraction;
                    f32 length =
                        NuFsqrt(result.w * result.w + result.x * result.x + result.y * result.y + result.z * result.z);
                    f32 inverse_length = NuFdiv(1.0f, length);
                    output[0] = result.x * inverse_length;
                    output[1] = result.y * inverse_length;
                    output[2] = result.z * inverse_length;
                    output[3] = result.w * inverse_length;
                }
            }
        }
    }
    return 0;
}

extern "C" __attribute__((optimize("O2"))) i32 ANI_SimpleAni3PlayerV4Joint(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, i32 joint_count,
                                           i32 first_joint) {
    if ((anim->format_flags & ANI3_FORMAT_QUATERNION_ROTATION) != 0) {
        if ((anim->format_flags & ANI3_FORMAT_QUATERNION_STORES_W) == 0) {
            return ANI_SimpleAni3PlayerV4Joint_Quat3(anim, frame, buffer, joint_count, first_joint);
        }
        return ANI_SimpleAni3PlayerV4Joint_Quat3W(anim, frame, buffer, joint_count, first_joint);
    }
    if (ForceEulerToQuat != 0) {
        return ANI_SimpleAni3PlayerV4Joint_EulerQuat(anim, frame, buffer, joint_count, first_joint);
    }

    buffer->use_quaternions = 0;
    const u8 *node_flags = anim->node_flags;

    u32 quarter;
    f32 fraction;
    i32 key_offset;
    if (anim->key_count == 1) {
        quarter = 0;
        fraction = 0.0f;
        key_offset = 0;
    } else {
        const f32 last_key = static_cast<f32>(anim->key_count - 1);
        f32 key = (frame - anim->first_frame) * last_key / static_cast<f32>(anim->frame_count - 1);
        if (key < 0.0f) {
            key = 0.0f;
        }
        i32 whole_key;
        if (!(last_key <= key)) {
            whole_key = static_cast<i32>(key);
            fraction = key - static_cast<f32>(whole_key);
        } else {
            whole_key = static_cast<i32>(last_key);
            fraction = last_key - static_cast<f32>(whole_key);
        }
        quarter = static_cast<u32>(whole_key) & 3;
        key_offset = (whole_key / 4) * anim->key_stride;
    }

    const i32 key_stride = anim->key_stride;
    const u16 *constants = reinterpret_cast<const u16 *>(anim->constants);
    ani3_scalemin_s *scale_min = anim->scale_min;
    u8 *keys = anim->keys + key_offset;
    const u16 *curve_types = anim->curve_types;

    const u8 *first_flags = node_flags + first_joint;
    const i32 decode_count = joint_count <= anim->node_count ? joint_count : anim->node_count;
    const u16 *skip_types = curve_types;
    ani3_scalemin_s *skip_scale_min = scale_min;
    u8 *joint_flags = buffer->joint_flags + first_joint;
    nuanimbuffjoint_s *joint = buffer->joints;
    const u8 *skip_flags = node_flags;
    for (; skip_flags < first_flags; ++skip_flags) {
        const u8 flags = *skip_flags;
        if (flags & 2) {
            if (skip_types[0] < 16) {
                keys += 4;
                ++skip_scale_min;
            }
            if (skip_types[1] < 16) {
                keys += 4;
                ++skip_scale_min;
            }
            if (skip_types[2] < 16) {
                keys += 4;
                ++skip_scale_min;
            }
        }
        if (flags & 1) {
            if (skip_types[3] < 16) {
                keys += 4;
                ++skip_scale_min;
            }
            if (skip_types[4] < 16) {
                keys += 4;
                ++skip_scale_min;
            }
            if (skip_types[5] < 16) {
                keys += 4;
                ++skip_scale_min;
            }
        }
        if (flags & 8) {
            if (skip_types[6] < 16) {
                keys += 4;
                ++skip_scale_min;
            }
            if (skip_types[7] < 16) {
                keys += 4;
                ++skip_scale_min;
            }
            if (skip_types[8] < 16) {
                keys += 4;
                ++skip_scale_min;
            }
        }
        skip_types += 9;
    }
    const i32 skipped_joints = skip_flags - node_flags;
    joint += skipped_joints;
    curve_types += skipped_joints * 9;
    node_flags = skip_flags;
    scale_min = skip_scale_min;

    const u8 *end_flags = first_flags + decode_count;
    for (; node_flags < end_flags; ++node_flags, ++joint) {
        const u8 flags = *node_flags;
        *joint_flags++ = flags;
        f32 *group_values = &joint->translation.x;

        for (i32 group = 0; group < 3; ++group) {
            const u16 *group_types = curve_types;
            curve_types += 3;
            if (__builtin_expect((flags & CurveGroupMasks[group]) == 0, 0)) {
                if (group == 2) {
                    group_values[0] = group_values[1] = group_values[2] = 1.0f;
                } else {
                    group_values[0] = group_values[1] = group_values[2] = 0.0f;
                }
            } else {
                group_values[0] = DecodeAni4Quat3Scalar(anim, key_stride, constants, group_types[0], quarter, fraction,
                                                        keys, scale_min);
                group_values[1] = DecodeAni4Quat3Scalar(anim, key_stride, constants, group_types[1], quarter, fraction,
                                                        keys, scale_min);
                group_values[2] = DecodeAni4Quat3Scalar(anim, key_stride, constants, group_types[2], quarter, fraction,
                                                        keys, scale_min);
            }

            group_values += 4;
        }
    }
    return 0;
}

}

__attribute__((optimize("O2"))) i32 ANI_SimpleAni3PlayerV4Joint_Blend_Quat3(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                            i32 joint_count, i32 first_joint, NUVEC *root_translation) {
    nuanimbuffjoint_s *joint = buffer->joints;
    f32 key =
        (frame - anim->first_frame) * static_cast<f32>(anim->key_count - 1) / static_cast<f32>(anim->frame_count - 1);
    const u8 *node_flags = anim->node_flags;
    const f32 inverse_blend = 1.0f - blend;
    key = key < 0.0f ? 0.0f : key;
    if (static_cast<f32>(static_cast<i32>(anim->key_count)) <= key)
        key = static_cast<f32>(anim->key_count - 1);
    const i32 whole = static_cast<i32>(key);
    const f32 fraction = key - static_cast<f32>(whole);
    const i32 key_stride = anim->key_stride;
    const u32 quarter = static_cast<u32>(whole) & 3;
    const u16 *constants = reinterpret_cast<const u16 *>(anim->constants);
    ani3_scalemin_s *scale_min = anim->scale_min;
    u8 *keys = anim->keys + (whole / 4) * key_stride;
    const i32 count = joint_count < anim->node_count ? joint_count : anim->node_count;
    const u16 *curve_types = anim->curve_types;
    if (first_joint != 0)
        root_translation = NULL;
    const u8 *first_flags = node_flags + first_joint;
    const u8 *end_flags = first_flags + count;
    for (; node_flags < first_flags; ++node_flags, ++joint) {
        if (*node_flags & 2) {
            SkipAni4V4Curve(curve_types[0], keys, scale_min);
            SkipAni4V4Curve(curve_types[1], keys, scale_min);
            SkipAni4V4Curve(curve_types[2], keys, scale_min);
        }
        if (*node_flags & 1) {
            SkipAni4V4Curve(curve_types[3], keys, scale_min);
            SkipAni4V4Curve(curve_types[4], keys, scale_min);
            SkipAni4V4Curve(curve_types[5], keys, scale_min);
        }
        if (__builtin_expect(!!(*node_flags & 8), 1)) {
            SkipAni4V4Curve(curve_types[6], keys, scale_min);
            SkipAni4V4Curve(curve_types[7], keys, scale_min);
            SkipAni4V4Curve(curve_types[8], keys, scale_min);
        }
        curve_types += 9;
    }
    u8 *joint_flags = buffer->joint_flags + first_joint;
    f32 *output = &joint->translation.x;
    for (; node_flags < end_flags; ++node_flags) {
        const u8 flags = *node_flags;
        *joint_flags++ |= flags;
        for (i32 group = 0; group < 3; ++group, curve_types += 3, output += 4) {
            if (!(flags & CurveGroupMasks[group])) {
                if (group == 0) {
                    output[0] *= inverse_blend;
                    output[1] *= inverse_blend;
                    output[2] *= inverse_blend;
                    if (root_translation) {
                        root_translation->x = root_translation->y = root_translation->z = 0.0f;
                        root_translation = NULL;
                    }
                } else if (group == 1) {
                    const f32 blended_w = output[3] * inverse_blend + blend;
                    output[0] *= inverse_blend;
                    output[1] *= inverse_blend;
                    output[3] = blended_w;
                    output[2] *= inverse_blend;
                    f32 length = NuFsqrt(output[3] * output[3] + output[0] * output[0] + output[1] * output[1] +
                                         output[2] * output[2]);
                    f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;
                    output[3] *= inverse_length;
                    output[0] *= inverse_length;
                    output[1] *= inverse_length;
                    output[2] *= inverse_length;
                } else {
                    output[0] = output[0] * inverse_blend + blend;
                    output[1] = output[1] * inverse_blend + blend;
                    output[2] = output[2] * inverse_blend + blend;
                }
            } else if (group != 1) {
                f32 sampled = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[0], quarter, fraction,
                                                    keys, scale_min);
                f32 delta = sampled - output[0];
                if (root_translation)
                    root_translation->x = sampled;
                output[0] += delta * blend;
                sampled = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[1], quarter, fraction, keys,
                                                scale_min);
                delta = sampled - output[1];
                if (root_translation)
                    root_translation->y = sampled;
                output[1] += delta * blend;
                sampled = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[2], quarter, fraction, keys,
                                                scale_min);
                delta = sampled - output[2];
                if (root_translation)
                    root_translation->z = -sampled;
                output[2] += delta * blend;
                root_translation = NULL;
            } else {
                NUQUAT first, second, result;
                DecodeAni4BlendQuat3Pair(anim, key_stride, constants, curve_types[0], quarter, keys, scale_min, first.x,
                                         second.x);
                DecodeAni4BlendQuat3Pair(anim, key_stride, constants, curve_types[1], quarter, keys, scale_min, first.y,
                                         second.y);
                DecodeAni4BlendQuat3Pair(anim, key_stride, constants, curve_types[2], quarter, keys, scale_min, first.z,
                                         second.z);
                first.w = NuFsqrt(1.0f - (first.x * first.x + first.y * first.y + first.z * first.z));
                second.w = NuFsqrt(1.0f - (second.x * second.x + second.y * second.y + second.z * second.z));
                NuQuatHarmonize(&first, &second);
                result.x = first.x * (1.0f - fraction) + second.x * fraction;
                result.y = first.y * (1.0f - fraction) + second.y * fraction;
                result.z = first.z * (1.0f - fraction) + second.z * fraction;
                result.w = first.w * (1.0f - fraction) + second.w * fraction;
                f32 length =
                    NuFsqrt(result.w * result.w + result.x * result.x + result.y * result.y + result.z * result.z);
                f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;
                result.w *= inverse_length;
                result.x *= inverse_length;
                result.y *= inverse_length;
                result.z *= inverse_length;
                VuQuatSlerpFast(reinterpret_cast<NUQUAT *>(output), reinterpret_cast<NUQUAT *>(output), &result, blend);
            }
        }
    }
    return 0;
}

__attribute__((optimize("O2"))) i32 ANI_SimpleAni3PlayerV4Joint_Blend_Quat3W(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                             i32 joint_count, i32 first_joint, NUVEC *root_translation) {
    const u8 *node_flags = anim->node_flags;
    const f32 inverse_blend = 1.0f - blend;
    f32 key =
        (frame - anim->first_frame) * static_cast<f32>(anim->key_count - 1) / static_cast<f32>(anim->frame_count - 1);
    key = key < 0.0f ? 0.0f : key;
    // Preserve the fractional interval after the last integer key.
    if (static_cast<f32>(static_cast<i32>(anim->key_count)) <= key)
        key = static_cast<f32>(anim->key_count - 1);
    const i32 whole = static_cast<i32>(key);
    const u32 quarter = static_cast<u32>(whole) & 3;
    const f32 fraction = key - static_cast<f32>(whole);
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u16 *curve_types = anim->curve_types;
    const i32 key_stride = anim->key_stride;
    u8 *keys = anim->keys + (whole / 4) * key_stride;
    const u16 *constants = reinterpret_cast<const u16 *>(anim->constants);
    const i32 count = joint_count < anim->node_count ? joint_count : anim->node_count;
    if (first_joint != 0)
        root_translation = NULL;
    const u8 *first_flags = node_flags + first_joint;
    for (const u8 *skip_flags = node_flags; skip_flags < first_flags; ++skip_flags) {
        for (i32 group = 0; group < 3; ++group) {
            i32 components = group == 1 ? 4 : 3;
            if (__builtin_expect(!!(*skip_flags & CurveGroupMasks[group]), 1)) {
                for (i32 component = 0; component < components; ++component) {
                    SkipAni4V4Curve(*curve_types++, keys, scale_min);
                }
            } else
                curve_types += components;
        }
    }
    node_flags += first_joint;
    const u8 *end_flags = node_flags + count;
    u8 *joint_flags = buffer->joint_flags + first_joint;
    nuanimbuffjoint_s *joint = buffer->joints + first_joint;
    for (; node_flags < end_flags; ++node_flags, ++joint) {
        const u8 flags = *node_flags;
        *joint_flags++ |= flags;
        f32 *output = &joint->translation.x;
        for (i32 group = 0; group < 3; ++group, output += 4) {
            if (!(flags & CurveGroupMasks[group])) {
                if (group == 0) {
                    output[0] *= inverse_blend;
                    output[1] *= inverse_blend;
                    output[2] *= inverse_blend;
                    if (root_translation) {
                        root_translation->x = root_translation->y = root_translation->z = 0.0f;
                        root_translation = NULL;
                    }
                } else if (group == 1) {
                    output[3] = output[3] * inverse_blend + blend;
                    output[0] *= inverse_blend;
                    output[1] *= inverse_blend;
                    output[2] *= inverse_blend;
                    f32 length = NuFsqrt(output[3] * output[3] + output[0] * output[0] + output[1] * output[1] +
                                         output[2] * output[2]);
                    f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;
                    output[3] *= inverse_length;
                    output[0] *= inverse_length;
                    output[1] *= inverse_length;
                    output[2] *= inverse_length;
                } else {
                    output[0] = output[0] * inverse_blend + blend;
                    output[1] = output[1] * inverse_blend + blend;
                    output[2] = output[2] * inverse_blend + blend;
                }
                curve_types += group == 1 ? 4 : 3;
            } else if (__builtin_expect(group != 1, 1)) {
                f32 sampled = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[0], quarter, fraction,
                                                    keys, scale_min);
                f32 delta_x = sampled - output[0];
                if (__builtin_expect(!!(root_translation), 1))
                    root_translation->x = sampled;
                output[0] += delta_x * blend;
                sampled = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[1], quarter, fraction, keys,
                                                scale_min);
                f32 delta_y = sampled - output[1];
                if (__builtin_expect(!!(root_translation), 1))
                    root_translation->y = sampled;
                output[1] += delta_y * blend;
                sampled = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[2], quarter, fraction, keys,
                                                scale_min);
                f32 delta_z = sampled - output[2];
                if (root_translation)
                    root_translation->z = -sampled;
                output[2] += delta_z * blend;
                root_translation = NULL;
                curve_types += 3;
            } else {
                NUQUAT first, second, result;
                DecodeAni4BlendQuat3WPair(anim, key_stride, constants, curve_types[0], quarter, keys, scale_min,
                                          first.x, second.x);
                DecodeAni4BlendQuat3WPair(anim, key_stride, constants, curve_types[1], quarter, keys, scale_min,
                                          first.y, second.y);
                DecodeAni4BlendQuat3WPair(anim, key_stride, constants, curve_types[2], quarter, keys, scale_min,
                                          first.z, second.z);
                DecodeAni4BlendQuat3WPair(anim, key_stride, constants, curve_types[3], quarter, keys, scale_min,
                                          first.w, second.w);
                curve_types += 4;
                result.x = first.x * (1.0f - fraction) + second.x * fraction;
                result.z = first.z * (1.0f - fraction) + second.z * fraction;
                result.y = first.y * (1.0f - fraction) + second.y * fraction;
                result.w = first.w * (1.0f - fraction) + second.w * fraction;
                f32 length =
                    NuFsqrt(result.w * result.w + result.x * result.x + result.y * result.y + result.z * result.z);
                f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;
                result.w *= inverse_length;
                result.x *= inverse_length;
                result.y *= inverse_length;
                result.z *= inverse_length;
                // Original W blending writes this local result and discards it.
                VuQuatSlerpFast(&result, reinterpret_cast<NUQUAT *>(output), &result, blend);
            }
        }
    }
    return 0;
}

extern "C" {
extern "C" __attribute__((optimize("O2"))) i32 ANI_SimpleAni3PlayerV4Joint_Blend_EulerQuat(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer,
                                                           f32 blend, i32 joint_count, i32 first_joint,
                                                           NUVEC *root_translation) {
    const u8 *node_flags = anim->node_flags;
    const f32 inverse_blend = 1.0f - blend;
    const f32 last_key = static_cast<f32>(anim->key_count - 1);
    f32 key = (frame - static_cast<f32>(anim->first_frame)) * last_key / static_cast<f32>(anim->frame_count - 1);
    key = 0.0f <= key ? key : 0.0f;
    // The blend variant compares against key_count, then selects key_count - 1.
    key = static_cast<f32>(anim->key_count) <= key ? last_key : key;
    const i32 whole = static_cast<i32>(key);
    const u32 quarter = static_cast<u32>(whole) & 3;
    const f32 fraction = key - static_cast<f32>(whole);
    const i32 key_offset = (whole / 4) * anim->key_stride;
    u8 *keys = anim->keys + key_offset;
    const u16 *constants = reinterpret_cast<const u16 *>(anim->constants);
    i32 count = joint_count < anim->node_count ? joint_count : anim->node_count;
    const u16 *curve_types = anim->curve_types;
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u8 *first_flags = node_flags + first_joint;
    for (const u8 *skip_flags = node_flags; skip_flags < first_flags; ++skip_flags) {
        if (*skip_flags & 2) {
            SkipAni4V4Curve(curve_types[0], keys, scale_min);
            SkipAni4V4Curve(curve_types[1], keys, scale_min);
            SkipAni4V4Curve(curve_types[2], keys, scale_min);
        }
        if (__builtin_expect(!!(*skip_flags & 1), 1)) {
            SkipAni4V4Curve(curve_types[3], keys, scale_min);
            SkipAni4V4Curve(curve_types[4], keys, scale_min);
            SkipAni4V4Curve(curve_types[5], keys, scale_min);
        }
        if (*skip_flags & 8) {
            SkipAni4V4Curve(curve_types[6], keys, scale_min);
            SkipAni4V4Curve(curve_types[7], keys, scale_min);
            SkipAni4V4Curve(curve_types[8], keys, scale_min);
        }
        curve_types += 9;
    }
    node_flags += first_joint;
    const u8 *end_flags = node_flags + count;
    u8 *joint_flags = buffer->joint_flags + first_joint;
    nuanimbuffjoint_s *joint = buffer->joints + first_joint;
    NUVEC *root = first_joint == 0 ? root_translation : NULL;
    for (; node_flags < end_flags; ++node_flags, ++joint) {
        const u8 flags = *node_flags;
        *joint_flags++ |= flags;
        f32 *output = &joint->translation.x;
        for (i32 group = 0; group < 3; ++group, curve_types += 3, output += 4) {
            if (!(flags & CurveGroupMasks[group])) {
                if (group == 0) {
                    output[0] *= inverse_blend;
                    output[1] *= inverse_blend;
                    output[2] *= inverse_blend;
                    if (root != NULL) {
                        root->x = root->y = root->z = 0.0f;
                        root = NULL;
                    }
                } else if (group == 1) {
                    output[0] *= inverse_blend;
                    output[3] = output[3] * inverse_blend + blend;
                    output[1] *= inverse_blend;
                    output[2] *= inverse_blend;
                    f32 length = NuFsqrt(output[3] * output[3] + output[0] * output[0] + output[1] * output[1] +
                                         output[2] * output[2]);
                    f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;
                    output[3] *= inverse_length;
                    output[0] *= inverse_length;
                    output[1] *= inverse_length;
                    output[2] *= inverse_length;
                } else {
                    output[0] = output[0] * inverse_blend + blend;
                    output[1] = output[1] * inverse_blend + blend;
                    output[2] = output[2] * inverse_blend + blend;
                }
            } else if (group != 1) {
                f32 decoded = DecodeAni4Quat3Scalar(anim, anim->key_stride, constants, curve_types[0], quarter,
                                                    fraction, keys, scale_min);
                f32 delta = decoded - output[0];
                if (root != NULL)
                    root->x = decoded;
                output[0] += blend * delta;
                decoded = DecodeAni4Quat3Scalar(anim, anim->key_stride, constants, curve_types[1], quarter, fraction,
                                                keys, scale_min);
                delta = decoded - output[1];
                if (root != NULL)
                    root->y = decoded;
                output[1] += blend * delta;
                decoded = DecodeAni4Quat3Scalar(anim, anim->key_stride, constants, curve_types[2], quarter, fraction,
                                                keys, scale_min);
                delta = decoded - output[2];
                if (root != NULL)
                    root->z = -decoded;
                root = NULL;
                output[2] += blend * delta;
            } else {
                NUVEC first_angles, second_angles;
                DecodeAni4EulerPair(anim, anim->key_stride, constants, curve_types[0], quarter, keys, scale_min,
                                    first_angles.x, second_angles.x);
                DecodeAni4EulerPair(anim, anim->key_stride, constants, curve_types[1], quarter, keys, scale_min,
                                    first_angles.y, second_angles.y);
                DecodeAni4EulerPair(anim, anim->key_stride, constants, curve_types[2], quarter, keys, scale_min,
                                    first_angles.z, second_angles.z);
                NUQUAT first, second;
                NuQuatFromEulerXYZ(&first, static_cast<i32>(first_angles.x * 10430.378f),
                                   static_cast<i32>(first_angles.y * 10430.378f),
                                   static_cast<i32>(first_angles.z * 10430.378f));
                NuQuatFromEulerXYZ(&second, static_cast<i32>(10430.378f * second_angles.x),
                                   static_cast<i32>(10430.378f * second_angles.y),
                                   static_cast<i32>(second_angles.z * 10430.378f));
                if (__builtin_expect(first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w < 0.0f, 1)) {
                    second.x = -second.x;
                    second.y = -second.y;
                    second.z = -second.z;
                    second.w = -second.w;
                }
                NUQUAT result;
                result.x = first.x * (1.0f - fraction) + second.x * fraction;
                result.y = first.y * (1.0f - fraction) + second.y * fraction;
                result.z = first.z * (1.0f - fraction) + second.z * fraction;
                result.w = (1.0f - fraction) * first.w + second.w * fraction;
                f32 length =
                    NuFsqrt(result.w * result.w + result.x * result.x + result.y * result.y + result.z * result.z);
                f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;
                result.w *= inverse_length;
                result.x *= inverse_length;
                result.y *= inverse_length;
                result.z *= inverse_length;
                VuQuatSlerpFast(reinterpret_cast<NUQUAT *>(output), reinterpret_cast<NUQUAT *>(output), &result, blend);
            }
        }
    }
    return 0;
}

extern "C" i32 ANI_SimpleAni3PlayerV4Joint_Blend(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                                 i32 joint_count, i32 first_joint, NUVEC *root_translation) {
    if ((anim->format_flags & ANI3_FORMAT_QUATERNION_ROTATION) != 0) {
        if ((anim->format_flags & ANI3_FORMAT_QUATERNION_STORES_W) != 0) {
            return ANI_SimpleAni3PlayerV4Joint_Blend_Quat3W(anim, frame, buffer, blend, joint_count, first_joint,
                                                            root_translation);
        } else {
            return ANI_SimpleAni3PlayerV4Joint_Blend_Quat3(anim, frame, buffer, blend, joint_count, first_joint,
                                                           root_translation);
        }
    }
    if (buffer->use_quaternions != 0) {
        return ANI_SimpleAni3PlayerV4Joint_Blend_EulerQuat(anim, frame, buffer, blend, joint_count, first_joint,
                                                           root_translation);
    }

    // Sampling advances a local cursor; the animation owns the unchanged table.
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u16 *constants = reinterpret_cast<const u16 *>(anim->constants);
    const u16 key_stride = anim->key_stride;
    const f32 last_key = static_cast<f32>(anim->key_count - 1);
    f32 key = (frame - anim->first_frame) * last_key / static_cast<f32>(anim->frame_count - 1);
    if (key < 0.0f) {
        key = 0.0f;
    }
    if (static_cast<f32>(anim->key_count) <= key) {
        key = last_key;
    }

    const i32 whole_key = static_cast<i32>(key);
    const u32 quarter = static_cast<u32>(whole_key) & 3;
    const f32 fraction = key - static_cast<f32>(whole_key);
    u8 *keys = anim->keys + (whole_key / 4) * key_stride;
    const u16 *curve_types = anim->curve_types;

    // Advance all packed stream cursors to the first requested joint.
    for (i32 joint = 0; joint < first_joint; ++joint) {
        const u8 flags = anim->node_flags[joint];
        if ((flags & NUANIMBUFF_JOINT_TRANSLATION) != 0) {
            SkipAni4V4Curve(curve_types[0], keys, scale_min);
            SkipAni4V4Curve(curve_types[1], keys, scale_min);
            SkipAni4V4Curve(curve_types[2], keys, scale_min);
        }
        if (__builtin_expect((flags & NUANIMBUFF_JOINT_ROTATION) != 0, 1)) {
            SkipAni4V4Curve(curve_types[3], keys, scale_min);
            SkipAni4V4Curve(curve_types[4], keys, scale_min);
            SkipAni4V4Curve(curve_types[5], keys, scale_min);
        }
        if ((flags & NUANIMBUFF_JOINT_SCALE) != 0) {
            SkipAni4V4Curve(curve_types[6], keys, scale_min);
            SkipAni4V4Curve(curve_types[7], keys, scale_min);
            SkipAni4V4Curve(curve_types[8], keys, scale_min);
        }
        curve_types += 9;
    }

    const i32 decode_count = joint_count <= anim->node_count ? joint_count : anim->node_count;
    NUVEC *root = first_joint == 0 ? root_translation : NULL;
    const i32 end_joint = first_joint + decode_count;
    const f32 inverse_blend = 1.0f - blend;

    for (i32 joint_index = first_joint; joint_index < end_joint; ++joint_index) {
        const u8 flags = anim->node_flags[joint_index];
        buffer->joint_flags[joint_index] |= flags;
        f32 *group_values = reinterpret_cast<f32 *>(&buffer->joints[joint_index]);

        for (i32 group = 0; group < 3; ++group) {
            if ((flags & CurveGroupMasks[group]) != 0) {
                f32 decoded = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[0], quarter, fraction,
                                                    keys, scale_min);
                f32 delta = decoded - group_values[0];
                if (group == 1) {
                    delta = WrapAni4BlendRotation(delta);
                }
                if (root != NULL) {
                    root->x = decoded;
                }
                group_values[0] += delta * blend;

                decoded = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[1], quarter, fraction, keys,
                                                scale_min);
                delta = decoded - group_values[1];
                if (group == 1) {
                    delta = WrapAni4BlendRotation(delta);
                }
                if (root != NULL) {
                    root->y = decoded;
                }
                group_values[1] += delta * blend;

                decoded = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[2], quarter, fraction, keys,
                                                scale_min);
                delta = decoded - group_values[2];
                if (group == 1) {
                    delta = WrapAni4BlendRotation(delta);
                }
                if (root != NULL) {
                    root->z = -decoded;
                }
                group_values[2] += delta * blend;
            } else if (group == 2) {
                group_values[0] = group_values[0] * inverse_blend + blend;
                group_values[1] = group_values[1] * inverse_blend + blend;
                group_values[2] = group_values[2] * inverse_blend + blend;
            } else {
                group_values[0] *= inverse_blend;
                group_values[1] *= inverse_blend;
                group_values[2] *= inverse_blend;
                if (root != NULL) {
                    root->y = 0.0f;
                    root->x = 0.0f;
                    root->z = 0.0f;
                }
            }

            if (group == 0) {
                root = NULL;
            }
            curve_types += 3;
            group_values += 4;
        }
    }
    return 0;
}

    // Original @0x2c6560.  The first contribution overwrites the buffer;
    // subsequent contributions use the blend player.
    void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation) {
        if (buffer == NULL) {
            buffer = static_cast<nuanimbuff_s *>(globalbuffer);
        }
        buffer->joint_count = object->joint_count;

        i32 end_joint = object->joint_count;
        if (first_joint != 0) {
            end_joint = findrange(reinterpret_cast<nugscn_s *>(object), first_joint) + 1;
        }
        if (overwrite != 0) {
            ANI_SimpleAni3PlayerV4Joint(animation, time - 1.0f, buffer, end_joint - first_joint, first_joint);
            if (root_translation != NULL) {
                root_translation->x = buffer->joints[0].translation.x;
                root_translation->y = buffer->joints[0].translation.y;
                root_translation->z = -buffer->joints[0].translation.z;
            }
        } else {
            ANI_SimpleAni3PlayerV4Joint_Blend(animation, time - 1.0f, buffer, blend, end_joint - first_joint,
                                              first_joint, root_translation);
        }
    }

extern "C" __attribute__((optimize("O2"))) void ANI_Ani3ExtractAllNodeCurves(ani3_animheader_s *anim, float frame, float *values, i32 node,
                                             char *curve_mask) {
    u32 curve_count = anim->curve_count;
    u32 quarter;
    i32 stride;
    float fraction;
    i32 key_offset;

    if (ForcePlayEndFrame != 0 && anim->end_frame != 0) {
        float key = static_cast<float>(anim->end_frame + anim->key_count - 4);
        i32 whole_key = static_cast<i32>(key);
        fraction = key - static_cast<float>(whole_key);
        quarter = static_cast<u32>(whole_key) & 3;
        stride = anim->key_stride;
        key_offset = (whole_key / 4) * stride;
    } else {
        if (__builtin_expect(anim->key_count == 1, 0)) {
            quarter = 0;
            fraction = 0.0f;
            stride = anim->key_stride;
            key_offset = 0;
        } else {
            float key = frame - static_cast<i32>(anim->first_frame);
            float last_key = static_cast<float>(anim->key_count - 1);
            key = key * last_key / static_cast<float>(anim->frame_count - 1);
            if (key < 0.0f) {
                key = 0.0f;
            }
            if (__builtin_expect(last_key <= key, 0)) {
                i32 whole_key = static_cast<i32>(last_key);
                fraction = last_key - static_cast<float>(whole_key);
                stride = anim->key_stride;
                quarter = static_cast<u32>(whole_key) & 3;
                key_offset = (whole_key / 4) * stride;
            } else {
                i32 whole_key = static_cast<i32>(key);
                fraction = key - static_cast<float>(whole_key);
                quarter = static_cast<u32>(whole_key) & 3;
                stride = anim->key_stride;
                key_offset = (whole_key / 4) * stride;
            }
        }
    }

    u16 *types = anim->curve_types;
    u8 *force_zero = reinterpret_cast<u8 *>(types + anim->node_count * curve_count);
    ani3_scalemin_s *scale_min = anim->scale_min;
    i16 *constants = anim->constants;
    const i32 constant_offset = anim->constant_index - 16;
    u16 *constant_values = reinterpret_cast<u16 *>(constants);
    u8 *keys = anim->keys + key_offset;

    for (i32 n = 0; n < node; ++n) {
        if (curve_count != 0) {
            u16 *end_types = types + curve_count;
            u16 *cursor = types;
            do {
                u16 type = *cursor++;
                if (type < 16) {
                    ++scale_min;
                    keys += KeyStructSizes[type];
                }
            } while (cursor != end_types);
            types = end_types;
            force_zero += curve_count;
        }
    }

    if (curve_count == 0) {
        return;
    }
    i32 quarter_shift = static_cast<i32>(quarter) * 6;
    i32 next_quarter_shift = (static_cast<i32>(quarter) * 3 + 3) * 2;
    for (u32 curve = 0; curve < curve_count; ++curve, ++values) {
        u16 type = types[curve];
        if (curve_mask != NULL && *curve_mask != curve) {
            switch (type) {
                case 7:
                    keys += 8;
                    ++scale_min;
                    break;
                case 6:
                    keys += 4;
                    ++scale_min;
                    break;
                case 8:
                case 10:
                    keys += 4;
                    break;
            }
            continue;
        }
        if (curve_mask != NULL) {
            ++curve_mask;
        }

        float key_fraction = (force_zero[curve] & 1) != 0 ? 0.0f : fraction;
        switch (type) {
            case 7:
                *values = CalcValue1648(reinterpret_cast<char *>(keys), quarter, stride, key_fraction, scale_min);
                keys += 8;
                ++scale_min;
                break;
            case 8:
                *values = static_cast<float>(constants[keys[quarter]]);
                keys += 4;
                break;
            case 10: {
                u32 index = keys[quarter];
                keys += 4;
                const u32 packed = static_cast<u32>(static_cast<i32>(constants[index + 1])) |
                                   (static_cast<u32>(static_cast<i32>(constants[index])) << 16);
                float value;
                memcpy(&value, &packed, sizeof(value));
                *values = value;
                break;
            }
            case 6: {
                u32 first = *reinterpret_cast<u32 *>(keys);
                i32 first_value = first & 0xff;
                u32 tangents = first >> 8;
                u32 next = *reinterpret_cast<u32 *>(keys + stride);
                i32 next_value = next & 0xff;
                float tangent0 = static_cast<float>((tangents >> quarter_shift) & 0x3f) * 0.01587302f;
                if (__builtin_expect(quarter != 3, 1)) {
                    float tangent1 = static_cast<float>((tangents >> (next_quarter_shift & 0x1f)) & 0x3f) * 0.01587302f;
                    float packed_value = (static_cast<float>(next_value) - static_cast<float>(first_value)) *
                                             ((tangent1 - tangent0) * key_fraction + tangent0) +
                                         static_cast<float>(first_value);
                    *values = packed_value * scale_min->scale + scale_min->minimum;
                } else {
                    float interpolated = (static_cast<float>(next_value) - static_cast<float>(first_value)) * tangent0 +
                                         static_cast<float>(first_value);
                    float tangent1 = static_cast<float>((next >> 8) & 0x3f) * 0.01587302f;
                    float after = static_cast<float>(static_cast<i32>(keys[stride * 2]));
                    float packed_value =
                        (((after - static_cast<float>(next_value)) * tangent1 + static_cast<float>(next_value)) -
                         interpolated) *
                            key_fraction +
                        interpolated;
                    *values = packed_value * scale_min->scale + scale_min->minimum;
                }
                keys += 4;
                ++scale_min;
                break;
            }
            default:
                u16 constant = constant_values[constant_offset + type];
                *values = static_cast<float>(constant) * anim->scale + anim->minimum;
                break;
        }
    }
}

    f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask) {
        f32 *values = *reinterpret_cast<f32 **>(reinterpret_cast<u8 *>(globalbuffer) + 8);
        ANI_Ani3ExtractAllNodeCurves(anim, frame - 1.0f, values, node, curve_mask);
        return values;
    }

    void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix) {
        u8 node_flags = animation->node_flags[node];
        f32 *values = NuAnimCurveExtractAllNodeCurves_3(animation, node, frame, NULL);

        if ((node_flags & NUANIM_NODE_HAS_ROTATION) != 0) {
            NUANGVEC rotation;
            rotation.x = static_cast<NUANG>(values[3] * 10430.378f);
            rotation.y = static_cast<NUANG>(values[4] * 10430.378f);
            rotation.z = static_cast<NUANG>(values[5] * 10430.378f);
            NuMtxSetRotateXYZVU0(matrix, &rotation);
        } else {
            NuMtxSetIdentity(matrix);
        }

        if ((node_flags & NUANIM_NODE_HAS_SCALE) != 0) {
            NuMtxPreScaleVU0(matrix, reinterpret_cast<NUVEC *>(&values[6]));
        }

        NuMtxTranslate(matrix, reinterpret_cast<NUVEC *>(values));

        // ANI3 animation data uses the opposite handedness for Z.
        matrix->m02 = -matrix->m02;
        matrix->m12 = -matrix->m12;
        matrix->m20 = -matrix->m20;
        matrix->m21 = -matrix->m21;
        matrix->m23 = -matrix->m23;
        matrix->m32 = -matrix->m32;
    }

    void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix) {
        NuAnimCurve2SetApplyToMatrix_3(reinterpret_cast<ani3_animheader_s *>(animation), node, frame, matrix);
    }

    void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim) {
        f32 inverse_blend = 1.0f - blend;
        u8 flags = override_anim ? override_anim->flags : 0;
        u8 rotate = flags & 1, rescale = flags & 4, translate = flags & 2;
        NUANGVEC angles;
        NUVEC r0, r1, s0, s1, translation, temporary;
        if ((first->flags & 1) || (second->flags & 1) || rotate) {
            for (i32 component = 0; component < 3; ++component) {
                reinterpret_cast<f32 *>(&r0)[component] =
                    first->curves[component + 3] ? NuAnimCurveCalcVal2(first->curves[component + 3], first_time)
                                                 : first->constants[component + 3];
                reinterpret_cast<f32 *>(&r1)[component] =
                    second->curves[component + 3] ? NuAnimCurveCalcVal2(second->curves[component + 3], second_time)
                                                  : second->constants[component + 3];
            }
            r0.x -= r0.x - r1.x > 3.1415927410125732f ? 6.2831854820251465f : 0.0f;
            r1.x -= r1.x - r0.x > 3.1415927410125732f ? 6.2831854820251465f : 0.0f;
            r0.y -= r0.y - r1.y > 3.1415927410125732f ? 6.2831854820251465f : 0.0f;
            r1.y -= r1.y - r0.y > 3.1415927410125732f ? 6.2831854820251465f : 0.0f;
            r0.z -= r0.z - r1.z > 3.1415927410125732f ? 6.2831854820251465f : 0.0f;
            r1.z -= r1.z - r0.z > 3.1415927410125732f ? 6.2831854820251465f : 0.0f;
            r0.x = r0.x * inverse_blend + r1.x * blend;
            r0.z = r0.z * inverse_blend + r1.z * blend;
            r0.y = r0.y * inverse_blend + r1.y * blend;
            if (rotate) {
                angles.z = static_cast<i32>((r0.z + override_anim->rotation.z) * 10430.3779296875f);
                angles.y = static_cast<i32>((r0.y + override_anim->rotation.y) * 10430.3779296875f);
                angles.x = static_cast<i32>((r0.x + override_anim->rotation.x) * 10430.3779296875f);
                if (flags & 8) {
                    angles.x &= 0xffff;
                    if (angles.x > 32767)
                        angles.x -= 65536;
                    if (angles.x > override_anim->rotation_limit_start[0])
                        angles.x = override_anim->rotation_limit_start[0];
                    else if (angles.x < override_anim->rotation_limit_end[0])
                        angles.x = override_anim->rotation_limit_end[0];
                }
                if (flags & 16) {
                    angles.y &= 0xffff;
                    if (angles.y > 32767)
                        angles.y -= 65536;
                    if (angles.y > override_anim->rotation_limit_start[1])
                        angles.y = override_anim->rotation_limit_start[1];
                    else if (angles.y < override_anim->rotation_limit_end[1])
                        angles.y = override_anim->rotation_limit_end[1];
                }
                if (flags & 32) {
                    angles.z &= 0xffff;
                    if (angles.z > 32767)
                        angles.z -= 65536;
                    if (angles.z > override_anim->rotation_limit_start[2])
                        angles.z = override_anim->rotation_limit_start[2];
                    else if (angles.z < override_anim->rotation_limit_end[2])
                        angles.z = override_anim->rotation_limit_end[2];
                }
            } else {
                angles.x = static_cast<i32>(r0.x * 10430.3779296875f);
                angles.z = static_cast<i32>(r0.z * 10430.3779296875f);
                angles.y = static_cast<i32>(r0.y * 10430.3779296875f);
            }
            NuMtxSetRotateXYZVU0(matrix, &angles);
        } else
            NuMtxSetIdentity(matrix);
        if ((first->flags & 0x20) || (second->flags & 0x20))
            NuMtxMulRVU0(matrix, matrix, &joint->animation_bind_matrix);
        if ((first->flags & 8) || (second->flags & 8) || rescale) {
            for (i32 component = 0; component < 3; ++component) {
                reinterpret_cast<f32 *>(&s0)[component] =
                    first->curves[component + 6] ? NuAnimCurveCalcVal2(first->curves[component + 6], first_time)
                                                 : first->constants[component + 6];
                reinterpret_cast<f32 *>(&s1)[component] =
                    second->curves[component + 6] ? NuAnimCurveCalcVal2(second->curves[component + 6], second_time)
                                                  : second->constants[component + 6];
            }
            scale->x = blend * s0.x + inverse_blend * s1.x;
            scale->y = blend * s0.y + inverse_blend * s1.y;
            scale->z = blend * s0.z + inverse_blend * s1.z;
            if (rescale) {
                scale->x += override_anim->scale.x;
                scale->y += override_anim->scale.y;
                scale->z += override_anim->scale.z;
            }
            NuMtxPreScaleVU0(matrix, scale);
            scale->x *= parent_scale->x;
            scale->y *= parent_scale->y;
            scale->z *= parent_scale->z;
        } else
            *scale = *parent_scale;
        if ((first->flags | second->flags) & 0x10) {
            if (parent_scale->x != 0.0f && parent_scale->y != 0.0f && parent_scale->z != 0.0f) {
                temporary.x = 1.0f / parent_scale->x;
                temporary.y = 1.0f / parent_scale->y;
                temporary.z = 1.0f / parent_scale->z;
            } else
                temporary.x = temporary.y = temporary.z = 0.0f;
            NuMtxScaleVU0(matrix, &temporary);
            scale->x *= temporary.x;
            scale->y *= temporary.y;
            scale->z *= temporary.z;
        }
        for (i32 component = 0; component < 3; ++component) {
            reinterpret_cast<f32 *>(&translation)[component] =
                (first->curves[component] ? NuAnimCurveCalcVal2(first->curves[component], first_time)
                                          : first->constants[component]) *
                inverse_blend;
            reinterpret_cast<f32 *>(&translation)[component] +=
                (second->curves[component] ? NuAnimCurveCalcVal2(second->curves[component], second_time)
                                           : second->constants[component]) *
                blend;
        }
        if (translate) {
            translation.x += override_anim->translation.x;
            translation.y += override_anim->translation.y;
            translation.z += override_anim->translation.z;
        }
        NuMtxTranslate(matrix, &translation);
        if (joint->data_0x51[0] & 8) {
            NUVEC *pivot = reinterpret_cast<NUVEC *>(joint->data_0x40);
            NuMtxPreTranslate(matrix, pivot);
            temporary.x = -pivot->x;
            temporary.y = -pivot->y;
            temporary.z = -pivot->z;
            NuMtxTranslate(matrix, &temporary);
        }
        matrix->m02 = -matrix->m02;
        matrix->m12 = -matrix->m12;
        matrix->m20 = -matrix->m20;
        matrix->m21 = -matrix->m21;
        matrix->m23 = -matrix->m23;
        matrix->m32 = -matrix->m32;
    }

    void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation) {
        NUVEC translation, temporary;
        NUANGVEC angles;
        if (root_translation)
            root_translation->x = root_translation->y = root_translation->z = 0.0f;
        if (locator_translation)
            locator_translation->x = locator_translation->y = locator_translation->z = 0.0f;
        u8 override_flags, rotate, translate, rescale;
        if (override_anim) {
            override_flags = override_anim->flags;
            rotate = override_flags & 1;
            rescale = override_flags & 4;
            translate = override_flags & 2;
        } else
            override_flags = rotate = translate = rescale = 0;
        if ((flags & 1) || rotate) {
            NUVEC rotation = {0.0f, 0.0f, 0.0f};
            if (flags & 1) {
                rotation.x = types[3] ? NuAnimCurve2CalcValEx(curves + 3, time, types[3]) : curves[3].data.constant;
                rotation.y = types[4] ? NuAnimCurve2CalcValEx(curves + 4, time, types[4]) : curves[4].data.constant;
                rotation.z = types[5] ? NuAnimCurve2CalcValEx(curves + 5, time, types[5]) : curves[5].data.constant;
            }
            if (rotate) {
                rotation.x += override_anim->rotation.x;
                rotation.z += override_anim->rotation.z;
                rotation.y += override_anim->rotation.y;
            }
            angles.z = static_cast<i32>(rotation.z * 10430.3779296875f);
            angles.x = static_cast<i32>(rotation.x * 10430.3779296875f);
            angles.y = static_cast<i32>(rotation.y * 10430.3779296875f);
            if (rotate) {
                if (override_flags & 8) {
                    angles.x &= 0xffff;
                    if (angles.x > 32767)
                        angles.x -= 65536;
                    if (angles.x > override_anim->rotation_limit_start[0])
                        angles.x = override_anim->rotation_limit_start[0];
                    else if (angles.x < override_anim->rotation_limit_end[0])
                        angles.x = override_anim->rotation_limit_end[0];
                }
                if (override_flags & 16) {
                    angles.y &= 0xffff;
                    if (angles.y > 32767)
                        angles.y -= 65536;
                    if (angles.y > override_anim->rotation_limit_start[1])
                        angles.y = override_anim->rotation_limit_start[1];
                    else if (angles.y < override_anim->rotation_limit_end[1])
                        angles.y = override_anim->rotation_limit_end[1];
                }
                if (override_flags & 32) {
                    angles.z &= 0xffff;
                    if (angles.z > 32767)
                        angles.z -= 65536;
                    if (angles.z > override_anim->rotation_limit_start[2])
                        angles.z = override_anim->rotation_limit_start[2];
                    else if (angles.z < override_anim->rotation_limit_end[2])
                        angles.z = override_anim->rotation_limit_end[2];
                }
            }
            NuMtxSetRotateXYZVU0(matrix, &angles);
        } else
            NuMtxSetIdentity(matrix);
        if (flags & 0x20)
            NuMtxMulRVU0(matrix, matrix, &joint->animation_bind_matrix);
        if ((flags & 8) || rescale) {
            if (flags & 8) {
                scale->x = types[6] ? NuAnimCurve2CalcValEx(curves + 6, time, types[6]) : curves[6].data.constant;
                scale->y = types[7] ? NuAnimCurve2CalcValEx(curves + 7, time, types[7]) : curves[7].data.constant;
                scale->z = types[8] ? NuAnimCurve2CalcValEx(curves + 8, time, types[8]) : curves[8].data.constant;
            } else
                scale->x = scale->y = scale->z = 0.0f;
            if (rescale) {
                scale->x += override_anim->scale.x;
                scale->y += override_anim->scale.y;
                scale->z += override_anim->scale.z;
            }
            NuMtxPreScaleVU0(matrix, scale);
            scale->x *= parent_scale->x;
            scale->y *= parent_scale->y;
            scale->z *= parent_scale->z;
        } else
            *scale = *parent_scale;
        if ((flags & 0x10) && parent_scale) {
            if (parent_scale->x != 0.0f && parent_scale->y != 0.0f && parent_scale->z != 0.0f) {
                temporary.x = 1.0f / parent_scale->x;
                temporary.y = 1.0f / parent_scale->y;
                temporary.z = 1.0f / parent_scale->z;
            } else
                temporary.x = temporary.y = temporary.z = 0.0f;
            NuMtxScaleVU0(matrix, &temporary);
            scale->x *= temporary.x;
            scale->y *= temporary.y;
            scale->z *= temporary.z;
        }
        translation.x = types[0] ? NuAnimCurve2CalcValEx(curves, time, types[0]) : curves[0].data.constant;
        translation.y = types[1] ? NuAnimCurve2CalcValEx(curves + 1, time, types[1]) : curves[1].data.constant;
        translation.z = types[2] ? NuAnimCurve2CalcValEx(curves + 2, time, types[2]) : curves[2].data.constant;
        if (root_translation) {
            root_translation->x = translation.x;
            root_translation->y = translation.y;
            root_translation->z = -translation.z;
        }
        if (translate) {
            translation.x += override_anim->translation.x;
            translation.y += override_anim->translation.y;
            translation.z += override_anim->translation.z;
        }
        NuMtxTranslate(matrix, &translation);
        if (joint->data_0x51[0] & 8) {
            NUVEC *pivot = reinterpret_cast<NUVEC *>(joint->data_0x40);
            NuMtxPreTranslate(matrix, pivot);
            temporary.x = -pivot->x;
            temporary.y = -pivot->y;
            temporary.z = -pivot->z;
            NuMtxTranslate(matrix, &temporary);
            if (locator_translation) {
                NuVecMtxRotate(locator_translation, pivot, matrix);
                NuVecSub(locator_translation, locator_translation, pivot);
                locator_translation->z = -locator_translation->z;
            }
        }
        matrix->m02 = -matrix->m02;
        matrix->m12 = -matrix->m12;
        matrix->m20 = -matrix->m20;
        matrix->m21 = -matrix->m21;
        matrix->m23 = -matrix->m23;
        matrix->m32 = -matrix->m32;
    }

    void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim) {
        NuAnimCurve2SetApplyToJointTransLoc(curves, types, flags, time, joint, scale, parent_scale, matrix,
                                            override_anim, NULL, NULL);
    }

extern "C" void NuSpecialList(NUGSCN *) {
    // The original body is an intentional no-op.
}

}

i32 NuSpecialFind(NUGSCN *scene, nuhspecial_s *dest, char *name, i32 flags) {
    (void)flags; // Present in the exported ABI; unused by the original body.

    nuhspecial_s *handle = dest;
    if (__builtin_expect(name != NULL && scene != NULL, 1)) {
        NUDLDLISTSCENE *display_scene = reinterpret_cast<NUDLDLISTSCENE *>(scene->display_list);
        if (display_scene != NULL) {
            NUDISPLAYSPECIAL *special = static_cast<NUDISPLAYSPECIAL *>(display_scene->specials);
            for (i32 i = 0; i < scene->display_list->nspecials; ++special, ++i) {
                if (NuStrICmp(name, special->name) == 0) {
                    handle->scene = scene;
                    handle->display_special = reinterpret_cast<NUDISPLAYSPECIAL_s *>(special);
                    handle->special = NULL;
                    return 1;
                }
            }
        } else {
            NuSpecialLegacyLayout *special = reinterpret_cast<NuSpecialLegacyLayout *>(scene->specials);
            for (i32 i = 0; i < scene->numspecial; ++special, ++i) {
                if (NuStrICmp(name, special->name) == 0) {
                    handle->scene = scene;
                    handle->special = special;
                    handle->display_special = NULL;
                    return 1;
                }
            }
        }
    }

    handle->scene = NULL;
    handle->special = NULL;
    handle->display_special = NULL;
    return 0;
}

extern "C" {
extern "C" i32 NuSpecialFindMultiWC(NUGSCN *scene, nuhspecial_s *dest, char (*wildcards)[20], char *pattern,
                                    i32 capacity, i32 flags) {
    i32 count = 0;
    if (scene != NULL) {
        if (scene->display_list != NULL) {
            NUDISPLAYSPECIAL *special = static_cast<NUDISPLAYSPECIAL *>(scene->display_list->specials);
            for (i32 i = 0; i < scene->display_list->nspecials; ++special, ++i) {
                if (NuStrICmpWC(pattern, special->name, wildcards != NULL ? *wildcards : NULL) == 0) {
                    if (dest != NULL) {
                        dest->scene = scene;
                        dest->display_special = special;
                        dest->special = NULL;
                        ++dest;
                    }
                    if (wildcards != NULL) {
                        ++wildcards;
                    }
                    if (++count >= capacity) {
                        return count;
                    }
                }
            }
        } else {
            NuSpecialLegacyLayout *special = reinterpret_cast<NuSpecialLegacyLayout *>(scene->specials);
            for (i32 i = 0; i < scene->numspecial; ++special, ++i) {
                if (NuStrICmpWC(pattern, special->name, wildcards != NULL ? *wildcards : NULL) == 0) {
                    if (dest != NULL) {
                        dest->scene = scene;
                        dest->special = special;
                        dest->display_special = NULL;
                        ++dest;
                    }
                    if (wildcards != NULL) {
                        ++wildcards;
                    }
                    if (++count >= capacity) {
                        return count;
                    }
                }
            }
        }
    }
    return count;
}

extern "C" i32 NuSpecialFindMulti(NUGSCN *scene, nuhspecial_s *dest, char *name, i32 capacity, i32 flags) {
    i32 count = 0;
    if (scene != NULL) {
        if (scene->display_list != NULL) {
            NUDISPLAYSPECIAL *special = static_cast<NUDISPLAYSPECIAL *>(scene->display_list->specials);
            for (i32 i = 0; i < scene->display_list->nspecials; ++special, ++i) {
                if (NuStrIStr(special->name, name) != NULL) {
                    ++count;
                    dest->scene = scene;
                    dest->display_special = special;
                    dest->special = NULL;
                    ++dest;
                    if (count >= capacity) {
                        return count;
                    }
                }
            }
        } else {
            NuSpecialLegacyLayout *special = reinterpret_cast<NuSpecialLegacyLayout *>(scene->specials);
            for (i32 i = 0; i < scene->numspecial; ++special, ++i) {
                if (__builtin_expect(NuStrIStr(special->name, name) != NULL, 0)) {
                    ++count;
                    dest->scene = scene;
                    dest->display_special = NULL;
                    dest->special = special;
                    ++dest;
                    if (count >= capacity) {
                        return count;
                    }
                }
            }
        }
    }
    return count;
}

extern "C" i32 NuSpecialTestAnim(nuhspecial_s *special) {
    if (special->display_special != NULL) {
        nuinstanim_s *animation = special->display_special->instance_animation;
        if (animation != NULL && animation != reinterpret_cast<nuinstanim_s *>(-1)) {
            return 1;
        }
    } else if (special->special != NULL) {
        NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
        if (static_cast<NuLegacyInstanceLayout *>(legacy->instance)->animation != NULL) {
            return 1;
        }
    }
    return 0;
}

extern "C" NUVEC *NuSpecialGetPos(void *special) {
    NuSpecialHandleLayout *handle = static_cast<NuSpecialHandleLayout *>(special);
    if (handle->display_special != NULL) {
        return reinterpret_cast<NUVEC *>(&static_cast<NUMTX *>(handle->display_special)->m30);
    }
    if (handle->special != NULL) {
        return reinterpret_cast<NUVEC *>(&static_cast<NUMTX *>(handle->special)->m30);
    }
    return NULL;
}

extern "C" NUVEC *NuSpecialGetDrawPos(void *special) {
    NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
    NuSpecialBoundsDisplayLayout *display = static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special);
    if (display != NULL) {
        usize instance_animation = reinterpret_cast<usize>(display->instance_animation);
        NUVEC *draw_position = NUMTX_GET_ROW_VEC(&display->draw_mtx, 3);
        NUVEC *animated_position = reinterpret_cast<NUVEC *>(instance_animation + offsetof(NUMTX, m30));
        return instance_animation != 0 && instance_animation != static_cast<usize>(-1) ? animated_position
                                                                                       : draw_position;
    }

    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
    if (legacy == NULL) {
        return NULL;
    }
    NUMTX *instance = reinterpret_cast<NUMTX *>(legacy->instance);
    NUMTX *draw_mtx = *reinterpret_cast<NUMTX **>(legacy->instance + 0x48);
    NUVEC *instance_position = NUMTX_GET_ROW_VEC(instance, 3);
    NUVEC *draw_position = reinterpret_cast<NUVEC *>(reinterpret_cast<usize>(draw_mtx) + offsetof(NUMTX, m30));
    return draw_mtx != NULL ? draw_position : instance_position;
}

extern "C" NUMTX *NuSpecialGetMtx(void *special) {
    NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
    if (handle->display_special != NULL) {
        return static_cast<NUMTX *>(handle->display_special);
    }
    return static_cast<NUMTX *>(handle->special);
}

extern "C" void NuSpecialSetMtx(nuhspecial_s *special, NUMTX *matrix) {
    if (special->display_special != NULL) {
        special->display_special->instance_mtx = *matrix;
    } else {
        *static_cast<NUMTX *>(special->special) = *matrix;
    }
}

extern "C" char *NuSpecialGetName(nuhspecial_s *special) {
    if (special == NULL) {
        return NULL;
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    if (display != NULL) {
        return display->name;
    }
    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    return legacy != NULL ? legacy->name : NULL;
}

extern "C" void NuSpecialVertexStates(NUSPECIALVERTEXSTATES *states) {
    nuspecial_vertex_states = states;
    ++render_state.state.global_id;
    ++render_state.state.vertex_groups_id;
}

extern "C" void NuSpecialVertexOffsets(i32 count, VARIPTR offsets) {
    nuspecial_vertex_offsets = offsets;
    nuspecial_vertex_noffsets = count;
}

extern "C" i32 NuSpecialExistsFn(void *special_ptr) {
    if (special_ptr != NULL) {
        nuhspecial_s *special = static_cast<nuhspecial_s *>(special_ptr);
        if (special->special != NULL) {
            return 1;
        }
        if (special->display_special != NULL) {
            return 1;
        }
    }
    return 0;
}

extern "C" void NuSpecialMtlMap(i32 count, NUMTL **materials) {
    if (count == 0) {
        nuspecial_draw_state &= ~NUSPECIAL_DRAW_MATERIAL_MAP;
        nurndr_forced_mtl_table = NULL;
        return;
    } else {
        nurndr_nforced_mtls = count;
        nuspecial_draw_state |= NUSPECIAL_DRAW_MATERIAL_MAP;
        nurndr_forced_mtl_table = materials;
        return;
    }
}

extern "C" void NuSpecialForceMtl(NUMTL *material) {
    if (__builtin_expect(material != NULL, 1)) {
        nuspecial_draw_state |= NUSPECIAL_DRAW_FORCE_MATERIAL;
    } else {
        nuspecial_draw_state &= ~NUSPECIAL_DRAW_FORCE_MATERIAL;
    }
    nurndr_forced_mtl = material;
}

extern "C" void NuSpecialMtl(NUMTL *material) {
    nurndr_forced_mtl = material;
}

extern "C" void NuSpecialConstAlpha(i32 enabled, f32 alpha) {
    if (enabled != 0) {
        nuspecial_const_alpha = alpha;
        nuspecial_draw_state |= 1;
    } else {
        nuspecial_draw_state &= ~1;
    }
    nuspecial_const_alpha_enabled = enabled;
}

extern "C" void NuSpecialConstTint(i32 enabled, NUVEC *tint) {
    if (enabled != 0) {
        memcpy(&nuspecial_const_tint, tint, sizeof(nuspecial_const_tint));
        nuspecial_draw_state |= 2;
    } else {
        nuspecial_draw_state &= ~2;
    }
    nuspecial_const_tint_enabled = enabled;
}

}

void NuSpecialReflection(i32 reflection) {
    nuspecial_reflection = reflection;
    RndrStateSetReflection(reflection);
}

extern "C" {
extern "C" void NuSpecialSetVisibility(void *special_ptr, i32 visible) {
    if (special_ptr == NULL) {
        return;
    }
    nuhspecial_s *special = static_cast<nuhspecial_s *>(special_ptr);
    NUGSCN *scene = special->scene;
    if (scene == NULL) {
        return;
    }

    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        if (instance != NULL) {
            instance->visible = visible;
        }
        if (visible != 0) {
            legacy->flags |= NULEGACYSPECIAL_FLAG_COLLISION;
        } else {
            legacy->flags &= ~NULEGACYSPECIAL_FLAG_COLLISION;
        }
        return;
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    if (display == NULL) {
        return;
    }

    if (visible != 0) {
        display->flags |= NUDISPLAYSPECIAL_FLAG_VISIBLE | NUDISPLAYSPECIAL_FLAG_COLLISION;
        NUDLDLISTSCENE *display_scene = reinterpret_cast<NUDLDLISTSCENE *>(scene->display_list);
        if ((display_scene->instance_visibility_enabled & NUDL_SCENE_INSTANCE_VISIBILITY_ENABLED) != 0) {
            display_scene->visibility_flags[display->instance_ix] |= NUDL_INSTANCE_FLAG_VISIBLE;
        }
        return;
    }

    display->flags &= ~(NUDISPLAYSPECIAL_FLAG_VISIBLE | NUDISPLAYSPECIAL_FLAG_COLLISION);
    NUDLDLISTSCENE *display_scene = reinterpret_cast<NUDLDLISTSCENE *>(scene->display_list);
    if ((display_scene->instance_visibility_enabled & NUDL_SCENE_INSTANCE_VISIBILITY_ENABLED) != 0) {
        display_scene->visibility_flags[display->instance_ix] &= ~NUDL_INSTANCE_FLAG_VISIBLE;
    }
}

extern "C" void NuSpecialSetCollision(nuhspecial_s *special, i32 enabled) {
    if (special == NULL || special->scene == NULL) {
        return;
    }
    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        if (enabled != 0) {
            legacy->flags |= NULEGACYSPECIAL_FLAG_COLLISION;
        } else {
            legacy->flags &= ~NULEGACYSPECIAL_FLAG_COLLISION;
        }
    } else if (special->display_special != NULL) {
        if (enabled != 0) {
            special->display_special->flags |= NUDISPLAYSPECIAL_FLAG_COLLISION;
        } else {
            special->display_special->flags &= ~NUDISPLAYSPECIAL_FLAG_COLLISION;
        }
    }
}

extern "C" i32 NuSpecialGetCollision(nuhspecial_s *special) {
    if (special == NULL || special->scene == NULL) {
        return 0;
    }
    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        return legacy->flags & NULEGACYSPECIAL_FLAG_COLLISION;
    }
    if (special->display_special != NULL) {
        return special->display_special->flags & NUDISPLAYSPECIAL_FLAG_COLLISION;
    }
    return 0;
}

extern "C" i32 NuSpecialGetVisibilityFn(void *special_ptr) {
    nuhspecial_s *special = static_cast<nuhspecial_s *>(special_ptr);
    if (special->scene == NULL) {
        return 0;
    }

    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        return instance->visible;
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    return display != NULL ? (display->flags >> 1) & 1 : 0;
}

extern "C" void NuSpecialSetNoVisiTest(nuhspecial_s *special, i32 enabled) {
    if (special->scene == NULL) {
        return;
    }

    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        instance->no_visibility_test = enabled;
        return;
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    if (display == NULL) {
        return;
    }
    if (enabled != 0) {
        display->flags |= NUDISPLAYSPECIAL_FLAG_NO_VISIBILITY_TEST;
    } else {
        display->flags &= ~NUDISPLAYSPECIAL_FLAG_NO_VISIBILITY_TEST;
    }
}

extern "C" i32 NuSpecialGetNoVisiTestFn(nuhspecial_s *special) {
    if (special->scene == NULL) {
        return 0;
    }

    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        return instance->no_visibility_test;
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    return display != NULL ? display->flags & NUDISPLAYSPECIAL_FLAG_NO_VISIBILITY_TEST : 0;
}

extern "C" void NuSpecialSetOnScreen(nuhspecial_s *special, i32 enabled) {
    if (special->scene == NULL)
        return;
    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        instance->on_screen = enabled;
        return;
    }
    NUDISPLAYSPECIAL *display = special->display_special;
    if (display == NULL)
        return;
    if (enabled != 0)
        display->flags |= NUDISPLAYSPECIAL_FLAG_ON_SCREEN;
    else
        display->flags &= ~NUDISPLAYSPECIAL_FLAG_ON_SCREEN;
}

extern "C" i32 NuSpecialGetOnScreenFn(nuhspecial_s *special) {
    if (special->scene != NULL && special->special != NULL) {
        NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        return instance->on_screen;
    }
    return 1;
}

extern "C" void NuSpecialSetRenderPlane(void) {
}

extern "C" i32 NuSpecialGetNumSpecials(NUGSCN *scene) {
    i32 count = scene->numspecial;
    if (count == 0 && scene->display_list != NULL) {
        count = scene->display_list->nspecials;
    }
    return count;
}

extern "C" i32 NuSpecialGetFirst(NUGSCN *scene, nuhspecial_s *special, i32 flags) {
    if (scene->specials != NULL) {
        special->scene = scene;
        special->special = scene->specials;
        special->display_special = NULL;
        return 1;
    }
    if (scene->display_list->nspecials != 0) {
        special->scene = scene;
        special->special = NULL;
        special->display_special = static_cast<NUDISPLAYSPECIAL *>(scene->display_list->specials);
        return 1;
    }
    special->scene = NULL;
    special->special = NULL;
    special->display_special = NULL;
    return 0;
}

extern "C" void NuSpecialGetNext(nuhspecial_s *special) {
    if (special->special != NULL) {
        special->special = static_cast<u8 *>(special->special) + 0x4c;
    } else if (special->display_special != NULL) {
        ++special->display_special;
    }
}

extern "C" void NuSpecialSetAlphaTest(void) {
}

extern "C" i32 NuSpecialGetInstanceix(nuhspecial_s *special) {
    NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
    if (legacy != NULL) {
        NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(handle->scene);
        for (i32 i = 0; i < scene->instance_count; ++i) {
            if (scene->instances + i * 0x50 == legacy->instance) {
                return i;
            }
        }
        return -1;
    }

    NuSpecialBoundsDisplayLayout *display = static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special);
    return display != NULL ? display->instance_ix : -1;
}

extern "C" i32 NuSpecialNumMtls(nuhspecial_s *special) {
    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(special->special);
    if (legacy != NULL) {
        NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(special->scene);
        NuSpecialLegacyInstanceBoundsLayout *instance =
            reinterpret_cast<NuSpecialLegacyInstanceBoundsLayout *>(legacy->instance);
        NuSpecialLegacyObjectBoundsLayout *object =
            static_cast<NuSpecialLegacyObjectBoundsLayout *>(scene->objects[instance->object_index]);
        while (object->next != NULL) {
            object = object->next;
        }
        i32 count = 0;
        for (NuSpecialLegacyMaterialLink *link = object->materials; link != NULL; link = link->next) {
            ++count;
        }
        return count;
    }
    if (special->display_special != NULL) {
        NUDISPLAYSPECIAL *display = special->display_special;
        i32 level = 0;
        while (display->clip_range[level] != 0.0f) {
            ++level;
        }
        return display->clip_objects[level].nmaterials;
    }
    return 0;
}

extern "C" NUMTL *NuSpecialGetMtl(nuhspecial_s *special, i32 index) {
    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(special->special);
    if (legacy != NULL) {
        NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(special->scene);
        NuSpecialLegacyInstanceBoundsLayout *instance =
            reinterpret_cast<NuSpecialLegacyInstanceBoundsLayout *>(legacy->instance);
        NuSpecialLegacyObjectBoundsLayout *object =
            static_cast<NuSpecialLegacyObjectBoundsLayout *>(scene->objects[instance->object_index]);
        while (object->next != NULL) {
            object = object->next;
        }
        NuSpecialLegacyMaterialLink *link = object->materials;
        while (index != 0) {
            if (link == NULL) {
                return NULL;
            }
            --index;
            link = link->next;
        }
        return link->material;
    } else if (special->display_special != NULL) {
        NUDISPLAYSPECIAL *display = special->display_special;
        i32 level = 0;
        while (display->clip_range[level] != 0.0f) {
            ++level;
        }
        return special->scene->display_list->mtls[display->clip_objects[level].material_ids[index]];
    }
    return NULL;
}

extern "C" void NuSpecialGetBounds(void *special, NUVEC *minimum, NUVEC *maximum) {
    NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
    if (handle->special == NULL) {
        NuSpecialBoundsDisplayLayout *display = static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special);
        if (display != NULL) {
            *minimum = display->min;
            *maximum = display->max;
        }
        return;
    }

    NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(handle->scene);
    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
    NuSpecialLegacyInstanceBoundsLayout *instance =
        reinterpret_cast<NuSpecialLegacyInstanceBoundsLayout *>(legacy->instance);
    NuSpecialLegacyObjectBoundsLayout *object =
        static_cast<NuSpecialLegacyObjectBoundsLayout *>(scene->objects[instance->object_index]);
    while (object->next != NULL) {
        object = object->next;
    }
    *minimum = object->minimum;
    *maximum = object->maximum;
}

extern "C" void NuDynamicLightTestShadowExtrusionsSpecial(NuDynamicLight *light, void *special, NUMTX *matrix) {
    VuVec minimum;
    VuVec maximum;
    NuSpecialGetBounds(special, &minimum.xyz, &maximum.xyz);
    NuVecMtxTransform(&minimum.xyz, &minimum.xyz, matrix);
    NuVecMtxTransform(&maximum.xyz, &maximum.xyz, matrix);
    light->testShadowExtrusions(minimum, maximum);
}

extern "C" void NuSpecialSetBounds(nuhspecial_s *special, NUVEC *minimum, NUVEC *maximum) {
    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(special->special);
    if (legacy != NULL) {
        NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(special->scene);
        NuSpecialLegacyInstanceBoundsLayout *instance =
            reinterpret_cast<NuSpecialLegacyInstanceBoundsLayout *>(legacy->instance);
        NuSpecialLegacyObjectBoundsLayout *object =
            static_cast<NuSpecialLegacyObjectBoundsLayout *>(scene->objects[instance->object_index]);
        while (object != NULL) {
            object->minimum = *minimum;
            object->maximum = *maximum;
            object = object->next;
        }
    } else {
        NuSpecialBoundsDisplayLayout *display =
            reinterpret_cast<NuSpecialBoundsDisplayLayout *>(special->display_special);
        display->min = *minimum;
        display->max = *maximum;
    }
}

extern "C" void NuSpecialGetRadius(void *special, NUVEC *position, f32 *radius) {
    NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
    if (handle->special != NULL) {
        NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(handle->scene);
        NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
        NuSpecialLegacyInstanceBoundsLayout *instance =
            reinterpret_cast<NuSpecialLegacyInstanceBoundsLayout *>(legacy->instance);
        NuSpecialLegacyObjectBoundsLayout *object =
            static_cast<NuSpecialLegacyObjectBoundsLayout *>(scene->objects[instance->object_index]);
        *radius = object->radius;
        *position = object->center;
    } else {
        NuSpecialBoundsDisplayLayout *display = static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special);
        *position = display->center;
        *radius = display->radius;
    }
}

extern "C" f32 NuSpecialGetOriginRadius(void *special) {
    NuSpecialHandleLayout *handle = static_cast<NuSpecialHandleLayout *>(special);
    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
    if (legacy != NULL) {
        NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(handle->scene);
        NuSpecialLegacyInstanceBoundsLayout *instance =
            reinterpret_cast<NuSpecialLegacyInstanceBoundsLayout *>(legacy->instance);
        NuSpecialLegacyObjectBoundsLayout *object =
            static_cast<NuSpecialLegacyObjectBoundsLayout *>(scene->objects[instance->object_index]);
        return object->origin_radius;
    }
    return NuVecMag(&static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special)->center) +
           static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special)->radius;
}

extern "C" NUMTX *NuSpecialGetDrawMtx(void *special) {
    NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
    if (legacy != NULL) {
        NUMTX *instance = reinterpret_cast<NUMTX *>(legacy->instance);
        NUMTX *draw_mtx = *reinterpret_cast<NUMTX **>(legacy->instance + 0x48);
        return draw_mtx != NULL ? draw_mtx : instance;
    }
    NuSpecialBoundsDisplayLayout *display = static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special);
    if (display != NULL) {
        usize draw_mtx = reinterpret_cast<usize>(display->instance_animation);
        if (draw_mtx != 0 && draw_mtx != static_cast<usize>(-1)) {
            return reinterpret_cast<NUMTX *>(display->instance_animation);
        }
        return &display->draw_mtx;
    }
    return NULL;
}

extern "C" NUMTX *NuSpecialGetInstanceMtx(nuhspecial_s *special) {
    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        return static_cast<NUMTX *>(legacy->instance);
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    if (display != NULL) {
        return &display->draw_mtx;
    }
    return NULL;
}

extern "C" void NuSpecialSetInstanceMtx(nuhspecial_s *special, NUMTX *matrix) {
    if (special != NULL && special->scene != NULL && special->special != NULL) {
        NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
        *static_cast<NUMTX *>(legacy->instance) = *matrix;
    }
}

extern "C" i32 NuSpecialClipTestExtents(void *special, void *matrix_arg) {
    NUMTX *matrix = static_cast<NUMTX *>(matrix_arg);
    NuSpecialHandleLayout *handle = static_cast<NuSpecialHandleLayout *>(special);
    if (handle->special != NULL) {
        NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(handle->scene);
        NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
        NuSpecialLegacyInstanceBoundsLayout *instance =
            reinterpret_cast<NuSpecialLegacyInstanceBoundsLayout *>(legacy->instance);
        NuSpecialLegacyObjectBoundsLayout *object =
            static_cast<NuSpecialLegacyObjectBoundsLayout *>(scene->objects[instance->object_index]);
        return NuCameraClipTestExtents(&object->minimum, &object->maximum, matrix, 0.0f, 0);
    }
    NuSpecialBoundsDisplayLayout *display = static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special);
    return NuCameraClipTestExtents(&display->min, &display->max, matrix, 0.0f, 0);
}

extern "C" nuinstanim_s *NuSpecialGetInstAnim(nuhspecial_s *special) {
    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        return instance->animation;
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    if (display != NULL && display->instance_animation != reinterpret_cast<nuinstanim_s *>(-1)) {
        return display->instance_animation;
    }
    return NULL;
}

extern "C" f32 NuSpecialGetAnimEndFrame(nuhspecial_s *special) {
    if (special == NULL) {
        return 0.0f;
    }
    NUGSCN *scene = special->scene;
    if (scene == NULL) {
        return 0.0f;
    }

    nuinstanim_s *instance_animation = NuSpecialGetInstAnim(special);
    if (instance_animation == NULL) {
        return 0.0f;
    }

    nuanimdata_s *animation = scene->instance_animation_data[instance_animation->anim_ix];
    if (animation != NULL) {
        return NuAnimEndFrameOld(animation);
    }

    if ((instance_animation->end_frame_lookup_bits & NUINSTANIM_END_FRAME_LOOKUP_MASK) == 0 ||
        scene->animation_end_frames == NULL) {
        return 0.0f;
    }

    const u16 lookup_index = instance_animation->end_frame_lookup_index;
    const u32 end_frame = scene->animation_end_frames[lookup_index - 1].end_frame;
    return static_cast<f32>(end_frame);
}

extern "C" i32 NuSpecialCompare(nuhspecial_s *first, nuhspecial_s *second) {
    if (first->special != NULL && first->special == second->special) {
        return 1;
    }
    if (first->display_special != NULL && first->display_special == second->display_special) {
        return 1;
    }
    return 0;
}

extern "C" void NuSpecialClear(void *special) {
    NuSpecialHandleLayout *handle = static_cast<NuSpecialHandleLayout *>(special);
    handle->scene = NULL;
    handle->special = NULL;
    handle->display_special = NULL;
}

extern "C" NUSPECIALVERTEXSTATES *NuVertexStatesCreate(VARIPTR *buffer, i32 count) {
    buffer->addr = ALIGN(buffer->addr, 4);
    NUSPECIALVERTEXSTATES *states = (NUSPECIALVERTEXSTATES *)buffer->void_ptr;
    buffer->u8_ptr += sizeof(NUSPECIALVERTEXSTATES);
    states->count = count;
    states->flags = 0;
    i32 blocks = count / 16;
    if (count & 15)
        ++blocks;
    states->block_count = blocks;
    i32 size = blocks * 16;
    states->values = (i8 *)ALIGN(buffer->addr, 16);
    buffer->addr = ALIGN(buffer->addr, 16) + size;
    for (i32 i = 0; i < size; ++i)
        states->values[i] = 0;
    return states;
}

extern "C" void NuVertexStatesSetGroupState(NUSPECIALVERTEXSTATES *states, i32 group, i32 value) {
    states->values[group] = value;
}

extern "C" i32 NuSpecialSetClipping(i32 enabled, i32 state) {
    i32 previous = nuspecial_clip_state;
    nuspecial_clip_state = enabled != 0 ? state : -1;
    return previous;
}

extern "C" void NuSpecialAddShadowLight(void) {
}

extern "C" void NuSpecialClearShadowLights(void) {
}

extern "C" i32 NuSpecialGetActiveShadowLights(void) {
    return nuspecial_shadowLightCount;
}

extern "C" i32 NuSpecialHasActiveShadowLights(void) {
    return nuspecial_shadowLightCount > 0;
}

extern "C" void *NuSpecialGetShadowLight(i32 index) {
    return nuspecial_shadowLight[index];
}

extern "C" i32 NuSpecialClipTestShadowLights(NUVEC *, NUVEC *, i32) {
    // The original body is an intentional no-op that returns zero.
    return 0;
}

extern "C" i32 NuSpecialHaveShadowClipTestResults(void) {
    return nuspecial_shadowLightHaveClipOverrides;
}

extern "C" i32 NuSpecialGetShadowClipTestResult(i32 index) {
    if (nuspecial_shadowLightHaveClipOverrides != 0) {
        return nuspecial_shadowLightClipOverride[index];
    }
    return -1;
}

extern "C" void NuSpecialClearShadowClipTestResults(void) {
    nuspecial_shadowLightHaveClipOverrides = 0;
}

    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

    void NuSplineList(void) {
    }

}

// Finds the spline named `name` in `scene`, or NULL if there is no such
// spline.
NUGSPLINE *NuSplineFind(NUGSCN *scene, char *name) {
    if (scene == NULL) {
        return NULL;
    }

    NUGSPLINE *spline = scene->splines;

    if (scene->numsplines <= 0) {
        return NULL;
    }

    for (i32 i = 0; i < scene->numsplines; i++, spline++) {
        if (NuStrICmp(name, spline->name) == 0) {
            return spline;
        }
    }

    return NULL;
}

i32 NuSplineFindAllSub(NUGSCN *scene, char *name, NUGSPLINE **results, i32 capacity) {
    if (capacity <= 0 || scene == NULL)
        return 0;
    i32 count = 0;
    NUGSPLINE *spline = scene->splines;
    if (scene->numsplines > 0) {
        for (i32 i = 0; i < scene->numsplines; spline++, i++) {
            if (NuStrIStr(spline->name, name) != NULL) {
                results[count++] = spline;
                if (count >= capacity)
                    break;
            }
        }
    }
    return count;
}

// The -1 length uses NuStrNICmp's prefix-comparison convention.
i32 NuSplineFindAllBeg(NUGSCN *scene, char *name, NUGSPLINE **results, i32 capacity) {
    if (capacity <= 0 || scene == NULL)
        return 0;
    i32 count = 0;
    NUGSPLINE *spline = scene->splines;
    if (scene->numsplines > 0) {
        for (i32 i = 0; i < scene->numsplines; spline++, i++) {
            if (NuStrNICmp(name, spline->name, -1) == 0) {
                results[count++] = spline;
                if (count >= capacity)
                    break;
            }
        }
    }
    return count;
}

NUGSPLINE *NuSplineFindNextBeg(NUGSCN *scene, char *name, NUGSPLINE *previous) {
    if (scene == NULL)
        return NULL;
    NUGSPLINE *spline = previous;
    if (spline == NULL)
        spline = scene->splines;
    else
        spline++;
    NUGSPLINE *end = scene->splines + scene->numsplines;
    for (; spline < end; spline++) {
        if (NuStrNICmp(name, spline->name, -1) == 0)
            return spline;
    }
    return NULL;
}

void NuSplineGetRandomPoint(NUGSPLINE *spline, NUVEC *point) {
    i32 index = lrand48() % spline->length;
    i32 next_index = (index + 1) % spline->length;
    NUVEC *start = &spline->pts[index * (i16)spline->pt_size];
    NUVEC *end = &spline->pts[next_index * (i16)spline->pt_size];
    f32 fraction = (lrand48() & 0x7fff) * (1.0f / 32768.0f);
    NuVecLerp(point, start, end, fraction);
}

extern "C" {
    void NuQFntSetPrintMode(u32 mode) {
        NuQFntMode = mode;
    }

}

void NuQFntPushPrintMode(u32 mode) {
    if (NuQFntModeStackIndex < 16)
        NuQFntModeStack[NuQFntModeStackIndex++] = NuQFntMode;
    NuQFntMode = mode;
}

void NuQFntPopPrintMode(void) {
    if (NuQFntModeStackIndex > 0)
        NuQFntMode = NuQFntModeStack[--NuQFntModeStackIndex];
}

extern "C" {
    u32 NuQFntGetPrintMode(void) {
        return NuQFntMode;
    }

}

void NuQFntSetICGap(NUQFNT *font, f32 ic_gap) {
    VUFNT *vufnt;

    if (font == NULL) {
        font = system_qfont;
    }

    if (font != NULL) {
        vufnt = (VUFNT *)font;
        vufnt->ic_gap = ic_gap;
    }
}

void NuQFntSetSpaceWidth(NUQFNT *, f32 width) {
    nuqfnt_space_width = width;
}

extern "C" {
    f32 NuQFntLenScale(void) {
        return qfnt_len_scale;
    }

    f32 NuQFntHeightScale(void) {
        return qfnt_height_scale;
    }

}

NUQFNT_CSMODE NuQFntSetCoordinateSystem(NUQFNT_CSMODE mode) {
    NUQFNT_CSMODE prev;

    prev = NuQFntCSMode;

    NuQFntCSMode = mode;

    switch (mode) {
        case NUQFNT_CSMODE_PIXEL:
            qfnt_rezscale_w = 1.0f;
            qfnt_rezscale_h = 0.5f;

            qfnt_offscale_x = 1.0f;
            qfnt_offscale_y = 1.0f;

            qfnt_len_scale = 1.0f;
            qfnt_height_scale = 1.0f;
            break;
        case NUQFNT_CSMODE_NORMALISED:
            qfnt_rezscale_w = 320.0f;
            qfnt_rezscale_h = -112.0f;

            qfnt_offscale_x = 320.0f;
            qfnt_offscale_y = 112.0f;

            // Multiplicative inverses of rezscale.
            qfnt_len_scale = 0.003125f;
            qfnt_height_scale = -0.008928572f;
            break;
        case NUQFNT_CSMODE_ABSOLUTE:
            qfnt_rezscale_w = 1.0f;
            qfnt_rezscale_h = 1.0f;

            qfnt_offscale_x = 0.0f;
            qfnt_offscale_y = 0.0f;

            qfnt_len_scale = 1.0f;
            qfnt_height_scale = -1.0f;
            break;
        default:
            qfnt_rezscale_w = 0.0625f;
            qfnt_rezscale_h = 0.0625f;

            qfnt_offscale_x = 0.0f;
            qfnt_offscale_y = 0.0f;

            qfnt_len_scale = 16.0f;
            qfnt_height_scale = 16.0f;
            break;
    }

    return prev;
}

extern "C" {
    void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode) {
        if (NuQFntCSModeStackIndex < 16) {
            NuQFntCSModeStack[NuQFntCSModeStackIndex] = NuQFntCSMode;
            ++NuQFntCSModeStackIndex;
        }
        NuQFntSetCoordinateSystem(mode);
    }

    void NuQFntPopCoordinateSystem(void) {
        if (NuQFntCSModeStackIndex > 0) {
            --NuQFntCSModeStackIndex;
            NuQFntSetCoordinateSystem(NuQFntCSModeStack[NuQFntCSModeStackIndex]);
        }
    }

    NUQFNT_CSMODE NuQFntGetCoordinateSystem(void) {
        return NuQFntCSMode;
    }

}

__attribute__((optimize("O2"))) i32 UnicodeToIndexFast(vucharidx_s *map, i32 count, u16 unicode) {
    i32 index = -1;

    if (map[count - 1].unicode >= unicode) {
        i32 low = 0;
        i32 high = count - 1;
        i32 middle = count / 2;
        while (map[middle].unicode != unicode) {
            if (low + 1 == high) {
                if (map[high].unicode == unicode)
                    return map[high].index;
                if (map[low].unicode == unicode)
                    return map[low].index;
                return -1;
            }
            if (unicode < map[middle].unicode) {
                high = middle;
                middle = (low + middle) >> 1;
            } else if (unicode > map[middle].unicode) {
                low = middle;
                middle = (high + middle) >> 1;
            } else if (high == low) {
                return -1;
            }
        }
        index = map[middle].index;
    }
    return index;
}

u16 NuQFntEncodeUnicodeChar(NUQFNT *font, u16 character) {
    if (font == NULL)
        font = system_qfont;
    if (font == NULL)
        return 0xffff;

    VUFNT *vufnt = reinterpret_cast<VUFNT *>(font);
    return UnicodeToIndexFast(vufnt->unicode_map, vufnt->unicode_count, character);
}

extern "C" {
    void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text) {
        if (font == NULL) {
            font = system_qfont;
        }
        if (font == NULL) {
            return;
        }
        VUFNT *vufnt = static_cast<VUFNT *>(font);
        while (*text != 0) {
            i32 index = UnicodeToIndexFast(vufnt->unicode_map, vufnt->unicode_count, *text);
            *text = static_cast<u16>(static_cast<u16>(index) == 0xffff ? '?' : index);
            ++text;
        }
    }

}

void NuQFntUTF8toQCode(NUQFNT *font, char *text, u16 *encoded) {
    if (font == NULL)
        font = system_qfont;
    if (font == NULL)
        return;

    NUWCHAR8 *cursor = reinterpret_cast<NUWCHAR8 *>(text);
    while (*cursor != '\0') {
        NUWCHAR16 character __attribute__((aligned(16)));
        cursor = NuUnicodeCharFromUTF8(&character, cursor);
        *encoded = NuQFntEncodeUnicodeChar(font, character);
        if (*encoded == 0xffff)
            *encoded = NuQFntEncodeUnicodeChar(font, '?');
        encoded++;
    }
    *encoded = 0;
}

f32 NuQFntPrintLenW(NUQFNT *font, u16 *text) {
    if (font == NULL)
        font = system_qfont;
    if (font == NULL)
        return 0.0f;

    VUFNT *vufnt = static_cast<VUFNT *>(font);
    f32 x_scale = *vufnt->x_scale;
    f32 length = 0.0f;
    u16 character;
    while ((character = *text++) != 0) {
        f32 width;
        switch (character) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if ((NuQFntMode & 1) != 0) {
                    u16 zero = NuQFntEncodeUnicodeChar(font, '0');
                    width = vufnt->glyphs[zero].width;
                    break;
                }
            default:
                width = vufnt->glyphs[character].width;
                break;
            case 0xffff:
                width = 0.0f;
                break;
            case ' ':
                width = nuqfnt_space_width == 0.0f ? vufnt->glyphs[' '].width : nuqfnt_space_width;
                break;
        }
        length += width + vufnt->ic_gap;
    }
    return length * qfnt_len_scale * x_scale;
}

f32 NuQFntPrintLenU(NUQFNT *font, char *text) {
    if (font == NULL)
        font = system_qfont;
    if (font == NULL)
        return 0.0f;

    VUFNT *vufnt = static_cast<VUFNT *>(font);
    const f32 x_scale = *vufnt->x_scale;
    f32 length = 0.0f;
    NUWCHAR8 *cursor = reinterpret_cast<NUWCHAR8 *>(text);
    while (*cursor != '\0') {
        NUWCHAR16 character __attribute__((aligned(16)));
        cursor = NuUnicodeCharFromUTF8(&character, cursor);
        u16 qcode = NuQFntEncodeUnicodeChar(font, character);
        if (qcode == 0xffff)
            qcode = NuQFntEncodeUnicodeChar(font, '?');

        f32 width;
        if (character == ' ' && nuqfnt_space_width != 0.0f) {
            width = nuqfnt_space_width;
        } else {
            if (character >= '0' && character <= '9' && (NuQFntMode & 1) != 0)
                qcode = NuQFntEncodeUnicodeChar(font, '0');
            width = vufnt->glyphs[qcode].width;
        }
        length += width + vufnt->ic_gap;
    }
    return length * qfnt_len_scale * x_scale;
}

f32 NuQFntHeight(NUQFNT *font) {
    if (font == NULL)
        font = system_qfont;
    if (font == NULL)
        return 0.0f;
    VUFNT *vufnt = static_cast<VUFNT *>(font);
    return *vufnt->y_scale * vufnt->height * qfnt_height_scale;
}

f32 NuQFntBaseline(NUQFNT *font) {
    VUFNT *vufnt;

    if (font == NULL) {
        font = system_qfont;
    }

    if (font != NULL) {
        vufnt = (VUFNT *)font;
        return *vufnt->y_scale * vufnt->baseline * qfnt_height_scale;
    }

    return 0.0f;
}

void NuQFntSetJustifiedTolerances(f32 squash, f32 stretch) {
    justify_squash = MAX(1.0f, squash);
    justify_stretch = MAX(1.0f, stretch);
}

void NuQFntSetRS(RNDRSTREAM *, NUQFNT *font) {
    static_cast<VUFNT *>(font)->mode = 1;
}

void NuQFntSetMtxRS(RNDRSTREAM *stream, NUQFNT *font, NUMTX *mtx) {
    VUFNT *vufnt;
    VUFNT_ANDROID *platform_font;

    vufnt = (VUFNT *)font;
    platform_font = (VUFNT_ANDROID *)vufnt->platform_data;

    platform_font->mtx = *mtx;
}

void NuQFntSetScaleRS(RNDRSTREAM *, NUQFNT *font, f32 x_scale, f32 y_scale) {
    VUFNT *vufnt = static_cast<VUFNT *>(font);
    f32 *y_scale_ptr = vufnt->y_scale;
    f32 *x_scale_ptr = vufnt->x_scale;
    *x_scale_ptr = x_scale;
    if ((NuQFntMode & 4) == 0)
        y_scale *= 0.5f;
    *y_scale_ptr = y_scale;
}

void NuQFntSetColourRS(RNDRSTREAM *, NUQFNT *font, u32 colour) {
    static_cast<VUFNT *>(font)->platform_data->colour = colour;
}

void NuQFntSet(NUQFNT *font) {
    if (font == NULL)
        font = system_qfont;
    if (font != NULL) {
        NuQFntSetRS(NULL, font);
        NuQFntSetSpaceWidth(font, reinterpret_cast<VUFNT *>(font)->space_width);
    }
}

void NuQFntSetScale(NUQFNT *font, f32 x_scale, f32 y_scale) {
    if (font == NULL)
        font = system_qfont;
    if (font != NULL)
        NuQFntSetScaleRS(NULL, font, x_scale, y_scale);
}

void NuQFntSetMtx(NUQFNT *font, NUMTX *mtx) {
    if (font == NULL) {
        font = system_qfont;
    }

    if (font != NULL) {
        NuQFntSetMtxRS(NULL, font, mtx);
    }
}

void NuQFntSetColour(NUQFNT *font, u32 colour) {
    if (font == NULL)
        font = system_qfont;
    if (font != NULL)
        NuQFntSetColourRS(NULL, font, colour);
}

extern "C" {
    f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments) {
        char text[1024];
        vsprintf(text, format, arguments);
        return NuQFntPrintLenU(font, text);
    }

}

void NuQFntMoveRS(RNDRSTREAM *, NUQFNT *font, f32 x, f32 y, f32 z) {
    VUFNT *vufnt = static_cast<VUFNT *>(font);
    if (NuQFntCSMode != NUQFNT_CSMODE_ABSOLUTE) {
        x = x * qfnt_rezscale_w + qfnt_offscale_x;
        y = y * qfnt_rezscale_h + qfnt_offscale_y - vufnt->baseline * *vufnt->y_scale;
    }
    vufnt->platform_data->x = x;
    vufnt->platform_data->y = y;
    vufnt->platform_data->z = z;
}

void NuQFntMove(NUQFNT *font, f32 x, f32 y, f32 z) {
    if (font == NULL)
        font = system_qfont;
    if (font != NULL)
        NuQFntMoveRS(NULL, font, x, y, z);
}

extern "C" {
    void RemapAddr(void *new_base, void *old_base, void **address) {
        *address = static_cast<u8 *>(new_base) + (static_cast<u8 *>(*address) - static_cast<u8 *>(old_base));
    }

    void NuQFntWrite(char *path, VUFNT *font) {
        char text[256];
        i32 i;
        sprintf(text, "%s.qfn", path);
        NUFILE file = NuFileOpen(text, NUFILE_WRITE);
        if (file != 0) {
            RemapAddr(NULL, font, reinterpret_cast<void **>(&font->glyphs));
            RemapAddr(NULL, font, reinterpret_cast<void **>(&font->unicode_map));
            NuFileWrite(file, font, font->size);
            RemapAddr(font, NULL, reinterpret_cast<void **>(&font->glyphs));
            RemapAddr(font, NULL, reinterpret_cast<void **>(&font->unicode_map));
            NuFileClose(file);
        }

        sprintf(text, "%s.htm", path);
        file = NuFileOpen(text, NUFILE_WRITE);
        if (file != 0) {
            sprintf(text, "<HTML><BODY><font face = courier><H2>Font name: %s</H2>\n", path);
            NuFileWrite(file, text, strlen(text));
            sprintf(text, "<P>Internal char list: (%d entries)\n<P>\n", font->glyph_count);
            NuFileWrite(file, text, strlen(text));
            for (i = 0; i < font->glyph_count; i++) {
                sprintf(text, "%.3d ('%c'): u=%.3d v=%.3d w=%.3d<BR>\n", i, i < 32 ? 1 : i & 255,
                        static_cast<i32>(font->glyphs[i].x), static_cast<i32>(font->glyphs[i].y),
                        static_cast<i32>(font->glyphs[i].width));
                NuFileWrite(file, text, strlen(text));
            }
            sprintf(text, "<P>Lookup table:\n<P>\n");
            NuFileWrite(file, text, strlen(text));
            for (i = 0; i < font->unicode_count; i++) {
                sprintf(text, "%.4d ('%c'): char=%.3d<BR>\n", font->unicode_map[i].unicode,
                        font->unicode_map[i].unicode & 255, font->unicode_map[i].index);
                NuFileWrite(file, text, strlen(text));
            }
            sprintf(text, "<P></BODY></HTML>");
            NuFileWrite(file, text, strlen(text));
            NuFileClose(file);
        }
    }

    void NuQFntWriteUniversalFont(char *path, VUFNT *font, char *texture_path) {
        char filename[256];
        char buffer[1024];
        NUFILE file;
        NUFILE texture_file;
        i32 remaining;
        i32 size;
        sprintf(filename, "%s.ufn", path);
        file = NuFileOpen(filename, NUFILE_WRITE);
        texture_file = NuFileOpen(texture_path, NUFILE_READ);
        if (file != 0 && texture_file != 0) {
            RemapAddr(NULL, font, reinterpret_cast<void **>(&font->glyphs));
            RemapAddr(NULL, font, reinterpret_cast<void **>(&font->unicode_map));
            NuFileWrite(file, font, font->size);
            RemapAddr(font, NULL, reinterpret_cast<void **>(&font->glyphs));
            RemapAddr(font, NULL, reinterpret_cast<void **>(&font->unicode_map));
            remaining = NuFileOpenSize(texture_file);
            while (remaining != 0) {
                size = NuFileRead(texture_file, buffer, remaining > 1024 ? 1024 : remaining);
                NuFileWrite(file, buffer, size);
                remaining -= size;
            }
            NuFileClose(texture_file);
            NuFileClose(file);
        }
    }

    void NuQFntSet2d(NUQFNT *font) {
        NuQFntPushPrintMode(2);
        NuQFntSet(font);
        NuQFntPopPrintMode();
    }

    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale) {
        NuQFntPushPrintMode(2);
        NuQFntSetScale(font, x_scale, y_scale);
        NuQFntPopPrintMode();
    }

    void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height) {
        if (font == NULL) {
            font = system_qfont;
        }
        if (font == NULL) {
            return;
        }
        VUFNT *vufnt = static_cast<VUFNT *>(font);
        u16 glyph_index = NuQFntEncodeUnicodeChar(font, 'M');
        f32 x_scale;
        f32 y_scale;
        if (glyph_index == 0xffff) {
            y_scale = height * 18.0f / vufnt->height;
            x_scale = y_scale / height;
        } else {
            x_scale = width * 7.0f / vufnt->glyphs[glyph_index].width;
            y_scale = height * 18.0f / vufnt->height;
        }
        NuQFntSetScale(font, x_scale, y_scale);
        NuQFntSetScale2d(font, x_scale, y_scale);
    }

    void NuQFntSetColour2d(NUQFNT *font, u32 colour) {
        NuQFntPushPrintMode(2);
        NuQFntSetColour(font, colour);
        NuQFntPopPrintMode();
    }

}

void NuQFntSetMtx2d(void *font, numtx_s *matrix) {
    NuQFntPushPrintMode(2);
    NuQFntSetMtx(font, matrix);
    NuQFntPopPrintMode();
}

extern "C" {
    void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z) {
        NuQFntPushPrintMode(2);
        NuQFntMove(font, x, y, z);
        NuQFntPopPrintMode();
    }

    void NuFntInit(void) {
    }

    void NuFntSetFixedWidthNumerals(void) {
    }

    i32 NuFntToUpper(void) {
        return 0x20;
    }

    i32 NuFntToLower(void) {
        return 0x20;
    }

    void NuFntSetPen(i32) {
    }

}

void NuFntSave(nufnt_s *, i32, char *) {
}

void NuFntDumpReadable(nufnt_s *, char *) {
}

i32 CheckCol(nutex_s *, i32, i32, i32, i32) {
    return true;
}

i32 NuFntFindStart(nutex_s *, i32 *, i32 *, i32, i32) {
    return 1;
}

i32 NuFntFindEnd(nutex_s *, i32 *, i32 *, i32, i32) {
    return 1;
}

extern "C" {
    void NuFntSet(i32) {
    }

    void NuFntScale(i32, i32) {
    }

    i32 NuFntGetScreenHeight(void) {
        return 1;
    }

    void NuFntPointSize(void) {
    }

    void NuFntMoveAbs(void) {
    }

    void NuFntMoveRel(void) {
    }

    void NuFntPos(void) {
    }

    i32 NuFntPrintLenV(void) {
        return 1;
    }

    i32 NuFntPrintLen(void) {
        return 1;
    }

}

i32 NuFntPrintChar(char) {
    return 1;
}

extern "C" {
    i32 NuFntPrintV(void) {
        return 1;
    }

    i32 NuFntPrint(void) {
        return 1;
    }

    void NuFntClose(void) {
    }

    void NuFntPrintEx(void) {
    }

    void *NuFntCreate(void) {
        return NULL;
    }

    void NuFntDestroy(void) {
    }

    void NuFntWrite(void) {
    }

    void *NuFntLoadPtr(void) {
        return NULL;
    }

extern "C" void NuTexAnimProgSysInit(void) {
    sys_progs = NULL;
    for (i32 i = 0; i < 63; ++i)
        ntalsysbuff[i].next = &ntalsysbuff[i + 1];
    ntalsysbuff[63].next = NULL;
    xdeflabtabcnt = 0;
    nta_sig_old = 0;
    nta_sig_off = 0;
    nta_sig_on = 0;
    ntal_free = ntalsysbuff;
    ntal_first = NULL;
    texanim_rand.value = 0;
}

extern "C" void NuTexAnimProgRelease(void) {
    sys_progs = NULL;
}

extern "C" nutexanimprog_s *NuTexAnimProgFind(char *name) {
    for (nutexanimprog_s *program = sys_progs; program != NULL; program = program->next) {
        if (NuStrICmp(name, program->name) == 0)
            return program;
    }
    return NULL;
}

extern "C" void NuTexAnimProgDestroy(nutexanimprog_s *program) {
    if (program->previous != NULL)
        program->previous->next = program->next;
    else
        sys_progs = program->next;
    if (program->next != NULL)
        program->next->previous = program->previous;
    if (program->flags & 1)
        NU_FREE(program);
}

}

void NuTexAnimProgInit(nutexanimprog_s *program) {
    if (program == NULL)
        return;
    for (i32 i = 0; i < 32; ++i) {
        program->off_signal[i] = -1;
        program->on_signal[i] = -1;
    }
    program->instruction_count = 0;
    program->on_mask = 0;
    program->off_mask = 0;
    program->name[0] = '\0';
    program->label_count = 0;
    program->flags &= ~1;
    program->mask = 1;
}

extern "C" {
extern "C" nutexanimprog_s *NuTexAnimProgCreate(VARIPTR *buffer, i32 instruction_count, char *name) {
    nutexanimprog_s *program;
    if (buffer != NULL) {
        program = reinterpret_cast<nutexanimprog_s *>(buffer->addr);
        buffer->addr += sizeof(nutexanimprog_s) + instruction_count;
    } else {
        program = static_cast<nutexanimprog_s *>(NU_ALLOC(sizeof(nutexanimprog_s) + instruction_count, 4, 1, "", 0));
    }
    if (program != NULL) {
        NuTexAnimProgInit(program);
        if (buffer == NULL)
            program->flags |= 1;
        if (name != NULL)
            NuStrCpy(program->name, name);
    }
    return program;
}

extern "C" void NuTexAnimProgWrite(char *path, nutexanimprog_s *program) {
    NUFILE file = NuFileOpen(path, NUFILE_WRITE);
    if (file != 0) {
        NuFileWrite(file, program, sizeof(nutexanimprog_s) + program->instruction_count * sizeof(i16));
        NuFileClose(file);
    }
}

extern "C" nutexanimprog_s *NuTexAnimProgRead(VARIPTR *buffer, char *path) {
    NUFILE file = NuFileOpen(path, NUFILE_READ);
    nutexanimprog_s *program = NULL;
    if (file != 0) {
        i32 size = static_cast<i32>(NuFileOpenSize(file));
        if (buffer != NULL) {
            program = reinterpret_cast<nutexanimprog_s *>(ALIGN(buffer->addr, 4));
            buffer->addr = reinterpret_cast<usize>(program) + size;
        } else {
            program = static_cast<nutexanimprog_s *>(NU_ALLOC(size, 4, 1, "", 0));
        }
        NuFileRead(file, program, size);
        NuFileClose(file);
        program->next = sys_progs;
        if (sys_progs != NULL)
            sys_progs->previous = program;
        program->previous = NULL;
        sys_progs = program;
        if (buffer != NULL)
            program->flags &= ~1;
        else
            program->flags |= 1;
    }
    return program;
}

extern "C" void NuTexAnimProgAssembleEnd(nutexanimprog_s *program) {
    i16 index = 0;
    while (index < program->instruction_count) {
        switch (program->instructions[index]) {
            case 0:
            case 1:
            case 17:
                index += 2;
                break;
            case 3:
                index += 5;
                break;
            case 4:
            case 7:
            case 13:
            case 15:
                index += 3;
                break;
            case 9:
                program->instructions[index + 1] = nta_labels[program->instructions[index + 1]];
                index += 2;
                break;
            case 10:
                program->instructions[index + 1] = nta_labels[program->instructions[index + 1]];
                index += 2;
                break;
            case 11:
                program->instructions[index + 3] = nta_labels[program->instructions[index + 3]];
                index += 4;
                break;
            case 2:
                index += 4;
                break;
            case 12:
            case 14:
            case 16:
                ++index;
                break;
        }
    }
}

extern "C" void NuTexAnimEnvDestroy(nutexanimenv_s *env) {
    if ((env->flags & 1) != 0) {
        NU_FREE(env);
    }
}

extern "C" nutexanimenv_s *NuTexAnimEnvCreate(VARIPTR *buffer, numtl_s *material, u16 *ids, nutexanimprog_s *program) {
    nutexanimenv_s *env;
    if (buffer == NULL) {
        env = static_cast<nutexanimenv_s *>(NU_ALLOC(sizeof(*env), alignof(nutexanimenv_s), 1, "", 0));
    } else {
        env = reinterpret_cast<nutexanimenv_s *>(ALIGN(buffer->addr, alignof(nutexanimenv_s)));
        buffer->addr = reinterpret_cast<usize>(env + 1);
    }
    if (env != NULL) {
        env->program = program;
        env->material = material;
        env->instruction_index = 0;
        env->loop_depth = 0;
        env->call_depth = 0;
        env->wait_base = 0;
        env->wait_random = 0;
        env->wait_remaining = 0;
        env->texture_ids = ids;
        env->texture_index = 0;
        if (buffer != NULL)
            env->flags &= ~1;
        else
            env->flags |= 1;
    }
    return env;
}

extern "C" void NuTexAnimEnvReset(nutexanimenv_s *env) {
    if (env == NULL) {
        return;
    }

    env->instruction_index = 0;
    env->loop_depth = 0;
    env->call_depth = 0;
    env->wait_base = 0;
    env->wait_random = 0;
    env->wait_remaining = 0;
    env->texture_index = 0;
}

}

nutexanimprog_s *NuTexAnimProgParseFile(i32 file, VARIPTR *buffer, VARIPTR, i32) {
    nutexanimprog_s *program;
    if (buffer == NULL) {
        program = static_cast<nutexanimprog_s *>(NU_ALLOC(0x400, 4, 1, "", 0));
    } else {
        program = reinterpret_cast<nutexanimprog_s *>(ALIGN(buffer->addr, alignof(nutexanimprog_s)));
    }
    labtabcnt = 0;
    memset(labtab, 0, sizeof(labtab));
    NUFPAR *parser = NuFParOpen(file);
    if (parser == NULL)
        return NULL;
    NuFParPushCom(parser, nutexanimcomtab);
    NuTexAnimProgInit(program);
    parprog = program;
    while (NuFParGetLine(parser) != 0) {
        i32 length = NuFParGetWord(parser);
        if (__builtin_expect(length != 0 && NuFParInterpretWord(parser) == 0 && parser->word_buf[0] != '\0' &&
            parser->word_buf[length - 1] == ':', 0)) {
            parser->word_buf[length - 1] = '\0';
            i32 label = NuTexAnimLabelIndex(parser->word_buf, labtab, &labtabcnt);
            nta_labels[label] = parprog->instruction_count;
        }
    }
    if (__builtin_expect(buffer != NULL, 0)) {
        buffer->i16_ptr = program->instructions + program->instruction_count;
    }
    NuFParClose(parser);
    NuTexAnimProgAssembleEnd(program);
    program->next = sys_progs;
    if (sys_progs != NULL)
        sys_progs->previous = program;
    program->previous = NULL;
    sys_progs = program;
    return program;
}

extern "C" {
extern "C" nutexanimprog_s *NuTexAnimProgReadScript(char *path, VARIPTR *buffer, VARIPTR end, i32 fps) {
    NUFILE file = NuFileOpen(path, NUFILE_READ);
    nutexanimprog_s *program = NULL;
    if (file != 0) {
        program = NuTexAnimProgParseFile(file, buffer, end, fps);
        NuFileClose(file);
    }
    return program;
}

extern "C" void NuTexAnimProgReadCFG(char *path, VARIPTR *buffer, VARIPTR end) {

    VARIPTR cursor;
    if (nutexanim_usepakfile != 0) {
        char filename[128];
        NuStrCpy(filename, path);
        char *extension = NuStrRChr(filename, '.');
        if (extension != NULL)
            *extension = '\0';
        NuStrCat(filename, ".pak");
        i32 size = static_cast<i32>(NuFileSize(filename));
        if (size == 0)
            return;

        cursor.addr = (end.addr - size) & ~static_cast<usize>(15);
        // Script names grow below the archive's original start, not its advanced load cursor.
        VARIPTR list = cursor;
        void *pak = NuFilePakLoad(filename, &cursor, end, 16);
        if (pak != NULL) {
            char *name = NuStrRChr(path, '/');
            if (name == NULL)
                name = NuStrRChr(path, '\\');
            name = name != NULL ? name + 1 : path;
            NuStrCpy(filename, name);
            extension = NuStrRChr(filename, '.');
            if (extension != NULL)
                *extension = '\0';
            NuStrCat(filename, ".cfg");

            i32 item = NuFilePakGetItem(pak, filename);
            void *data;
            NuFilePakGetItemInfo(pak, item, &data, &size);
            NUFILE file = NuMemFileOpen(data, size, NUFILE_READ);
            if (file != 0) {
                nufpar_s *parser = NuFParOpen(file);
                if (parser != NULL) {
                    *--list.char_ptr = '\0';
                    auto parser_word_buf = parser->word_buf;
                    while (NuFParGetLine(parser)) {
                        NuFParGetWord(parser);
                        name = NuStrRChr(parser_word_buf, '/');
                        if (name == NULL)
                            name = NuStrRChr(parser_word_buf, '\\');
                        name = name != NULL ? name + 1 : parser_word_buf;
                        list.char_ptr -= NuStrLen(name) + 1;
                        NuStrCpy(list.char_ptr, name);
                    }
                    NuFParClose(parser);
                }
                NuFileClose(file);
            }
            for (cursor = list; *cursor.char_ptr != '\0'; cursor.char_ptr += NuStrLen(cursor.char_ptr) + 1) {
                item = NuFilePakGetItem(pak, cursor.char_ptr);
                NuFilePakGetItemInfo(pak, item, &data, &size);
                file = NuMemFileOpen(data, size, NUFILE_READ);
                if (file != 0) {
                    NuTexAnimProgParseFile(file, buffer, list, 0);
                    NuFileClose(file);
                }
            }
            return;
        }
        buffer->addr = ALIGN(buffer->addr, 16);
    }

    *end.char_ptr = '\0';
    VARIPTR list;
    list.char_ptr = end.char_ptr - 1;
    nufpar_s *parser = NuFParCreate(path);
    if (parser != NULL) {
        *list.char_ptr = '\0';
        while (NuFParGetLine(parser)) {
            NuFParGetWord(parser);
            list.char_ptr -= NuStrLen(parser->word_buf) + 1;
            NuStrCpy(list.char_ptr, parser->word_buf);
        }
        NuFParDestroy(parser);
    }
    for (cursor = list; *cursor.char_ptr != '\0'; cursor.char_ptr += NuStrLen(cursor.char_ptr) + 1)
        NuTexAnimProgReadScript(cursor.char_ptr, buffer, list, 0);
    buffer->addr = ALIGN(buffer->addr, 16);
}

extern "C" void NuTexAnimEnvProc(nutexanimenv_s *env) {
    nutexanimprog_s *program = env->program;
    if (program == NULL || (program->mask & nta_script_mask) == 0)
        return;
    numtl_s *material = env->material;
    u32 signals = nta_sig_off & program->off_mask;
    if (signals != 0) {
        for (i32 i = 0; i < 32; ++i) {
            if ((signals & (1u << i)) != 0) {
                env->instruction_index = program->off_signal[i];
                env->wait_remaining = 0;
                env->call_depth = 0;
                env->loop_depth = 0;
                break;
            }
        }
    }
    signals = nta_sig_on & program->on_mask;
    if (signals != 0) {
        for (i32 i = 0; i < 32; ++i) {
            if ((signals & (1u << i)) != 0) {
                env->wait_remaining = 0;
                env->instruction_index = program->on_signal[i];
                env->call_depth = 0;
                env->loop_depth = 0;
                break;
            }
        }
    }
    if (env->wait_remaining != 0) {
        env->wait_remaining -= nta_iframetime;
        if (env->wait_remaining > 0)
            return;
    }
    for (;;) {
        i16 *instruction = program->instructions + env->instruction_index;
        i32 next_instruction;
        switch (static_cast<u16>(instruction[0])) {
            // Each texture opcode commits the material and yields immediately.
            case 0: {
                env->texture_index = instruction[1];
                next_instruction = env->instruction_index + 2;
                u16 *texture_id = &env->texture_ids[env->texture_index];
                material->tex_id = *texture_id;
                material->shader_desc.diffuse_map_tex_id[0] = *texture_id & 0x7fff;
                env->instruction_index = next_instruction;
                env->wait_remaining += env->wait_base;
                if (env->wait_random != 0)
                    env->wait_remaining += NuRand(&texanim_rand) % env->wait_random;
                if (env->wait_remaining < 0)
                    env->wait_remaining = 0;
                return;
            }
            case 1: {
                i32 random = NuRand(&texanim_rand);
                instruction = program->instructions + env->instruction_index;
                env->texture_index = random % instruction[1];
                next_instruction = env->instruction_index + 2;
                u16 *texture_id = &env->texture_ids[env->texture_index];
                material->tex_id = *texture_id;
                material->shader_desc.diffuse_map_tex_id[0] = *texture_id & 0x7fff;
                env->instruction_index = next_instruction;
                env->wait_remaining += env->wait_base;
                if (env->wait_random != 0)
                    env->wait_remaining += NuRand(&texanim_rand) % env->wait_random;
                if (env->wait_remaining < 0)
                    env->wait_remaining = 0;
                return;
            }
            case 2: {
                i32 texture = env->texture_index + instruction[1];
                if (texture < instruction[2])
                    texture = instruction[2];
                if (texture > instruction[3])
                    texture = instruction[3];
                env->texture_index = texture;
                next_instruction = env->instruction_index + 4;
                u16 *texture_id = &env->texture_ids[env->texture_index];
                material->tex_id = *texture_id;
                material->shader_desc.diffuse_map_tex_id[0] = *texture_id & 0x7fff;
                env->instruction_index = next_instruction;
                env->wait_remaining += env->wait_base;
                if (__builtin_expect(env->wait_random != 0, 1))
                    env->wait_remaining += NuRand(&texanim_rand) % env->wait_random;
                if (env->wait_remaining < 0)
                    env->wait_remaining = 0;
                return;
            }
            case 3: {
                i32 range = instruction[2] - instruction[1];
                i32 random = NuRand(&texanim_rand);
                i32 texture = random % (range + 1);
                instruction = program->instructions + env->instruction_index;
                texture += instruction[1] + env->texture_index;
                if (texture < instruction[3])
                    texture = instruction[3];
                if (texture > instruction[4])
                    texture = instruction[4];
                env->texture_index = texture;
                next_instruction = env->instruction_index + 5;
                u16 *texture_id = &env->texture_ids[env->texture_index];
                material->tex_id = *texture_id;
                material->shader_desc.diffuse_map_tex_id[0] = *texture_id & 0x7fff;
                env->instruction_index = next_instruction;
                env->wait_remaining += env->wait_base;
                if (env->wait_random != 0)
                    env->wait_remaining += NuRand(&texanim_rand) % env->wait_random;
                if (env->wait_remaining < 0)
                    env->wait_remaining = 0;
                return;
            }
            case 4:
                env->wait_remaining += instruction[1];
                if (instruction[2] != 0) {
                    i32 random = NuRand(&texanim_rand);
                    instruction = program->instructions + env->instruction_index;
                    env->wait_remaining += random % instruction[2];
                }
                env->instruction_index += 3;
                if (env->wait_remaining < 0)
                    env->wait_remaining = 0;
                return;
            case 7:
                env->wait_base = instruction[1];
                env->wait_random = instruction[2];
                env->instruction_index += 3;
                continue;
            case 9:
                env->instruction_index = instruction[1];
                continue;
            case 10:
                env->return_stack[env->call_depth++] = env->instruction_index + 2;
                env->instruction_index = instruction[1];
                continue;
            case 11:
                if (TextureCondition(instruction[1], env->texture_index, instruction[2]))
                    env->instruction_index = instruction[3];
                else
                    env->instruction_index += 4;
                continue;
            case 12:
                env->instruction_index = env->return_stack[--env->call_depth];
                continue;
            case 13:
                env->loop_counts[env->loop_depth] = instruction[1];
                if (instruction[2] != 0) {
                    i32 random = NuRand(&texanim_rand);
                    instruction = program->instructions + env->instruction_index;
                    env->loop_counts[env->loop_depth] += random % instruction[2];
                }
                env->instruction_index += 3;
                env->loop_starts[env->loop_depth++] = env->instruction_index;
                continue;
            case 14:
                if (env->loop_counts[env->loop_depth - 1] == 0) {
                    --env->loop_depth;
                    ++env->instruction_index;
                } else {
                    env->instruction_index = env->loop_starts[env->loop_depth - 1];
                    --env->loop_counts[env->loop_depth - 1];
                }
                continue;
            case 15:
                if (!(TextureCondition(instruction[1], env->texture_index, instruction[2])) &&
                    env->loop_counts[env->loop_depth - 1] != 0) {
                    --env->loop_counts[env->loop_depth - 1];
                    env->instruction_index = env->loop_starts[env->loop_depth - 1];
                } else {
                    --env->loop_depth;
                    env->instruction_index += 3;
                }
                continue;
            case 16:
                if (env->wait_remaining < 0)
                    env->wait_remaining = 0;
                return;
            case 17:
                for (nutexanimlist_s *list = ntal_first; list != NULL; list = list->next) {
                    for (nutexanim_s *anim = list->first; anim != NULL; anim = anim->next) {
                        nutexanimenv_s *other = anim->env;
                        if (other == NULL || other == env || other->program == NULL)
                            continue;
                        for (i32 i = 0; i < other->program->label_count; ++i) {
                            if (other->program->label_ids[i] == instruction[1]) {
                                other->instruction_index = other->program->label_offsets[i];
                                other->wait_remaining = 0;
                                other->call_depth = 0;
                                other->loop_depth = 0;
                                break;
                            }
                        }
                    }
                }
                env->instruction_index += 2;
                continue;
            default:
                continue;
        }
    }
}

extern "C" void NuTexAnimSetSignals(u32 signals) {
    const u32 combined = nta_sig_old | signals;
    nta_sig_on = ~nta_sig_old & signals;
    nta_sig_off = combined ^ signals;
    nta_sig_old = signals;
}

extern "C" void NuTexAnimProcessList(nutexanim_s *anim) {
    for (; anim != NULL; anim = anim->next) {
        if (anim->env != NULL)
            NuTexAnimEnvProc(anim->env);
    }
}

}

void NuTexAnimResetList(nutexanim_s *anim) {
    for (; anim != NULL; anim = anim->next) {
        if (anim->env != NULL) {
            NuTexAnimEnvReset(anim->env);
        }
    }
}

extern "C" {
extern "C" void NuTexAnimAddList(nutexanim_s *anim) {
    if (anim == NULL)
        return;
    NuThreadCriticalSectionBegin(g_texAnimCriticalSection);
    nutexanimlist_s *node = ntal_free;
    if (node != NULL) {
        if (node->next == NULL)
            NuSevereWarning("Ran out of texture anim slots!");
        ntal_free = ntal_free->next;
        node->previous = NULL;
        node->next = ntal_first;
        node->first = anim;
        if (ntal_first != NULL)
            ntal_first->previous = node;
        ntal_first = node;
    }
    NuThreadCriticalSectionEnd(g_texAnimCriticalSection);
}

extern "C" void NuTexAnimRemoveList(void *anim) {
    NuThreadCriticalSectionBegin(g_texAnimCriticalSection);
    for (nutexanimlist_s *node = ntal_first; node != NULL; node = node->next) {
        if (node->first == anim) {
            if (node->next != NULL)
                node->next->previous = node->previous;
            if (node->previous != NULL)
                node->previous->next = node->next;
            else
                ntal_first = node->next;
            node->next = ntal_free;
            ntal_free = node;
            NuThreadCriticalSectionEnd(g_texAnimCriticalSection);
            return;
        }
    }
    NuThreadCriticalSectionEnd(g_texAnimCriticalSection);
}

extern "C" void NuTexAnimSetMask(i32 mask) {
    script_mask = static_cast<u16>(mask);
}

extern "C" void NuTexAnimProcess(f32 frame_time) {
    nta_iframetime = static_cast<i32>(frame_time * 4096.0f);
    nta_script_mask = script_mask;
    NuThreadCriticalSectionBegin(g_texAnimCriticalSection);
    for (nutexanimlist_s *node = ntal_first; node != NULL; node = node->next)
        NuTexAnimProcessList(node->first);
    NuThreadCriticalSectionEnd(g_texAnimCriticalSection);
}

extern "C" void NuTexAnimRestart(void) {
    NuThreadCriticalSectionBegin(g_texAnimCriticalSection);
    for (nutexanimlist_s *node = ntal_first; node != NULL; node = node->next)
        NuTexAnimResetList(node->first);
    NuThreadCriticalSectionEnd(g_texAnimCriticalSection);
}

extern "C" nutexanim_s *NuTexAnimFind(nutexanim_s *animation, char *name) {
    while (animation != NULL) {
        if (NuStrICmp(animation->name, name) == 0) {
            return animation;
        }
        animation = animation->next;
    }
    return NULL;
}

extern "C" void NuTexAnimDestroy(nutexanim_s *animation) {
    if (animation->env != NULL) {
        NuTexAnimEnvDestroy(animation->env);
    }
    if (animation->flags != 0) {
        NU_FREE(animation);
    }
}

extern "C" nutexanim_s *NuTexAnimCreate(VARIPTR *buffer, nutexanimprog_s *program, numtl_s *material, u16 *texture_ids,
                                        i32 texture_count) {
    nutexanim_s *animation;
    if (buffer != NULL) {
        animation = reinterpret_cast<nutexanim_s *>(ALIGN(buffer->addr, 4));
        buffer->addr = reinterpret_cast<usize>(reinterpret_cast<u16 *>(animation + 1) + texture_count);
    } else {
        animation =
            static_cast<nutexanim_s *>(NU_ALLOC(sizeof(nutexanim_s) + texture_count * sizeof(u16), 4, 1, "", 0));
    }
    animation->texture_count = texture_count;
    animation->material = material;
    animation->previous = NULL;
    animation->next = NULL;
    animation->name = NULL;
    animation->texture_ids = reinterpret_cast<u16 *>(animation + 1);
    memcpy(animation->texture_ids, texture_ids, texture_count * sizeof(u16));
    animation->env = NuTexAnimEnvCreate(buffer, material, animation->texture_ids, program);
    return animation;
}

extern "C" void NuTexAnimProcessEx(f32 frame_time, u16 mask) {
    nta_iframetime = static_cast<i32>(frame_time * 4096.0f);
    nta_script_mask = mask;
    NuThreadCriticalSectionBegin(g_texAnimCriticalSection);
    for (nutexanimlist_s *node = ntal_first; node != NULL; node = node->next)
        NuTexAnimProcessList(node->first);
    NuThreadCriticalSectionEnd(g_texAnimCriticalSection);
}

    // Original @0x2cce60. Build bind-pose joint matrices, applying the optional
    // per-joint rotation/translation overrides before parent concatenation.
    void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices) {
        nuhgobjjointoverride_s *override_by_joint[256];
        if (override_count != 0) {
            memset(override_by_joint, 0, static_cast<usize>(object->joint_count) * sizeof(*override_by_joint));
            for (u8 i = 0; i < override_count; ++i) {
                const u8 override_index = overrides[i].joint_index;
                if (override_index < object->joint_override_map_count) {
                    const u8 joint_index = object->joint_override_map[override_index];
                    if (joint_index != 0xff) {
                        override_by_joint[joint_index] = &overrides[i];
                    }
                }
            }
        }

        for (u8 joint_index = 0; joint_index < object->joint_count; ++joint_index) {
            NUMTX *output = &matrices[joint_index];
            NUMTX_ALIGNED16 transformed_matrix;
            NUMTX *local_matrix;
            nuhgobjjointoverride_s *joint_override = override_count != 0 ? override_by_joint[joint_index] : NULL;
            if (joint_override != NULL) {
                transformed_matrix = object->bind_matrices[joint_index];
                constexpr f32 kRadiansToNuAngle = 10430.378f;
                NUANGVEC angles = {
                    static_cast<NUANG>(joint_override->rotation_x * kRadiansToNuAngle),
                    static_cast<NUANG>(joint_override->rotation_y * kRadiansToNuAngle),
                    static_cast<NUANG>(joint_override->rotation_z * kRadiansToNuAngle),
                };
                NUMTX override_matrix;
                NuMtxSetRotateXYZVU0(&override_matrix, &angles);
                NuMtxTranslate(&override_matrix, &joint_override->translation);
                NuMtxMulVU0(&transformed_matrix, &override_matrix, &transformed_matrix);
                local_matrix = &transformed_matrix;
            } else {
                local_matrix = &object->bind_matrices[joint_index];
            }

            const u8 parent_index = object->joints[joint_index].parent_index;
            if (parent_index == 0xff) {
                *output = *local_matrix;
            } else {
                NuMtxMulVU0(output, local_matrix, &matrices[parent_index]);
            }
        }
    }

    void NuHGobjEvalAnim(void) {
    }

    // Original @0x2cd150.
    void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data) {
        nuanimbuff_s buffer;
        NUVEC root_translation = {0.0f, 0.0f, 0.0f};
        NuAnimBuffCreateScratch(&buffer);
        NuAnimBuffAccumulate_3(&buffer, animation, time, 1, 0.0f, 0, object, NULL);
        if (override_count != 0 && JointProcAnimFn != NULL) {
            JointProcAnimFn(&buffer, object, override_count, overrides);
        }
        NuAnimBuffEvaluate_3(&buffer, object, matrices, animation, root_fn, &root_translation, root_data);
        NuAnimBuffDestroyScratch(&buffer);
    }

    void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data) {
        if (animation->magic == 0x414e4934 || animation->magic == 0x414e4935) {
            NuHGobjEvalAnim2Root_3(object, animation, time, override_count, overrides, matrices, root_fn, root_data);
            return;
        }
        if (object->joint_count > ddmaxjoints) {
            ddmaxjoints = object->joint_count;
        }
        nuanimdata2_s *legacy = reinterpret_cast<nuanimdata2_s *>(animation);
        NUVEC scales[256];
        NUJOINTANIM_s *joint_overrides[256];
        NUMTX local_matrix __attribute__((aligned(16)));
        nuanimtime_s anim_time;
        NUVEC translation, locator_translation;
        scales[255] = {1.0f, 1.0f, 1.0f};
        NuAnimData2CalcTime(legacy, time, &anim_time);
        if (override_count != 0) {
            memset(joint_overrides, 0, object->joint_count * sizeof(*joint_overrides));
            for (u8 i = 0; i < override_count; ++i) {
                if (overrides[i].joint_index < object->joint_override_map_count) {
                    u8 index = object->joint_override_map[overrides[i].joint_index];
                    if (index != 255) {
                        joint_overrides[index] = &overrides[i];
                    }
                }
            }
        }
        for (u8 i = 0; i < object->joint_count; ++i) {
            NUMTX *output = &matrices[i];
            if (i >= static_cast<i16>(legacy->node_count)) {
                *output = numtx_identity;
                continue;
            }
            NUJOINTANIM_s *override_anim = NULL;
            if (override_count != 0) {
                override_anim = joint_overrides[i];
            }
            nuanimcurve2_s *curves = &legacy->curves[i * static_cast<i16>(legacy->curve_count)];
            i8 *types = reinterpret_cast<i8 *>(legacy->curve_types) + i * static_cast<i16>(legacy->curve_count);
            NUVEC *scale = &scales[i];
            u8 flags = legacy->node_flags[i];
            if (i == 0) {
                NuAnimCurve2SetApplyToJointTransLoc(curves, types, static_cast<i8>(flags), &anim_time,
                                                    &object->joints[i], scale, &scales[object->joints[i].parent_index],
                                                    &local_matrix, override_anim, &translation, &locator_translation);
                if (root_fn != NULL) {
                    root_fn(&local_matrix, root_data, &translation, NULL, &locator_translation, 0.0f);
                }
            } else {
                NuAnimCurve2SetApplyToJoint(curves, types, static_cast<i8>(flags), &anim_time, &object->joints[i],
                                            scale, &scales[object->joints[i].parent_index], &local_matrix,
                                            override_anim);
            }
            u8 parent = object->joints[i].parent_index;
            if (parent == 255) {
                if ((flags & 0x40) != 0) {
                    *scale = {1.0f, 1.0f, 1.0f};
                }
                *output = local_matrix;
            } else {
                NuMtxMulVU0(output, &local_matrix, &matrices[parent]);
            }
        }
    }

    // Original @0x2cd730.
    void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices) {
        NuHGobjEvalAnim2Root(object, animation, time, override_count, overrides, matrices, NULL, NULL);
    }

    void **NuHGobjEvalDwa(i32 render_count, i16 *render_indices, void *animation, f32 frame) {
        if (animation == NULL || render_count == 0)
            return NULL;

        nuanimtime_s time __attribute__((aligned(16)));
        NuAnimDataCalcTime(animation, frame, &time);

        f32 **weights_by_render;
        if (render_indices != NULL) {
            weights_by_render = NuRndrCreateBlendShapeDWAPointers(render_count);
            memset(weights_by_render, 0, (static_cast<usize>(render_count) * sizeof(void *) + 15) >> 4);
        } else {
            weights_by_render = NuRndrCreateBlendShapeDWAPointers(1);
            memset(weights_by_render, 0, 1);
            render_count = 1;
        }
        if (weights_by_render == NULL)
            return NULL;

        for (i32 render = 0; render < render_count; ++render) {
            i32 node = 0;
            NuLegacyDwaChunk *chunk;
            if (render_indices != NULL) {
                node = render_indices[render];
                if (node < 0)
                    continue;
                chunk = NuLegacyDwaGetChunk(animation, time.chunk);
                if (node >= chunk->node_count) {
                    weights_by_render[render] = NuRndrCreateBlendShapeDeformerWeightsArray(0);
                    continue;
                }
            } else {
                chunk = NuLegacyDwaGetChunk(animation, time.chunk);
            }
            nuanimcurveset_s *set = chunk->curve_sets[node];
            if (set == NULL) {
                weights_by_render[render] = NuRndrCreateBlendShapeDeformerWeightsArray(0);
                continue;
            }

            i32 curve_count = static_cast<i8>(set->curve_count);
            f32 *weights = NuRndrCreateBlendShapeDeformerWeightsArray(curve_count);
            weights_by_render[render] = weights;
            if (weights == NULL || curve_count <= 0)
                continue;
            for (i32 curve = 0; curve < curve_count; ++curve) {
                nuanimcurveset_s *curve_set = NuLegacyDwaGetChunk(animation, time.chunk)->curve_sets[node];
                if (curve_set->curves[curve] != NULL) {
                    weights[curve + 1] = NuAnimCurveCalcVal2(curve_set->curves[curve], &time);
                } else {
                    weights[curve + 1] = curve_set->constants[curve];
                }
            }
        }
        return reinterpret_cast<void **>(weights_by_render);
    }

    void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame) {
        if (animation == NULL || render_count == 0) {
            return NULL;
        }

        nuanimtime_s time;
        NuAnimData2CalcTime(animation, frame, &time);

        usize clear_size;
        if (render_indices == NULL) {
            render_count = 1;
            clear_size = 1;
        } else {
            clear_size = (static_cast<usize>(render_count) * sizeof(void *) + 0xf) >> 4;
        }

        f32 **weights_by_render = NuRndrCreateBlendShapeDWAPointers(render_count);
        if (weights_by_render == NULL) {
            return NULL;
        }
        memset(weights_by_render, 0, clear_size);

        for (i32 render = 0; render < render_count; ++render) {
            i32 node = 0;
            if (render_indices != NULL) {
                node = render_indices[render];
                if (node < 0)
                    continue;
                if (node >= NuAnimNumNodes(animation)) {
                    weights_by_render[render] = NuRndrCreateBlendShapeDeformerWeightsArray(0);
                    continue;
                }
            }

            i32 curve_count = static_cast<i16>(animation->curve_count);
            f32 *weights = NuRndrCreateBlendShapeDeformerWeightsArray(curve_count);
            weights_by_render[render] = weights;
            i32 curve_offset = animation->curve_count * node;
            nuanimcurve2_s *curves = animation->curves + curve_offset;
            i8 *types = reinterpret_cast<i8 *>(animation->curve_types) + curve_offset;
            if (weights == NULL) {
                continue;
            }
            for (i32 curve = 0; curve < curve_count; ++curve, ++curves) {
                if (types[curve] == 0) {
                    weights[curve + 1] = curves->data.constant;
                } else {
                    weights[curve + 1] = NuAnimCurve2CalcValEx(curves, &time, types[curve]);
                }
            }
        }
        return reinterpret_cast<void **>(weights_by_render);
    }

    void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend) {
        if ((animation_a == NULL && animation_b == NULL) || render_count == 0)
            return NULL;
        nuanimtime_s time_a __attribute__((aligned(16)));
        nuanimtime_s time_b __attribute__((aligned(16)));
        if (animation_a != NULL)
            NuAnimDataCalcTime(animation_a, frame_a, &time_a);
        if (animation_b != NULL)
            NuAnimDataCalcTime(animation_b, frame_b, &time_b);
        f32 **weights_by_render;
        if (render_indices != NULL) {
            weights_by_render = NuRndrCreateBlendShapeDWAPointers(render_count);
            memset(weights_by_render, 0, (static_cast<usize>(render_count) * sizeof(void *) + 15) >> 4);
        } else {
            weights_by_render = NuRndrCreateBlendShapeDWAPointers(1);
            memset(weights_by_render, 0, 1);
            render_count = 1;
        }
        if (weights_by_render == NULL)
            return NULL;
        for (i32 render = 0; render < render_count; ++render) {
            i32 node = render_indices == NULL ? 0 : render_indices[render];
            if (node < 0)
                continue;
            i32 count_a = 0;
            if (animation_a != NULL) {
                NuLegacyDwaChunk *chunk = NuLegacyDwaGetChunk(animation_a, time_a.chunk);
                if (node < chunk->node_count && chunk->curve_sets[node])
                    count_a = static_cast<i8>(chunk->curve_sets[node]->curve_count);
            }
            i32 count_b = 0;
            if (animation_b != NULL) {
                NuLegacyDwaChunk *chunk = NuLegacyDwaGetChunk(animation_b, time_b.chunk);
                if (node < chunk->node_count && chunk->curve_sets[node])
                    count_b = static_cast<i8>(chunk->curve_sets[node]->curve_count);
            }
            i32 curve_count = count_a < count_b ? count_b : count_a;
            f32 *weights = NuRndrCreateBlendShapeDeformerWeightsArray(curve_count);
            weights_by_render[render] = weights;
            if (__builtin_expect(weights == NULL || curve_count <= 0, 0))
                continue;
            for (i32 curve = 0; curve < curve_count; ++curve) {
                f32 value_a = 0.0f;
                if (curve < count_a) {
                    nuanimcurveset_s *set = NuLegacyDwaGetChunk(animation_a, time_a.chunk)->curve_sets[node];
                    value_a =
                        set->curves[curve] ? NuAnimCurveCalcVal2(set->curves[curve], &time_a) : set->constants[curve];
                }
                f32 value_b = 0.0f;
                if (curve < count_b) {
                    nuanimcurveset_s *set = NuLegacyDwaGetChunk(animation_b, time_b.chunk)->curve_sets[node];
                    value_b =
                        set->curves[curve] ? NuAnimCurveCalcVal2(set->curves[curve], &time_b) : set->constants[curve];
                }
                weights[curve + 1] = value_b * blend + (1.0f - blend) * value_a;
            }
        }
        return reinterpret_cast<void **>(weights_by_render);
    }

    void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend) {
        if (!(animation_a == NULL && animation_b == NULL) && render_count != 0) {
            nuanimtime_s time_a;
            nuanimtime_s time_b;
            if (animation_a != NULL) {
                NuAnimData2CalcTime(animation_a, frame_a, &time_a);
            }
            if (animation_b != NULL) {
                NuAnimData2CalcTime(animation_b, frame_b, &time_b);
            }

            usize clear_size;
            if (render_indices == NULL) {
                render_count = 1;
                clear_size = 1;
            } else {
                clear_size = (static_cast<usize>(render_count) * sizeof(void *) + 0xf) >> 4;
            }
            f32 **weights_by_render = NuRndrCreateBlendShapeDWAPointers(render_count);
            if (weights_by_render == NULL) {
                return NULL;
            }
            memset(weights_by_render, 0, clear_size);

            nuanimcurve2_s *curves_a = NULL;
            u8 *types_a = NULL;
            nuanimcurve2_s *curves_b = NULL;
            u8 *types_b = NULL;
            for (i32 render = 0; render < render_count; ++render) {
                i32 node = render_indices == NULL ? 0 : render_indices[render];
                if (node < 0) {
                    continue;
                }

                i32 count_a;
                if (animation_a != NULL && node < NuAnimNumNodes(animation_a)) {
                    count_a = static_cast<i16>(animation_a->curve_count);
                    curves_a = animation_a->curves + count_a * node;
                    types_a = animation_a->curve_types + count_a * node;
                } else {
                    count_a = 0;
                }

                i32 count_b;
                if (animation_b != NULL && node < NuAnimNumNodes(animation_b)) {
                    count_b = static_cast<i16>(animation_b->curve_count);
                    curves_b = animation_b->curves + count_b * node;
                    types_b = animation_b->curve_types + count_b * node;
                } else {
                    count_b = 0;
                }

                i32 curve_count = count_a < count_b ? count_b : count_a;
                f32 *weights = NuRndrCreateBlendShapeDeformerWeightsArray(curve_count);
                weights_by_render[render] = weights;
                if (weights == NULL) {
                    continue;
                }

                nuanimcurve2_s *curve_a = curves_a;
                nuanimcurve2_s *curve_b = curves_b;
                for (i32 curve = 0; curve < curve_count; ++curve, ++curve_a, ++curve_b) {
                    f32 value_a = 0.0f;
                    if (curve < count_a) {
                        const i32 type = static_cast<i8>(types_a[curve]);
                        value_a = type == 0 ? curve_a->data.constant : NuAnimCurve2CalcValEx(curve_a, &time_a, type);
                    }
                    f32 value_b = 0.0f;
                    if (curve < count_b) {
                        const i32 type = static_cast<i8>(types_b[curve]);
                        value_b = type == 0 ? curve_b->data.constant : NuAnimCurve2CalcValEx(curve_b, &time_b, type);
                    }
                    weights[curve + 1] = value_b * blend + (1.0f - blend) * value_a;
                }
            }
            return reinterpret_cast<void **>(weights_by_render);
        }
        return NULL;
    }

    // Original @0x2ce450. Evaluate and blend two legacy animation chunks
    // before concatenating the local transforms into the joint hierarchy.
    void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices) {
        if (object->joint_count > ddmaxjoints) {
            ddmaxjoints = object->joint_count;
        }
        NUVEC scales[256];
        NUJOINTANIM_s *joint_overrides[256];
        NUMTX local_matrix __attribute__((aligned(16)));
        nuanimtime_s time_a;
        nuanimtime_s time_b;
        scales[255] = {1.0f, 1.0f, 1.0f};
        NuAnimDataCalcTime(animation_a, frame_a, &time_a);
        NuLegacyDwaChunk *chunk_a = NuLegacyDwaGetChunk(animation_a, time_a.chunk);
        NuAnimDataCalcTime(animation_b, frame_b, &time_b);
        NuLegacyDwaChunk *chunk_b = NuLegacyDwaGetChunk(animation_b, time_b.chunk);
        if (override_count != 0) {
            memset(joint_overrides, 0, object->joint_count * sizeof(*joint_overrides));
            for (u8 index = 0; index < override_count; ++index) {
                u8 override_index = overrides[index].joint_index;
                if (override_index < object->joint_override_map_count) {
                    u8 joint_index = object->joint_override_map[override_index];
                    if (joint_index != 0xff) {
                        joint_overrides[joint_index] = &overrides[index];
                    }
                }
            }
        }
        for (u8 index = 0; index < object->joint_count; ++index) {
            NUMTX *output = &matrices[index];
            NUJOINTANIM_s *joint_override = override_count != 0 ? joint_overrides[index] : NULL;
            nuanimcurveset_s *second = chunk_b->curve_sets[index];
            nuanimcurveset_s *first = chunk_a->curve_sets[index];
            NUMTX *local = &local_matrix;
            if (first != NULL && second != NULL) {
                nuhgobjjoint_s *joint = &object->joints[index];
                NuAnimCurveSetApplyBlendToJoint2(first, &time_a, second, &time_b, blend, joint, &scales[index],
                                                 &scales[joint->parent_index], &local_matrix, joint_override);
            } else {
                local = &object->bind_matrices[index];
            }
            u8 parent_index = object->joints[index].parent_index;
            if (parent_index == 0xff) {
                *output = *local;
            } else {
                NuMtxMulVU0(output, local, &matrices[parent_index]);
            }
        }
    }

extern "C++" {
// Original @0x2ce760.
void NuHGobjEvalAnimBlend2Root_3(nugscn_s *scene, ani3_animheader_s *animation_a, f32 time_a,
                                 ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                 NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data) {
    nuhgobj_s *object = reinterpret_cast<nuhgobj_s *>(scene);
    nuanimbuff_s buffer;
    NUVEC root_a;
    NUVEC root_b;

    NuAnimBuffCreateScratch(&buffer);
    NUVEC root_translation = {0.0f, 0.0f, 0.0f};
    const char use_quaternions = NuAnimGetUseQuatsFlag() | ((animation_a->format_flags | animation_b->format_flags) &
                                                           ANI3_FORMAT_QUATERNION_ROTATION);
    NuAnimPushSetUseQuatsFlag(use_quaternions);
    NuAnimBuffAccumulate_3(&buffer, animation_a, time_a, 1, 0.0f, 0, object, &root_a);
    NuAnimBuffAccumulate_3(&buffer, animation_b, time_b, 0, blend, 0, object, &root_b);
    if (override_count != 0 && JointProcAnimFn != NULL) {
        JointProcAnimFn(&buffer, object, override_count, overrides);
    }
    NuAnimBuffEvaluate_3(&buffer, object, matrices, animation_a, root_fn, &root_translation, root_data);
    NuAnimBuffDestroyScratch(&buffer);
    NuAnimPopUseQuatsFlag();
}
}

    // Original @0x2ce8e0. ANI4/ANI5 are the only accepted animation formats.
    void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data) {
        if (animation_a->magic != 0x414e4934 && animation_a->magic != 0x414e4935) {
            return;
        }
        NuHGobjEvalAnimBlend2Root_3(reinterpret_cast<nugscn_s *>(object), animation_a, time_a, animation_b, time_b,
                                    blend, override_count, overrides, matrices, root_fn, root_data);
    }

    // Original @0x2ce980.
    void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices) {
        NuHGobjEvalAnimBlend2Root(object, animation_a, time_a, animation_b, time_b, blend, override_count, overrides,
                                  matrices, NULL, NULL);
    }

// ---------------------------------------------------------------------------
// Scene lifecycle
// ---------------------------------------------------------------------------

extern "C" i32 NuRndrBeginScene(i32 /*begin_flags*/) {
    // Clear the in-flight scene to a known baseline.  Field names below
    // still use the original unknown_* tags because the 0x218-byte layout
    // has not been fully recovered; values match the PS2 initial defaults.
    currentScene.unknown_4c = 0;
    currentScene.render_scene_id = 0xffffffff;
    currentScene.clear_flags = 0;
    currentScene.state_ptr = nullptr;
    currentScene.z_pre_pass = 0;
    currentScene.unknown_28 = 0;
    currentScene.unknown_58 = 0;
    currentScene.unknown_38 = 0;
    currentScene.unknown_3c = 0;
    currentScene.unknown_40 = 0xffffffff;
    currentScene.unknown_48 = 0;
    currentScene.unknown_ac = 0;
    currentScene.texture_blend_enabled = 0;
    currentScene.unknown_e4 = 0;
    currentScene.unknown_e0 = 0;
    currentScene.unknown_178 = 0;
    currentScene.unknown_174 = 0;
    currentScene.unknown_188 = 0;
    currentScene.unknown_214 = 0;
    return 1;
}

extern "C" i32 NuRndrBeginSceneEx(i32 begin_flags, i32, i32) {
    return NuRndrBeginScene(begin_flags);
}

extern "C" void NuRndrSetDebBaseRange(NUVEC *base, NUVEC *range) {
    NuRndrDebBase = *base;
    NuRndrDebRange = *range;
    NuRndrDebRangeInv.x = 1.0f / NuRndrDebRange.x;
    NuRndrDebRangeInv.y = 1.0f / NuRndrDebRange.y;
    NuRndrDebRangeInv.z = 1.0f / NuRndrDebRange.z;
}

extern "C" void NuRndrSetDebBox(NUVEC *range) {
    static NUMTX cammtx;
    if (NuRndrStopUpdate == 0) {
        cammtx = global_camera.mtx;
    }
    const NUVEC size = *range;
    // The original uses a narrow near rectangle and a wider far rectangle,
    // transformed by the cached camera before finding their world bounds.
    NUVEC corner0 = {
        (((size.x * cammtx.m00) * -0.2f + cammtx.m30) + (size.y * cammtx.m10) * -0.2f) + (size.z * cammtx.m20) * -0.05f,
        (((size.x * cammtx.m01) * -0.2f + cammtx.m31) + (size.y * cammtx.m11) * -0.2f) + (size.z * cammtx.m21) * -0.05f,
        (((size.x * cammtx.m02) * -0.2f + cammtx.m32) + (size.y * cammtx.m12) * -0.2f) +
            (size.z * cammtx.m22) * -0.05f};
    NUVEC minimum = corner0;
    NUVEC maximum = corner0;
    NUVEC corner1 = {
        (((size.x * cammtx.m00) * 0.2f + cammtx.m30) + (size.y * cammtx.m10) * -0.2f) + (size.z * cammtx.m20) * -0.05f,
        (((size.x * cammtx.m01) * 0.2f + cammtx.m31) + (size.y * cammtx.m11) * -0.2f) + (size.z * cammtx.m21) * -0.05f,
        (((size.x * cammtx.m02) * 0.2f + cammtx.m32) + (size.y * cammtx.m12) * -0.2f) + (size.z * cammtx.m22) * -0.05f};
    minimum.x = corner1.x < minimum.x ? corner1.x : minimum.x;
    minimum.y = corner1.y < minimum.y ? corner1.y : minimum.y;
    minimum.z = corner1.z < minimum.z ? corner1.z : minimum.z;
    maximum.y = corner1.y > maximum.y ? corner1.y : maximum.y;
    maximum.x = corner1.x > maximum.x ? corner1.x : maximum.x;
    maximum.z = corner1.z > maximum.z ? corner1.z : maximum.z;
    NUVEC corner2 = {
        (((size.x * cammtx.m00) * -0.2f + cammtx.m30) + (size.y * cammtx.m10) * 0.2f) + (size.z * cammtx.m20) * -0.05f,
        (((size.x * cammtx.m01) * -0.2f + cammtx.m31) + (size.y * cammtx.m11) * 0.2f) + (size.z * cammtx.m21) * -0.05f,
        (((size.x * cammtx.m02) * -0.2f + cammtx.m32) + (size.y * cammtx.m12) * 0.2f) + (size.z * cammtx.m22) * -0.05f};
    minimum.y = corner2.y < minimum.y ? corner2.y : minimum.y;
    minimum.x = corner2.x < minimum.x ? corner2.x : minimum.x;
    minimum.z = corner2.z < minimum.z ? corner2.z : minimum.z;
    maximum.x = corner2.x > maximum.x ? corner2.x : maximum.x;
    maximum.y = corner2.y > maximum.y ? corner2.y : maximum.y;
    maximum.z = corner2.z > maximum.z ? corner2.z : maximum.z;
    NUVEC corner3 = {
        (((size.x * cammtx.m00) * 0.2f + cammtx.m30) + (size.y * cammtx.m10) * 0.2f) + (size.z * cammtx.m20) * -0.05f,
        (((size.x * cammtx.m01) * 0.2f + cammtx.m31) + (size.y * cammtx.m11) * 0.2f) + (size.z * cammtx.m21) * -0.05f,
        (((size.x * cammtx.m02) * 0.2f + cammtx.m32) + (size.y * cammtx.m12) * 0.2f) + (size.z * cammtx.m22) * -0.05f};
    minimum.x = corner3.x < minimum.x ? corner3.x : minimum.x;
    minimum.y = corner3.y < minimum.y ? corner3.y : minimum.y;
    minimum.z = corner3.z < minimum.z ? corner3.z : minimum.z;
    maximum.x = corner3.x > maximum.x ? corner3.x : maximum.x;
    maximum.y = corner3.y > maximum.y ? corner3.y : maximum.y;
    maximum.z = corner3.z > maximum.z ? corner3.z : maximum.z;
    NUVEC corner4 = {
        (((size.x * cammtx.m00) * -0.5f + cammtx.m30) + (size.y * cammtx.m10) * -0.5f) + (size.z * cammtx.m20) * 0.8f,
        (((size.x * cammtx.m01) * -0.5f + cammtx.m31) + (size.y * cammtx.m11) * -0.5f) + (size.z * cammtx.m21) * 0.8f,
        (((size.x * cammtx.m02) * -0.5f + cammtx.m32) + (size.y * cammtx.m12) * -0.5f) + (size.z * cammtx.m22) * 0.8f};
    minimum.x = corner4.x < minimum.x ? corner4.x : minimum.x;
    minimum.y = corner4.y < minimum.y ? corner4.y : minimum.y;
    minimum.z = corner4.z < minimum.z ? corner4.z : minimum.z;
    maximum.x = corner4.x > maximum.x ? corner4.x : maximum.x;
    maximum.y = corner4.y > maximum.y ? corner4.y : maximum.y;
    maximum.z = corner4.z > maximum.z ? corner4.z : maximum.z;
    NUVEC corner5 = {
        (((size.x * cammtx.m00) * 0.5f + cammtx.m30) + (size.y * cammtx.m10) * -0.5f) + (size.z * cammtx.m20) * 0.8f,
        (((size.x * cammtx.m01) * 0.5f + cammtx.m31) + (size.y * cammtx.m11) * -0.5f) + (size.z * cammtx.m21) * 0.8f,
        (((size.x * cammtx.m02) * 0.5f + cammtx.m32) + (size.y * cammtx.m12) * -0.5f) + (size.z * cammtx.m22) * 0.8f};
    minimum.y = corner5.y < minimum.y ? corner5.y : minimum.y;
    minimum.x = corner5.x < minimum.x ? corner5.x : minimum.x;
    minimum.z = corner5.z < minimum.z ? corner5.z : minimum.z;
    maximum.x = corner5.x > maximum.x ? corner5.x : maximum.x;
    maximum.y = corner5.y > maximum.y ? corner5.y : maximum.y;
    maximum.z = corner5.z > maximum.z ? corner5.z : maximum.z;
    NUVEC corner6 = {
        (((size.x * cammtx.m00) * -0.5f + cammtx.m30) + (size.y * cammtx.m10) * 0.5f) + (size.z * cammtx.m20) * 0.8f,
        (((size.x * cammtx.m01) * -0.5f + cammtx.m31) + (size.y * cammtx.m11) * 0.5f) + (size.z * cammtx.m21) * 0.8f,
        (((size.x * cammtx.m02) * -0.5f + cammtx.m32) + (size.y * cammtx.m12) * 0.5f) + (size.z * cammtx.m22) * 0.8f};
    minimum.x = corner6.x < minimum.x ? corner6.x : minimum.x;
    minimum.z = corner6.z < minimum.z ? corner6.z : minimum.z;
    minimum.y = corner6.y < minimum.y ? corner6.y : minimum.y;
    maximum.x = corner6.x > maximum.x ? corner6.x : maximum.x;
    maximum.y = corner6.y > maximum.y ? corner6.y : maximum.y;
    maximum.z = corner6.z > maximum.z ? corner6.z : maximum.z;
    NUVEC corner7 = {
        (((size.x * cammtx.m00) * 0.5f + cammtx.m30) + (size.y * cammtx.m10) * 0.5f) + (size.z * cammtx.m20) * 0.8f,
        (((size.x * cammtx.m01) * 0.5f + cammtx.m31) + (size.y * cammtx.m11) * 0.5f) + (size.z * cammtx.m21) * 0.8f,
        (((size.x * cammtx.m02) * 0.5f + cammtx.m32) + (size.y * cammtx.m12) * 0.5f) + (size.z * cammtx.m22) * 0.8f};
    minimum.x = corner7.x < minimum.x ? corner7.x : minimum.x;
    minimum.y = corner7.y < minimum.y ? corner7.y : minimum.y;
    minimum.z = corner7.z < minimum.z ? corner7.z : minimum.z;
    maximum.x = corner7.x > maximum.x ? corner7.x : maximum.x;
    maximum.y = corner7.y > maximum.y ? corner7.y : maximum.y;
    maximum.z = corner7.z > maximum.z ? corner7.z : maximum.z;
    NuRndrDebBase.x = (maximum.x + minimum.x) * 0.5f - size.x * 0.5f;
    NuRndrDebBase.z = (maximum.z + minimum.z) * 0.5f - size.z * 0.5f;
    NuRndrDebBase.y = (maximum.y + minimum.y) * 0.5f - size.y * 0.5f;
    NuRndrDebRange = size;
    NuRndrDebRangeInv.x = 1.0f / size.x;
    NuRndrDebRangeInv.y = 1.0f / size.y;
    NuRndrDebRangeInv.z = 1.0f / size.z;
}

extern "C" void NuRndrStartReflectionRender(i32) {
    NuSpecialReflection(1);
}

extern "C" void NuRndrEndReflectionRender(void) {
    NuSpecialReflection(0);
}

}

i32 NuRndIsReflectionGobj(void) {
    return global_GobjIsReflectedGeometry;
}

extern "C" {
extern "C" i32 NuRndrWasDrawnUnreflectedGobj(void) {
    return global_GobjWasDrawnUnreflected;
}

extern "C" void NuRndrStartShadowReceiveRender(void) {
    global_GobjIsShadowReceive = 1;
}

extern "C" void NuRndrEndShadowReceiveRender(void) {
    global_GobjIsShadowReceive = 0;
}

}

i32 NuRndIsShadowReceiveRenderGobj(void) {
    return global_GobjIsShadowReceive;
}

extern "C" {
extern "C" void NuRndrSetCullDebug(void) {
    STUBBED();
}

extern "C" i32 NuRndrGetCullDebug(void) {
    STUBBED();
    return 0;
}

extern "C" void NuRndrSetWind(f32 speed, f32 scale) {
    global_windspeed = speed;
    global_windscale = scale;
}

extern "C" i32 NuRndrGlobalFrameCount(void) {
    return global_frame_count;
}

extern "C" void WindShear(NUMTX *output, NUMTX *input, i32 wind_scale, i32 wind_speed) {
    u32 seed = static_cast<u32>(wind_speed);
    const f32 amplitude = (static_cast<f32>(wind_scale) / 65535.0f) * global_windscale;
    const f32 speed = (static_cast<f32>(wind_speed) / 65535.0f) * global_windspeed;
    const f32 random = NuRandFloatSeeded(&seed);
    const u32 frame = static_cast<u32>(NuRndrGlobalFrameCount());
    const f32 phase = random * 3.142f + random * 3.142f + static_cast<f32>(frame) * speed;
    const f32 shear_x =
        (NU_SIN_LUT(4.2f * phase * 10430.3779296875f) * 0.25f +
         (NU_SIN_LUT(2.1f * phase * 10430.3779296875f) * 0.5f + NU_SIN_LUT(phase * 10430.3779296875f))) *
        amplitude;
    const f32 shear_z =
        (0.25f * NU_COS_LUT(4.4f * phase * 10430.3779296875f) +
         (0.5f * NU_COS_LUT(2.3f * phase * 10430.3779296875f) + NU_COS_LUT(phase * 1.1f * 10430.3779296875f))) *
        amplitude;
    *output = *input;
    output->m10 = input->m00 * shear_x + input->m10 + input->m20 * shear_z;
    output->m11 = input->m01 * shear_x + input->m11 + input->m21 * shear_z;
    output->m12 = input->m02 * shear_x + input->m12 + input->m22 * shear_z;
    output->m13 = shear_x * input->m03 + input->m13 + shear_z * input->m23;
}

extern "C" void NuRndrGlobalFrameCountPause(i32 paused) {
    global_frame_count_paused = paused;
}

}

void NuRndrCalcRandEllipsePos(nuvec4_s *position, numtx_s *matrix, nuvec_s *axes) {
    f32 x = (f32)(i32)NuWindRand() - 32768.0f;
    f32 y = (f32)(i32)NuWindRand() - 32768.0f;
    f32 z = (f32)(i32)NuWindRand() - 32768.0f;
    f32 scale = 1.0f / NuFsqrt(x * x + y * y + z * z);
    x *= scale;
    y *= scale;
    z *= scale;
    position->x = axes[1].x * x + axes[2].x * y + axes[3].x * z + axes[0].x;
    position->y = axes[1].y * x + axes[2].y * y + axes[3].y * z + axes[0].y;
    position->z = axes[1].z * x + axes[2].z * y + axes[3].z * z + axes[0].z;
    position->w = 1.0f;
    NuVec4MtxTransformVU0(position, position, matrix);
}

void NuRndrCalcRandCylinderPos(nuvec4_s *position, numtx_s *matrix, nuvec_s *axes) {
    f32 x = (f32)((i32)NuWindRand() - 32768) * (1.0f / 32768.0f);
    f32 y = (f32)(i32)NuWindRand() - 32768.0f;
    f32 z = (f32)(i32)NuWindRand() - 32768.0f;
    f32 scale = 1.0f / NuFsqrt(y * y + z * z);
    y *= scale;
    z *= scale;
    position->x = axes[1].x * x + axes[2].x * y + axes[3].x * z + axes[0].x;
    position->y = axes[1].y * x + axes[2].y * y + axes[3].y * z + axes[0].y;
    position->z = axes[1].z * x + axes[2].z * y + axes[3].z * z + axes[0].z;
    position->w = 1.0f;
    NuVec4MtxTransformVU0(position, position, matrix);
}

extern "C" {
    i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask) {
        if (__builtin_expect(NuCameraClipTestExtents(&object->bounds_min, &object->bounds_max, world_matrix, 0.0f, 0) == 0, 0)) {
            return 0;
        }
        nuhgobjshadowgroup_s *groups = object->shadow_groups;
        if (__builtin_expect(groups == NULL || object->suppress_shadow_surface_points != 0, 0)) {
            return 1;
        }
        i32 total = 0;
        for (nuhgobjshadowgroup_s *group = groups; group->joint_index != 0xff; ++group) {
            for (i32 i = 0; i < group->ellipse_count; ++i) {
                if ((*reinterpret_cast<u32 *>(&group->ellipses[i][4]) & 1) == 0) {
                    ++total;
                }
            }
            for (i32 i = 0; i < group->cylinder_count; ++i) {
                if ((*reinterpret_cast<u32 *>(&group->cylinders[i][4]) & 1) == 0) {
                    ++total;
                }
            }
        }
        if (total == 0) {
            return 1;
        }
        i16 counts[256];
        for (i32 i = 0; i < total; ++i) {
            counts[i] = 0;
        }
        for (i32 i = 0; i < count; ++i) {
            ++counts[(static_cast<i32>(NuWindRand()) >> 8) % total];
        }
        if (count > 0) {
            groups = object->shadow_groups;
        }
        i32 index = 0;
        NUMTX combined;
        NUVEC4 point;
        for (nuhgobjshadowgroup_s *group = groups; group->joint_index != 0xff; ++group) {
            if (count == 0) {
                continue;
            }
            i32 matrix_ready = 0;
            for (i32 i = 0; i < group->ellipse_count; ++i) {
                if (__builtin_expect(exclusion_mask != 0 && (*reinterpret_cast<u32 *>(&group->ellipses[i][4]) & exclusion_mask) != 0, 0)) {
                    continue;
                }
                i32 n = counts[index];
                if (__builtin_expect(n != 0, 1)) {
                    if (matrix_ready == 0) {
                        NuMtxMulVU0(&combined, &joint_matrices[group->joint_index], world_matrix);
                    }
                    for (i32 j = 0; j < n; ++j) {
                        NuRndrCalcRandEllipsePos(&point, &combined, reinterpret_cast<NUVEC *>(group->ellipses[i]));
                        --count;
                        positions[count].x = point.x;
                        positions[count].y = point.y;
                        positions[count].z = point.z;
                    }
                    matrix_ready = 1;
                }
                ++index;
            }
            for (i32 i = 0; i < group->cylinder_count; ++i) {
                // Original 0x2cfcf7 uses the ellipse table for this filter too.
                if (exclusion_mask != 0 && (*reinterpret_cast<u32 *>(&group->ellipses[i][4]) & exclusion_mask) != 0) {
                    continue;
                }
                i16 n = counts[index];
                if (n != 0) {
                    if (matrix_ready == 0) {
                        NuMtxMulVU0(&combined, &joint_matrices[group->joint_index], world_matrix);
                    }
                    for (i32 j = 0; j < n; ++j) {
                        NuRndrCalcRandCylinderPos(&point, &combined, reinterpret_cast<NUVEC *>(group->cylinders[i]));
                        --count;
                        positions[count].x = point.x;
                        positions[count].y = point.y;
                        positions[count].z = point.z;
                    }
                    matrix_ready = 1;
                }
                ++index;
            }
        }
        return 1;
    }

extern "C" void NuRndrAnglesZX(NUVEC *direction, NUVEC *angles) {
    NUVEC rotated;
    angles->x = (f32)NuAtan2D(direction->z, direction->y);
    NuVecRotateX(&rotated, direction, -(i32)angles->x);
    angles->z = -(f32)NuAtan2D(rotated.x, rotated.y);
}

}

void NuLightBurnoutEffect(i32, f32 threshold, f32 intensity, f32 flare) {
    currentScene.unknown_28 = 1;
    currentScene.burnout_intensity = intensity <= 2.0f ? (i32)(127.0f * intensity) : 255;
    currentScene.burnout_flare = flare;
    currentScene.burnout_threshold = threshold;
    currentScene.bloom.enabled = 1;
    currentScene.bloom.directional = 0;
    currentScene.bloom.unknown_18 = 0.0f;
    currentScene.bloom.near_angle = 0.0f;
    currentScene.bloom.near_scale = 0.0f;
    currentScene.bloom.far_angle = 180.0f;
    currentScene.bloom.far_scale = intensity;
    currentScene.bloom.intensity = 1.0f;
    currentScene.bloom.blur_iterations = flare;
    currentScene.bloom.blend = 0.0f;
    currentScene.bloom.threshold = threshold;
}

extern "C" {
    void NuPostBloom(i32, const NuBloomParameters *parameters) {
        currentScene.bloom = *parameters;
    }

    void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters) {
        currentScene.dof = *parameters;
        if (NuRndrDoingScreenGrab != 0) {
            currentScene.dof.enabled = 0;
        }
    }

    void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance) {
        currentScene.dof.enabled = 1;
        currentScene.dof.strength = strength;
        currentScene.dof.near_distance = near_distance;
        currentScene.dof.far_distance = far_distance;
        currentScene.dof.bias = 0;
        currentScene.dof.mode = 3;
        if (NuRndrDoingScreenGrab != 0) {
            currentScene.dof.enabled = 0;
        }
    }

    void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance) {
        near_distance =
            (far_distance - near_distance) * (near_distance / (near_distance + far_distance)) + near_distance;
        NuDepthOfFieldEffect(1.0f, (far_distance * near_distance) / ((far_distance - near_distance) + far_distance),
                             near_distance);
    }

    void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength) {
        near_distance =
            (far_distance - near_distance) * (near_distance / (near_distance + far_distance)) + near_distance;
        NuDepthOfFieldEffect(strength, (far_distance * near_distance) / ((far_distance - near_distance) + far_distance),
                             near_distance);
    }

extern "C" void NuTextureBlendEffect(i32 arg0, i32 arg1, NUVEC4 *parameters) {
    currentScene.texture_blend_arg0 = arg0;
    currentScene.texture_blend_arg1 = arg1;
    currentScene.texture_blend_enabled = 1;
    currentScene.texture_blend_parameters = *parameters;
}

    void NuCameraMotionBlurParams(const NUMTX *current) {
        currentScene.unknown_e0 = 1;
        currentScene.motion_current = *current;
    }

    void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff) {
        currentScene.unknown_e4 = 1;
        currentScene.motion_previous = *previous;
        currentScene.motion_current = *current;
        currentScene.motion_scale = scale;
        currentScene.motion_maximum = maximum;
        currentScene.motion_falloff = falloff;
    }

    void NuAccumulationMotionBlurParams(i32 frames, f32 blend, f32 mode) {
        currentScene.accumulation_blend = blend;
        currentScene.unknown_174 = 1;
        currentScene.accumulation_frames = frames;
        currentScene.accumulation_mode = mode;
    }

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

    void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, f32 mode) {
        currentScene.accumulation_blend = blend;
        currentScene.unknown_174 = 1;
        currentScene.unknown_178 = 1;
        currentScene.accumulation_frames = frames;
        currentScene.accumulation_mode = mode;
        motionBlurAccumActiveThisFrame = 1;
    }

    void NuDeferredShadingRender(void) {
        currentScene.unknown_48 = 1;
    }

    void NuDeferredShadingSetParameterf(i32 parameter, f32 value) {
        switch (parameter) {
            case 0:
                currentScene.unknown_4c = value;
                break;
            case 1:
                currentScene.unknown_50 = value;
                break;
            case 2:
                currentScene.unknown_54 = value;
                break;
        }
    }

extern "C++" {
enum nudeferredshadingenum_e : i32 {
    NU_DEFERRED_SHADING_PARAMETER_0 = 0,
    NU_DEFERRED_SHADING_PARAMETER_1 = 1,
    NU_DEFERRED_SHADING_PARAMETER_2 = 2,
};

void NuDynamicLightingGetParameterfv(nudeferredshadingenum_e parameter, float *out) {
    switch (parameter) {
        case NU_DEFERRED_SHADING_PARAMETER_0:
            *out = currentScene.unknown_4c;
            break;
        case NU_DEFERRED_SHADING_PARAMETER_1:
            *out = currentScene.unknown_50;
            break;
        case NU_DEFERRED_SHADING_PARAMETER_2:
            *out = currentScene.unknown_54;
            break;
    }
}
}

    void NuLightSpeedBlurOldCameraPos(const NUMTX *camera) {
        currentScene.speed_blur.current = *camera;
        NuMtxInv(&currentScene.speed_blur.current, &currentScene.speed_blur.current);
    }

    void NuBackbufferCopy(i32 texture_id) {
        currentScene.unknown_214 = static_cast<u32>(texture_id);
    }

}

void NuMarkSceneAsZPrePass() {
    currentScene.z_pre_pass = 1;
}

extern "C" {
extern "C" void NuRndrSetGlobalMinMipLevel(i32 level) {
    g_minmiplevel = level;
}

extern "C" void NuRndrSetGlobalMipMapBias(f32 bias) {
    g_mipmapbias = bias;
}

}

NUCAMERA *NuCameraCreate() {
    NUCAMERA *cam = NU_ALLOC_T(NUCAMERA, 1, "", NUMEMORY_CATEGORY_NONE);

    NuMtxSetIdentity(&cam->mtx);

    cam->near_clip = 0.15f;
    cam->far_clip = 10000.0f;
    cam->portal_nearclip = 0.0f;
    cam->fov = 0.75f;
    cam->aspect = 0.75f;

    cam->unknown_58 = 1.0f;
    cam->unknown_5c = 1.0f;
    cam->unknown_50 = 0.0f;
    cam->unknown_54 = 0.0f;

    cam->scale.z = 1.0f;
    cam->scale.y = 1.0f;
    cam->scale.x = 1.0f;

    cam->unknown_64 = 0.0f;
    cam->unknown_60 = 0.0f;

    return cam;
}

void NuCameraDestroy(NUCAMERA *cam) {
    if (cam != NULL) {
        NuMemoryGet()->GetThreadMem()->BlockFree(cam, 0);
    }
}

extern "C" {
    void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection) {
        static NUMTX vmtx;
        if (use_cached_projection == 0 || (clip->flags & NUCAMERA_CLIP_PROJECTION_VALID) == 0) {
            clip->flags |= NUCAMERA_CLIP_PROJECTION_VALID;
            i32 angle = (i32)(0.5f * camera->fov * 10430.378f);
            f32 cotangent = NuTrigTable[(angle + 0x4000) >> 1 & 0x7fff] / NuTrigTable[angle >> 1 & 0x7fff];
            f32 far_clip = clip->far_clip;
            f32 near_clip = clip->near_clip;
            f32 x = clip->x_scale * camera->aspect * cotangent;
            f32 y = cotangent * clip->y_scale;
            memset(&clip->projection, 0, sizeof(clip->projection));
            clip->projection.m00 = x;
            clip->projection.m11 = y;
            clip->projection.m22 = 2.0f / (far_clip - near_clip);
            clip->projection.m23 = 1.0f;
            clip->projection.m32 = -2.0f * near_clip / (far_clip - near_clip);
        }
        NuMtxInvVU0(&vmtx, &camera->mtx);
        NuMtxMulVU0(&clip->clip, &vmtx, &clip->projection);
    }

}

void NuCameraBuildClipPlanes(void) {
    f32 near_dist;
    f32 near_dist_sq;

    f32 left_right;
    f32 top_bottom;

    f32 left_right_inv;
    f32 top_bottom_inv;

    NUMTX frustum_planes;
    NUMTX scissor_planes;

    f32 dot;

    near_dist = global_camera.near_clip;
    near_dist_sq = global_camera.near_clip * global_camera.near_clip;

    // Build the frustum planes.
    left_right = zx * near_dist;
    top_bottom = zy * near_dist;

    left_right_inv = 1.0f / NuFsqrt(left_right * left_right + near_dist_sq);
    top_bottom_inv = 1.0f / NuFsqrt(top_bottom * top_bottom + near_dist_sq);

    NuMtxSetZero(&frustum_planes);

    frustum_planes.m00 = -near_dist * left_right_inv;
    frustum_planes.m20 = left_right * left_right_inv;

    frustum_planes.m01 = -frustum_planes.m00;
    frustum_planes.m21 = frustum_planes.m20;

    frustum_planes.m12 = -near_dist * top_bottom_inv;
    frustum_planes.m22 = top_bottom * top_bottom_inv;

    frustum_planes.m13 = -frustum_planes.m12;
    frustum_planes.m23 = frustum_planes.m22;

    // Build the scissor planes.
    left_right = zxs * near_dist;
    top_bottom = zys * near_dist;

    left_right_inv = 1.0f / NuFsqrt(left_right * left_right + near_dist_sq);
    top_bottom_inv = 1.0f / NuFsqrt(top_bottom * top_bottom + near_dist_sq);

    NuMtxSetZero(&scissor_planes);

    scissor_planes.m00 = -near_dist * left_right_inv;
    scissor_planes.m20 = left_right * left_right_inv;

    scissor_planes.m01 = -scissor_planes.m00;
    scissor_planes.m21 = scissor_planes.m20;

    scissor_planes.m12 = -near_dist * top_bottom_inv;
    scissor_planes.m22 = top_bottom * top_bottom_inv;

    scissor_planes.m13 = -scissor_planes.m12;
    scissor_planes.m23 = scissor_planes.m22;

    NuMtxMulH(&ClipPlanes.frustum_planes, &vmtx, &frustum_planes);
    NuMtxMulH(&ClipPlanes.scissor_planes, &vmtx, &scissor_planes);

    dot = global_camera.mtx.m20 * global_camera.mtx.m30 + global_camera.mtx.m21 * global_camera.mtx.m31 +
          global_camera.mtx.m22 * global_camera.mtx.m32;

    memcpy(&ClipPlanes.near_plane, NUMTX_GET_ROW_VEC(&global_camera.mtx, 2), sizeof(NUVEC));
    ClipPlanes.near_plane.w = -dot - global_camera.near_clip;

    ClipPlanes.near_far_planes.m00 = -global_camera.mtx.m20;
    ClipPlanes.near_far_planes.m10 = -global_camera.mtx.m21;
    ClipPlanes.near_far_planes.m20 = -global_camera.mtx.m22;
    ClipPlanes.near_far_planes.m30 = dot + global_camera.far_clip;

    ClipPlanes.near_far_planes.m01 = ClipPlanes.near_plane.x;
    ClipPlanes.near_far_planes.m11 = ClipPlanes.near_plane.y;
    ClipPlanes.near_far_planes.m21 = ClipPlanes.near_plane.z;
    ClipPlanes.near_far_planes.m31 = ClipPlanes.near_plane.w;

    ClipPlanes.near_far_planes.m02 = NuFabs(ClipPlanes.near_far_planes.m00);
    ClipPlanes.near_far_planes.m12 = NuFabs(ClipPlanes.near_far_planes.m10);
    ClipPlanes.near_far_planes.m22 = NuFabs(ClipPlanes.near_far_planes.m20);
    ClipPlanes.near_far_planes.m32 = NuFabs(ClipPlanes.near_far_planes.m30);

    ClipPlanes.near_far_planes.m03 = NuFabs(ClipPlanes.near_far_planes.m01);
    ClipPlanes.near_far_planes.m13 = NuFabs(ClipPlanes.near_far_planes.m11);
    ClipPlanes.near_far_planes.m23 = NuFabs(ClipPlanes.near_far_planes.m21);
    ClipPlanes.near_far_planes.m33 = NuFabs(ClipPlanes.near_far_planes.m31);

    ClipPlanes.abs_frustum_planes.m00 = NuFabs(ClipPlanes.frustum_planes.m00);
    ClipPlanes.abs_frustum_planes.m01 = NuFabs(ClipPlanes.frustum_planes.m01);
    ClipPlanes.abs_frustum_planes.m02 = NuFabs(ClipPlanes.frustum_planes.m02);
    ClipPlanes.abs_frustum_planes.m03 = NuFabs(ClipPlanes.frustum_planes.m03);

    ClipPlanes.abs_frustum_planes.m10 = NuFabs(ClipPlanes.frustum_planes.m10);
    ClipPlanes.abs_frustum_planes.m11 = NuFabs(ClipPlanes.frustum_planes.m11);
    ClipPlanes.abs_frustum_planes.m12 = NuFabs(ClipPlanes.frustum_planes.m12);
    ClipPlanes.abs_frustum_planes.m13 = NuFabs(ClipPlanes.frustum_planes.m13);

    ClipPlanes.abs_frustum_planes.m20 = NuFabs(ClipPlanes.frustum_planes.m20);
    ClipPlanes.abs_frustum_planes.m21 = NuFabs(ClipPlanes.frustum_planes.m21);
    ClipPlanes.abs_frustum_planes.m22 = NuFabs(ClipPlanes.frustum_planes.m22);
    ClipPlanes.abs_frustum_planes.m23 = NuFabs(ClipPlanes.frustum_planes.m23);

    ClipPlanes.abs_frustum_planes.m30 = 0.0f;
    ClipPlanes.abs_frustum_planes.m31 = 0.0f;
    ClipPlanes.abs_frustum_planes.m32 = 0.0f;
    ClipPlanes.abs_frustum_planes.m33 = 0.0f;

    ClipPlanes.abs_scissor_planes.m00 = NuFabs(ClipPlanes.scissor_planes.m00);
    ClipPlanes.abs_scissor_planes.m01 = NuFabs(ClipPlanes.scissor_planes.m01);
    ClipPlanes.abs_scissor_planes.m02 = NuFabs(ClipPlanes.scissor_planes.m02);
    ClipPlanes.abs_scissor_planes.m03 = NuFabs(ClipPlanes.scissor_planes.m03);

    ClipPlanes.abs_scissor_planes.m10 = NuFabs(ClipPlanes.scissor_planes.m10);
    ClipPlanes.abs_scissor_planes.m11 = NuFabs(ClipPlanes.scissor_planes.m11);
    ClipPlanes.abs_scissor_planes.m12 = NuFabs(ClipPlanes.scissor_planes.m12);
    ClipPlanes.abs_scissor_planes.m13 = NuFabs(ClipPlanes.scissor_planes.m13);

    ClipPlanes.abs_scissor_planes.m20 = NuFabs(ClipPlanes.scissor_planes.m20);
    ClipPlanes.abs_scissor_planes.m21 = NuFabs(ClipPlanes.scissor_planes.m21);
    ClipPlanes.abs_scissor_planes.m22 = NuFabs(ClipPlanes.scissor_planes.m22);
    ClipPlanes.abs_scissor_planes.m23 = NuFabs(ClipPlanes.scissor_planes.m23);

    ClipPlanes.abs_scissor_planes.m30 = 0.0f;
    ClipPlanes.abs_scissor_planes.m31 = 0.0f;
    ClipPlanes.abs_scissor_planes.m32 = 0.0f;
    ClipPlanes.abs_scissor_planes.m33 = 0.0f;
}

extern "C" {
    i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth) {
        NUMTX transform;
        NUVEC corners[8];
        NUVEC view[8];
        u8 results[8];
        if (far_clip == 0.0f)
            far_clip = global_camera.far_clip;
        if (world_mtx == NULL || world_mtx == &numtx_identity)
            transform = vmtx;
        else
            NuMtxMul(&transform, world_mtx, &vmtx);
        corners[0].x = min->x;
        corners[0].y = min->y;
        corners[0].z = min->z;
        corners[1].x = max->x;
        corners[1].y = max->y;
        corners[1].z = max->z;
        corners[2].x = min->x;
        corners[2].y = min->y;
        corners[2].z = max->z;
        corners[3].x = max->x;
        corners[3].y = max->y;
        corners[3].z = min->z;
        corners[4].x = min->x;
        corners[4].y = max->y;
        corners[4].z = min->z;
        corners[5].x = min->x;
        corners[5].y = max->y;
        corners[5].z = max->z;
        corners[6].x = max->x;
        corners[6].y = min->y;
        corners[6].z = min->z;
        corners[7].x = max->x;
        corners[7].y = min->y;
        corners[7].z = max->z;
        for (i32 i = 0; i < 8; ++i)
            NuVecMtxTransform(&view[i], &corners[i], &transform);
        *min_depth = 3.4028234663852886e+38f;
        for (i32 i = 0; i < 8; ++i)
            results[i] = 0;
        for (i32 i = 0; i < 8; ++i) {
            *min_depth = view[i].z < *min_depth ? view[i].z : *min_depth;
            if (view[i].z < global_camera.near_clip && (flags & NUCAMERA_EXTENTS_SKIP_NEAR) == 0)
                results[i] |= NUCAMERA_OUT_NEAR;
            if (view[i].z > far_clip)
                results[i] |= NUCAMERA_OUT_FAR;
            if (view[i].x < -view[i].z * zx)
                results[i] |= NUCAMERA_OUT_LEFT;
            if (view[i].x > view[i].z * zx)
                results[i] |= NUCAMERA_OUT_RIGHT;
            if (view[i].y < -view[i].z * zy)
                results[i] |= NUCAMERA_OUT_BOTTOM;
            if (view[i].y > view[i].z * zy)
                results[i] |= NUCAMERA_OUT_TOP;
        }
        if ((results[0] & results[1] & results[2] & results[3] & results[4] & results[5] & results[6] & results[7]) !=
            0)
            return 0;
        if ((results[0] | results[1] | results[2] | results[3] | results[4] | results[5] | results[6] | results[7]) ==
            0)
            return 1;
        if ((flags & NUCAMERA_EXTENTS_SKIP_SCISSOR) != 0)
            return 2;
        for (i32 i = 0; i < 8; ++i)
            results[i] = 0;
        for (i32 i = 0; i < 8; ++i) {
            if (view[i].z < global_camera.near_clip && (flags & NUCAMERA_EXTENTS_SKIP_NEAR) == 0)
                results[i] |= NUCAMERA_OUT_NEAR;
            if (view[i].z > far_clip)
                results[i] |= NUCAMERA_OUT_FAR;
            if (view[i].x < -view[i].z * zxs)
                results[i] |= NUCAMERA_OUT_LEFT;
            if (view[i].x > view[i].z * zxs)
                results[i] |= NUCAMERA_OUT_RIGHT;
            if (view[i].y < -view[i].z * zys)
                results[i] |= NUCAMERA_OUT_BOTTOM;
            if (view[i].y > view[i].z * zys)
                results[i] |= NUCAMERA_OUT_TOP;
        }
        if ((results[0] & results[1] & results[2] & results[3] & results[4] & results[5] & results[6] & results[7]) !=
            0)
            return 1;
        if ((results[0] | results[1] | results[2] | results[3] | results[4] | results[5] | results[6] | results[7]) ==
            0)
            return 1;
        return 2;
    }

    void NuCameraForceFarclip(i32 enabled) {
        force_camera_farclip = enabled;
    }

}

void NuCameraGet(NUCAMERA *out) {
    *out = global_camera;
}

extern "C" {
    void NuCameraSetAxes(NUVEC *axes) {
        cam_axes = *axes;
    }

    void NuCameraGetAxes(NUVEC *axes) {
        *axes = cam_axes;
    }

}

NUCAMERA *NuCameraGetCam(void) {
    return &global_camera;
}

NUMTX *NuCameraGetMtx(void) {
    return &global_camera.mtx;
}

void NuMtxCalcCheapFaceY_v2(NUMTX *m, NUVEC *v) {
    NUMTX *camera = NuCameraGetMtx();
    NuVecCross(NUMTX_GET_ROW_VEC(m, 0), NUMTX_GET_ROW_VEC(camera, 2), v);
    NuVecNorm(NUMTX_GET_ROW_VEC(m, 0), NUMTX_GET_ROW_VEC(m, 0));
    *NUMTX_GET_ROW_VEC(m, 1) = *v;
    NuVecCross(NUMTX_GET_ROW_VEC(m, 2), NUMTX_GET_ROW_VEC(m, 0), NUMTX_GET_ROW_VEC(m, 1));
    m->m33 = 1.0f;
    m->m23 = 0.0f;
    m->m13 = 0.0f;
    m->m03 = 0.0f;
    m->m32 = 0.0f;
    m->m31 = 0.0f;
    m->m30 = 0.0f;
}

void NuMtxCalcFaceOn(NUMTX *m, NUVEC *v) {
    NUVEC world_up = {0.0f, 1.0f, 0.0f};
    NUVEC right;
    NUVEC up;
    NUVEC forward;
    NUVEC camera_position;

    NuMtxGetTranslation(NuCameraGetMtx(), &camera_position);
    NuVecSub(&forward, &camera_position, v);
    NuVecNorm(&forward, &forward);

    f32 projection = NuVecDot(&world_up, &forward);
    up.x = world_up.x - forward.x * projection;
    up.y = world_up.y - forward.y * projection;
    up.z = world_up.z - forward.z * projection;
    NuVecNorm(&up, &up);
    NuVecCross(&right, &up, &forward);

    m->m00 = right.x;
    m->m10 = up.x;
    m->m20 = forward.x;
    m->m01 = right.y;
    m->m11 = up.y;
    m->m21 = forward.y;
    m->m02 = right.z;
    m->m12 = up.z;
    m->m22 = forward.z;
    m->m33 = 1.0f;
    m->m23 = 0.0f;
    m->m13 = 0.0f;
    m->m30 = v->x;
    m->m31 = v->y;
    m->m32 = v->z;
    m->m03 = 0.0f;
}

void NuMtxCalcFaceY(NUMTX *m, NUVEC *v) {
    NUVEC world_up = {0.0f, 1.0f, 0.0f};
    NUVEC right;
    NUVEC up;
    NUVEC forward;
    NUVEC camera_position;

    NuMtxGetTranslation(NuCameraGetMtx(), &camera_position);
    NuVecSub(&forward, &camera_position, v);
    NuVecCross(&right, &world_up, &forward);
    NuVecNorm(&right, &right);
    NuVecCross(&up, &world_up, &right);
    NuVecNorm(&up, &up);

    m->m00 = right.x;
    m->m10 = world_up.x;
    m->m20 = up.x;
    m->m01 = right.y;
    m->m11 = world_up.y;
    m->m21 = up.y;
    m->m02 = right.z;
    m->m12 = world_up.z;
    m->m22 = up.z;
    m->m33 = 1.0f;
    m->m23 = 0.0f;
    m->m13 = 0.0f;
    m->m03 = 0.0f;
    m->m30 = v->x;
    m->m31 = v->y;
    m->m32 = v->z;
}

NUMTX *NuCameraGetViewMtx(void) {
    return &vmtx;
}

void NuMtxCalcDebrisFaceOn(NUMTX *m) {
    NUMTX *view = NuCameraGetViewMtx();
    m->m00 = -view->m00;
    m->m10 = view->m01;
    m->m20 = -view->m02;
    m->m01 = -view->m10;
    m->m11 = view->m11;
    m->m21 = -view->m12;
    m->m02 = -view->m20;
    m->m12 = view->m21;
    m->m22 = -view->m22;
    m->m23 = 0.0f;
    m->m13 = 0.0f;
    m->m03 = 0.0f;
    m->m33 = 1.0f;
}

void NuMtxCalcCheapFaceY(NUMTX *m, NUVEC *v) {
    NUMTX *view = NuCameraGetViewMtx();
    NUVEC right;
    NUVEC up;

    right.x = -view->m00;
    right.y = 0.0f;
    right.z = -view->m20;
    NuVecNorm(&right, &right);

    m->m00 = right.x;
    m->m01 = right.y;
    m->m02 = right.z;

    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    m->m10 = up.x;
    m->m11 = up.y;
    m->m12 = up.z;

    NUVEC forward = {-view->m02, 0.0f, -view->m22};
    NuVecNorm(&forward, &forward);

    m->m20 = forward.x;
    m->m21 = forward.y;
    m->m22 = forward.z;
    m->m33 = 1.0f;
    m->m23 = 0.0f;
    m->m13 = 0.0f;
    m->m03 = 0.0f;
    m->m30 = v->x;
    m->m31 = v->y;
    m->m32 = v->z;
}

void NuMtxCalcCheapFaceOn(NUMTX *m, NUVEC *v) {
    NUMTX *view = NuCameraGetViewMtx();
    m->m00 = -view->m00;
    m->m10 = view->m01;
    m->m20 = -view->m02;
    m->m01 = -view->m10;
    m->m11 = view->m11;
    m->m21 = -view->m12;
    m->m02 = -view->m20;
    m->m12 = view->m21;
    m->m22 = -view->m22;
    m->m03 = 0.0f;
    m->m23 = 0.0f;
    m->m30 = v->x;
    m->m13 = 0.0f;
    m->m31 = v->y;
    m->m32 = v->z;
    m->m33 = 1.0f;
}

NUMTX *NuCameraGetProjectionMtx(void) {
    return &pmtx;
}

extern "C" {
    void NuLightSpeedBlur(i32 reuse_camera, f32 scale) {
        static NUMTX _viewProj, _preViewProj;
        if (speedblur_enabled == 0)
            return;
        currentScene.speed_blur.enabled = 1;
        if (reuse_camera == 0) {
            NuMtxMulH(&_preViewProj, &currentScene.speed_blur.current, NuCameraGetProjectionMtx());
            NUMTX *projection = NuCameraGetProjectionMtx();
            NuMtxMulH(&_viewProj, NuCameraGetViewMtx(), projection);
        }
        currentScene.speed_blur.previous = _viewProj;
        currentScene.speed_blur.current = _preViewProj;
        currentScene.speed_blur.unknown_84 = nuapi.frametime;
        currentScene.speed_blur.scale = scale;
    }

    NUMTX *NuCameraGetClippingMtx(void) {
        return &cmtx;
    }

}

NUMTX *NuCameraGetScalingMtx(void) {
    return &smtx;
}

extern "C" {
    NUMTX *NuCameraGetVPCSMtx(void) {
        return &vpsmtx;
    }

    NUMTX *NuCameraGetPCSMtx(void) {
        return &psmtx;
    }

}

NUMTX *NuCameraGetVPCMtx(void) {
    return &vpc_vport_mtx;
}

NUMTX *NuCameraGetVPMtx(void) {
    return &vpmtx;
}

bool OcclusionManager::IsOccludedOBB(nuvec_s const *minimum, nuvec_s const *maximum, numtx_s const *matrix) {
    if (!initialized || !enabled)
        return false;
    if (!current_set->queries_prepared) {
        NUMTX *projection = NuCameraGetVPMtx();
        NUMTX *view = NuCameraGetViewMtx();
        current_set->PrepareForQueries(view, projection);
    }
    ++unknown_164;
    bool occluded = current_set->IsOccludedOBB(minimum, maximum, matrix);
    if (occluded)
        ++unknown_160;
    return occluded;
}

extern "C" {
    i32 NuOcclusionManagerIsOccludedOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix) {
        return g_OcclusionManager.IsOccludedOBB(minimum, maximum, matrix);
    }

}

bool OcclusionManager::IsOccludedSphere(nuvec_s const *center, float radius) {
    if (!initialized || !enabled)
        return false;
    if (!current_set->queries_prepared) {
        NUMTX *projection = NuCameraGetVPMtx();
        NUMTX *view = NuCameraGetViewMtx();
        current_set->PrepareForQueries(view, projection);
    }
    ++unknown_164;
    bool occluded = current_set->IsOccludedSphere(center, radius);
    if (occluded)
        ++unknown_160;
    return occluded;
}

extern "C" {
    i32 NuOcclusionManagerIsOccludedSphere(const NUVEC *center, f32 radius) {
        return g_OcclusionManager.IsOccludedSphere(center, radius);
    }

}

void OcclusionManager::AddOccluder(nuvec_s const *a, nuvec_s const *b, nuvec_s const *c, nuvec_s const *d) {
    if (!initialized || !enabled || building_set->count >= building_set->capacity)
        return;
    if (unknown_158 > 0.0f || unknown_15c > 0.0f) {
        NUVEC4 projected[4];
        NuVec4MtxTransform(&projected[0], const_cast<NUVEC *>(a), NuCameraGetVPMtx());
        NuVec4MtxTransform(&projected[1], const_cast<NUVEC *>(b), NuCameraGetVPMtx());
        NuVec4MtxTransform(&projected[2], const_cast<NUVEC *>(c), NuCameraGetVPMtx());
        if (unknown_158 > 0.0f) {
            NUVEC ac = {projected[2].x - projected[0].x, projected[2].y - projected[0].y,
                        projected[2].z - projected[0].z};
            NUVEC ab = {projected[1].x - projected[0].x, projected[1].y - projected[0].y,
                        projected[1].z - projected[0].z};
            NUVEC normal = {ac.z * ab.y - ac.y * ab.z, ab.z * ac.x - ac.z * ab.x, ab.x * ac.y - ab.y * ac.x};
            NuVecNorm(&normal, &normal);
            if (unknown_158 > fabsf(normal.z))
                return;
        }
        NuVec4MtxTransform(&projected[3], const_cast<NUVEC *>(d), NuCameraGetVPMtx());
        float min_x = 1000000.0f, min_y = 1000000.0f;
        float max_x = -1000000.0f, max_y = -1000000.0f;
        for (u32 i = 0; i < 4; ++i) {
            NUVEC4 &v = projected[i];
            if (v.w < 0.001f)
                return;
            v.x /= v.w;
            v.y /= v.w;
            v.z /= v.w;
            min_x = v.x < min_x ? v.x : min_x;
            min_y = v.y < min_y ? v.y : min_y;
            max_x = v.x > max_x ? v.x : max_x;
            max_y = v.y > max_y ? v.y : max_y;
        }
        if (unknown_15c > 0.0f && unknown_15c > (max_x - min_x) * (max_y - min_y) * 0.25f)
            return;
    }
    building_set->AddOccluder(a, b, c, d);
}

extern "C" {
    void NuOcclusionManagerAddOccluderQuad(const NUVEC *a, const NUVEC *b, const NUVEC *c, const NUVEC *d) {
        g_OcclusionManager.AddOccluder(a, b, c, d);
    }

}

void OcclusionManager::AddOccluder(nuvec_s const *minimum, nuvec_s const *maximum, numtx_s const *matrix) {
    if (!initialized || !enabled)
        return;
    const f32 min_x = minimum->x;
    const f32 min_y = minimum->y;
    const f32 min_z = minimum->z;
    const f32 max_x = maximum->x;
    const f32 max_y = maximum->y;
    const f32 max_z = maximum->z;
    NUVEC4 corners[8] = {{min_x, max_y, min_z, 1.0f}, {max_x, max_y, max_z, 1.0f}, {max_x, min_y, max_z, 1.0f},
                         {min_x, min_y, min_z, 1.0f}, {min_x, max_y, max_z, 1.0f}, {max_x, max_y, min_z, 1.0f},
                         {max_x, min_y, min_z, 1.0f}, {min_x, min_y, max_z, 1.0f}};
    for (u32 i = 0; i < 8; ++i) {
        NUVEC *vertex = reinterpret_cast<NUVEC *>(&corners[i]);
        NuVecMtxTransform(vertex, vertex, const_cast<NUMTX *>(matrix));
    }
    AddOccluder(reinterpret_cast<NUVEC *>(&corners[0]), reinterpret_cast<NUVEC *>(&corners[1]),
                reinterpret_cast<NUVEC *>(&corners[2]), reinterpret_cast<NUVEC *>(&corners[3]));
    AddOccluder(reinterpret_cast<NUVEC *>(&corners[4]), reinterpret_cast<NUVEC *>(&corners[5]),
                reinterpret_cast<NUVEC *>(&corners[6]), reinterpret_cast<NUVEC *>(&corners[7]));
}

extern "C" {
    void NuOcclusionManagerAddOccluderOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix) {
        g_OcclusionManager.AddOccluder(minimum, maximum, matrix);
    }

}

void OcclusionManager::AddOccluder(nuvec_s const *center, float radius) {
    if (!initialized || !enabled)
        return;
    NUMTX *view = NuCameraGetViewMtx();
    NUVEC right = {view->m00 * radius, view->m10 * radius, view->m20 * radius};
    NUVEC up = {view->m01 * radius, view->m11 * radius, view->m21 * radius};
    NUVEC forward = {view->m02 * radius, view->m12 * radius, radius * view->m22};
    NUVEC origin = *center;
    NUVEC4 a = {((up.x + origin.x) - right.x) - forward.x, ((up.y + origin.y) - right.y) - forward.y,
                ((up.z + origin.z) - right.z) - forward.z, 1.0f};
    NUVEC4 b = {((up.x + origin.x) + right.x) + forward.x, ((up.y + origin.y) + right.y) + forward.y,
                ((up.z + origin.z) + right.z) + forward.z, 1.0f};
    NUVEC4 c = {(right.x + (origin.x - up.x)) + forward.x, (right.y + (origin.y - up.y)) + forward.y,
                (right.z + (origin.z - up.z)) + forward.z, 1.0f};
    NUVEC4 d = {((origin.x - up.x) - right.x) - forward.x, ((origin.y - up.y) - right.y) - forward.y,
                ((origin.z - up.z) - right.z) - forward.z, 1.0f};
    AddOccluder(reinterpret_cast<NUVEC *>(&a), reinterpret_cast<NUVEC *>(&b), reinterpret_cast<NUVEC *>(&c),
                reinterpret_cast<NUVEC *>(&d));
}

extern "C" {
    void NuOcclusionManagerAddOccluderSphere(const NUVEC *center, f32 radius) {
        g_OcclusionManager.AddOccluder(center, radius);
    }

    NUMTX *NuCameraGetPCMtx(void) {
        return &pc_vport_mtx;
    }

}

void NuCameraGetClipMtx(NUMTX *viewport, NUMTX *scissor) {
    if (viewport != NULL) {
        memcpy(viewport, &vpc_vport_mtx, sizeof(NUMTX));
    }

    if (scissor != NULL) {
        memcpy(scissor, &vpc_sci_mtx, sizeof(NUMTX));
    }
}

static void computePortalExtents(NUVEC **vertices, i16 *vertex_count, NUVEC *maximum, NUVEC *minimum) {
    static NUMTX clip;
    static NUMTX inv_clip_mtx;
    NuCameraGetClipMtx(&clip, NULL);
    NuMtxInvH(&inv_clip_mtx, &clip);
    NuVecMtxTransformH(maximum, &(*vertices)[0], &clip);
    *minimum = *maximum;
    for (i32 i = 1; i < *vertex_count; ++i) {
        NUVEC point;
        NuVecMtxTransformH(&point, &(*vertices)[i], &clip);
        NuVecMax(maximum, maximum, &point);
        NuVecMin(minimum, minimum, &point);
    }
    NuVecMin(maximum, maximum, &nuvec_one);
    NuVecMax(maximum, maximum, &nuvec_minus_one);
    NuVecMin(minimum, minimum, &nuvec_one);
    NuVecMax(minimum, minimum, &nuvec_minus_one);
}

NUMTX *NuCameraGetClipPlanes(void) {
    return &clip_planes;
}

void NuCameraRayCast(NUVEC *pnt, f32 x, f32 y) {
    pnt->x = x * zx;
    pnt->y = y * zy;
    pnt->z = 1.0f;

    NuVecMtxRotate(pnt, pnt, &global_camera.mtx);
}

extern "C" {
    void NuCameraClearStateBuffer(void) {
        cam_state_count = 0;
    }

    i32 NuCameraSaveState(void) {
        if (cam_state_count >= 16)
            return 0;
        NUCAMERASTATE *state = &cam_state[cam_state_count];
        ++cam_state_count;
        state->camera = global_camera;
        state->view = vmtx;
        state->projection = pmtx;
        state->screen = smtx;
        state->view_projection = vpmtx;
        state->view_projection_scissor = vpc_sci_mtx;
        state->view_projection_viewport = vpc_vport_mtx;
        state->viewport = pc_vport_mtx;
        state->view_projection_screen = vpsmtx;
        state->projection_screen = psmtx;
        state->effects = camfx;
        state->zx = zx;
        state->zy = zy;
        state->zxs = zxs;
        state->zys = zys;
        state->planes = ClipPlanes;
        return cam_state_count;
    }

    void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix) {
        NUMTX transform;
        NUVEC *end = world + count;
        if (matrix == NULL)
            transform = vmtx;
        else
            NuMtxMul(&transform, matrix, &vmtx);
        while (world < end)
            NuVecMtxTransform(view++, world++, &transform);
    }

    void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix) {
        NUVEC *end = world + count;
        NUMTX transform;
        if (matrix == NULL)
            transform = vpc_vport_mtx;
        else
            NuMtxMulH(&transform, matrix, &vpc_vport_mtx);
        while (world < end)
            NuVecMtxTransformH(screen++, world++, &transform);
    }

extern "C++" {
void NuCameraTransformScissorClip(nuvec_s *screen, nuvec_s *world, i32 count, numtx_s *matrix) {
    NUVEC *end = world + count;
    NUMTX transform;
    if (matrix == NULL)
        transform = vpc_sci_mtx;
    else
        NuMtxMulH(&transform, matrix, &vpc_sci_mtx);
    while (world < end)
        NuVecMtxTransformH(screen++, world++, &transform);
}
}

    void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix) {
        NUMTX transform;
        NUVEC *end = world + count;
        if (matrix == NULL)
            transform = vpsmtx;
        else
            NuMtxMulH(&transform, matrix, &vpsmtx);
        while (world < end)
            NuVecMtxTransformH(screen++, world++, &transform);
    }

    void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix) {
        NUMTX_ALIGNED16 transform; // The original realigns the frame for this matrix.
        NUVEC4 *end = world + count;
        if (matrix == NULL)
            transform = vpsmtx;
        else
            NuMtxMulVU0(&transform, matrix, &vpsmtx);
        while (world < end)
            NuVec4MtxTransformHVU0(screen++, world++, &transform);
    }

}

SAGA_HOST_WEAK i32 NuCameraClipTestSphere(NUVEC *pnt, f32 radius, NUMTX *world_mtx) {
    NUCAMERA *cam = NuCameraGetCam();
    NUMTX *view = NuCameraGetViewMtx();

    NUVEC pnt2;
    if (world_mtx != NULL) {
        NuVecMtxTransform(&pnt2, pnt, world_mtx);
        NuVecMtxTransform(&pnt2, &pnt2, view);
    } else {
        NuVecMtxTransform(&pnt2, pnt, view);
    }

    if (0.0f > (pnt2.z - cam->near_clip) + radius) {
        return 1;
    }
    if (0.0f > (cam->far_clip - pnt2.z) + radius) {
        return 1;
    }

    NUVEC4 g_dot;
    NuVec4MtxTransform(&g_dot, &pnt2, NuCameraGetClipPlanes());
    if (-radius > g_dot.x) {
        return 1;
    }
    if (-radius > g_dot.y) {
        return 1;
    }
    if (-radius > g_dot.z) {
        return 1;
    }
    if (-radius > g_dot.w) {
        return 1;
    }

    return 0;
}

extern "C" {
    i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx) {
        enum CLIP_POINT_FLAGS {
            CLIP_POINT_LEFT = 0x01,
            CLIP_POINT_RIGHT = 0x02,
            CLIP_POINT_TOP = 0x04,
            CLIP_POINT_BOTTOM = 0x08,
            CLIP_POINT_FAR = 0x10,
            CLIP_POINT_NEAR = 0x20,
        };

        NUMTX clip_matrix;
        if (world_mtx != NULL) {
            NuMtxMulH(&clip_matrix, world_mtx, &vmtx);
        } else {
            clip_matrix = vmtx;
        }

        i32 common_flags = -1;
        for (i32 index = 0; index < count; ++index) {
            NUVEC transformed;
            NuVecMtxTransform(&transformed, &points[index], &clip_matrix);

            i32 flags = 0;
            if (transformed.z < global_camera.near_clip) {
                flags |= CLIP_POINT_NEAR;
            }
            if (transformed.z > global_camera.far_clip) {
                flags |= CLIP_POINT_FAR;
            }
            if (transformed.x < -transformed.z * zx) {
                flags |= CLIP_POINT_LEFT;
            }
            if (transformed.x > transformed.z * zx) {
                flags |= CLIP_POINT_RIGHT;
            }
            if (transformed.y < -transformed.z * zy) {
                flags |= CLIP_POINT_BOTTOM;
            }
            if (transformed.y > transformed.z * zy) {
                flags |= CLIP_POINT_TOP;
            }
            common_flags &= flags;
        }
        return common_flags;
    }

    i32 NuCameraClipTestPointVport(NUVEC *point) {
        return NuVecClipTestPointVU0(point, &vpc_vport_mtx) == 0;
    }

    i32 NuCameraClipTestPointScissor(NUVEC *point) {
        return NuVecClipTestPointVU0(point, &vpc_sci_mtx) == 0;
    }

}

f32 NuCameraDistSqr(NUVEC *v) {
    return NuVecDistSqr(v, NUMTX_GET_ROW_VEC(&global_camera.mtx, 3), NULL);
}

f32 NuCameraDist(NUVEC *v) {
    return NuVecDist(v, NUMTX_GET_ROW_VEC(&global_camera.mtx, 3), NULL);
}

void NuCameraGetTrans(NUVEC *v) {
    memcpy(v, &global_camera.mtx.m30, sizeof(NUVEC));
}

void NuCameraGetPosition(NUVEC *v) {
    memcpy(v, &global_camera.mtx.m30, sizeof(NUVEC));
}

extern "C" {
    void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical) {
        *horizontal = zx;
        *vertical = zy;
    }

    f32 NuCameraFocalLenToFOV(f32 focal_length) {
        return NuAtan2DAF(18.0f / focal_length, 1.0f) * 0.0000958738019107841f * 2.0f;
    }

    f32 NuCameraFOVToFocalLen(f32 fov) {
        i32 angle = (i32)(0.5f * fov * 10430.378f);
        return 18.0f / (NuTrigTable[angle >> 1 & 0x7fff] / NuTrigTable[(angle + 0x4000) >> 1 & 0x7fff]);
    }

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

    f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop) {
        f32 fstop = root_fstop * root_fstop;
        if (fstop == 0.0f || focal_length == 0.0f)
            return 0.0f;
        return focal_length / fstop;
    }

    f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture) {
        f32 ratio;
        if (aperture == 0.0f || focal_length == 0.0f)
            ratio = 0.0f;
        else
            ratio = focal_length / aperture;
        return NuFsqrt(ratio);
    }

}

void NuCameraCalcRay(float screen_x, float screen_y, NUVEC *ray_start, NUVEC *ray_end, NUCAMERA *cam) {
    NUVEC near_point;
    NUVEC far_point;

    if (cam == NULL) {
        cam = &global_camera;
    }

    float tan = NU_TAN_LUT(cam->fov * 0.5f * 10430.378f);
    near_point = far_point = {
        .x = (screen_x + screen_x - 1.0f) * tan / cam->aspect,
        .y = (1.0f - (screen_y + screen_y)) * tan,
        .z = 1.0f,
    };

    near_point.x *= cam->near_clip;
    near_point.y *= cam->near_clip;
    near_point.z *= cam->near_clip;
    far_point.x *= cam->far_clip;
    far_point.y *= cam->far_clip;
    far_point.z *= cam->far_clip;

    NuVecMtxTransform(ray_start, &near_point, &cam->mtx);
    NuVecMtxTransform(ray_end, &far_point, &cam->mtx);
}

extern "C" {
    void NuVisiOcclusion(NuVisibilityResult *result) {
        NUMTX *camera = NuCameraGetMtx();
        NuVisibilityOcclusionGrid *grid = static_cast<NuVisibilityOcclusionGrid *>(result->occlusion_data);
        f32 x = camera->m30;
        if (x < grid->min_x || x > grid->max_x) {
            return;
        }
        f32 z = camera->m32;
        if (z < grid->min_z || z > grid->max_z) {
            return;
        }

        f32 grid_x = (x - grid->min_x) * grid->scale_x;
        f32 grid_z = (z - grid->min_z) * grid->scale_z;
        i32 column = static_cast<i32>(grid_x);
        if (static_cast<f32>(column) > grid_x) {
            --column;
        }
        i32 row = static_cast<i32>(grid_z);
        if (static_cast<f32>(row) > grid_z) {
            --row;
        }
        i32 cell_index = row * grid->width + column;
        u8 *volatile *cell_slot = grid->cells + cell_index;
        u8 *cell = *cell_slot;
        result->instance_tree_bits = cell;
        UsingOcclusionData = cell != NULL;
        if (grid->mode != 1 || cell == NULL) {
            return;
        }

        i32 *last_view_box = &CurrentViewBox;
        u8 *output = OcclusionBitArray;
        result->instance_tree_bits = output;
        if (cell_index == CurrentViewBox) {
            return;
        }
        *last_view_box = cell_index;
        u8 *end = OcclusionBitArray + grid->output_size;
        u8 *input = *cell_slot;
        while (output < end) {
            u8 command = *input++;
            u32 length = command >> 2;
            u8 kind = command & 3;
            if (kind > static_cast<u8>(1)) {
                memmove(output, input, length);
                input += length;
            } else {
                u8 fill = -kind;
                memset(output, fill, length);
            }
            output += length;
        }
    }

extern "C" void NuVisiBoxTree(NuBoxTreeVisibility *visibility, nugscn_s *scene) {
    if (visibility->tree != NULL && do_boxtree != 0) {
        for (u32 i = 0; i < visibility->tree->root_count; ++i) {
            nuvisiboxtree_s *tree = visibility->tree;
            BoxTreeRndrRec(tree, visibility->clip_results, &tree->nodes[tree->roots[i]], visibility->instance_count,
                           tree->far_clip[i], scene);
        }
        visibility->state |= 8;
    } else {
        visibility->state &= ~8;
    }
}

}

void NuHtmlFlush(i32 force) {
    if ((txt > flush) | force) {
        NuFileWriteString(hfh, buffer);
        txt = buffer;
        flush = buffer + 0xc00;
    }
}

extern "C" {
extern "C" void NuHtmlBegin(void *file) {
    hfh = static_cast<NUFILE>(reinterpret_cast<usize>(file));
    txt = buffer;
    flush = buffer + 0xc00;
}

extern "C" void NuHtmlWrite(const char *text, ...) {
    if (text == NULL || text[0] == '\0') {
        text = unknown;
    }
    va_list ap;
    va_start(ap, text);
    vsprintf(txt, text, ap);
    va_end(ap);
    txt += NuStrLen(txt);
    NuHtmlFlush(0);
}

extern "C" void NuHtmlHeading1(const char *fmt, ...) {
    if (fmt == NULL || fmt[0] == '\0') {
        fmt = unknown;
    }
    NuHtmlWrite("<table width=100%c bgcolor=#CFCFE5><tr><td><font face=arial size=+3> ", '%');
    va_list ap;
    va_start(ap, fmt);
    vsprintf(txt, fmt, ap);
    va_end(ap);
    txt += NuStrLen(txt);
    NuHtmlWrite("</font></table>\n");
}

extern "C" void NuHtmlHeading2(const char *fmt, ...) {
    if (fmt == NULL || fmt[0] == '\0') {
        fmt = unknown;
    }
    NuHtmlWrite("<table width=100%c bgcolor=#DFDFE5><tr><td><font face=arial size=+2> ", '%');
    va_list ap;
    va_start(ap, fmt);
    vsprintf(txt, fmt, ap);
    va_end(ap);
    txt += NuStrLen(txt);
    NuHtmlWrite("</font></table>\n");
}

extern "C" void NuHtmlHeading3(const char *fmt, ...) {
    if (fmt == NULL || fmt[0] == '\0') {
        fmt = unknown;
    }
    NuHtmlWrite("<table width=100%c bgcolor=#FFDFE5><tr><td><font face=arial size=+0> ", '%');
    va_list ap;
    va_start(ap, fmt);
    vsprintf(txt, fmt, ap);
    va_end(ap);
    txt += NuStrLen(txt);
    NuHtmlWrite("</font></table>\n");
}

extern "C" void NuHtmlBanner(void) {
    NuHtmlWrite("<table   width=100%c height=20 bgcolor=#FF0000><tr><td><font face=arial size=+2></font></table>", '%');
}

extern "C" void NuHtmlEnd(void) {
    NuHtmlFlush(1);
    hfh = 0;
}

extern "C" void NuHtmlBitmap(char *filename, i32 width, i32 height, char *caption, char *source) {
    char tag[256];
    NuHtmlWrite("<P>");
    if (height != 0 && width != 0) {
        sprintf(tag, "<IMG ALIGN=\"left\" HEIGHT=\"%d\" WIDTH=\"%d\" SRC=\"%s\">", height, width, filename);
    } else {
        sprintf(tag, "<IMG ALIGN=\"left\" SRC=\"%s\">", filename);
    }
    NuHtmlWrite(tag);
    if (caption != NULL) {
        NuHtmlWrite(caption);
    }
    NuHtmlWrite("<BR CLEAR=\"all\"> <P>&nbsp;<P>");
    if (!(source != NULL && NuStrCmp(filename, source) != 0)) {
        return;
    }
    NuFileCopy(filename, source);
}

}

void NuHtmlTitle(char *title) {
    NuHtmlWrite("<HR><Center><B>");
    NuHtmlWrite(title);
    NuHtmlWrite("</B></Center><HR>");
    NuHtmlWrite("<P><P>");
}

void NuHtmlGraphArray(char **strings) {
    char *text = *strings++;
    while (text != NULL) {
        NuHtmlWrite(text);
        text = *strings++;
    }
}

extern "C" {
    void NuHtmlHBarGraph(char *title, i32 width, i32 height, i32 *values, i32 count, i32 maximum, char **labels,
                         u32 *colours, i32 colour_count) {
        char text[256];
        sprintf(text,
                "<TABLE bgColor=white height=16 width=%d cellSpacing=0 cellPadding=\"1\" border=1>"
                "<TR><TD><Center>%s</Center></TD></TR></TABLE>\r\n",
                width, title);
        NuHtmlWrite(text);
        sprintf(text,
                "<TABLE bgColor=white height=%d width=%d cellSpacing=0 cellPadding=0 border=1>"
                "<tr><TD VAlign=top>\r\n",
                height, width);
        NuHtmlWrite(text);
        i32 row_height = height / (count + 1);
        sprintf(text, "<TABLE height=%d width=\"100%%%%\" cellSpacing=1 cellPadding=0 border=0 >\r\n", row_height);
        NuHtmlWrite(text);
        f32 scale = 88.0f / static_cast<f32>(maximum);
        const char *tick = "<TD width=\"%d%%%%\" bgColor=gray Align=right>"
                           "<DIV style=\"FONT-SIZE: 10pt; WIDTH: 10pt;\">%d</DIV></TD>\r\n";
        sprintf(text, tick, 12, 0);
        NuHtmlWrite(text);
        i32 quarter = maximum / 4;
        sprintf(text, tick, 22, quarter);
        NuHtmlWrite(text);
        sprintf(text, tick, 22, quarter * 2);
        NuHtmlWrite(text);
        sprintf(text, tick, 22, quarter * 3);
        NuHtmlWrite(text);
        sprintf(text, tick, 22, quarter * 4);
        NuHtmlWrite(text);
        sprintf(text, "</TD></tr></TABLE>\r\n\r\n");
        NuHtmlWrite(text);
        i32 colour_index = 0;
        for (i32 index = 0; index < count; ++index) {
            i32 bar_width = static_cast<i32>(static_cast<f32>(values[index]) * scale);
            if (bar_width > 88) {
                bar_width = 88;
            }
            u32 colour = 0;
            if (colours != NULL && colour_count != 0) {
                colour = colours[colour_index++];
                if (colour_index >= colour_count) {
                    colour_index = 0;
                }
            }
            sprintf(text, "<TABLE height=%d width=\"100%%%%\" cellSpacing=1 cellPadding=0 border=0><TR>\r\n",
                    row_height);
            NuHtmlWrite(text);
            const char *label = " ";
            if (labels != NULL && labels[index] != NULL) {
                label = labels[index];
            }
            sprintf(text,
                    "<TD align=right width=\"%d%%%%\" bgColor=lightgrey>"
                    "<DIV style=\"FONT-SIZE: 10pt\">%s</TD>\r\n",
                    12, label);
            NuHtmlWrite(text);
            sprintf(text, "<TD width=\"%d%%%%\" bgColor=%06X></TD>\r\n", bar_width, colour);
            NuHtmlWrite(text);
            sprintf(text, "<TD width=50 bgColor=white></TD></TR></TABLE>\r\n");
            NuHtmlWrite(text);
        }
        sprintf(text, "</TD></tr></TABLE>\r\n\r\n");
        NuHtmlWrite(text);
    }

    void NuHtmlVBarGraph(char *title, i32 width, i32 height, i32 *values, i32 count, i32 maximum, char **labels,
                         u32 *colours, i32 colour_count) {
        char text[256];
        i32 bar_width = static_cast<i32>(static_cast<f32>(width - 50) / static_cast<f32>(count));
        sprintf(text,
                "<TABLE bgColor=white height=16 width=%d cellSpacing=0 cellPadding=\"1\" border=1>"
                "<TR><TD><Center>%s</Center></TD></TR></TABLE>\r\n",
                width + 5, title);
        NuHtmlWrite(text);
        sprintf(text,
                "<TABLE bgColor=white height=%d width=%d cellSpacing=0 cellPadding=\"1\" border=1 "
                "VAlign=bottom><TR><TD>\r\n",
                height, width);
        NuHtmlWrite(text);
        sprintf(text, "<TABLE height=\"100%%%%\" width=%d cellSpacing=1 cellPadding=0 border=0 Align=left>\r\n", 50);
        NuHtmlWrite(text);
        f32 scale = 80.0f / static_cast<f32>(maximum);
        i32 quarter = maximum / 4;
        i32 tick_value = maximum;
        const char *tick = "<TR><TD valign=top align=right Height=\"%d%%%%\" bgColor=gray>"
                           "<DIV style=\"FONT-SIZE: 10pt\">%d</TD></TR>\r\n";
        sprintf(text, tick, 20, tick_value);
        NuHtmlWrite(text);
        tick_value -= quarter;
        sprintf(text, tick, 20, tick_value);
        NuHtmlWrite(text);
        tick_value -= quarter;
        sprintf(text, tick, 20, tick_value);
        NuHtmlWrite(text);
        tick_value -= quarter;
        sprintf(text, tick, 20, tick_value);
        NuHtmlWrite(text);
        sprintf(text, tick, 20, 0);
        NuHtmlWrite(text);
        NuHtmlWrite("</CENTER></TABLE>\r\n");
        i32 colour_index = 0;
        for (i32 index = 0; index < count; ++index) {
            u32 colour = 0;
            if (colours != NULL && colour_count != 0) {
                colour = colours[colour_index++];
                if (colour_index >= colour_count) {
                    colour_index = 0;
                }
            }
            i32 bar_height = static_cast<i32>(static_cast<f32>(values[index]) * scale);
            sprintf(text,
                    "<TABLE height=\"100%%%%\" width=%d cellSpacing=1 cellPadding=0 border=0 "
                    "Align=left><TR><TD></TD></TR>\r\n",
                    bar_width);
            NuHtmlWrite(text);
            if (bar_height > 80) {
                bar_height = 80;
            }
            sprintf(text, "<TR><TD Height=\"%d%%%%\" bgColor=%06X></TD></TR>\r\n", bar_height, colour);
            NuHtmlWrite(text);
            const char *label = " ";
            if (labels != NULL && labels[index] != NULL) {
                label = labels[index];
            }
            sprintf(text,
                    "<TR><TD Height=\"%d%%%%\" bgColor=lightgrey><DIV style=\"FONT-SIZE: 10pt; WIDTH: 10pt; "
                    "WRITING-MODE: tb-rl\">%s</DIV></TD></TR></TABLE>\r\n",
                    20, label);
            NuHtmlWrite(text);
        }
        NuHtmlWrite("</TABLE></TD></tr></TABLE>\r\n");
    }

}

void setpoint(f32 x) {
    curx = x;
    cury = 0.0f;
}

void setnextpoint(f32 x, f32 y) {
    nextx = x;
    dx = (x - curx) / y;
    size = static_cast<i32>(y);
}

i32 getnextdatapoint(f32 *value, i32 *delta) {
    const f32 next_y = cury + 1.0f;
    const f32 old_x = curx;
    *value = old_x;
    const i32 old_value = static_cast<i32>(old_x);
    cury = next_y;
    --size;
    const f32 next_x = old_x + dx;
    curx = next_x;
    *delta = static_cast<i32>(next_x) - old_value;
    if (size != 0) {
        return 0;
    }
    curx = nextx;
    return -1;
}

extern "C" {
    void NuHtmlHLineGraph(char *title, i32 width, i32 height, i32 *values, i32 count, i32 maximum, char **labels) {
        char text[256];
        sprintf(text,
                "<TABLE bgColor=white height=16 width=%d cellSpacing=0 cellPadding=\"1\" border=1>"
                "<TR><TD><Center>%s</Center></TD></TR></TABLE>\r\n",
                width, title);
        NuHtmlWrite(text);
        sprintf(text,
                "<TABLE bgColor=white height=%d width=%d cellSpacing=0 cellPadding=0 border=1>"
                "<tr><TD VAlign=top>\r\n",
                height, width);
        NuHtmlWrite(text);
        sprintf(text, "<TABLE height=10 width=\"100%%%%\" cellSpacing=0 cellPadding=0 border=0 >\r\n");
        NuHtmlWrite(text);

        i32 quarter_width = static_cast<i32>(static_cast<f32>(width) * 0.88f * 0.25f);
        i32 tick_width = static_cast<i32>(static_cast<f32>(quarter_width) / 600.0f * 100.0f);
        i32 label_width = static_cast<i32>(static_cast<f32>(width) * 0.12f);
        f32 scale = static_cast<f32>(quarter_width * 4) / static_cast<f32>(maximum);
        const char *tick = "<TD width=\"%d%%%%\" bgColor=gray Align=right>"
                           "<DIV style=\"FONT-SIZE: 10pt; WIDTH: 10pt;\">%d</DIV></TD>\r\n";
        sprintf(text, tick, 12, 0);
        NuHtmlWrite(text);
        setpoint(static_cast<f32>(values[0]) * scale);
        setnextpoint(static_cast<f32>(values[0]) * scale, 8.0f);
        i32 quarter_maximum = maximum / 4;
        sprintf(text, tick, tick_width, quarter_maximum);
        NuHtmlWrite(text);
        sprintf(text, tick, tick_width, quarter_maximum * 2);
        NuHtmlWrite(text);
        sprintf(text, tick, tick_width, quarter_maximum * 3);
        NuHtmlWrite(text);
        sprintf(text, tick, tick_width, quarter_maximum * 4);
        NuHtmlWrite(text);
        sprintf(text, "</TD></tr></TABLE>");
        NuHtmlWrite(text);

        i32 next_value = 1;
        for (i32 row = 0; row < count; ++row) {
            char *label = labels != NULL ? labels[row] : NULL;
            sprintf(text,
                    "<TABLE  BORDER=0 CELLPADDING=0 CELLSPACING=0><TD width=\"12%%%%\" "
                    "height=30 bgColor=lightgrey> <DIV style=\"FONT-SIZE: 10pt\">%s</TD>\r\n",
                    label != NULL ? label : " ");
            NuHtmlWrite(text);
            sprintf(text, "<TD width=\"88%%%%\" bgColor=green>\r\n");
            NuHtmlWrite(text);
            for (i32 line = 0; line < 16; ++line) {
                f32 value;
                i32 delta;
                if (getnextdatapoint(&value, &delta) < 0) {
                    setnextpoint(static_cast<f32>(values[next_value++]) * scale, 17.0f);
                    getnextdatapoint(&value, &delta);
                    if (row + 1 == count) {
                        dx = 0.0f;
                    }
                }
                if (delta < 0) {
                    value += static_cast<f32>(delta);
                    delta = -delta;
                } else if (delta == 0) {
                    delta = 1;
                }
                i32 before = static_cast<i32>(value);
                i32 after = width - (label_width + before) - delta - 4;
                sprintf(text,
                        "  <TABLE width=\"100%%%%\" BORDER=0 CELLPADDING=0 CELLSPACING=0> "
                        "<TD width=%d height=2 bgColor=white></TD> <TD width=%d bgColor=black> </TD> "
                        "<TD width=%d  bgColor=white> </TD></TABLE>\r\n",
                        before, delta, after);
                NuHtmlWrite(text);
            }
            sprintf(text, "<TR></TD> </TD></TABLE> \r\n\r\n");
            NuHtmlWrite(text);
        }
        sprintf(text, "</TABLE>");
        NuHtmlWrite(text);
    }

// Retail does not turn the clone call into a sibling call.
extern "C" __attribute__((optimize("no-optimize-sibling-calls"))) i32 NuTimeBarCreateSetEx(VARIPTR *buffer,
                                                                                           VARIPTR unused_buffer,
                                                                                           i32 *colours) {
    return CreateTimeBar(buffer, colours, 16);
}

extern "C" i32 NuTimeBarCreateSetEx2(VARIPTR *buffer, VARIPTR unused_buffer, i32 field_28) {
    i32 set = NuTimeBarCreateSetEx(buffer, unused_buffer, NULL);
    NuTimeBar_SetList[set + 1]->field_28 = field_28;
    return set;
}

extern "C" i32 NuTimeBarCreateSet(i32 *colours) {
    return NuTimeBarCreateSetEx(NULL, VARIPTR{}, colours);
}

extern "C" void NuTimeBarDestroySet(i32 set) {
    TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
    if (timebar->uses_buffer == 0) {
        NU_FREE(timebar->accumulators[0]);
        NU_FREE(timebar->accumulators[1]);
        NU_FREE(timebar->start_times);
        NU_FREE(timebar->field_14);
        NU_FREE(timebar->field_18);
        NU_FREE(timebar->colours);
        NU_FREE(timebar->toggle_flags);
        NU_FREE(timebar->slot_names);
        NU_FREE(timebar);
    }
    NuTimeBar_SetList[set + 1] = NULL;
}

// original 0x2d7630
// Reset the accumulator for a single slot, or — when called as
// Reset(-1, 0) — reset every slot in set 0.  The per-slot toggle is
// flipped so the next End/SlotSet lands in the other buffer, then the
// newly-inactive accumulator is zeroed.
extern "C" void NuTimeBarSlotReset(i32 set, i32 slot) {
    if (NuTimeBar_Initialised == 0) {
        return;
    }

    if (slot == 0 && set == -1) {
        TimeBarSet *timebar = NuTimeBar_SetList[0];
        if (timebar->slot_count > 0) {
            do {
                timebar->toggle_flags[slot] ^= 1;
                timebar->accumulators[timebar->toggle_flags[slot] == 0][slot] = 0;
                ++slot;
            } while (slot < timebar->slot_count);
        }
    } else {
        TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
        timebar->toggle_flags[slot] ^= 1;
        timebar->accumulators[timebar->toggle_flags[slot] == 0][slot] = 0;
    }
}

// original 0x2d76b0
// Mark the beginning of a timed region.  Records the current tick and
// the label pointer for the slot; the matching End computes the delta.
extern "C" void _NuTimeBarSlotBegin(i32 set, i32 slot, const char *name) {
    const i32 initialised = NuTimeBar_Initialised;
    if (initialised == 0) {
        return;
    }

    TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
    NuTimeGet(&timebar->start_times[slot]);
    timebar->slot_names[slot] = name;
}

// original 0x2d7710
// Close the timed region opened by _NuTimeBarSlotBegin, accumulate the
// elapsed microseconds into the double-buffered slot, and return the
// updated accumulator value.  Returns 0 when profiling is disabled.
extern "C" u32 _NuTimeBarSlotEnd(i32 set, i32 slot) {
    NUTIME now;
    u32 value = 0;
    if (NuTimeBar_Initialised != 0) {
        TimeBarSet *timebar = NuTimeBar_SetList[set + 1];

        NuTimeGet(&now);

        NUTIME delta;
        NuTimeSub(&delta, &now, &timebar->start_times[slot]);
        f32 elapsed_us = NuTimeMicroSeconds(&delta);

        timebar->accumulators[1 - timebar->toggle_flags[slot]][slot] += static_cast<i32>(elapsed_us);
        value = static_cast<u32>(timebar->accumulators[1 - timebar->toggle_flags[slot]][slot]);
    }
    return value;
}

// original 0x2d77c0
// Directly overwrite the accumulator for a slot (used by the render
// thread to publish GPU timings that come from GL queries rather than
// CPU wall-clock).  Respects the same double-buffer selection as End.
extern "C" void NuTimeBarSlotSet(i32 set, i32 slot, i32 value) {
    if (NuTimeBar_Initialised != 0) {
        TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
        i32 accumulator = 1 - timebar->toggle_flags[slot];
        timebar->accumulators[accumulator][slot] = value;
    }
}

// original 0x2d7820
// Update the display name for a slot without touching its timing.
extern "C" void NuTimeBarSlotSetName(i32 set, i32 slot, const char *name) {
    if (NuTimeBar_Initialised != 0) {
        TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
        timebar->slot_names[slot] = name;
    }
}

extern "C" void NuTimeBarSlotSetEx(i32 set, i32 slot, i32 value, const char *name) {
    if (NuTimeBar_Initialised == 0) {
        return;
    }
    if (slot == 6 && set == -1) {
        value =
            static_cast<i32>(static_cast<u32>(static_cast<f64>(static_cast<f32>(static_cast<u32>(value))) * 63.556));
    }
    TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
    i32 accumulator = 1 - timebar->toggle_flags[slot];
    timebar->accumulators[accumulator][slot] = value;
    timebar->slot_names[slot] = name;
}

extern "C" i32 NuTimeBarSlotLastValue(i32 set, i32 slot) {
    TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
    u32 elapsed = static_cast<u32>(timebar->accumulators[timebar->toggle_flags[slot]][slot]);
    return static_cast<i32>((static_cast<f32>(elapsed) * 60.0f / 1000000.0f) * 262.5f);
}

extern "C" i32 NuTimeBarSlotLastValueMicroseconds(i32 set, i32 slot) {
    TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
    return timebar->accumulators[timebar->toggle_flags[slot]][slot];
}

extern "C" void NuTimeBarResetPeaks(void) {
    NuTimeBar_PeakReset = 1;
}

extern "C" void NuTimeBarSetScaleY(void) {
    STUBBED();
}

extern "C" void NuTimeBarEnable(i32 enabled) {
    NuTimeBar_EngineEnabled = enabled;
}

extern "C" void NuTimeBarIndicateGpuFrameOut(i32 enabled) {
    NuTimeBar_GpuFrameOutEnabled = enabled;
}

extern "C" void BuildCamSpaceClipPlanes(void) {
    f32 near_dist = global_camera.near_clip;
    f32 near_sq = near_dist * near_dist;
    TreeInitialised = 1;
    f32 x = zx * near_dist;
    f32 y = zy * near_dist;
    f32 x_inv = 1.0f / NuFsqrt(x * x + near_sq);
    f32 y_inv = 1.0f / NuFsqrt(y * y + near_sq);
    NuMtxSetZero(&CamSpaceFrustrumPlanes);
    CamSpaceFrustrumPlanes.m12 = near_dist * y_inv;
    CamSpaceFrustrumPlanes.m13 = -CamSpaceFrustrumPlanes.m12;
    CamSpaceFrustrumPlanes.m22 = -y * y_inv;
    CamSpaceFrustrumPlanes.m23 = CamSpaceFrustrumPlanes.m22;
    CamSpaceFrustrumPlanes.m20 = -x * x_inv;
    CamSpaceFrustrumPlanes.m21 = CamSpaceFrustrumPlanes.m20;
    CamSpaceFrustrumPlanes.m00 = -near_dist * x_inv;
    CamSpaceFrustrumPlanes.m01 = -CamSpaceFrustrumPlanes.m00;
    x = zxs * near_dist;
    y = zys * near_dist;
    x_inv = 1.0f / NuFsqrt(x * x + near_sq);
    y_inv = 1.0f / NuFsqrt(y * y + near_sq);
    NuMtxSetZero(&CamSpaceScissorPlanes);
    CamSpaceScissorPlanes.m00 = -near_dist * x_inv;
    CamSpaceScissorPlanes.m01 = -CamSpaceScissorPlanes.m00;
    CamSpaceScissorPlanes.m20 = -x * x_inv;
    CamSpaceScissorPlanes.m21 = CamSpaceScissorPlanes.m20;
    CamSpaceScissorPlanes.m12 = near_dist * y_inv;
    CamSpaceScissorPlanes.m22 = -y * y_inv;
    CamSpaceScissorPlanes.m13 = -CamSpaceScissorPlanes.m12;
    CamSpaceScissorPlanes.m23 = CamSpaceScissorPlanes.m22;
}

extern "C" void ChooseCorrectLOD(i32 *index, NUVEC *center, f32 *lod_ranges) {
    if (lod_ranges[*index] == 0.0f) {
        return;
    }

    NUVEC delta;
    NuVecSub(&delta, center, reinterpret_cast<NUVEC *>(&global_camera.mtx.m30));
    f32 distance_sqr = NuVecMagSqr(&delta);
    i32 selected = *index;
    if (distance_sqr < lod_ranges[selected]) {
        do {
            ++selected;
        } while (distance_sqr < lod_ranges[selected]);
        *index = selected;
    }
}

extern "C" void BuildWorldSpaceClipPlanes(void) {
    NuMtxMulH(&FrustrumPlanes, &vmtx, &CamSpaceFrustrumPlanes);
    NuMtxMulH(&ScissorPlanes, &vmtx, &CamSpaceScissorPlanes);
    f32 dot = global_camera.mtx.m30 * global_camera.mtx.m20 + global_camera.mtx.m31 * global_camera.mtx.m21 +
              global_camera.mtx.m32 * global_camera.mtx.m22;
    NearPlane = *reinterpret_cast<NUVEC4 *>(&global_camera.mtx.m20);
    NearPlane.w = -dot;
    AbsNearPlane.x = NuFabs(NearPlane.x);
    AbsNearPlane.y = NuFabs(NearPlane.y);
    AbsNearPlane.z = NuFabs(NearPlane.z);
    AbsNearPlane.w = 0;
    AbsFrustrumPlanes.m00 = NuFabs(FrustrumPlanes.m00);
    AbsFrustrumPlanes.m01 = NuFabs(FrustrumPlanes.m01);
    AbsFrustrumPlanes.m02 = NuFabs(FrustrumPlanes.m02);
    AbsFrustrumPlanes.m03 = NuFabs(FrustrumPlanes.m03);
    AbsFrustrumPlanes.m10 = NuFabs(FrustrumPlanes.m10);
    AbsFrustrumPlanes.m11 = NuFabs(FrustrumPlanes.m11);
    AbsFrustrumPlanes.m12 = NuFabs(FrustrumPlanes.m12);
    AbsFrustrumPlanes.m13 = NuFabs(FrustrumPlanes.m13);
    AbsFrustrumPlanes.m20 = NuFabs(FrustrumPlanes.m20);
    AbsFrustrumPlanes.m21 = NuFabs(FrustrumPlanes.m21);
    AbsFrustrumPlanes.m22 = NuFabs(FrustrumPlanes.m22);
    AbsFrustrumPlanes.m23 = NuFabs(FrustrumPlanes.m23);
    AbsFrustrumPlanes.m30 = AbsFrustrumPlanes.m31 = AbsFrustrumPlanes.m32 = AbsFrustrumPlanes.m33 = 0;
    AbsScissorPlanes.m00 = NuFabs(ScissorPlanes.m00);
    AbsScissorPlanes.m01 = NuFabs(ScissorPlanes.m01);
    AbsScissorPlanes.m02 = NuFabs(ScissorPlanes.m02);
    AbsScissorPlanes.m03 = NuFabs(ScissorPlanes.m03);
    AbsScissorPlanes.m10 = NuFabs(ScissorPlanes.m10);
    AbsScissorPlanes.m11 = NuFabs(ScissorPlanes.m11);
    AbsScissorPlanes.m12 = NuFabs(ScissorPlanes.m12);
    AbsScissorPlanes.m13 = NuFabs(ScissorPlanes.m13);
    AbsScissorPlanes.m20 = NuFabs(ScissorPlanes.m20);
    AbsScissorPlanes.m21 = NuFabs(ScissorPlanes.m21);
    AbsScissorPlanes.m22 = NuFabs(ScissorPlanes.m22);
    AbsScissorPlanes.m23 = NuFabs(ScissorPlanes.m23);
    AbsScissorPlanes.m30 = AbsScissorPlanes.m31 = AbsScissorPlanes.m32 = AbsScissorPlanes.m33 = 0;
}

    i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor) {
        f32 near_radius = AbsNearPlane.x * extent->x + AbsNearPlane.y * extent->y + AbsNearPlane.z * extent->z;
        f32 depth = center->x * NearPlane.x + center->y * NearPlane.y + center->z * NearPlane.z + NearPlane.w;
        f32 far_distance = depth - far_clip;
        f32 near_distance = depth - global_camera.near_clip;

        if (far_distance > near_radius || -near_radius > near_distance) {
            return 0;
        }

        NUVEC4 radius;
        f32 distance0 = center->x * FrustrumPlanes.m00 + center->y * FrustrumPlanes.m10 +
                        center->z * FrustrumPlanes.m20 + FrustrumPlanes.m30;
        f32 distance1 = center->x * FrustrumPlanes.m01 + center->y * FrustrumPlanes.m11 +
                        center->z * FrustrumPlanes.m21 + FrustrumPlanes.m31;
        f32 distance2 = center->x * FrustrumPlanes.m02 + center->y * FrustrumPlanes.m12 +
                        center->z * FrustrumPlanes.m22 + FrustrumPlanes.m32;
        f32 distance3 = center->x * FrustrumPlanes.m03 + center->y * FrustrumPlanes.m13 +
                        center->z * FrustrumPlanes.m23 + FrustrumPlanes.m33;
        NuVecMtxTransform(reinterpret_cast<NUVEC *>(&radius), extent, &AbsFrustrumPlanes);

        if (distance0 > radius.x || distance1 > radius.y || distance2 > radius.z || distance3 > radius.w) {
            return 0;
        }

        if (distance0 > -radius.x || distance1 > -radius.y || distance2 > -radius.z || distance3 > -radius.w) {
            if (use_scissor == 0) {
                return 2;
            }

            distance0 = center->x * ScissorPlanes.m00 + center->y * ScissorPlanes.m10 + center->z * ScissorPlanes.m20 +
                        ScissorPlanes.m30;
            distance1 = center->x * ScissorPlanes.m01 + center->y * ScissorPlanes.m11 + center->z * ScissorPlanes.m21 +
                        ScissorPlanes.m31;
            distance2 = center->x * ScissorPlanes.m02 + center->y * ScissorPlanes.m12 + center->z * ScissorPlanes.m22 +
                        ScissorPlanes.m32;
            distance3 = center->x * ScissorPlanes.m03 + center->y * ScissorPlanes.m13 + center->z * ScissorPlanes.m23 +
                        ScissorPlanes.m33;
            NuVecMtxTransform(reinterpret_cast<NUVEC *>(&radius), extent, &AbsScissorPlanes);

            if (distance0 > radius.x || distance1 > radius.y || distance2 > radius.z || distance3 > radius.w) {
                return 0;
            }
            if (distance0 > -radius.x || distance1 > -radius.y || distance2 > -radius.z || distance3 > -radius.w) {
                return 2;
            }
            return 1;
        }

        if (use_scissor != 0) {
            return 1;
        }
        if (far_distance > -near_radius || near_radius > near_distance) {
            return 2;
        }
        return 1;
    }

}

extern "C" {
extern "C++" {
        static void ClipInstTree(NuInstanceTree_s *tree, i32 tree_index) {
            NuInstanceTreeNode *pending[100];
            NuInstanceTreeNode **end = pending;
            *end++ = &tree->nodes[tree->roots[tree_index]];
            f32 far_clip = tree->far_clips[tree_index];
            if (far_clip == 0.0f)
                far_clip = global_camera.far_clip;
            do {
                NuInstanceTreeNode *node = *--end;
                NuInstanceTreeBounds *bounds = &tree->node_bounds[node->bounds_index];
                i32 result = NuCameraIntersectsAABB(&bounds->center, &bounds->extent, far_clip, 0);
                if (result == 1) {
                    u16 *instance = tree->instance_indices + node->first_instance;
                    u16 *last = instance + node->subtree_instance_count;
                    for (; instance != last; ++instance)
                        static_cast<u8 *>(myvisi->visibility_context)[*instance >> 2] |= 1 << ((*instance & 3) * 2);
                } else if (result != 0) {
                    if (node->child_count != 0) {
                        NuInstanceTreeNode *child = tree->nodes + node->first_child;
                        NuInstanceTreeNode *last_child = child + node->child_count;
                        for (; child != last_child; ++child)
                            *end++ = child;
                    }
                    if (node->instance_count != 0) {
                        u16 *instance = tree->instance_indices + node->first_instance;
                        u16 *last = instance + node->instance_count;
                        NuInstanceTreeBounds *instance_bounds = tree->instance_bounds + node->first_instance;
                        for (; instance != last; ++instance, ++instance_bounds) {
                            u16 index = *instance;
                            i32 classification =
                                NuCameraIntersectsAABB(&instance_bounds->center, &instance_bounds->extent, far_clip, 1);
                            static_cast<u8 *>(myvisi->visibility_context)[index >> 2] |= classification
                                                                                         << ((index & 3) * 2);
                        }
                    }
                }
            } while (end != pending);
        }

}
}

extern "C" {
    void NuVisiInstTree(void *visibility_result, NUGSCN *scene) {
        NuVisibilityResult *result = static_cast<NuVisibilityResult *>(visibility_result);
        bool enabled = result->instance_tree != NULL && do_InstTree != 0;
        result->state = (result->state & ~0x10) | (enabled << 4);
        if ((result->state & 0x10) != 0) {
            myvisi = result;
            if (!TreeInitialised)
                BuildCamSpaceClipPlanes();
            BuildWorldSpaceClipPlanes();
            NuInstanceTree_s *tree = static_cast<NuInstanceTree_s *>(result->instance_tree);
            for (i32 i = tree->tree_count - 1; i >= 0; --i) {
                ClipInstTree(static_cast<NuInstanceTree_s *>(result->instance_tree), i);
            }
        }
    }

    i32 NuPortalEnabled(i32 enabled) {
        const i32 previous = portals_enabled;
        portals_enabled = enabled;
        return previous;
    }

}

void NuPortalInit(void) {
    fstack.void_ptr = fstackmem;
    fstack_top.addr = ALIGN((usize)fstackmem, 16);
    fstack_limit.void_ptr = fstackmem + 0x3000;
}

void NuPortalSetActive(NUGSCN *scene, i32 portal_id, i32 active) {
    if (scene->max_portals != 0) {
        NUPORTAL *portal = scene->portals;
        if (active == 0) {
            for (u32 index = 0; index < scene->max_portals; index++, portal++) {
                if (portal->id == portal_id) {
                    portal->is_active = 0;
                }
            }
        } else {
            for (u32 index = 0; index < scene->max_portals; index++, portal++) {
                if (portal->id == portal_id) {
                    portal->is_active |= NUPORTAL_FLAG_ACTIVE;
                }
            }
        }
    }
}

void NuPortalSetActiveDirect(NUPORTAL *portal, i32 active) {
    if (portal == NULL) {
        return;
    }

    if (active) {
        portal->is_active |= NUPORTAL_FLAG_ACTIVE;
    } else {
        portal->is_active = 0;
    }
}

extern "C" {
    void NuPortalResetActive(NUGSCN *scene) {
        for (u32 i = 0; i < scene->max_portals; ++i) {
            scene->portals[i].is_active |= NUPORTAL_FLAG_ACTIVE | NUPORTAL_FLAG_DEFAULT_ACTIVE;
        }
    }

}

void NuPortalMaxDepth(struct nugscn_s *scene, i32 depth) {
    scene->portal_depth = depth;
}

extern "C" {
    i32 NuPortalNumRooms(NUGSCN *scene) {
        return scene != NULL ? static_cast<u16>(scene->num_rooms) : 0;
    }

    i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position) {
        if (position == NULL || scene == NULL) {
            return -1;
        }

        i16 candidates[2] = {0, 0};
        if (scene->num_rooms <= 0) {
            return -1;
        }

        i16 candidate_count = 0;
        for (i32 room_index = 0; room_index < scene->num_rooms; ++room_index) {
            NUROOM *room = &scene->rooms[room_index];
            f32 plane_distance = 0.0f;
            NUPLANE *plane = room->planes;
            for (i32 plane_index = 0; plane_index < room->plane_count; ++plane_index, ++plane) {
                plane_distance = plane->a * position->x + plane->b * position->y + plane->c * position->z + plane->d;
                if (plane_distance > 0.0f) {
                    break;
                }
            }
            if (!(plane_distance <= 0.0f)) {
                continue;
            }

            // Later matches do not change the selected pair; avoid overflowing the candidate slots.
            if (__builtin_expect(candidate_count >= 3, 0)) {
                continue;
            }

            // Retail snapshots the old candidate rooms before inserting this room.
            NUROOM &first = scene->rooms[candidates[0]];
            NUROOM &second = scene->rooms[candidates[1]];
            if (candidate_count == 2) {
                if (!(first.priority < second.priority)) {
                    candidates[1] = static_cast<i16>(room_index);
                } else {
                    candidates[0] = static_cast<i16>(room_index);
                }
                candidate_count = 3;
                continue;
            }

            candidates[candidate_count++] = static_cast<i16>(room_index);
            if (__builtin_expect(candidate_count != 2, 0)) {
                continue;
            }

            if ((first.flags & NUROOM_FLAG_OVERLAPPING) == 0 && (second.flags & NUROOM_FLAG_OVERLAPPING) == 0) {
                break;
            }
        }

        if (__builtin_expect(candidate_count == 1, 0)) {
            return static_cast<u16>(candidates[0]);
        }
        if (candidate_count == 0) {
            return -1;
        }

        NUROOM &first = scene->rooms[candidates[0]];
        NUROOM &second = scene->rooms[candidates[1]];
        for (i32 first_portal = 0; first_portal < first.portal_count; ++first_portal) {
            const i16 portal_index = first.portal_indices[first_portal];
            for (i32 second_portal = 0; second_portal < second.portal_count; ++second_portal) {
                if (__builtin_expect(second.portal_indices[second_portal] == portal_index, 0)) {

                    const NUPORTAL &portal = scene->portals[portal_index];
                    const f32 side = portal.plane.a * position->x + portal.plane.b * position->y +
                                     portal.plane.c * position->z + portal.plane.d;
                    return side < 0.0f ? portal.front_room : portal.back_room;
                }
            }
        }
        return -1;
    }

extern "C" i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum) {
    i32 fully_inside = 0;
    for (i32 i = 0; i < frustum->plane_count; ++i) {
        const NUPLANE &plane = frustum->planes[i];
        const f32 distance =
            plane.a * sphere->center.x + plane.b * sphere->center.y + plane.c * sphere->center.z + plane.d;
        if (distance < -sphere->radius) {
            return 0;
        }
        if (distance > sphere->radius) {
            ++fully_inside;
        }
    }
    const f32 camera_distance =
        cam_plane.a * sphere->center.x + cam_plane.b * sphere->center.y + cam_plane.c * sphere->center.z + cam_plane.d;
    if (camera_distance < -sphere->radius) {
        return 0;
    }
    if (camera_distance > sphere->radius) {
        ++fully_inside;
    }
    return fully_inside == frustum->plane_count + 1 ? 1 : 2;
}

extern "C" i32 clipTestBox(NUVEC *minimum, NUVEC *maximum, NUPLANE *planes, i32 plane_count) {
    NUVEC min = *minimum;
    NUVEC max = *maximum;
    i32 inside_vertices = 0;
    for (i32 plane_index = 0; plane_index < plane_count; ++plane_index) {
        const NUPLANE &plane = planes[plane_index];
        i32 inside_plane = 0;
        if (!(plane.a * min.x + plane.b * min.y + plane.c * min.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (plane.a * max.x + plane.b * min.y + plane.c * min.z + plane.d >= 0.0f)
            ++inside_plane, ++inside_vertices;
        if (!(plane.a * max.x + plane.b * max.y + plane.c * min.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (!(plane.a * min.x + plane.b * max.y + plane.c * min.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (!(plane.a * min.x + plane.b * min.y + plane.c * max.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (!(plane.a * max.x + plane.b * min.y + plane.c * max.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (!(plane.a * max.x + plane.b * max.y + plane.c * max.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (!(plane.a * min.x + plane.b * max.y + plane.c * max.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (inside_plane == 0) {
            return 0;
        }
    }
    return inside_vertices == plane_count * 8 ? 1 : 2;
}

extern "C" i32 NuPortalClipTestBox(NUVEC *center, NUVEC *extent, NUFRUSTRUM *frustum) {
    for (i32 i = 0; i < frustum->plane_count; ++i) {
        const NUPLANE &plane = frustum->planes[i];
        const f32 distance = plane.a * center->x + plane.b * center->y + plane.c * center->z + plane.d;
        const f32 radius = NuFabs(plane.a) * extent->x + NuFabs(plane.b) * extent->y + NuFabs(plane.c) * extent->z;
        if (distance < -radius) {
            return 0;
        }
    }
    return 1;
}

    i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id) {
        if (scene == NULL || scene->max_portals == 0) {
            return 1;
        }
        for (i32 i = 0; i < scene->num_portal_frusta; ++i) {
            if (scene->portal_frusta[i]->room_id == room_id) {
                return 1;
            }
        }
        return 0;
    }

    void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility) {
        if (scene == NULL || scene->max_portals == 0) {
            return;
        }
        for (i32 i = 0; i < scene->num_rooms; ++i) {
            room_visibility[i] = 0;
        }
        room_visibility[scene->camera_room] = 1;
        for (i32 i = 0; i < scene->num_portal_frusta; ++i) {
            const i16 room_id = scene->portal_frusta[i]->room_id;
            if (room_id >= 0) {
                room_visibility[room_id] = 1;
            }
        }
    }

    i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id) {
        if (scene->max_portals == 0 || room_id == scene->camera_room) {
            return 1;
        }
        for (i32 i = 0; i < scene->num_portal_frusta; ++i) {
            NUFRUSTRUM *frustum = scene->portal_frusta[i];
            if (frustum != NULL && frustum->room_id == room_id) {
                NUPORTALSPHERE sphere;
                sphere.center.x = position->x;
                sphere.center.y = position->y;
                sphere.center.z = position->z;
                sphere.radius = radius;
                i32 result = clipTestSphere(&sphere, frustum);
                if (result != 0) {
                    return result;
                }
            }
        }
        return 0;
    }

}

void NuPortalEnableDebugDraw(i32 enabled) {
    draw_portals = enabled;
}

void NuPortalSetOverride(NUVEC *position) {
    override_campos = position;
}

extern "C" {
    void SetForceScenePlayBack(i32 enabled) {
        ForceScenePlayBack = static_cast<u8>(enabled);
    }

    i32 StateAnimEvaluate2(StateAnim *state, u8 *index, char *value, f32 frame) {
        i32 current = *index;
        i32 count = state->count;
        if (current >= count) {
            current = count - 1;
        }
        if (current < 0) {
            current = 0;
        }
        char old_value = state->values[current];
        if (!(frame >= state->times[current])) {
            if (current != 0) {
                i32 previous = current - 1;
                current = previous;
                while (!(frame >= state->times[previous])) {
                    if (previous == 0) {
                        break;
                    }
                    --previous;
                    current = previous;
                }
            }
        } else {
            while (true) {
                if (current >= count - 1) {
                    break;
                }
                const i32 next = current + 1;
                if (state->times[next] > frame) {
                    break;
                }
                current = next;
            }
        }
        char new_value = state->values[current];
        *value = new_value;
        *index = static_cast<u8>(current);
        return old_value != new_value;
    }

    __attribute__((optimize("O2", "tree-vectorize"))) i32 StateAnimEvaluate(StateAnim *state, u8 *index, u8 *value, f32 frame) {
        u8 next = *index;
        if (next < state->count) {
            i32 changed = 0;
            do {
                if (!(frame >= state->times[next])) {
                    if (changed) {
                        return 1;
                    }
                    break;
                }
                changed = 1;
                *value = state->values[next];
                next = static_cast<u8>(*index + 1);
                *index = next;
            } while (next < state->count);
            if (next >= state->count) {
                return 1;
            }
        }

        i32 changed = 0;
        if (next == 0) {
            return 0;
        }
        do {
            if (!(state->times[next - 1] > frame)) {
                return changed;
            }
            next--;
            *index = next;
            *value = next == 0 ? state->values[0] : state->values[next - 1];
            changed = 1;
            next = *index;
        } while (next != 0);
        return 1;
    }

    StateAnim *StateAnimFixPtrs(StateAnim *state, isize delta) {
        if (state == NULL) {
            return NULL;
        }
        state = reinterpret_cast<StateAnim *>(reinterpret_cast<usize>(state) + delta);
        if (state == NULL) {
            return NULL;
        }
        state->times =
            state->times != NULL ? reinterpret_cast<f32 *>(reinterpret_cast<usize>(state->times) + delta) : NULL;
        state->values =
            state->values != NULL ? reinterpret_cast<u8 *>(reinterpret_cast<usize>(state->values) + delta) : NULL;
        return state;
    }

}

void ReadInstAnimBlockDlist(i32 file, nugscn_s *scene) {
    NUDLDLISTSCENE *display_list = scene->display_list;
    scene->num_instance_animations = static_cast<i16>(NuFileReadInt(file));
    NuFileReadInt(file);
    nuinstanim_s *animations = static_cast<nuinstanim_s *>(NuMemFileAddr(file));
    scene->instance_animations = animations;

    if (animations != NULL) {
        if (display_list->nspecials > 0) {
            NUDISPLAYSPECIAL *specials = static_cast<NUDISPLAYSPECIAL *>(display_list->specials);
            for (i32 special_index = 0; special_index < display_list->nspecials; ++special_index) {
                NUDISPLAYSPECIAL *special = &specials[special_index];
                const isize animation_index = reinterpret_cast<isize>(special->instance_animation);
                if (animation_index == -1) {
                    special->instance_animation = NULL;
                    continue;
                }

                nuinstanim_s *animation = &animations[animation_index];
                special->instance_animation = animation;
                animation->mtx = special->instance_mtx;
                animation->instance_ix = static_cast<u16>(special_index);
            }
        }
    }

    if (scene->num_instance_animations != 0) {
        scene->instance_animation_matrices = static_cast<NUMTX *>(
            NuGScnBufferAllocAligned(static_cast<i32>(scene->num_instance_animations) * sizeof(NUMTX), 0x10));
    }
}

void ReadInstanceIDs(i32 file, nugscn_s *scene) {
    scene->num_instance_ids = NuFileReadInt(file);
    NuFileReadInt(file);
    scene->instance_ids = NuMemFileAddr(file);
}

void ReadInstAnimBlock(i32 file, nugscn_s *scene) {
    scene->num_instance_animations = static_cast<i16>(NuFileReadInt(file));
    const i32 indexed_format = NuFileReadInt(file);
    scene->instance_animations = static_cast<nuinstanim_s *>(NuMemFileAddr(file));

    if (indexed_format == 0) {
        nuinstanim_s *animation = scene->instance_animations;
        if (animation != NULL) {
            for (i32 instance_index = 0; instance_index < scene->num_instances; ++instance_index) {
                LegacyInstanceAnimationLink *instance =
                    reinterpret_cast<LegacyInstanceAnimationLink *>(scene->instances + instance_index * 0x50);
                if (instance->animation == NULL) {
                    continue;
                }

                instance->animation = animation;
                animation->mtx = instance->matrix;
                animation->instance_ix = static_cast<u16>(instance_index);
                ++animation;
            }
        }
    } else if (scene->instance_animations != NULL && scene->num_instance_animations > 0) {
        for (i32 animation_index = 0; animation_index < scene->num_instance_animations; ++animation_index) {
            nuinstanim_s *animation = &scene->instance_animations[animation_index];
            LegacyInstanceAnimationLink *instance =
                reinterpret_cast<LegacyInstanceAnimationLink *>(scene->instances + animation->instance_ix * 0x50);
            instance->animation = animation;
            animation->mtx = instance->matrix;
        }
    }

    if (scene->num_instance_animations != 0) {
        scene->instance_animation_matrices = static_cast<NUMTX *>(
            NuGScnBufferAllocAligned(static_cast<i32>(scene->num_instance_animations) * sizeof(NUMTX), 0x10));
    }
}

extern "C" {
extern "C" i32 NuGScnNumSpecials(NUGSCN *scene) {
    if (scene->display_list != NULL)
        return scene->display_list->nspecials;
    return scene->numspecial;
}

extern "C" void NuGScnGetSpecial(nuhspecial_s *special, NUGSCN *scene, i32 index) {
    if (scene != NULL && special != NULL) {
        if (scene->display_list != NULL && index < scene->display_list->nspecials) {
            special->scene = scene;
            special->special = NULL;
            special->display_special = &static_cast<NUDISPLAYSPECIAL *>(scene->display_list->specials)[index];
            return;
        }
        if (index < scene->numspecial) {
            special->scene = scene;
            special->special = &reinterpret_cast<NuSpecialLegacyLayout *>(scene->specials)[index];
            special->display_special = NULL;
            return;
        }
    }
    special->scene = NULL;
    special->special = NULL;
    special->display_special = NULL;
}

    void NuSpecialSetInstAnimTime(nuhspecial_s *special, f32 frame) {
        NUGSCN *scene = special->scene;
        if (scene == NULL) {
            return;
        }
        nuinstanim_s *animation = NuSpecialGetInstAnim(special);
        if (animation != NULL) {
            animation->ltime = frame;
            if (!((animation->end_frame_lookup_bits & NUINSTANIM_END_FRAME_LOOKUP_MASK) != 0 &&
                  scene->animation_end_frames != NULL)) {
                return;
            }
            StateAnim *state =
                reinterpret_cast<StateAnim *>(&scene->animation_end_frames[animation->end_frame_lookup_index - 1]);
            char value;
            u8 index = static_cast<u8>(static_cast<u32>(animation->flags) >> NUINSTANIM_STATE_INDEX_SHIFT);
            StateAnimEvaluate2(state, &index, &value, frame);
            animation->flags =
                static_cast<NUINSTANIM_FLAGS>((static_cast<u32>(animation->flags) & ~NUINSTANIM_STATE_INDEX_MASK) |
                                              (static_cast<u32>(index) << NUINSTANIM_STATE_INDEX_SHIFT));
        }
    }

extern "C" void NuGScnUpdate(NUGSCN *gscn, f32 frame_delta) {
    NUMTX *animation_matrix = gscn->instance_animation_matrices;
    NUDLDLISTSCENE *display_list = gscn->display_list;
    if (gscn->instance_animation_data == NULL) {
        return;
    }

    nuinstanim_s *instance_animation = gscn->instance_animations;

    // The lookup array is reference-counted by the allocation word immediately
    // before its first entry.  A sole owner means the optional table was not
    // populated and the original runtime discards it here.
    if (gscn->animation_end_frames != NULL && reinterpret_cast<i32 *>(gscn->animation_end_frames)[-1] <= 1) {
        gscn->animation_end_frames = NULL;
    }

    i16 animation_count = gscn->num_instance_animations;
    if (animation_count <= 0) {
        return;
    }

    // A waiting animation retains the previous evaluation frame.
    f32 frame = 0.0f;
    i32 i = 0;
    do {
        if (__builtin_expect(instance_animation->anim_ix < gscn->num_instance_animation_data, 1)) {
            NUDISPLAYSPECIAL *display_special =
                &static_cast<NUDISPLAYSPECIAL *>(display_list->specials)[instance_animation->instance_ix];
            const u32 display_flags = display_special->flags;
            nuanimendlookup_s *state_animation = NULL;
            nuanimdata_s *animation = NULL;

            if (__builtin_expect((instance_animation->end_frame_lookup_bits & NUINSTANIM_END_FRAME_LOOKUP_MASK) != 0 &&
                gscn->animation_end_frames != NULL, 1)) {
                const u16 lookup_index = instance_animation->end_frame_lookup_index;
                state_animation = &gscn->animation_end_frames[lookup_index - 1];
                animation = gscn->instance_animation_data[instance_animation->anim_ix];
            } else {
                if ((display_flags & NUDISPLAYSPECIAL_FLAG_VISIBLE) == 0) {
                    goto next_animation;
                }
                animation = gscn->instance_animation_data[instance_animation->anim_ix];
                if (animation == NULL) {
                    goto next_animation;
                }
            }

            f32 end_frame;
            if (animation != NULL) {
                end_frame = NuAnimEndFrameOld(animation);
            } else if (__builtin_expect(state_animation != NULL, 0)) {
                end_frame = static_cast<f32>(static_cast<u32>(state_animation->end_frame));
            } else {
                goto next_animation;
            }

            if (end_frame == 0.0f) {
                animation_count = gscn->num_instance_animations;
                goto next_animation;
            }

            if (instance_animation->playing != 0) {
                instance_animation->ltime += frame_delta * instance_animation->tfactor;
                if (instance_animation->waiting != 0) {
                    if (__builtin_expect(!(instance_animation->ltime >= instance_animation->tfirst), 0)) {
                        goto evaluate_animation;
                    }
                    instance_animation->waiting = 0;
                    instance_animation->ltime -= instance_animation->tfirst - 1.0f;
                }

                frame = instance_animation->ltime;
                const f32 interval_end = instance_animation->tinterval + end_frame;
                if (!(frame >= interval_end)) {
                    if (frame > end_frame) {
                        frame = end_frame;
                    } else if (frame < 1.0f) {
                        instance_animation->playing = 0;
                        instance_animation->ltime = 1.0f;
                        frame = 1.0f;
                    }
                } else {
                    if (__builtin_expect(instance_animation->repeating != 0, 0)) {
                        const f32 repeat_length = interval_end - 1.0f;
                        const f32 repeats = NuFloor((instance_animation->ltime - 1.0f) / repeat_length);
                        const f32 live_repeat_length = instance_animation->tinterval + end_frame - 1.0f;
                        instance_animation->ltime -= live_repeat_length * repeats;
                        frame = instance_animation->ltime;
                    } else {
                        instance_animation->playing = 0;
                        instance_animation->ltime = end_frame;
                        frame = end_frame;
                    }
                }

                if (__builtin_expect(instance_animation->backwards != 0, 1)) {
                    frame = end_frame + 1.0f - frame;
                }
            } else {
                frame = instance_animation->ltime;
            }

        evaluate_animation:
            if (frame != instance_animation->prev_eval_time) {
                if (state_animation != NULL) {
                    u8 state_index =
                        static_cast<u8>((static_cast<u32>(instance_animation->flags) & NUINSTANIM_STATE_INDEX_MASK) >>
                                        NUINSTANIM_STATE_INDEX_SHIFT);
                    char state_value;
                    const i32 state_changed = StateAnimEvaluate2(reinterpret_cast<StateAnim *>(state_animation),
                                                                 &state_index, &state_value, frame);
                    const u32 state_flags =
                        (static_cast<u32>(instance_animation->flags) & ~NUINSTANIM_STATE_INDEX_MASK) |
                        (static_cast<u32>(state_index) << NUINSTANIM_STATE_INDEX_SHIFT);
                    instance_animation->flags = static_cast<NUINSTANIM_FLAGS>(state_flags);

                    if (state_changed) {
                        if (instance_animation->instance_ix != 0xffff) {
                            display_special = &static_cast<NUDISPLAYSPECIAL *>(
                                display_list->specials)[instance_animation->instance_ix];
                            if (state_value == 0) {
                                display_special->flags &= ~NUDISPLAYSPECIAL_FLAG_VISIBLE;
                                animation = NULL;
                            } else {
                                display_special->flags |= NUDISPLAYSPECIAL_FLAG_VISIBLE;
                            }
                        } else if (state_value == 0) {
                            animation = NULL;
                        }
                    }
                }

                if (animation != NULL) {
                    NuAnimData2CalcMatrix(animation, 0, frame, animation_matrix);
                }
                instance_animation->prev_eval_time = frame;
            }

            if (animation != NULL) {
                instance_animation->mtx = *animation_matrix;
                instance_animation->mtx.m30 += display_special->draw_mtx.m30;
                instance_animation->mtx.m31 += display_special->draw_mtx.m31;
                instance_animation->mtx.m32 += display_special->draw_mtx.m32;
            }
            animation_count = gscn->num_instance_animations;
        }
    next_animation:
        ++i;
        ++instance_animation;
        ++animation_matrix;
    } while (i < animation_count);
}

}

i32 NuTexGetUnresolvedTextureTIDPS() {
    return 0;
}

extern "C" {
    void NuHGobjSetClippingRootTrackerOverride(i32 enabled) {
        CutSceneBoundingBoxTrackRoot = static_cast<u8>(enabled);
    }

}

// Original @0x2da1a0. The public type is nugscn_s in the mangled symbol, but
// GHG objects carry the hierarchy layout consumed here.
i32 NuCameraClipHGobj(nugscn_s *scene, numtx_s *world_matrix, numtx_s *root_matrix) {
    nuhgobj_s *object = reinterpret_cast<nuhgobj_s *>(scene);
    if (CutSceneBoundingBoxTrackRoot == 0) {
        NUVEC min = object->bounds_min;
        NUVEC max = object->bounds_max;
        return NuCameraClipTestExtents(&min, &max, world_matrix, 0.0f, 0);
    }

    NUVEC half_extents;
    NuVecSub(&half_extents, &object->bounds_max, &object->bounds_min);
    NuVecScale(&half_extents, &half_extents, 0.5f);
    NUVEC min;
    NuVecNeg(&min, &half_extents);

    NUMTX_ALIGNED16 translated_world = *world_matrix;
    NuMtxPreTranslate(&translated_world, NUMTX_GET_ROW_VEC(root_matrix, 3));
    return NuCameraClipTestExtents(&min, &half_extents, &translated_world, 0.0f, 0);
}

void NuGScnMtlLayerMask(nugscn_s *scene, unsigned char mask) {
    NUDLDLISTSCENE *display_list = scene->display_list;
    display_list->material_layer_mask = mask;
}

extern "C" {
    void NuHGobjJointMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result) {
        u8 joint_index = object->joint_override_map[index];
        NuMtxMulVU0(result, &joint_matrices[joint_index], world_matrix);
    }

    void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result) {
        nuhgobjpoi_s *point = &object->points_of_interest[object->point_of_interest_map[index]];
        NuMtxMulVU0(result, &point->local_matrix, &joint_matrices[point->joint_index]);
        NuMtxMulVU0(result, result, world_matrix);
    }

extern "C" void NuGScnToVideoMem(NUGSCNVIDEOMEMFN callback) {
    gscene_to_video_mem = callback;
}

extern "C" void NuGScnFromVideoMem(NUGSCNVIDEOMEMFN callback) {
    video_mem_to_gscene = callback;
}

    void NuHGobjToVideoMem(NUHGOBJVIDEOMEMFN callback) {
        hgobj_to_video_mem = callback;
    }

    void NuHGobjFromVideoMem(NUHGOBJVIDEOMEMFN callback) {
        video_mem_to_hgobj = callback;
    }

extern "C" i32 NuHGobjGetLayerIndex(char *name, nuhgobj_s *object) {
    for (i32 i = 0; i < object->render_count; ++i) {
        if (NuStrICmp(name, object->render_parts[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

    i32 NuHGobjReversibleCharacters(i32 enabled) {
        i32 previous = nuapi.reversible_characters;
        nuapi.reversible_characters = enabled;
        return previous;
    }

    i32 NuHGobjForceShadowsOnCharacters(i32 enabled) {
        i32 previous = nuapi.force_shadows_on_characters;
        nuapi.force_shadows_on_characters = enabled;
        return previous;
    }

}

void NuVpInit(void) {
    NuPs2GetViewport(&vpDevice);

    vpDevice.center_x = 0.5f;
    vpDevice.center_y = 0.5f;
    vpDevice.clip_min_x = 0.0f;
    vpDevice.clip_min_y = 0.0f;
    vpDevice.clip_max_x = 1.0f;
    vpDevice.clip_max_y = 1.0f;
    vpCurrent = vpDevice;
    vport_inval = 1;

    NuVpUpdate();
}

void NuVpRestore(void) {
    vpCurrent = vpDevice;
    vport_inval = 1;
}

void NuVpSetZRange(f32 minimum, f32 maximum) {
    vpCurrent.min_z = minimum;
    vpCurrent.max_z = maximum;
    vport_inval = 1;
}

void NuVpSetCentre(f32 x, f32 y) {
    vpCurrent.center_x = x;
    vpCurrent.center_y = y;
    vport_inval = 1;
}

void NuVpSetClipping(f32 left, f32 top, f32 right, f32 bottom) {
    vpCurrent.clip_min_x = left < 0.0f ? 0.0f : left;
    vpCurrent.clip_min_y = top < 0.0f ? 0.0f : top;
    vpCurrent.clip_max_x = right > 1.0f ? 1.0f : right;
    vpCurrent.clip_max_y = bottom > 1.0f ? 1.0f : bottom;
    vport_inval = 1;
}

void NuVpSetCurrent(NUVIEWPORT *viewport) {
    vpCurrent = *viewport;
    vport_inval = 1;
}

void NuVpGetCurrent(NUVIEWPORT *viewport) {
    *viewport = vpCurrent;
}

void NuVpSetPosition2(f32 x, f32 y) {
    x += g_NuVpRegion.x_offset;
    y += g_NuVpRegion.y_offset;
    vpCurrent.x =
        (i32)(((f32)nurndr_pixel_width * x / (f32)PS2_VREZ_W + ((f32)PS2_VCNTR_X - (f32)(nurndr_pixel_width >> 1))) *
              16.0f);
    vpCurrent.y =
        (i32)(((f32)nurndr_pixel_height * y / (f32)PS2_VREZ_H + ((f32)PS2_VCNTR_Y - (f32)(nurndr_pixel_height >> 1))) *
              16.0f);
    vport_inval = 1;
}

void NuVpSetPosition(f32 x, f32 y) {
    NuVpSetPosition2(NuFdiv(x * 0.0625f - ((f32)PS2_VCNTR_X - (f32)(nurndr_pixel_width >> 1)),
                            NuFdiv((f32)nurndr_pixel_width, (f32)PS2_VREZ_W)),
                     NuFdiv(y * 0.0625f - ((f32)PS2_VCNTR_Y - (f32)(nurndr_pixel_height >> 1)),
                            NuFdiv((f32)nurndr_pixel_height, (f32)PS2_VREZ_H)));
}

void NuVpGetPosition2(f32 *x, f32 *y) {
    *x = NuFdiv((f32)vpCurrent.x * 0.0625f - ((f32)PS2_VCNTR_X - (f32)(nurndr_pixel_width >> 1)),
                NuFdiv((f32)nurndr_pixel_width, (f32)PS2_VREZ_W));
    *y = NuFdiv((f32)vpCurrent.y * 0.0625f - ((f32)PS2_VCNTR_Y - (f32)(nurndr_pixel_height >> 1)),
                NuFdiv((f32)nurndr_pixel_height, (f32)PS2_VREZ_H));
}

void NuVpSetSize2(f32 width, f32 height) {
    width *= g_NuVpRegion.width_scale;
    height *= g_NuVpRegion.height_scale;
    vpCurrent.width = (i32)((f32)nurndr_pixel_width * width / (f32)PS2_VREZ_W * 16.0f);
    vpCurrent.height = (i32)((f32)nurndr_pixel_height * height / (f32)PS2_VREZ_H * 16.0f);
    vport_inval = 1;
}

void NuVpSetSize(f32 width, f32 height) {
    NuVpSetSize2(width * 0.0625f / ((f32)nurndr_pixel_width / (f32)PS2_VREZ_W),
                 height * 0.0625f / ((f32)nurndr_pixel_height / (f32)PS2_VREZ_H));
}

void NuVpGetSize2(f32 *width, f32 *height) {
    *width = ((f32)vpCurrent.width * 0.0625f) / ((f32)nurndr_pixel_width / (f32)PS2_VREZ_W);
    *height = ((f32)vpCurrent.height * 0.0625f) / ((f32)nurndr_pixel_height / (f32)PS2_VREZ_H);
}

extern "C" {
extern "C" void NuRndrClear(i32 clear_flags, i32 bg_colour, f32 alpha) {
    auto *scene = &currentScene;

    if (NuIOS_IsLowEndDevice() && g_BackgroundUsedFogColour) {
        bg_colour = g_BackgroundColour;
    }

    scene->clear_alpha = alpha;
    scene->clear_flags |= clear_flags;
    scene->bg_colour = bg_colour;
    NuVpGetPosition2(&scene->vp_x, &scene->vp_y);
    NuVpGetSize2(&scene->vp_w, &scene->vp_h);
}

}

void NuVpSetCurrent2(NUVIEWPORT2 *viewport) {
    NuVpSetPosition2(viewport->x, viewport->y);
    NuVpSetSize2(viewport->width, viewport->height);
    // The original leaves the depth range unchanged.
    vpCurrent.center_x = viewport->center_x;
    vpCurrent.center_y = viewport->center_y;
    vpCurrent.clip_min_x = viewport->clip_min_x;
    vpCurrent.clip_min_y = viewport->clip_min_y;
    vpCurrent.clip_max_x = viewport->clip_max_x;
    vpCurrent.clip_max_y = viewport->clip_max_y;
    vpCurrent.clip_width = viewport->clip_width;
    vpCurrent.clip_height = viewport->clip_height;
    vpCurrent.scissor_width = viewport->scissor_width;
    vpCurrent.scissor_height = viewport->scissor_height;
    vport_inval = 1;
}

void NuVpGetCurrent2(NUVIEWPORT2 *viewport) {
    NuVpGetPosition2(&viewport->x, &viewport->y);
    NuVpGetSize2(&viewport->width, &viewport->height);
    // The original does not write the depth range in this representation.
    viewport->center_x = vpCurrent.center_x;
    viewport->center_y = vpCurrent.center_y;
    viewport->clip_min_x = vpCurrent.clip_min_x;
    viewport->clip_min_y = vpCurrent.clip_min_y;
    viewport->clip_max_x = vpCurrent.clip_max_x;
    viewport->clip_max_y = vpCurrent.clip_max_y;
    viewport->clip_width = vpCurrent.clip_width;
    viewport->clip_height = vpCurrent.clip_height;
    viewport->scissor_width = vpCurrent.scissor_width;
    viewport->scissor_height = vpCurrent.scissor_height;
}

f32 NuVpVirtualWidth(f32 value) {
    return value / (f32)vpCurrent.width;
}

f32 NuVpVirtualHeight(f32 value) {
    return value / (f32)vpCurrent.height;
}

f32 NuVpPixelWidth(f32 value) {
    return (f32)vpCurrent.width * value + (f32)vpCurrent.x;
}

f32 NuVpPixelHeight(f32 value) {
    return (f32)vpCurrent.height * value + (f32)vpCurrent.y;
}

void NuVpGetClippingMtx(NUMTX *dest) {
    if (dest == NULL) {
        return;
    }
    *dest = vp_cmtx;
}

void NuVpGetScalingMtx(NUMTX *dest) {
    if (dest == NULL) {
        return;
    }

    *dest = vp_smtx;
}

NUVIEWPORT *NuVpGetCurrentViewport(void) {
    return &vpCurrent;
}

extern "C" {
    void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast) {
        static NUMTX cmtx;
        if (fast == 0) {
            NUVIEWPORT *viewport = NuVpGetCurrentViewport();
            f32 far_clip;
            if (camera->unknown_64 == 0.0f)
                far_clip = camera->far_clip - nucamera_farclip_hack;
            else
                far_clip = camera->unknown_64 - nucamera_farclip_hack;
            f32 near_clip = camera->unknown_60;
            if (near_clip == 0.0f)
                near_clip = camera->near_clip;
            const i32 angle = static_cast<i32>(camera->fov * 0.5f * 10430.378f);
            const f32 cotangent = NuTrigTable[(angle + 0x4000) >> 1 & 0x7fff] / NuTrigTable[angle >> 1 & 0x7fff];
            const f32 x = viewport->scissor_width * camera->aspect * cotangent;
            const f32 y = cotangent * viewport->scissor_height;
            memset(&cmtx, 0, sizeof(cmtx));
            cmtx.m00 = x;
            cmtx.m11 = y;
            cmtx.m23 = 1.0f;
            cmtx.m22 = (far_clip + near_clip) / (far_clip - near_clip);
            cmtx.m32 = (far_clip * -2.0f * near_clip) / (far_clip - near_clip);
        }
        NuMtxMulVU0(out, view, &cmtx);
    }

    void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast) {
        if (fast == 0) {
            NUVIEWPORT *viewport = NuVpGetCurrentViewport();
            f32 far_clip;
            if (camera->unknown_64 == 0.0f)
                far_clip = camera->far_clip - nucamera_farclip_hack;
            else
                far_clip = camera->unknown_64 - nucamera_farclip_hack;
            f32 near_clip = camera->unknown_60;
            if (near_clip == 0.0f)
                near_clip = camera->near_clip;
            const i32 angle = static_cast<i32>(camera->fov * 0.5f * 10430.378f);
            const f32 cotangent = NuTrigTable[(angle + 0x4000) >> 1 & 0x7fff] / NuTrigTable[angle >> 1 & 0x7fff];
            const f32 x = camera->aspect * cotangent * viewport->clip_width;
            const f32 y = cotangent * viewport->clip_height;
            memset(&pc_vport_mtx, 0, sizeof(pc_vport_mtx));
            pc_vport_mtx.m00 = x;
            pc_vport_mtx.m11 = y;
            pc_vport_mtx.m23 = 1.0f;
            pc_vport_mtx.m22 = (far_clip + near_clip) / (far_clip - near_clip);
            pc_vport_mtx.m32 = (far_clip * -2.0f * near_clip) / (far_clip - near_clip);
        }
        NuMtxMulVU0(out, view, &pc_vport_mtx);
    }

    void NuCameraSetEx(NUCAMERA *cam, i32 fast) {
        global_camera = *cam;
        FaceYDirStream(NuAtan2D(-global_camera.mtx.m20, -global_camera.mtx.m22));

        if (fast == 0) {
            NuVpUpdate();
        }

        if (camfx == 0) {
            NuMtxInvVU0(&vmtx, &global_camera.mtx);
            NuMtxScaleVU0(&vmtx, &global_camera.scale);
        } else {
            NUMTX camera_inverse;
            NUMTX reflect_inverse;
            NUMTX mirror;
            NuMtxSetIdentity(&mirror);
            mirror.m11 = -1.0f;
            NuMtxInv(&camera_inverse, &global_camera.mtx);
            NuMtxInv(&reflect_inverse, &global_reflect.mtx);
            vmtx = reflect_inverse;
            if (camfx == 1)
                NuMtxMul(&vmtx, &vmtx, &mirror);
            NuMtxMul(&vmtx, &vmtx, &global_reflect.mtx);
            NuMtxMul(&vmtx, &vmtx, &camera_inverse);
        }

        if (__builtin_expect(fast == 0, 0)) {
            NuCameraSetProjectionMtx(&pmtx, global_camera.fov, global_camera.aspect, global_camera.near_clip,
                                     global_camera.far_clip);
            pmtx.m00 *= global_camera.unknown_58;
            pmtx.m11 *= global_camera.unknown_5c;
            pmtx.m20 += global_camera.unknown_50;
            pmtx.m21 += global_camera.unknown_54;
        }

        NuCameraSetVPortClipMtx(&vpc_vport_mtx, &vmtx, &global_camera, fast);
        if (current_clip_scissor_to_viewport != 0) {
            current_clip_scissor_to_viewport = 0;
            fast = 0;
        }
        NuCameraSetScissorClipMtx(&vpc_sci_mtx, &vmtx, &global_camera, fast);
        NuVpGetScalingMtx(&smtx);
        NuMtxMulH(&vpmtx, &vmtx, &pmtx);
        NuMtxMulH(&vpsmtx, &vpmtx, &smtx);
        NuMtxMulH(&psmtx, &pmtx, &smtx);

        if (fast == 0) {
            i32 angle = static_cast<i32>(global_camera.fov * 0.5f * 10430.378f);
            clip_planes.m22 = NuTrigTable[angle >> 1 & 0x7fff];
            clip_planes.m12 = NuTrigTable[(angle + 0x4000) >> 1 & 0x7fff];
            zy = clip_planes.m22 / clip_planes.m12;
            zx = zy / global_camera.aspect;

            // PS2_SREZ_W/H are 4096; PS2_REZ_W/H track the current render
            // dimensions. These ratios define the scissor frustum.
            zxs = static_cast<f32>(PS2_SREZ_W) * zx / static_cast<f32>(PS2_REZ_W);
            zys = static_cast<f32>(PS2_SREZ_H) * zy / static_cast<f32>(PS2_REZ_H);

            clip_planes.m13 = -clip_planes.m12;
            clip_planes.m02 = 0.0f;
            clip_planes.m32 = 0.0f;
            clip_planes.m03 = 0.0f;
            clip_planes.m23 = clip_planes.m22;
            clip_planes.m33 = 0.0f;

            f32 side = zy / cam->aspect;
            clip_planes.m01 = 1.0f / NuFsqrt(side * side + 1.0f);
            clip_planes.m00 = -clip_planes.m01;
            clip_planes.m10 = 0.0f;
            clip_planes.m11 = 0.0f;
            clip_planes.m20 = side * clip_planes.m01;
            clip_planes.m21 = clip_planes.m20;
            clip_planes.m30 = 0.0f;
            clip_planes.m31 = 0.0f;
            NuCameraBuildClipPlanes();
        }

        NuRndrSetViewMtx(&vpsmtx, &vpc_vport_mtx, &vpc_sci_mtx);
        NuRndrLightingStateCurrent.field_0x60 = 1;
        NuRndrLightingStateCurrent.field_0x74 = 0;
        NuRndrSetSpecularLightPS(NULL, NULL);
        NuRndrStateUpdateCameraState();
        if (NuOcclusionManagerIsInitialised())
            NuOcclusionManagerOnCameraSet();
    }

    void NuCameraSet(NUCAMERA *cam) {
        NuCameraSetEx(cam, 0);
    }

    void NuCameraRelock(void) {
        if (prev_lock == 1)
            NuCameraSet(&locked_camera);
    }

    void NuCameraUnlock(void) {
        if (prev_lock == 1)
            NuCameraSet(&cam_copy);
    }

    void NuCameraLock(i32 lock) {
        if (lock != prev_lock && prev_lock == 0) {
            locked_camera = *NuCameraGetCam();
        }
        if (lock != 0) {
            cam_copy = *NuCameraGetCam();
            NuCameraSet(&locked_camera);
        }
        prev_lock = lock;
    }

    void NuCameraSetReflect(NUCAMERA *camera, NuCameraReflect *reflect) {
        global_reflect = *reflect;
        NuCameraSet(camera);
        *reflect = global_reflect;
    }

}

void NuViewPortSet(f32 left, f32 top, f32 right, f32 bottom) {
    static NUVIEWPORT2 vp = {
        0.0f, 0.0f, 640.0f, 224.0f, 8388607.0f, 0.0f, 0.5f, 0.5f, 0.01f, 0.01f, 0.99f, 0.99f, 0.0f, 0.0f, 0.0f, 0.0f,
    };
    vp.x = left;
    vp.y = top;
    vp.width = right - left;
    vp.height = bottom - top;
    NuVpSetCurrent2(&vp);
    NuVpUpdate();
}

void NuVpSetSourceRect(float left, float top, float right, float bottom) {
    const float width = static_cast<float>(PS2_VREZ_W);
    const float height = static_cast<float>(PS2_VREZ_H);
    const float source_width = right - left;
    const float source_height = bottom - top;
    const float scaled_width = (width / source_width) * width;
    float position_scale_x = scaled_width / source_width;
    const float scaled_height = (height / source_height) * height;
    const float position_x = -left * position_scale_x * 0.5f;
    const float position_y = -top * (scaled_height / source_height) * 0.5f;
    NuVpSetPosition2(position_x, position_y);
    NuVpSetSize2(scaled_width, scaled_height);
}

void NuVpSetDestRect(float left, float top, float right, float bottom) {
    NuVpSetPosition2(left, top);
    NuVpSetSize2(right - left, bottom - top);
}

void NuVpGetRegions(f32 *source_x, f32 *source_y, f32 *source_right, f32 *source_bottom, f32 *dest_x, f32 *dest_y,
                    f32 *dest_right, f32 *dest_bottom) {
    *source_x = g_NuVpRegion.source_x;
    *source_y = g_NuVpRegion.source_y;
    *source_right = g_NuVpRegion.source_width;
    *source_bottom = g_NuVpRegion.source_height;
    *dest_x = g_NuVpRegion.dest_x;
    *dest_y = g_NuVpRegion.dest_y;
    *dest_right = g_NuVpRegion.dest_width;
    *dest_bottom = g_NuVpRegion.dest_height;
}

void NuVpResetRegions(void) {
    f32 height = (f32)PS2_VREZ_H;
    f32 width = (f32)PS2_VREZ_W;

    NuVpSetRegions(0.0f, 0.0f, width, height, 0.0f, 0.0f, width, height);
}

extern "C" {
extern "C" i32 NuRndrHighResScreenGrab(char *prefix, f32 scale, f32 a, f32 b, f32 c, i32 number) {
    char path[256];
    static f32 yTiles, xTiles, yPos, xPos;
    static ScreenGrabTileParams params;
    static NUFILE fh;
    static i32 header_size, delay, xOffsetHack;
    void *tile = NULL;
    f32 remainder = NuFmod(scale, 0.5f);
    scale -= remainder;
    if (remainder > 0.25f)
        scale += 0.5f;
    f32 tiles;
    if (scale < 0.5f) {
        scale = 0.5f;
        tiles = 1.0f;
    } else if (scale > 4.0f) {
        scale = 4.0f;
        tiles = 6.0f;
    } else {
        f32 n = scale / 0.75f;
        tiles = static_cast<f32>(static_cast<i32>(n));
        if (!(n <= tiles))
            tiles += 1.0f;
    }
    yTiles = tiles;
    xTiles = tiles;
    if (dump_state == -1) {
        NuRndrDoingScreenGrab = 1;
        NuRndrScreenGrabTileInit(&params, static_cast<i32>(tiles * tiles), b, a, c);

        if (number >= 0)
            sprintf(path, "%s%d.bmp", prefix, number);
        else {
            sprintf(path, "%s.bmp", prefix);
            i32 index = 0;
            while (NuFileSize(path) > 0)
                sprintf(path, "%s%03d.bmp", prefix, index++);
        }
        struct __attribute__((packed)) BitmapHeader {
            u16 type;
            u32 file_size;
            u16 reserved1, reserved2;
            u32 offset;
            u32 info_size, width, height;
            u16 planes, bits;
            u32 compression, image_size, xppm, yppm, colours, important;
        } header;
        header.type = 0x4d42;
        header.file_size = PS2_REZ_W * 12 * PS2_REZ_H + 56;
        header.reserved1 = header.reserved2 = 0;
        header.offset = 56;
        header.info_size = 40;
        header.width = static_cast<u32>(static_cast<f32>(PS2_REZ_W) * scale);
        header.height = static_cast<u32>(static_cast<f32>(PS2_REZ_H) * scale);
        header.planes = 1;
        header.bits = 24;
        header.compression = 0;
        header.image_size = header.width * header.height * 3;
        header.xppm = header.yppm = 1;
        header.colours = header.important = 0;
        fh = NuFileOpen(path, static_cast<NUFILEMODE>(1));
        if (fh == 0) {
            NuRndrScreenGrabTileDeInit(&params);
            dump_state = -1;
            yPos = xPos = yTiles = xTiles = 0.0f;
            NuVpResetRegions();
            return 0;
        }
        NuFileWrite(fh, &header, 16);
        NuFileWrite(fh, &header.info_size, 40);
        NuFileSeek(fh, static_cast<u32>(header.image_size - 1), static_cast<NUFILESEEK>(1));
        u8 padding = 0;
        NuFileWrite(fh, &padding, 1);
        dump_state = 0;
        header_size = header.offset;
        delay = 3;
        xOffsetHack = 0;
        xPos = yPos = 0.0f;
        f32 w = static_cast<f32>(PS2_VREZ_W), h = static_cast<f32>(PS2_VREZ_H);
        f32 sx = w * 0.0f * 0.75f, sy = h * 0.0f * 0.75f;
        NuVpSetRegions((sx + -0.125f * w) / scale, (sy + -0.125f * h) / scale, (w + sx + -0.125f * w) / scale,
                       (h + sy + -0.125f * h) / scale, 0, 0, w, h);
        return 1;
    }
    if (delay != 0) {
        --delay;
        return 1;
    }
    NuRndrScreenGrabTileBegin(&tile);
    if (__builtin_expect(tile == NULL, 0)) {
        NuRndrScreenGrabTileDeInit(&params);
        dump_state = -1;
        NuFileClose(fh);
        yPos = xPos = yTiles = xTiles = 0.0f;
        NuVpResetRegions();
        return 0;
    }
    u8 *pixels = static_cast<u8 *>(tile);
    i32 bytes = params.channels[3] == -1 ? 3 : 4;
    if (!(bytes == 3 && params.channels[0] == 0 && params.channels[1] == 1 && params.channels[2] == 2)) {
        u8 *out = pixels;
        for (i32 y = 0; y < PS2_REZ_H; ++y) {
            for (i32 x = 0; x < bytes * PS2_REZ_W; x += bytes) {
                u8 rgba[4];
                u8 *in = static_cast<u8 *>(tile) + params.width * y * bytes + x;
                rgba[0] = in[0];
                rgba[1] = in[1];
                rgba[2] = in[2];
                rgba[3] = in[3];
                out[0] = rgba[params.channels[0]];
                out[1] = rgba[params.channels[1]];
                out[2] = rgba[params.channels[2]];
                out += 3;
            }
        }
    }
    f32 w = static_cast<f32>(PS2_REZ_W), h = static_cast<f32>(PS2_REZ_H);
    f32 full_height = scale * h;
    i32 width = static_cast<i32>(xPos != xTiles - 1.0f ? 0.75f * w : scale * w - ((xTiles - 1.0f) * 0.75f) * w);
    i32 height = static_cast<i32>(yPos != yTiles - 1.0f ? 0.75f * h : full_height - ((yTiles - 1.0f) * 0.75f) * h);
    u8 *data = pixels + static_cast<u32>(((static_cast<f32>(static_cast<i32>(params.width)) * 3.0f) * 0.125f) * h) +
               static_cast<u32>((0.125f * w) * 3.0f) +
               static_cast<u32>(((static_cast<f32>(xOffsetHack * 3) * w) / static_cast<f32>(PS2_VREZ_W)) * scale);
    if (yPos == yTiles - 1.0f)
        data += static_cast<u32>(((0.75f * h - static_cast<f32>(height)) * w) * 3.0f);
    i64 offset = static_cast<i64>(header_size) +
                 static_cast<i32>((((full_height - (0.75f * yPos) * h) * w) * scale) * 3.0f) +
                 static_cast<i32>(((0.75f * xPos) * w) * 3.0f) -
                 static_cast<i32>((static_cast<f32>(height * PS2_REZ_W) * scale) * 3.0f);
    if (height > 0) {
        i32 y = 0;
        do {
            NuFileSeek(fh, offset, static_cast<NUFILESEEK>(0));
            offset += static_cast<i32>((static_cast<f32>(PS2_REZ_W) * scale) * 3.0f);
            NuFileWrite(fh, data, width * 3);
            data += params.width * 3;
        } while (++y != height);
    }
    NuRndrScreenGrabTileEnd(&tile);
    ++dump_state;
    if (!(static_cast<f32>(dump_state) < yTiles * xTiles)) {
        NuRndrScreenGrabTileDeInit(&params);
        dump_state = -1;
        NuFileClose(fh);
        yPos = xPos = yTiles = xTiles = 0.0f;
        NuVpResetRegions();
        NuRndrDoingScreenGrab = 0;
        return 0;
    }
    xPos += 1.0f;
    if (xPos >= xTiles) {
        xPos = 0.0f;
        yPos += 1.0f;
    }
    w = static_cast<f32>(PS2_VREZ_W);
    h = static_cast<f32>(PS2_VREZ_H);
    f32 x = (xPos * w) * 0.75f;
    f32 left = (-0.125f * w + x) / scale;
    f32 right = (x + w + -0.125f * w) / scale;
    xOffsetHack = 0;
    if (right > w) {
        xOffsetHack = static_cast<i32>(right - w);
        left -= static_cast<f32>(xOffsetHack);
        right = w;
    }
    f32 y = (yPos * h) * 0.75f;
    NuVpSetRegions(left, (y + -0.125f * h) / scale, right, (h + y + -0.125f * h) / scale, 0, 0, w, h);
    delay = 3;
    return 1;
}

extern "C" void NuRndrSolidTri(NUVEC *a, NUVEC *b, NUVEC *c, i32 colour) {
    NuPrim3DBegin(0, 5, NULL, NULL);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(a->x, a->y, a->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(b->x, b->y, b->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(c->x, c->y, c->z);
    NuPrim3DEnd();
}

extern "C" void NuRndrWireTri(NUVEC *a, NUVEC *b, NUVEC *c, i32 colour) {
    NuPrim3DBegin(3, 5, NULL, NULL);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(a->x, a->y, a->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(b->x, b->y, b->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(c->x, c->y, c->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(a->x, a->y, a->z);
    NuPrim3DEnd();
}

extern "C" void NuRndrLine2di(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, NUMTL *material) {
    const f32 sx = static_cast<f32>(x0) * 0.0625f;
    const f32 sy = static_cast<f32>(y0) * 0.0625f;
    const f32 ex = static_cast<f32>(x1) * 0.0625f;
    const f32 ey = static_cast<f32>(y1) * 0.0625f;
    NuPrim2DBegin(2, 7, material);
    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(sx, sy, 0.0f);
    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(1.0f, 1.0f);
    NuPrim2DAddXYZ(ex, ey, 0.0f);
    NuPrim2DEnd();
}

extern "C" i32 NuRndrLine2d(NuLineVertex2D *vertices, NUMTL *material) {
    NuPrim2DBegin(2, 7, material);
    NuRndrPrimSetColour(vertices[0].colour);
    NuRndrPrimUV(vertices[0].u, vertices[0].v);
    NuPrim2DAddXYZ(vertices[0].x, vertices[0].y, 0.0f);
    NuRndrPrimSetColour(vertices[1].colour);
    NuRndrPrimUV(vertices[1].u, vertices[1].v);
    NuPrim2DAddXYZ(vertices[1].x, vertices[1].y, 0.0f);
    NuPrim2DEnd();
    return 1;
}

extern "C" void NuRndrRect(f32 x, f32 y, f32 z, f32 width, f32 height, f32 u0, f32 v0, f32 u1, f32 v1, i32 colour,
                           NUMTL *material) {
    NuPrim2DBegin(4, 7, material);
    NuRndrPrimUV(u0, v0);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(x, y, z);
    NuRndrPrimUV(u1, v1);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(x + width, y + height, z);
    NuPrim2DEnd();
}

extern "C" void NuRndrRect2d(f32 x, f32 y, f32 z, f32 width, f32 height, i32 colour, NUMTL *material) {
    NuPrim2DBegin(4, 7, material);
    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(x, y, z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(1.0f, 1.0f);
    NuPrim2DAddXYZ(x + width, y + height, z);
    NuPrim2DEnd();
}

extern "C" void NuRndrRect2di(i32 x, i32 y, i32 w, i32 h, i32 colour, numtl_s *mtl) {
    const f32 sx = static_cast<f32>(x) * 0.0625f;
    const f32 sy = static_cast<f32>(y) * 0.0625f;
    const f32 sw = static_cast<f32>(w) * 0.0625f;
    const f32 sh = static_cast<f32>(h) * 0.0625f;

    NuPrim2DBegin(4, 7, mtl);
    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(sx, sy, 0.0f);
    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(1.0f, 1.0f);
    NuPrim2DAddXYZ(sx + sw, sy + sh, 0.0f);
    NuPrim2DEnd();
}

}

void NuRndrRectUV2d(f32 x, f32 y, f32, f32 width, f32 height, f32 u0, f32 v0, f32 u1, f32 v1, i32 colour,
                    numtl_s *material) {
    NuPrim2DBegin(4, 7, material);
    NuRndrPrimUV(u0, v0);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(x, y, 0.0f);
    NuRndrPrimUV(u1, v1);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(x + width, y + height, 0.0f);
    NuPrim2DEnd();
}

extern "C" {
extern "C" void NuRndrRectUV2di(i32 x, i32 y, i32 w, i32 h, f32 u0, f32 v0, f32 u1, f32 v1, i32 colour, numtl_s *mtl) {
    const f32 sx = static_cast<f32>(x) * 0.0625f;
    const f32 sy = static_cast<f32>(y) * 0.0625f;
    const f32 sw = static_cast<f32>(w) * 0.0625f;
    const f32 sh = static_cast<f32>(h) * 0.0625f;

    NuPrim2DBegin(4, 7, mtl);
    NuRndrPrimUV(u0, v0);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(sx, sy, 0.0f);
    NuRndrPrimUV(u1, v1);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(sx + sw, sy + sh, 0.0f);
    NuPrim2DEnd();
}

extern "C" i32 NuRndrLineStrip2d(NuLineVertex2D *vertices, NUMTL *material, i32 count) {
    NuPrim2DBegin(2, 7, material);
    for (i32 i = 0; i < count; ++i) {
        if (!g_NuPrim_NeedsOverbrightening)
            ((PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr)->color = 0xff7f7f7f;
        else
            ((PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr)->color = 0xffffffff;
        u8 *vertex = g_NuPrim_StreamBufferPtr->u8_ptr;
        if (!g_NuPrim_NeedsHalfUVs) {
            *(f32 *)(vertex + 0x10) = 0.0f;
            *(f32 *)(vertex + 0x14) = 0.0f;
        } else {
            *(u16 *)(vertex + 0x10) = 0;
            *(u16 *)(vertex + 0x12) = 0;
        }
        NuPrim2DAddXYZ(vertices[i].x * 0.0625f, vertices[i].y * 0.0625f, 0.0f);
    }
    NuPrim2DEnd();
    return 1;
}

extern "C" void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, NUMTL *material) {
    f32 sx = (f32)x * 0.0625f;
    f32 sy = (f32)y * 0.0625f;
    f32 sw = (f32)width * 0.0625f;
    f32 sh = (f32)height * 0.0625f;
    NuPrim2DBegin(3, 7, material);
    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(sx, sy, 0.0f);
    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(1.0f, 0.0f);
    f32 ex = sx + sw;
    NuPrim2DAddXYZ(ex, sy, 0.0f);
    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(1.0f, 1.0f);
    f32 ey = sy + sh;
    NuPrim2DAddXYZ(ex, ey, 0.0f);
    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(0.0f, 1.0f);
    NuPrim2DAddXYZ(sx, ey, 0.0f);
    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(sx, sy, 0.0f);
    NuPrim2DEnd();
}

extern "C" void NuRndrGradRect2di(i32 x, i32 y, i32 w, i32 h, i32 *colour, numtl_s *mtl) {
    const f32 sx = static_cast<f32>(x) * 0.0625f;
    const f32 sy = static_cast<f32>(y) * 0.0625f;
    const f32 sw = static_cast<f32>(w) * 0.0625f;
    const f32 sh = static_cast<f32>(h) * 0.0625f;

    NuPrim2DBegin(1, 7, mtl);
    NuRndrPrimSetColour(colour[0]);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(sx, sy, 0.0f);
    NuRndrPrimSetColour(colour[1]);
    NuRndrPrimUV(1.0f, 0.0f);
    NuPrim2DAddXYZ(sx + sw, sy, 0.0f);
    NuRndrPrimSetColour(colour[2]);
    NuRndrPrimUV(0.0f, 1.0f);
    NuPrim2DAddXYZ(sx, sy + sh, 0.0f);
    NuRndrPrimSetColour(colour[3]);
    NuRndrPrimUV(1.0f, 1.0f);
    NuPrim2DAddXYZ(sx + sw, sy + sh, 0.0f);
    NuPrim2DEnd();
}

extern "C" i32 NuRndrLine3d(NURND_VERTEX3D *vertices, NUMTL *material, NUMTX *matrix) {
    NuPrim3DBegin(2, 7, material, matrix);
    {
        NuRndrPrimSetColour(vertices[0].colour);
        NuRndrPrimUV(vertices[0].u, vertices[0].v);
        f32 x = vertices[0].position.x;
        f32 y = vertices[0].position.y;
        f32 z = vertices[0].position.z;
        PrimVertexRaw *vertex = (PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr;
        vertex->x = x;
        vertex->y = y;
        vertex->z = z;
        g_NuPrim_StreamBufferPtr->u8_ptr += sizeof(PrimVertexRaw);
        ++g_NuPrim_VertexCount;
    }
    {
        NuRndrPrimSetColour(vertices[1].colour);
        NuRndrPrimUV(vertices[1].u, vertices[1].v);
        f32 x = vertices[1].position.x;
        f32 y = vertices[1].position.y;
        f32 z = vertices[1].position.z;
        PrimVertexRaw *vertex = (PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr;
        vertex->x = x;
        vertex->y = y;
        vertex->z = z;
        g_NuPrim_StreamBufferPtr->u8_ptr += sizeof(PrimVertexRaw);
        ++g_NuPrim_VertexCount;
    }
    NuPrim3DEnd();
    return 1;
}

extern "C" void NuRndr3dLine(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, i32 colour) {
    NuPrim3DBegin(2, 5, NULL, &numtx_identity);
    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(0.0f, 0.0f);
    NuRndrPrimPosition(x0, y0, z0);
    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(1.0f, 1.0f);
    NuRndrPrimPosition(x1, y1, z1);
    NuPrim3DEnd();
}

extern "C" void NuRndrLine3dDbg(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, i32 colour) {
    NuPrim3DBegin(2, 5, nullptr, &numtx_identity);
    if (!g_NuPrim_NeedsOverbrightening)
        ((PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr)->color =
            ((colour >> 1) & 0x007f7f7f) | (colour & 0xff000000);
    else
        ((PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr)->color = colour;
    PrimVertexRaw *vertex = (PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr;
    vertex->x = x0;
    vertex->y = y0;
    vertex->z = z0;
    g_NuPrim_StreamBufferPtr->u8_ptr += sizeof(PrimVertexRaw);
    ++g_NuPrim_VertexCount;
    if (g_NuPrim_NeedsOverbrightening)
        ((PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr)->color = colour;
    else
        ((PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr)->color =
            ((colour >> 1) & 0x007f7f7f) | (colour & 0xff000000);
    vertex = (PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr;
    vertex->x = x1;
    vertex->y = y1;
    vertex->z = z1;
    g_NuPrim_StreamBufferPtr->u8_ptr += sizeof(PrimVertexRaw);
    ++g_NuPrim_VertexCount;
    NuPrim3DEnd();
}

}

static void drawClippedPortal(NUPORTAL *portal, NUPLANE **planes, u8 *plane_count, NUVEC *minimum, NUVEC *maximum) {
    if (draw_portals == 0)
        return;

    NUVEC rays[4];
    NUVEC hits[4];
    NuCameraRayCast(&rays[0], minimum->x, minimum->y);
    NuCameraRayCast(&rays[1], minimum->x, maximum->y);
    NuCameraRayCast(&rays[2], maximum->x, maximum->y);
    NuCameraRayCast(&rays[3], maximum->x, minimum->y);
    NuPlnLine3(&portal->plane, &world_campos, &rays[0], &hits[0]);
    NuPlnLine3(&portal->plane, &world_campos, &rays[1], &hits[1]);
    NuPlnLine3(&portal->plane, &world_campos, &rays[2], &hits[2]);
    NuPlnLine3(&portal->plane, &world_campos, &rays[3], &hits[3]);
    NuCameraUnlock();

    const f32 left = minimum->x * 0.5f + 0.5f;
    const f32 top = 0.5f - minimum->y * 0.5f;
    const f32 right = maximum->x * 0.5f + 0.5f;
    const f32 bottom = 0.5f - maximum->y * 0.5f;
    NuPortalLineVertex2D lines[8];
    lines[0].x = left;
    lines[0].y = top;
    lines[1].x = left;
    lines[1].y = bottom;
    lines[2].x = left;
    lines[2].y = bottom;
    lines[3].x = right;
    lines[3].y = bottom;
    lines[4].x = right;
    lines[4].y = bottom;
    lines[5].x = right;
    lines[5].y = top;
    lines[6].x = right;
    lines[6].y = top;
    lines[7].x = left;
    lines[7].y = top;
    for (i32 i = 0; i < 8; ++i) {
        lines[i].unused[0] = 1.0f;
        lines[i].colour = 0xff00;
    }
    NuRndrLineStrip2d(reinterpret_cast<NuLineVertex2D *>(lines), NULL, 8);

    NuRndrLine3dDbg(hits[0].x, hits[0].y, hits[0].z, hits[1].x, hits[1].y, hits[1].z, static_cast<i32>(0xff0000ffU));
    NuRndrLine3dDbg(hits[1].x, hits[1].y, hits[1].z, hits[2].x, hits[2].y, hits[2].z, static_cast<i32>(0xff0000ffU));
    NuRndrLine3dDbg(hits[2].x, hits[2].y, hits[2].z, hits[3].x, hits[3].y, hits[3].z, static_cast<i32>(0xff0000ffU));
    NuRndrLine3dDbg(hits[3].x, hits[3].y, hits[3].z, hits[0].x, hits[0].y, hits[0].z, static_cast<i32>(0xff0000ffU));

    NUVEC nearest_point;
    for (i32 i = 0; i < 4; ++i) {
        NuVecNorm(&rays[i], &rays[i]);
        if (*plane_count != 0) {
            f32 nearest_distance = FLT_MAX;
            for (u32 plane_index = 0; plane_index < *plane_count; ++plane_index) {
                NUVEC point;
                const f32 distance = NuPlnLine3(&(*planes)[plane_index], &hits[i], &rays[i], &point);
                if (distance > 0.0f && distance < nearest_distance) {
                    nearest_point = point;
                    nearest_distance = distance;
                }
            }
        }
        NuRndrLine3dDbg(nearest_point.x, nearest_point.y, nearest_point.z, hits[i].x, hits[i].y, hits[i].z,
                        static_cast<i32>(0xff0000ffU));
    }
    NuCameraRelock();
}

static void roomRecursive(NUGSCN *scene, NUFRUSTRUM *frustum, i16 room_id, i16 previous_room, i32 depth) {
    if (depth >= 2) {
        for (i32 i = 0; i < depth - 1; ++i) {
            if (__builtin_expect(rooms_visited[i] != room_id, 0)) {
                continue;
            }
            return;
        }
    }
    if (__builtin_expect(depth >= 16, 0)) {
        return;
    }

    NUROOM *room = &scene->rooms[room_id];
    rooms_visited[depth] = room_id;
    if (room_id != camera_roomid) {
        clipRoomAgainstFrustrum(scene, room, frustum);
    } else {
        FlagRoomInstancesAsVisible(room, scene);
    }
    room->flags |= NUROOM_FLAG_VISITED;
    if (depth >= scene->portal_depth || room->portal_count == 0) {
        return;
    }

    auto room_priority = room->priority;
    for (i32 portal_index = 0; portal_index < room->portal_count; ++portal_index) {
        NUPORTAL *portal = &scene->portals[room->portal_indices[portal_index]];
        i16 next_room = portal->front_room == room_id ? portal->back_room : portal->front_room;
        if (next_room == previous_room || (portal->is_active & NUPORTAL_FLAG_ACTIVE) == 0) {
            continue;
        }

        NUROOM *next = &scene->rooms[next_room];
        const f32 camera_side = planeDistance(portal->plane, world_campos);
        if (next->priority > room_priority && ((room_id == portal->back_room && camera_side < 0.0f) ||
                                               (room_id == portal->front_room && camera_side > 0.0f))) {
            continue;
        }

        i32 inside_tests = 0;
        bool rejected = false;
        for (i32 plane_index = 0; plane_index < frustum->plane_count; ++plane_index) {
            i32 outside_vertices = 0;
            for (i32 vertex_index = 0; vertex_index < portal->vertex_count; ++vertex_index) {
                if (!(planeDistance(frustum->planes[plane_index], portal->vertices[vertex_index]) >= 0.0f)) {
                    ++outside_vertices;
                } else {
                    ++inside_tests;
                }
            }
            if (outside_vertices == portal->vertex_count) {
                rejected = true;
                break;
            }
        }
        if (rejected) {
            continue;
        }

        NUFRUSTRUM *next_frustum = NULL;
        if (inside_tests != frustum->plane_count * portal->vertex_count) {
            i32 in_front = 0;
            for (i32 i = 0; i < portal->vertex_count; ++i) {
                if (planeDistance(cam_plane, portal->vertices[i]) > 0.0f) {
                    ++in_front;
                }
            }
            if (in_front == 0) {
                continue;
            }
            if (in_front != portal->vertex_count) {
                next_frustum = copyFrustrum(frustum, next_room);
            } else {
                NUVEC portal_minimum;
                NUVEC portal_maximum;
                computePortalExtents(&portal->vertices, &portal->vertex_count, &portal_maximum, &portal_minimum);
                if (!(frustum->minimum.x < portal_maximum.x) || frustum->minimum.y >= portal_maximum.y ||
                    portal_minimum.x >= frustum->maximum.x || portal_minimum.y >= frustum->maximum.y) {
                    continue;
                }
                if (portal_minimum.x < frustum->minimum.x)
                    portal_minimum.x = frustum->minimum.x;
                if (portal_minimum.y < frustum->minimum.y)
                    portal_minimum.y = frustum->minimum.y;
                if (portal_maximum.x > frustum->maximum.x)
                    portal_maximum.x = frustum->maximum.x;
                if (portal_maximum.y > frustum->maximum.y)
                    portal_maximum.y = frustum->maximum.y;
                drawClippedPortal(portal, &next->planes, &next->plane_count, &portal_minimum, &portal_maximum);
                next_frustum = buildFrustrum(&portal_minimum, &portal_maximum, next_room);
            }
        } else {
            if (draw_portals != 0) {
                NuCameraUnlock();
                NUVEC direction;
                for (i32 i = 0; i < portal->vertex_count; ++i) {
                    NUVEC *first = &portal->vertices[i];
                    NUVEC *second = &portal->vertices[(i + 1) % portal->vertex_count];
                    NuVecSub(&direction, first, second);
                    NuRndrLine3dDbg(first->x, first->y, first->z, second->x, second->y, second->z,
                                    static_cast<i32>(0xffff00ffU));
                }
                NUVEC nearest_point = {0.0f, 0.0f, 0.0f};
                for (i32 i = 0; i < portal->vertex_count; ++i) {
                    NUVEC *vertex = &portal->vertices[i];
                    NuVecSub(&direction, vertex, &world_campos);
                    NuVecNorm(&direction, &direction);
                    f32 nearest_distance = FLT_MAX;
                    for (i32 plane_index = 0; plane_index < next->plane_count; ++plane_index) {
                        NUVEC point;
                        const f32 distance = NuPlnLine3(&next->planes[plane_index], vertex, &direction, &point);
                        if (!(distance <= 0.0f) && distance < nearest_distance) {
                            nearest_point = point;
                            nearest_distance = distance;
                        }
                    }
                    NuRndrLine3dDbg(nearest_point.x, nearest_point.y, nearest_point.z, vertex->x, vertex->y, vertex->z,
                                    static_cast<i32>(0xffff00ffU));
                }
                NuCameraRelock();
            }
            next_frustum = buildPortalFrustrum(portal, next_room);
        }
        roomRecursive(scene, next_frustum, next_room, room_id, depth + 1);
    }
}

extern "C" {
extern "C" void NuRndrSphereEx(NUVEC *centre, f32 radius, i32 colour, f32 vertical_scale, i32 angle_step) {
    f32 radius_squared = radius * radius;
    for (i32 band = 0; band < 8; ++band) {
        f32 lower_radius = radius * NU_SIN_LUT((band < 4 ? band : 8 - band) * 4096);
        f32 upper_radius = radius * NU_SIN_LUT((band < 3 ? band + 1 : 7 - band) * 4096);
        f32 lower_y = NuFsqrt(radius_squared - lower_radius * lower_radius) * vertical_scale;
        f32 upper_y = NuFsqrt(radius_squared - upper_radius * upper_radius) * vertical_scale;
        if (__builtin_expect(band > 4, 0))
            lower_y = -lower_y;
        if (__builtin_expect(band > 3, 0))
            upper_y = -upper_y;
        lower_y += centre->y;
        upper_y += centre->y;
        for (i32 angle = 0; angle < 65536; angle += angle_step) {
            NuRndrLine3dDbg(centre->x + lower_radius * NU_COS_LUT(angle), lower_y,
                            centre->z + lower_radius * NU_SIN_LUT(angle), centre->x + upper_radius * NU_COS_LUT(angle),
                            upper_y, centre->z + upper_radius * NU_SIN_LUT(angle), colour);
            NuRndrLine3dDbg(centre->x + lower_radius * NU_COS_LUT(angle), lower_y,
                            centre->z + lower_radius * NU_SIN_LUT(angle),
                            centre->x + lower_radius * NU_COS_LUT(angle + angle_step), lower_y,
                            centre->z + lower_radius * NU_SIN_LUT(angle + angle_step), colour);
        }
    }
}

extern "C" void NuRndrSphere(NUVEC *centre, f32 radius, i32 colour, f32 vertical_scale) {
    f32 radius_squared = radius * radius;
    for (i32 band = 0; band < 8; ++band) {
        f32 lower_radius = radius * NU_SIN_LUT((band < 4 ? band : 8 - band) * 4096);
        f32 upper_radius = radius * NU_SIN_LUT((band < 3 ? band + 1 : 7 - band) * 4096);
        f32 lower_y = NuFsqrt(radius_squared - lower_radius * lower_radius) * vertical_scale;
        f32 upper_y = NuFsqrt(radius_squared - upper_radius * upper_radius) * vertical_scale;
        if (__builtin_expect(band > 4, 0))
            lower_y = -lower_y;
        if (__builtin_expect(band > 3, 0))
            upper_y = -upper_y;
        lower_y += centre->y;
        upper_y += centre->y;
        for (i32 angle = 0; angle < 65536; angle += 2048) {
            NuRndrLine3dDbg(centre->x + lower_radius * NU_COS_LUT(angle), lower_y,
                            centre->z + lower_radius * NU_SIN_LUT(angle), centre->x + upper_radius * NU_COS_LUT(angle),
                            upper_y, centre->z + upper_radius * NU_SIN_LUT(angle), colour);
            NuRndrLine3dDbg(centre->x + lower_radius * NU_COS_LUT(angle), lower_y,
                            centre->z + lower_radius * NU_SIN_LUT(angle),
                            centre->x + lower_radius * NU_COS_LUT(angle + 2048), lower_y,
                            centre->z + lower_radius * NU_SIN_LUT(angle + 2048), colour);
        }
    }
}

extern "C" i32 NuPortalVisibility(NUGSCN *scene) {
    Initialise_PS(scene);
    NUVEC *camera_position = override_campos != NULL ? override_campos : &world_campos;
    if (portals_enabled == 0) {
        SetAllInstancesVisible(scene);
        return 0;
    }
    if (scene->max_portals == 0 || scene->num_rooms == 0 || scene->portal_instance_count == 0) {
        return 0;
    }

    scene->num_portal_frusta = 0;
    nfrustra = &scene->num_portal_frusta;
    frustra = scene->portal_frusta;
    SetAllInstancesHidden(scene);
    cam = NuCameraGetCam();
    local_inv_view_mtx = *NuCameraGetMtx();
    world_campos = *NUMTX_GET_ROW_VEC(&global_camera.mtx, 3);

    const f32 near_clip = cam->near_clip;
    camera_roomid = static_cast<i16>(NuPortalWhichRoom(scene, camera_position));
    scene->camera_room = camera_roomid;
    if (camera_roomid == -1) {
        return 0;
    }

    const f32 forward_x = local_inv_view_mtx.m20;
    const f32 forward_y = local_inv_view_mtx.m21;
    const f32 forward_z = local_inv_view_mtx.m22;
    near_clip_plane.a = forward_x;
    near_clip_plane.b = forward_y;
    near_clip_plane.c = forward_z;
    near_clip_plane.d =
        -((near_clip * forward_x + world_campos.x) * forward_x + (near_clip * forward_y + world_campos.y) * forward_y +
          (near_clip * forward_z + world_campos.z) * forward_z);

    cam_plane.a = forward_x;
    cam_plane.b = forward_y;
    cam_plane.c = forward_z;
    cam_plane.d = -(forward_x * world_campos.x + forward_y * world_campos.y + forward_z * world_campos.z);

    NUFRUSTRUM *frustum = buildFrustrum(&nuvec_minus_one, &nuvec_one, -2);
    if (draw_portals != 0) {
        NUVEC rays[4];
        NUVEC corners[4];
        NuCameraRayCast(&rays[0], -1.0f, -1.0f);
        NuCameraRayCast(&rays[1], -1.0f, 1.0f);
        NuCameraRayCast(&rays[2], 1.0f, 1.0f);
        NuCameraRayCast(&rays[3], 1.0f, -1.0f);
        NuVecScale(&corners[0], &rays[0], 0.2f);
        NuVecAdd(&corners[0], &corners[0], &world_campos);
        NuVecScale(&corners[1], &rays[1], 0.2f);
        NuVecAdd(&corners[1], &corners[1], &world_campos);
        NuVecScale(&corners[2], &rays[2], 0.2f);
        NuVecAdd(&corners[2], &corners[2], &world_campos);
        NuVecScale(&corners[3], &rays[3], 0.2f);
        NuVecAdd(&corners[3], &corners[3], &world_campos);
        NuCameraUnlock();
#define DRAW_PORTAL_CORNER(index, next)                                                                                \
    NuRndrLine3dDbg(world_campos.x, world_campos.y, world_campos.z, corners[index].x, corners[index].y,                \
                    corners[index].z, -1);                                                                             \
    NuRndrLine3dDbg(corners[index].x, corners[index].y, corners[index].z, corners[next].x, corners[next].y,            \
                    corners[next].z, static_cast<i32>(0xffff0000U))
        DRAW_PORTAL_CORNER(0, 1);
        DRAW_PORTAL_CORNER(1, 2);
        DRAW_PORTAL_CORNER(2, 3);
        DRAW_PORTAL_CORNER(3, 0);
#undef DRAW_PORTAL_CORNER
        NuCameraRelock();
    }
    for (u32 i = 0; i < static_cast<u32>(scene->num_rooms); ++i) {
        scene->rooms[i].flags &= ~NUROOM_FLAG_VISITED;
    }
    roomRecursive(scene, frustum, camera_roomid, -1, 0);
    return 1;
}

    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context) {
        void *result = NULL;
        if (visibility_context != NULL && do_visibility != 0) {
            result = &scene->visibility_result_instance_count;
            NuCameraLock(VisiSysCameraLock);
            scene->visibility_context = NULL;
            scene->instance_tree_visibility_flags = NULL;
            NUGSCN *source_scene = scene->visibility_source_scene;
            scene->visibility_result_instance_count =
                source_scene == NULL ? scene->num_instances : source_scene->num_instances;
            scene->visibility_state &= 0xea;
            LoadedOcclusionData = result != NULL;
            if (do_occlusion != 0 && scene->occlusion_data != NULL) {
                NuVisiOcclusion(static_cast<NuVisibilityResult *>(result));
            }
            if (scene->portal_visibility_marker == NULL || portals_enabled == 0) {
                if (scene->instance_visibility_tree != NULL && do_InstTree != 0) {
                    scene->visibility_context = visibility_context;
                    NuVisiInstTree(result, scene);
                    scene->visibility_state |= 1;
                }
            } else {
                NuPortalVisibility(scene);
            }
            NuCameraUnlock();
        }
        return result;
    }

}

void NuRndrRectUV2diZ(i32 x, i32 y, i32 width, i32 height, f32 u0, f32 v0, f32 u1, f32 v1, i32 colour,
                      numtl_s *material, i32 z) {
    const f32 sx = static_cast<f32>(x) * 0.0625f;
    const f32 sy = static_cast<f32>(y) * 0.0625f;
    const f32 sw = static_cast<f32>(width) * 0.0625f;
    const f32 sh = static_cast<f32>(height) * 0.0625f;
    const f32 sz = static_cast<f32>(z) * (1.0f / 16777215.0f);
    NuPrim2DBegin(4, 7, material);
    NuRndrPrimUV(u0, v0);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(sx, sy, sz);
    NuRndrPrimUV(u1, v1);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(sx + sw, sy + sh, sz);
    NuPrim2DEnd();
}

extern "C" {
extern "C" void NuRndrLineStrip2di(i32 *positions, f32 *uvs, i32 count, i32 colour, NUMTL *material) {
    NuPrim2DBegin(2, 7, material);
    if (count > 0) {
        const i32 end = count * 2;
        for (i32 i = 0; i != end; i += 2) {
            NuRndrPrimSetColour(colour);
            if (uvs) {
                NuRndrPrimUV(uvs[i], uvs[i + 1]);
            }
            NuPrim2DAddXYZ((f32)positions[i] * 0.0625f, (f32)positions[i + 1] * 0.0625f, 0.0f);
        }
    }
    NuPrim2DEnd();
}

extern "C" void NuRndrTriStrip2di(i32 *positions, f32 *uvs, i32 count, i32 colour, NUMTL *material) {
    NuPrim2DBegin(1, 7, material);
    if (count > 0) {
        const i32 end = count * 2;
        for (i32 i = 0; i != end; i += 2) {
            NuRndrPrimSetColour(colour);
            if (uvs) {
                NuRndrPrimUV(uvs[i], uvs[i + 1]);
            }
            NuPrim2DAddXYZ((f32)positions[i] * 0.0625f, (f32)positions[i + 1] * 0.0625f, 0.0f);
        }
    }
    NuPrim2DEnd();
}

}

void NuRndrRectUV2dNoScale(float, float, float, float, float, float, float, float, i32, numtl_s *) {
    STUBBED();
}

extern "C" {
extern "C" void NuRndrGradRectUV2di(i32 x, i32 y, i32 w, i32 h, f32 u0, f32 v0, f32 u1, f32 v1, u32 *colours,
                                    numtl_s *mtl) {
    const f32 sx = static_cast<f32>(x) * 0.0625f;
    const f32 sy = static_cast<f32>(y) * 0.0625f;
    const f32 width = static_cast<f32>(w) * 0.0625f;
    const f32 height = static_cast<f32>(h) * 0.0625f;

    NuPrim2DBegin(1, 7, mtl);
    NuRndrPrimSetColour(colours[0]);
    NuRndrPrimUV(u0, v0);
    NuPrim2DAddXYZ(sx, sy, 0.0f);
    NuRndrPrimSetColour(colours[1]);
    NuRndrPrimUV(u1, v0);
    const f32 ex = sx + width;
    NuPrim2DAddXYZ(ex, sy, 0.0f);
    NuRndrPrimSetColour(colours[2]);
    NuRndrPrimUV(u0, v1);
    const f32 ey = sy + height;
    NuPrim2DAddXYZ(sx, ey, 0.0f);
    NuRndrPrimSetColour(colours[3]);
    NuRndrPrimUV(u1, v1);
    NuPrim2DAddXYZ(ex, ey, 0.0f);
    NuPrim2DEnd();
}

}

void NuRndrGradRect2diZ(i32 x, i32 y, i32 width, i32 height, i32 *colours, numtl_s *material, i32 z) {
    const f32 sx = static_cast<f32>(x) * 0.0625f;
    const f32 sy = static_cast<f32>(y) * 0.0625f;
    const f32 sw = static_cast<f32>(width) * 0.0625f;
    const f32 sh = static_cast<f32>(height) * 0.0625f;
    const f32 sz = static_cast<f32>(z) * (1.0f / 16777215.0f);
    NuPrim2DBegin(1, 7, material);
    NuRndrPrimSetColour(colours[0]);
    NuPrim2DAddXYZ(sx, sy, sz);
    NuRndrPrimSetColour(colours[1]);
    NuPrim2DAddXYZ(sx + sw, sy, sz);
    NuRndrPrimSetColour(colours[2]);
    NuPrim2DAddXYZ(sx, sy + sh, sz);
    NuRndrPrimSetColour(colours[3]);
    NuPrim2DAddXYZ(sx + sw, sy + sh, sz);
    NuPrim2DEnd();
}

extern "C" {
extern "C" void NuRndrSphereMtx(NUMTX *matrix, i32 colour, i32 segments, NUMTL *material) {
    f32 step = 65536.0f / (f32)segments;
    NuPrim3DBegin(3, 5, material, matrix);
    f32 longitude = 0.0f;
    for (i32 ring = 0; ring < segments / 2; ++ring, longitude += step) {
        f32 sin_longitude = NU_SIN_LUT(longitude);
        f32 cos_longitude = NU_SIN_LUT(longitude + 16384.0f);
        f32 angle = 0.0f;
        for (i32 i = 0; i <= segments; ++i, angle += step) {
            f32 radius = NU_SIN_LUT(angle);
            NuRndrPrimSetColour(colour);
            NuRndrPrimPosition(radius * sin_longitude, NU_SIN_LUT(angle + 16384.0f), radius * cos_longitude);
        }
    }
    NuPrim3DEnd();
    f32 latitude = step;
    for (i32 ring = 1; ring < segments / 2; ++ring, latitude += step) {
        f32 radius = NU_SIN_LUT(latitude);
        f32 y = NU_SIN_LUT(latitude + 16384.0f);
        NuPrim3DBegin(3, 5, material, matrix);
        f32 angle = 0.0f;
        for (i32 i = 0; i <= segments; ++i, angle += step) {
            NuRndrPrimSetColour(colour);
            NuRndrPrimPosition(NU_SIN_LUT(angle) * radius, y, NU_SIN_LUT(angle + 16384.0f) * radius);
        }
        NuPrim3DEnd();
    }
}

extern "C" void NuRndrSphereTRS(NUVEC *translation, NUANGVEC *rotation, NUVEC *scale, i32 colour, i32 segments,
                                NUMTL *material) {
    NUMTX matrix;
    if (rotation)
        NuMtxSetRotateXYZ(&matrix, rotation);
    else
        matrix = numtx_identity;
    if (translation)
        NuMtxTranslate(&matrix, translation);
    if (scale)
        NuMtxPreScale(&matrix, scale);
    NuRndrSphereMtx(&matrix, colour, segments, material);
}

extern "C" void NuRndrBoundingBox(NUVEC *minimum, NUVEC *maximum, NUMTX *matrix, i32 colour) {
    NuPrim3DBegin(3, 5, NULL, matrix);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(minimum->x, minimum->y, minimum->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(minimum->x, maximum->y, minimum->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(maximum->x, maximum->y, minimum->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(maximum->x, minimum->y, minimum->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(minimum->x, minimum->y, minimum->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(minimum->x, minimum->y, maximum->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(minimum->x, maximum->y, maximum->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(maximum->x, maximum->y, maximum->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(maximum->x, minimum->y, maximum->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(minimum->x, minimum->y, maximum->z);
    NuPrim3DEnd();
    NuPrim3DBegin(2, 5, NULL, matrix);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(minimum->x, maximum->y, minimum->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(minimum->x, maximum->y, maximum->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(maximum->x, maximum->y, minimum->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(maximum->x, maximum->y, maximum->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(maximum->x, minimum->y, minimum->z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(maximum->x, minimum->y, maximum->z);
    NuPrim3DEnd();
}

}

void DrawAABox(_vuv_s *position, _vuv_s *size, i32 colour) {
    const NUVEC *centre = reinterpret_cast<const NUVEC *>(position);
    const NUVEC *extent = reinterpret_cast<const NUVEC *>(size);
    NUVEC minimum __attribute__((aligned(16)));
    NUVEC maximum __attribute__((aligned(16)));
    maximum.x = centre->x + extent->x;
    maximum.y = centre->y + extent->y;
    maximum.z = centre->z + extent->z;
    minimum.x = centre->x - extent->x;
    minimum.y = centre->y - extent->y;
    minimum.z = centre->z - extent->z;
    NuCameraUnlock();
    NuRndrBoundingBox(&minimum, &maximum, &numtx_identity, colour);
    NuCameraRelock();
}

void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled) {
    struct OCTREE_STACK_ENTRY_s {
        nuoctreenode_s *node;
        i32 child_index;
    };

    if (enabled == 0 || root == NULL)
        return;

    OCTREE_STACK_ENTRY_s stack[128];
    i32 depth = 0;
    stack[0].child_index = 0;
    stack[0].node = root;

    while (depth >= 0) {
        nuoctreenode_s *node = stack[depth].node;
        f32 minimum_depth;
        i32 clip = NuCameraClipTestExtentsGeneric(&node->minimum, &node->maximum, &numtx_identity, node->far_clip, 1,
                                                  &minimum_depth);
        if (clip == 0) {
        } else if (clip == 1) {
            if (draw_octree != 0)
                NuRndrBoundingBox(&node->minimum, &node->maximum, &numtx_identity, 0xff0000ff);
            i32 *index = node->fully_visible_indices;
            i32 *end = index + node->fully_visible_count;
            if (node->fully_visible_count > 0) {
                do {
                    i32 value = *index++;
                    visibility[value >> 2] |= 1 << ((value & 3) << 1);
                } while (index != end);
            }
        } else {
            i32 *index = node->partially_visible_indices;
            i32 *end = index + node->partially_visible_count;
            if (node->partially_visible_count > 0) {
                do {
                    i32 value = *index++;
                    visibility[value >> 2] |= 2 << ((value & 3) << 1);
                } while (index != end);
            }
            if (node->child_count == 0 && draw_octree != 0)
                NuRndrBoundingBox(&node->minimum, &node->maximum, &numtx_identity, 0xff00ff00);

            i32 child_index = stack[depth].child_index++;
            if (child_index != node->child_count) {
                ++depth;
                stack[depth].child_index = 0;
                stack[depth].node = node->children[child_index];
                continue;
            }
        }
        --depth;
    }
}

extern "C" {
    void NuVisiOctree(NuVisibilityResult *result) {
        struct NuOctreeVisibilityData {
            nuoctreenode_s *root;
        };
        NuOctreeVisibilityData *octree = *reinterpret_cast<NuOctreeVisibilityData **>(result->pad_04);
        if (octree != NULL && do_octree != 0) {
            OctreeRndr(static_cast<u8 *>(result->visibility_context), octree->root, result->instance_count);
            result->state |= 4;
        } else {
            result->state &= ~4;
        }
    }

extern "C" void NuRndrAxisBright(NUMTX *matrix, f32 length, i32 brightness) {
    f32 base = length * 0.8f;
    f32 width = length * 0.1f;
    i32 red = 0xff000000u | (brightness & 255);
    i32 green = 0xff000000u | ((static_cast<u32>(brightness) << 8) & 0xffff);
    i32 blue = 0xff000000u | ((brightness & 255) << 16);
    NuPrim3DBegin(2, 5, NULL, matrix);
    NuRndrPrimSetColour(red);
    NuRndrPrimPosition(0, 0, 0);
    NuRndrPrimSetColour(red);
    NuRndrPrimPosition(length, 0, 0);
    NuRndrPrimSetColour(green);
    NuRndrPrimPosition(0, 0, 0);
    NuRndrPrimSetColour(green);
    NuRndrPrimPosition(0, length, 0);
    NuRndrPrimSetColour(blue);
    NuRndrPrimPosition(0, 0, 0);
    NuRndrPrimSetColour(blue);
    NuRndrPrimPosition(0, 0, length);
    NuPrim3DEnd();
    NuPrim3DBegin(3, 5, NULL, matrix);
    NuRndrPrimSetColour(red);
    NuRndrPrimPosition(base, -width, 0);
    NuRndrPrimSetColour(red);
    NuRndrPrimPosition(length, 0, 0);
    NuRndrPrimSetColour(red);
    NuRndrPrimPosition(base, width, 0);
    NuPrim3DEnd();
    NuPrim3DBegin(3, 5, NULL, matrix);
    NuRndrPrimSetColour(green);
    NuRndrPrimPosition(0, base, width);
    NuRndrPrimSetColour(green);
    NuRndrPrimPosition(0, length, 0);
    NuRndrPrimSetColour(green);
    NuRndrPrimPosition(0, base, -width);
    NuPrim3DEnd();
    NuPrim3DBegin(3, 5, NULL, matrix);
    NuRndrPrimSetColour(blue);
    NuRndrPrimPosition(0, width, base);
    NuRndrPrimSetColour(blue);
    NuRndrPrimPosition(0, 0, length);
    NuRndrPrimSetColour(blue);
    NuRndrPrimPosition(0, -width, base);
    NuPrim3DEnd();
}

extern "C" void NuRndrAxes(NUMTX *matrix, f32 length) {
    NuRndrAxisBright(matrix, length, 255);
}

extern "C" void NuRndrAxisArrowsMtx(NUMTX *matrix, f32 length, NUMTL *material) {
    f32 base = length * 0.8f;
    f32 width = length * 0.1f;
    NuPrim3DBegin(2, 5, material, matrix);
    NuRndrPrimSetColour(0xff0000ff);
    NuRndrPrimPosition(0, 0, 0);
    NuRndrPrimSetColour(0xff0000ff);
    NuRndrPrimPosition(length, 0, 0);
    NuRndrPrimSetColour(0xff00ff00);
    NuRndrPrimPosition(0, 0, 0);
    NuRndrPrimSetColour(0xff00ff00);
    NuRndrPrimPosition(0, length, 0);
    NuRndrPrimSetColour(0xffff0000);
    NuRndrPrimPosition(0, 0, 0);
    NuRndrPrimSetColour(0xffff0000);
    NuRndrPrimPosition(0, 0, length);
    NuPrim3DEnd();
    NuPrim3DBegin(3, 5, material, matrix);
    NuRndrPrimSetColour(0xff0000ff);
    NuRndrPrimPosition(base, -width, 0);
    NuRndrPrimSetColour(0xff0000ff);
    NuRndrPrimPosition(length, 0, 0);
    NuRndrPrimSetColour(0xff0000ff);
    NuRndrPrimPosition(base, width, 0);
    NuPrim3DEnd();
    NuPrim3DBegin(3, 5, material, matrix);
    NuRndrPrimSetColour(0xff00ff00);
    NuRndrPrimPosition(0, base, width);
    NuRndrPrimSetColour(0xff00ff00);
    NuRndrPrimPosition(0, length, 0);
    NuRndrPrimSetColour(0xff00ff00);
    NuRndrPrimPosition(0, base, -width);
    NuPrim3DEnd();
    NuPrim3DBegin(3, 5, material, matrix);
    NuRndrPrimSetColour(0xffff0000);
    NuRndrPrimPosition(0, width, base);
    NuRndrPrimSetColour(0xffff0000);
    NuRndrPrimPosition(0, 0, length);
    NuRndrPrimSetColour(0xffff0000);
    NuRndrPrimPosition(0, -width, base);
    NuPrim3DEnd();
}

extern "C" void NuRndrAxisArrows(NUVEC *position, void *, f32 length, NUMTL *material) {
    NUMTX matrix;
    NuMtxSetIdentity(&matrix);
    NuMtxTranslate(&matrix, position);
    NuRndrAxisArrowsMtx(&matrix, length, material);
}

extern "C" void NuRndrGrid(NUVEC *centre, NUVEC *size, i32 columns, i32 rows) {
    NUMTX matrix = numtx_identity;
    matrix.m30 = centre->x;
    matrix.m31 = 0.0f;
    matrix.m32 = centre->z;
    matrix.m00 = size->x * 0.5f;
    matrix.m11 = 1.0f;
    matrix.m22 = size->z * 0.5f;
    f32 dx = 2.0f / (f32)columns;
    f32 dz = 2.0f / (f32)rows;
    NuPrim3DBegin(2, 5, NULL, &matrix);
    f32 x = -1.0f;
    for (i32 i = 0; i <= columns; ++i, x += dx) {
        NuRndrPrimSetColour(0xff00ff00);
        NuRndrPrimPosition(x, 0.0f, -1.0f);
        NuRndrPrimSetColour(0xff00ff00);
        NuRndrPrimPosition(x, 0.0f, 1.0f);
    }
    f32 z = -1.0f;
    for (i32 i = 0; i <= rows; ++i, z += dz) {
        NuRndrPrimSetColour(0xff00ff00);
        NuRndrPrimPosition(-1.0f, 0.0f, z);
        NuRndrPrimSetColour(0xff00ff00);
        NuRndrPrimPosition(1.0f, 0.0f, z);
    }
    NuPrim3DEnd();
}

extern "C" i32 NuRndrStrip3d(NURND_VERTEX3D *vertices, numtl_s *material, NUMTX *matrix, i32 count) {
    if (count == 0)
        return 1;
    NuPrim3DBegin(1, 7, material, matrix);
    for (i32 i = 0; i < count; ++i) {
        NURND_VERTEX3D *source = &vertices[i];
        NuRndrPrimSetColour(source->colour);
        NuRndrPrimUV(source->u, source->v);
        f32 z = source->position.z;
        f32 y = source->position.y;
        f32 x = source->position.x;
        PrimVertexRaw *vertex = (PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr;
        vertex->x = x;
        vertex->y = y;
        vertex->z = z;
        g_NuPrim_StreamBufferPtr->u8_ptr += sizeof(PrimVertexRaw);
        ++g_NuPrim_VertexCount;
    }
    NuPrim3DEnd();
    return 1;
}

extern "C" i32 NuRndrTriStrip3dClip(NURND_VERTEX3D *vertices, i32 count, NUMTX *matrix, numtl_s *material) {
    return NuRndrStrip3d(vertices, material, matrix, count);
}

extern "C" i32 NuRndrTri3dClip(NURND_VERTEX3D *vertices, i32 count, NUMTX *matrix, numtl_s *material) {
    if (count == 0)
        return 1;
    NuPrim3DBegin(0, 7, material, matrix);
    for (i32 i = 0; i < count; ++i) {
        NuRndrPrimSetColour(vertices[i].colour);
        NuRndrPrimUV(vertices[i].u, vertices[i].v);
        f32 z = vertices[i].position.z;
        f32 y = vertices[i].position.y;
        f32 x = vertices[i].position.x;
        PrimVertexRaw *vertex = (PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr;
        vertex->x = x;
        vertex->y = y;
        vertex->z = z;
        g_NuPrim_StreamBufferPtr->u8_ptr += sizeof(PrimVertexRaw);
        ++g_NuPrim_VertexCount;
    }
    NuPrim3DEnd();
    return 1;
}

extern "C" __attribute__((force_align_arg_pointer)) i32 NuRndrCircle(f32 x, f32 y, f32 radius, f32 aspect, i32 count, f32 u0, f32 v0, f32 u1, f32 v1, i32 colour,
                            numtl_s *material) {
    NuPrim2DBegin(0, 7, material);
    radius *= circle_scale_radius;
    u1 -= u0;
    v1 -= v0;
    f32 previous_y = radius + y;
    f32 previous_x = radius * 0.0f * aspect + x;
    f32 previous_u = 0.0f + u0;
    f32 previous_v = v1 + v0;
    f32 step = 6.284f / static_cast<f32>(count - 1);
    for (i32 i = 0; i < count; ++i) {
        f32 angle = static_cast<f32>(i) * step;
        f32 sine = NuSinf(angle);
        f32 cosine = NuCosf(angle);
        f32 half_cosine = 0.5f * cosine;
        f32 half_sine = 0.5f * sine;
        f32 next_x = sine * radius * aspect + x;
        f32 next_y = cosine * radius + y;
        f32 next_u = (half_sine + 0.5f) * u1 + u0;
        f32 next_v = v1 * (0.5f - half_cosine) + v0;
        NuRndrPrimSetColour(colour);
        NuRndrPrimUV(0.5f * u1 + u0, 0.5f * v1 + v0);
        NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * x, static_cast<f32>(PS2_VREZ_H) * y, 0.0f);
        NuRndrPrimSetColour(colour);
        NuRndrPrimUV(previous_u, previous_v);
        NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * previous_x, static_cast<f32>(PS2_VREZ_H) * previous_y, 0.0f);
        NuRndrPrimSetColour(colour);
        NuRndrPrimUV(next_u, next_v);
        NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * next_x, static_cast<f32>(PS2_VREZ_H) * next_y, 0.0f);
        previous_x = next_x;
        previous_y = next_y;
        previous_u = next_u;
        previous_v = next_v;
    }
    NuPrim2DEnd();
    return 1;
}

}

void NuWindInitialise(NUWIND *wind) {
    if (wind != NULL) {
        wind->unk2.x = 1.0f;
        wind->unk2.y = 1.0f;
        wind->unk2.z = 0.0f;
        wind->unk2.w = 0.0f;
        wind->unk3 = 0.0f;
        wind->unk1 = -1;

        for (usize i = 0; i < 8; ++i) {
            wind->unk0[i] = -1;
        }
    }
}

extern "C" {
    void NuWindSetCurrent(NUWIND *wind, i32 index) {
        if ((u32)index > 7 || wind == NULL) {
            nuapi.wind = NULL;
        } else if (wind->unk0[index] >= 0) {
            wind->unk1 = index;
            nuapi.wind = wind;
        }
    }

}

void NuWindSetWorldSize(NUWIND *wind, f32 size) {
    if (wind != NULL) {
        if (size >= 1.0f) {
            wind->unk2.x = size;
        } else {
            wind->unk2.x = 1.0f;
        }
    }
}

void NuWindSetSpeed(NUWIND *wind, f32 speed) {
    if (wind != NULL) {
        if (speed >= 1.0f) {
            wind->unk2.y = speed;
        } else {
            wind->unk2.y = 1.0f;
        }
    }
}

i32 NuWindCurrent(NUWIND *wind) {
    if (wind == NULL || wind->unk1 < 0)
        return -1;
    return wind->unk0[wind->unk1];
}

void NuWindAnimate(NUWIND *wind, f32 frametime) {
    if (wind != NULL) {
        wind->unk2.z += (1.0f / 256.0f) * wind->unk2.y * frametime;
        if (wind->unk2.z >= 1.0f) {
            wind->unk2.z = NuFmod(wind->unk2.z, 1.0f);
        }
        f32 elapsed = wind->unk2.w + frametime;
        f32 phase = wind->unk3 + 5.0f * frametime;
        wind->unk2.w = elapsed;
        wind->unk3 = phase;
    }
}

void ClearScreen() {
    ++NuPrimCSPos;
    NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_NORMALISED);
    NuPrim2DBegin(1, 7, NULL);
    NuRndrPrimSetColour(0x80000000);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(-1.0f, -1.0f, 0.0f);
    NuRndrPrimSetColour(0x80000000);
    NuRndrPrimUV(1.0f, 0.0f);
    NuPrim2DAddXYZ(1.0f, -1.0f, 0.0f);
    NuRndrPrimSetColour(0x80000000);
    NuRndrPrimUV(0.0f, 1.0f);
    NuPrim2DAddXYZ(-1.0f, 1.0f, 0.0f);
    NuRndrPrimSetColour(0x80000000);
    NuRndrPrimUV(1.0f, 1.0f);
    NuPrim2DAddXYZ(1.0f, 1.0f, 0.0f);
    NuPrim2DEnd();
    --NuPrimCSPos;
    NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
}

void DrawAlphaGrid(i32 rows, i32 cols, NuBloomParameters *parameters) {
    f32 inv_col = 1.0f / (cols - 1);
    f32 inv_row = 1.0f / (rows - 1);
    f32 x0 = 0.0f * inv_row * 2.0f - 1.0f;
    f32 y_start = 0.0f * inv_col * 2.0f - 1.0f;
    f32 x1 = inv_row * 2.0f - 1.0f;
    f32 y_next = inv_col * 2.0f - 1.0f;
    f32 step_x = inv_row * 2.0f;
    f32 step_y = inv_col * 2.0f;
    f32 near_scale = parameters->near_scale * 128.0f;
    f32 far_scale = parameters->far_scale * 128.0f;
    f32 near_angle = parameters->near_angle / 180.0f;
    f32 far_angle = parameters->far_angle / 180.0f;
    f32 angle_delta = far_angle - near_angle;
    f32 scale_delta = far_scale - near_scale;
    NUCAMERA camera;
    f32 brightness;
    NuCameraGet(&camera);
    static i32 first = 1;
    static f32 camFov;
    static f32 adjacent;
    static i32 row, col;
    if (first) {
        camFov = camera.fov;
        adjacent = 1.0f / NU_TAN_LUT((i32)(camera.fov * 0.5f * 10430.3779296875f));
        first = 0;
    }
    if (camera.fov != camFov) {
        adjacent = 1.0f / NU_TAN_LUT((i32)(camera.fov * 0.5f * 10430.3779296875f));
        // The original writes the saved FOV back into this camera copy.
        camera.fov = camFov;
    }
    camera.mtx.m30 = 0.0f;
    camera.mtx.m31 = 0.0f;
    camera.mtx.m32 = 0.0f;
    camera.mtx.m33 = 1.0f;
    ++NuPrimCSPos;
    NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_NORMALISED);
    for (row = 0; row < rows - 1; ++row) {
        NuPrim2DBegin(2, 5, NULL);
        f32 y0 = y_start;
        f32 y1 = y_next;
        for (col = 0; col < cols; ++col) {
            NUVEC direction;
            ALPHA_GRID_VERTEX(x0, y0);
            ALPHA_GRID_VERTEX(x1, y0);
            if (col != cols - 1) {
                ALPHA_GRID_VERTEX(x0, y0);
                ALPHA_GRID_VERTEX(x0, y1);
                ALPHA_GRID_VERTEX(x1, y0);
                ALPHA_GRID_VERTEX(x1, y1);
                ALPHA_GRID_VERTEX(x1, y0);
                ALPHA_GRID_VERTEX(x0, y1);
            }
            y0 += step_y;
            y1 += step_y;
        }
        NuPrim2DEnd();
        x0 += step_x;
        x1 += step_x;
    }
    --NuPrimCSPos;
    NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
}

extern "C" {
// original 0x2e6e90 — rotate to the next stream buffer, recording the high
// watermark. The infinite-loop tail is the original's buffer-overflow trap.
extern "C" void NuRndrSwapStreamBuffers(void) {
    u32 last_used;

    last_used = rndrstream_free.addr - rndrstream[rndrstream_buffid].addr;
    rndrstream_used = last_used;
    rndrstream_last_used = last_used;
    if (last_used > rndrstream_max_used) {
        rndrstream_max_used = last_used;
    }
    if (static_cast<u32>(nurndr_maxstreamsize) < last_used) {
        for (;;) {
        }
    }
    rndrstream_buffid++;
    if (rndrstream_buffid == rndrstream_nbuffers) {
        rndrstream_buffid = 0;
    }
    rndrstream_free.addr = ALIGN(rndrstream[rndrstream_buffid].addr, 16);
    rndrstream_end.addr = rndrstream[rndrstream_buffid].addr + nurndr_maxstreamsize;
}

}

void NuRndrStreamInit(i32 stream_buffer_size, VARIPTR *buffer) {
    i32 i;

    if (buffer == NULL) {
        for (i = 0; i < rndrstream_nbuffers; i++) {
            rndrstream[i].void_ptr = NU_ALLOC(stream_buffer_size, 4, 1, "", NUMEMORY_CATEGORY_NONE);
            memset(rndrstream[i].void_ptr, 0, stream_buffer_size);
        }
    } else {
        for (i = 0; i < rndrstream_nbuffers; i++) {
            buffer->addr = ALIGN(buffer->addr, 0x80);
            void *stream = buffer->void_ptr;
            rndrstream[i].void_ptr = stream;

            memset(stream, 0, stream_buffer_size);

            buffer->void_ptr = (void *)(buffer->addr + stream_buffer_size);
        }
    }

    rndrstream_buffid = 0;
    nurndr_maxstreamsize = stream_buffer_size;
    rndrstream_end.addr = rndrstream[0].addr + stream_buffer_size;
    rndrstream_free.addr = ALIGN(rndrstream[0].addr, 16);
}

void NuRndrInitEx(i32 stream_buffer_size, VARIPTR *buffer, VARIPTR buffer_end) {
    NuRndrStreamInit(stream_buffer_size, buffer);
}

i32 NuLgtRand() {
    NuLgtSeed = (NuLgtSeed * 0x24cd + 1) & 0xffff;
    return NuLgtSeed;
}

extern "C" {
    void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length) {
        if (NuLgtLaserCnt > 63)
            return;
        NULGTLASER *laser = &NuLgtLaserData[NuLgtLaserCnt];
        laser->width_wobble = width_wobble;
        laser->segment_length = segment_length;
        laser->arc = 0;
        laser->end_width_ratio = end_width / width;
        laser->width = width;
        laser->type = static_cast<u8>(type);
        laser->start = *start;
        laser->length = length;
        laser->end.x = delta->x + start->x;
        laser->end.y = delta->y + start->y;
        laser->end.z = delta->z + start->z;
        laser->colour = colour;
        // The original tests the arc cursor here, even for a straight laser.
        if ((NuLgtArcLaserFrame & 1) == 0 || NuLgtLaserData[NuLgtArcLaserCnt].seed == 0) {
            NuLgtLaserData[NuLgtLaserCnt].seed = NuLgtRand();
        }
        NuLgtRand();
        NuLgtRand();
        NuLgtRand();
        NuLgtRand();
        NuLgtRand();
        NuLgtRand();
        ++NuLgtLaserCnt;
    }

    void NuLgtLaserDraw(i32 paused) {
        if (paused)
            NuLgtLaserCnt = NuLgtLaserOldCnt;
        if (NuLgtArcMtl[0].material == NULL || NuLgtLaserCnt == 0) {
            NuLgtLaserOldCnt = 0;
            return;
        }
        u32 saved_seed = NuLgtSeed;
        f32 v1 = NuLgtArcMtl[0].v1;
        f32 v0 = NuLgtArcMtl[0].v0;
        // Four strip corners, two projected endpoints, and two clipped world endpoints.
        NUVEC4 *vertices = (NUVEC4 *)NuScratchAlloc32(0x80);
        NUMTX clip_matrix;
        NuCameraGetClipMtx(&clip_matrix, NULL);
        f32 aspect = (f32)PS2_REZ_W / (f32)PS2_REZ_H;
        NULGTLASER *laser = NuLgtLaserData;
        for (i32 i = 0; i < NuLgtLaserCnt; ++i, ++laser) {
            vertices[4].x = laser->start.x;
            vertices[4].y = laser->start.y;
            vertices[4].z = laser->start.z;
            vertices[4].w = 1.0f;
            NuVec4MtxTransformVU0(&vertices[4], &vertices[4], &clip_matrix);
            vertices[5].x = laser->end.x;
            vertices[5].y = laser->end.y;
            vertices[5].z = laser->end.z;
            vertices[5].w = 1.0f;
            NuVec4MtxTransformVU0(&vertices[5], &vertices[5], &clip_matrix);
            if (!(vertices[4].w >= 0.5f) && !(vertices[5].w >= 0.5f))
                continue;
            i32 red, green, blue, alpha;
            if (vertices[4].w < 0.5f) {
                f32 numerator = vertices[5].w - 0.5f;
                f32 denominator = vertices[5].w - vertices[4].w;
                if (!laser->arc) {
                    green = (laser->colour >> 8) & 0xff;
                    blue = (laser->colour >> 16) & 0xff;
                    red = laser->colour & 0xff;
                    alpha = (laser->colour >> 24) & 0xff;
                } else {
                    red = (i32)((f32)(laser->colour & 0xff) * numerator / denominator);
                    green = (i32)((f32)((laser->colour >> 8) & 0xff) * numerator / denominator);
                    blue = (i32)((f32)((laser->colour >> 16) & 0xff) * numerator / denominator);
                    alpha = (i32)((f32)((laser->colour >> 24) & 0xff) * numerator / denominator);
                }
                vertices[6].x = (laser->start.x - laser->end.x) * numerator / denominator + laser->end.x;
                vertices[6].y = (laser->start.y - laser->end.y) * numerator / denominator + laser->end.y;
                vertices[6].z = (laser->start.z - laser->end.z) * numerator / denominator + laser->end.z;
                vertices[6].w = 1.0f;
            } else {
                green = (laser->colour >> 8) & 0xff;
                blue = (laser->colour >> 16) & 0xff;
                red = laser->colour & 0xff;
                alpha = (laser->colour >> 24) & 0xff;
                vertices[6].x = laser->start.x;
                vertices[6].y = laser->start.y;
                vertices[6].z = laser->start.z;
                vertices[6].w = 1.0f;
            }
            f32 dx = vertices[6].x - laser->end.x;
            f32 dy = vertices[6].y - laser->end.y;
            f32 dz = vertices[6].z - laser->end.z;
            f32 remaining = NuFsqrt(dx * dx + dy * dy + dz * dz);
            if (vertices[5].w < 0.5f) {
                f32 numerator = vertices[4].w - 0.5f;
                f32 denominator = vertices[4].w - vertices[5].w;
                vertices[7].x = (laser->end.x - laser->start.x) * numerator / denominator + laser->start.x;
                vertices[7].y = (laser->end.y - laser->start.y) * numerator / denominator + laser->start.y;
                vertices[7].z = (laser->end.z - laser->start.z) * numerator / denominator + laser->start.z;
                vertices[7].w = 1.0f;
            } else {
                vertices[7].x = laser->end.x;
                vertices[7].y = laser->end.y;
                vertices[7].z = laser->end.z;
                vertices[7].w = 1.0f;
            }
            NuVec4MtxTransformVU0(&vertices[4], &vertices[6], &clip_matrix);
            NuVec4ScaleXYZVU0(&vertices[4], &vertices[4], 1.0f / vertices[4].w);
            NuVec4MtxTransformVU0(&vertices[5], &vertices[7], &clip_matrix);
            NuVec4ScaleXYZVU0(&vertices[5], &vertices[5], 1.0f / vertices[5].w);
            if (vertices[4].x < -1.0f && vertices[5].x < -1.0f)
                continue;
            if (__builtin_expect(vertices[4].x > 1.0f && vertices[5].x > 1.0f, 0))
                continue;
            if (vertices[4].y < -1.0f && !(vertices[5].y >= -1.0f))
                continue;
            if (vertices[4].y > 1.0f && vertices[5].y > 1.0f)
                continue;
            NUVEC perpendicular = {(vertices[5].y - vertices[4].y) * aspect, vertices[4].x - vertices[5].x, 0.0f};
            NuVecNorm(&perpendicular, &perpendicular);
            perpendicular.x *= laser->width * aspect;
            perpendicular.y *= laser->width;
            vertices[7].x -= vertices[6].x;
            vertices[7].y -= vertices[6].y;
            vertices[7].z -= vertices[6].z;
            vertices[7].w -= vertices[6].w;
            f32 length =
                NuFsqrt(vertices[7].x * vertices[7].x + vertices[7].y * vertices[7].y + vertices[7].z * vertices[7].z);
            if (!(length > 0.0f))
                continue;
            f32 step = laser->segment_length / length;
            if (!(step <= 1.0f))
                step = 1.0f;
            // Per-laser seeds make repeated and paused rendering deterministic.
            NuLgtSeed = laser->seed;
            f32 u0 = NuLgtArcMtl[0].u0;
            f32 u1 = NuLgtArcMtl[0].u1;
            vertices[4].x = vertices[6].x;
            vertices[4].y = vertices[6].y;
            vertices[4].z = vertices[6].z;
            vertices[4].w = 1.0f;
            NuVec4MtxTransformVU0(&vertices[4], &vertices[4], &clip_matrix);
            f32 width_scale = 1.0f / vertices[4].w;
            NuVec4ScaleXYZVU0(&vertices[4], &vertices[4], width_scale);
            if (laser->length > remaining) {
                if (remaining < 0.0f)
                    width_scale *= laser->end_width_ratio;
                else
                    width_scale += (laser->end_width_ratio * width_scale - width_scale) * (laser->length - remaining) /
                                   laser->length;
            }
            vertices[1].x = vertices[4].x - perpendicular.x * width_scale;
            vertices[1].y = vertices[4].y - perpendicular.y * width_scale;
            vertices[1].z = vertices[4].z;
            vertices[3].x = vertices[4].x + perpendicular.x * width_scale;
            vertices[3].y = vertices[4].y + perpendicular.y * width_scale;
            vertices[3].z = vertices[4].z;
            remaining -= laser->segment_length;
            f32 fraction = 0.0f;
            i32 start_alpha = alpha;
            for (;;) {
                f32 next_fraction = fraction + step;
                i32 end_alpha = start_alpha;
                if (next_fraction < 1.0f) {
                    if (laser->arc)
                        end_alpha = (i32)((1.0f - fraction - step) * (f32)alpha);
                    vertices[5].x = next_fraction * vertices[7].x + vertices[6].x;
                    vertices[5].y = next_fraction * vertices[7].y + vertices[6].y;
                    vertices[5].z = next_fraction * vertices[7].z + vertices[6].z;
                } else {
                    if (__builtin_expect(!!(laser->arc), 0))
                        end_alpha = 0;
                    vertices[5].x = vertices[6].x + vertices[7].x;
                    vertices[5].y = vertices[6].y + vertices[7].y;
                    vertices[5].z = vertices[6].z + vertices[7].z;
                    u0 = NuLgtArcMtl[0].u1 + (1.0f - fraction) * (NuLgtArcMtl[0].u0 - NuLgtArcMtl[0].u1) / step;
                }
                vertices[5].w = 1.0f;
                NuVec4MtxTransformVU0(&vertices[5], &vertices[5], &clip_matrix);
                width_scale = 1.0f / vertices[5].w;
                NuVec4ScaleXYZVU0(&vertices[5], &vertices[5], width_scale);
                if (laser->length > remaining) {
                    if (remaining < 0.0f)
                        width_scale *= laser->end_width_ratio;
                    else
                        width_scale += (width_scale * laser->end_width_ratio - width_scale) *
                                       (laser->length - remaining) / laser->length;
                }
                f32 wobble_x = 0.0f, wobble_y = 0.0f;
                if (1.0f - 1.5f * step > fraction && laser->seed != 0) {
                    f32 random = (f32)((NuLgtRand() & 0xff) - 128);
                    wobble_x = perpendicular.x * random * laser->width_wobble * width_scale;
                    wobble_y = perpendicular.y * random * laser->width_wobble * width_scale;
                }
                vertices[0].x = vertices[5].x - perpendicular.x * width_scale + wobble_x;
                vertices[0].y = vertices[5].y - perpendicular.y * width_scale + wobble_y;
                vertices[0].z = vertices[5].z;
                vertices[2].x = vertices[5].x + perpendicular.x * width_scale + wobble_x;
                vertices[2].y = vertices[5].y + perpendicular.y * width_scale + wobble_y;
                vertices[2].z = vertices[5].z;
                ++NuPrimCSPos;
                NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_NORMALISED);
                NuPrim2DBegin(1, 7, NuLgtArcMtl[0].material);
                NuLgtVertex(&vertices[0],
                            ((red & 255) | ((green & 255) << 8) | ((blue & 255) << 16)) | ((u32)end_alpha << 24), u0,
                            v0);
                NuLgtVertex(&vertices[1],
                            ((red & 255) | ((green & 255) << 8) | ((blue & 255) << 16)) | ((u32)start_alpha << 24), u1,
                            v0);
                NuLgtVertex(&vertices[2],
                            ((red & 255) | ((green & 255) << 8) | ((blue & 255) << 16)) | ((u32)end_alpha << 24), u0,
                            v1);
                NuLgtVertex(&vertices[3],
                            ((red & 255) | ((green & 255) << 8) | ((blue & 255) << 16)) | ((u32)start_alpha << 24), u1,
                            v1);
                NuPrim2DEnd();
                NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[--NuPrimCSPos]);
                vertices[1].x = vertices[0].x;
                vertices[1].y = vertices[0].y;
                vertices[1].z = vertices[0].z;
                vertices[3].x = vertices[2].x;
                vertices[3].y = vertices[2].y;
                vertices[3].z = vertices[2].z;
                remaining -= laser->segment_length;
                if (!(1.0f > next_fraction))
                    break;
                fraction = next_fraction;
                start_alpha = end_alpha;
            }
        }
        NuLgtSeed = saved_seed;
        NuLgtLaserOldCnt = NuLgtLaserCnt;
        NuLgtLaserCnt = 0;
        NuScratchRelease();
    }

extern "C++" {
void NuLightMotionBlurEffect(i32, float) {
}
}

    void NuLightSpeedBlurScale(f32 x, f32 y) {
        NuLightsx = x;
        NuLightsy = y;
    }

}

void NuLgtSetArcMatEx(i32 type, numtl_s *material, f32 u0, f32 v0, f32 u1, f32 v1) {
    if (type > 3)
        return;
    NuLgtArcMtl[type].material = material;
    NuLgtArcMtl[type].u0 = u0;
    NuLgtArcMtl[type].v0 = v0;
    NuLgtArcMtl[type].u1 = u1;
    NuLgtArcMtl[type].v1 = v1;
}

extern "C" {
    void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1) {
        NuLgtSetArcMatEx(0, material, u0, v0, u1, v1);
    }

}

void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                     f32 bend_amount, i32 colour, i32 flags) {
    i32 index = NuLgtArcLaserCnt;
    if (index >= 16)
        return;
    NuLgtArcLaserData[index].type = (u8)type;
    NuLgtArcLaserData[index].start = *start;
    NuLgtArcLaserData[index].end = *end;
    NuLgtArcLaserData[index].bend = *bend;
    NuLgtArcLaserData[index].width = width;
    NuLgtArcLaserData[index].wobble = wobble;
    NuLgtArcLaserData[index].bend_amount = bend_amount;
    NuLgtArcLaserData[index].segment_length = segment_length;
    u32 green = ((u32)colour & 0xff00) * 2;
    u32 blue = ((u32)colour & 0xff0000) >> 15;
    u32 red = ((u32)colour & 0xff) * 2;
    u32 alpha = ((u32)colour >> 24) * 2;
    if (red > 255)
        red = 255;
    if (blue > 255)
        blue = 255;
    if (green > 0xff00)
        green = 0xff00;
    if (alpha > 255)
        alpha = 255;
    NuLgtArcLaserData[index].colour = (blue | (red << 16)) | (green | (alpha << 24));
    NuLgtArcLaserData[index].flags = flags;
    if ((NuLgtArcLaserFrame & 1) == 0 || NuLgtArcLaserData[index].seed == 0)
        NuLgtArcLaserData[index].seed = NuLgtRand();
    for (i32 i = 0; i < 6; ++i)
        NuLgtRand();
    ++NuLgtArcLaserCnt;
}

extern "C" {
    void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour) {
        NuLgtArcLaserEx(type, start, end, bend, width, segment_length, wobble, bend_amount, colour, 0);
    }

}

void NuDisplayListSetInstSurfGeom(void *) {
    STUBBED();
}

VARIPTR *DisplayListRenderBuffer() {
    return &rndrstream_free;
}

void NuDisplayListCreate(nudisplayscene_s *raw_scene, variptr_u *buffer, variptr_u buffer_end, i32 item_count, i32 material_count,
                         i32, i32, i32 sort_priority_count, i32, i32 allocate_materials) {
    NUDLDLISTSCENE *scene = raw_scene;
    scene->nitems = item_count;
    scene->nmtls = material_count;

    NUDISPLAYLISTITEM *items = reinterpret_cast<NUDISPLAYLISTITEM *>(ALIGN(buffer->addr, 0x10));
    scene->items = items;
    NUMTL **mtls = reinterpret_cast<NUMTL **>(items + item_count);
    scene->mtls = mtls;
    scene->dlist_mtls = reinterpret_cast<NUDISPLAYLIST **>(ALIGN(reinterpret_cast<usize>(mtls + material_count), 0x10));
    buffer->addr = reinterpret_cast<usize>(scene->dlist_mtls + material_count);
    memset(mtls, 0, material_count * sizeof(NUMTL *));
    memset(scene->dlist_mtls, 0, material_count * sizeof(NUDISPLAYLIST *));

    if (allocate_materials != 0) {
        NUMTL *materials = reinterpret_cast<NUMTL *>(ALIGN(buffer->addr, 0x10));
        buffer->addr = reinterpret_cast<usize>(materials + material_count);
        for (i32 i = 0; i < material_count; ++i) {
            scene->mtls[i] = materials++;
        }
    }

    NUDISPLAYLIST *display_lists = reinterpret_cast<NUDISPLAYLIST *>(ALIGN(buffer->addr, 0x10));
    buffer->addr = reinterpret_cast<usize>(display_lists + material_count);
    memset(display_lists, 0, material_count * sizeof(NUDISPLAYLIST));

    NUDISPLAYLISTITEM *first_items = reinterpret_cast<NUDISPLAYLISTITEM *>(ALIGN(buffer->addr, 0x10));
    buffer->addr = reinterpret_cast<usize>(first_items + material_count);
    memset(first_items, 0, material_count * sizeof(NUDISPLAYLISTITEM));

    for (i32 i = 0; i < material_count; ++i) {
        NUDISPLAYLIST *display_list = &display_lists[i];
        scene->dlist_mtls[i] = display_list;
        display_list->state = reinterpret_cast<NURNDRSTATE *>(ALIGN(buffer->addr, 4));
        buffer->addr = reinterpret_cast<usize>(display_list->state + 1);
        display_list->first = &first_items[i];

        display_list->state->mtl = NULL;
        display_list->state->tex_id = -1;
        display_list->state->global_id = -1;
        display_list->state->lights_id = -1;
        display_list->state->camera_id = -1;
        display_list->state->fog_id = -1;
        display_list->state->konst_id = -1;
        display_list->state->reflection_id = -1;
    }
    scene->local_state = NULL;

    for (u32 i = 0; i < scene->nmtls; ++i) {
        NUDISPLAYLISTITEM *first = scene->dlist_mtls[i]->first;
        first->type = 0x8d;
        first->id = 1;
        first->next = NULL;
    }

    const i32 used_bytes = (material_count + 7) / 8;
    i32 used_size = used_bytes / 16 * 16 + 0x10;
    u8 *used = reinterpret_cast<u8 *>(ALIGN(buffer->addr, 0x10));
    scene->mtl_used[0] = used;
    used += used_size;
    scene->mtl_used[1] = used;
    scene->nsort_pris = sort_priority_count;
    NUSORTPRI *sort_pris = reinterpret_cast<NUSORTPRI *>(used + used_size);
    scene->sort_pris = sort_pris;
    buffer->addr = reinterpret_cast<usize>(sort_pris + sort_priority_count);
    memset(sort_pris, 0, sort_priority_count * sizeof(NUSORTPRI));

    for (i32 i = 0; i < sort_priority_count; ++i) {
        scene->sort_pris[i].display_scene = scene;
        scene->sort_pris[i].nmtls = 0;
        scene->sort_pris[i].mtl_first = 0;
    }

    DisplayListCreatePS(scene, buffer, buffer_end);
    DisplayListBeforeFrameReset(scene);
}

// ──────────────────────────────────────────────────────────────────────────────
// Dynamic material display lists
// ──────────────────────────────────────────────────────────────────────────────

nudisplaylistitem_s *NuDisplayListCreateMtlDlist(nudisplaylistitem_s * /*item*/, NUMTL *mtl, VARIPTR *buff,
                                                 VARIPTR /*buff_end*/) {
    auto *out = reinterpret_cast<nudisplaylistitem_s *>(buff->addr);
    buff->addr += 8 * kItemSize;

    out->type = 0x85;
    out->id = kItemId_Next;
    out->next = out + 7;

    NuDisplayListAddClut(out + 1, mtl->tex_id);
    NuDisplayListAddTexture(out + 2, mtl->tex_id);
    NuDisplayListAddMaterialState(out + 3, mtl);
    NuDisplayListAddMicrocode(out + 4, mtl);
    NuDisplayListAddLightState(out + 5, mtl);

    out[6].type = 0x8b;
    out[6].id = kItemId_Next;
    out[6].next = out + 7;
    out[7].type = kItemType_Terminator;
    out[7].id = kItemId_Ret;
    out[7].next = nullptr;

    return out;
}

extern "C" {
    void NuDisplayListBeginCriticalSection(void) {
        NuThreadCriticalSectionBegin(global_dlist_manager.loading_critical_section);
    }

    void NuDisplayListEndCriticalSection(void) {
        NuThreadCriticalSectionEnd(global_dlist_manager.loading_critical_section);
    }

    // original 0x2f8bb0 — defer dynamic material display-list construction to
    // DisplayListLinkDynamicMtls at the next render-buffer swap.
    void NuDisplayListCreateMtl(NUMTL *mtl) {
        NuThreadCriticalSectionBegin(global_dlist_manager.loading_critical_section);
        if (global_dlist_manager.nnew_materials != 0x80) {
            global_dlist_manager.new_materials[global_dlist_manager.nnew_materials] = mtl;
            global_dlist_manager.nnew_materials++;
            NuThreadCriticalSectionEnd(global_dlist_manager.loading_critical_section);
        }
    }

    void NuDisplayListDestroyMtl(NUMTL *mtl) {
        NuThreadCriticalSectionBegin(global_dlist_manager.loading_critical_section);
        if (mtl->display_list != NULL && global_dlist_manager.dyn_mtl_dlist.nmtls != 0) {
            for (u32 i = 0; i < global_dlist_manager.dyn_mtl_dlist.nmtls; ++i) {
                if (global_dlist_manager.dyn_mtl_dlist.mtls[i] == mtl) {
                    if (i != static_cast<u32>(-1) && global_dlist_manager.ndel_materials != 0x80) {
                        global_dlist_manager.del_materials[global_dlist_manager.ndel_materials] = mtl;
                        ++global_dlist_manager.ndel_materials;
                    }
                    break;
                }
            }
        }
        NuThreadCriticalSectionEnd(global_dlist_manager.loading_critical_section);
    }

    // original 0x2e8cc0
    void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl) {
        if (list->state->mtl == mtl) {
            return;
        }

        VARIPTR *buf = NuDisplayListGetBuffer();
        buf->addr = ALIGN(buf->addr, 0x10);
        list->mtl_last->next = buf->void_ptr;

        nudisplaylistitem_s *item;
        if (mtl->tex_id < 1 || mtl->tex_id == list->state->tex_id) {
            item = reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr);
            buf->addr += 0x40;
        } else {
            auto *bytes = reinterpret_cast<u8 *>(buf->void_ptr);
            buf->addr += 0x60;

            auto *nop = reinterpret_cast<nudisplaylistitem_s *>(bytes);
            nop[0].type = 0x87;
            nop[0].id = 0;
            nop[0].next = nullptr;
            nop[1].type = 0x87;
            nop[1].id = 0;
            nop[1].next = nullptr;
            list->state->tex_id = mtl->tex_id;
            item = nop + 2;
        }

        item[0].type = 0x80;
        item[0].id = 3;
        item[0].next = mtl;
        item[1].type = 0x87;
        item[1].id = 0;
        item[1].next = nullptr;
        item[2].type = 0x87;
        item[2].id = 0;
        item[2].next = nullptr;
        item[3].type = 0x8d;
        item[3].id = 1;
        item[3].next = nullptr;

        list->mtl_last = item + 3;
        list->state->mtl = mtl;
    }

}

void DisplayListLinkDynamicMtls(void) {
    NUDLIST_MANAGER *mgr = &global_dlist_manager;
    if (mgr->nnew_materials == 0 && mgr->ndel_materials == 0)
        return;

    // ── Removal pass ──
    for (i32 di = 0; di < mgr->ndel_materials; ++di) {
        NUMTL *mtl = mgr->del_materials[di];
        if (!mtl->display_list)
            continue;

        // Locate slot.
        const i32 idx = DynMtlFind(mtl);
        if (idx == -1)
            continue;

        mgr->material_used[idx]--;
        if (mgr->material_used[idx] != 0)
            continue;

        // Find owning sortpri (last key match wins).
        i32 key = MtlSortKey(mtl);
        i32 sp_idx = -1;
        if (__builtin_expect(mgr->dyn_mtl_dlist.nsort_pris >= 1, 1)) {
            for (i32 j = 0; j < mgr->dyn_mtl_dlist.nsort_pris; ++j) {
                if (key == mgr->dyn_mtl_dlist.sort_pris[j].sort_pri)
                    sp_idx = j;
            }
        }
        nusortpri_s *sp = &mgr->dyn_mtl_dlist.sort_pris[sp_idx];
        nudisplaylistitem_s *mtl_item = mtl->display_list->mtl_item;

        {
            if (sp->items == mtl_item) {
                sp->items = static_cast<nudisplaylistitem_s *>(mtl_item[7].next);
                mtl_item[7].type = kItemType_Terminator;
                mtl_item[7].id = kItemId_Ret;
                mtl_item[7].next = nullptr;
            } else {
                for (i32 j = 0; j < sp->nmtls; ++j) {
                    nudisplaylistitem_s *prev =
                        mgr->dyn_mtl_dlist.mtls[sp->mtl_first + j]->display_list->mtl_item;
                    if (mtl_item == prev[7].next) {
                        prev[7].next = mtl_item[7].next;
                        mtl_item[7].type = kItemType_Terminator;
                        mtl_item[7].id = kItemId_Ret;
                        mtl_item[7].next = nullptr;
                        break;
                    }
                }
            }
        }

        // Free 0x80-byte stream buffer.
        for (i32 j = 0; j < static_cast<i32>(mgr->dyn_mtl_dlist.nmtls); ++j) {
            if (mgr->mtl_buffers_used[j] != 0 &&
                mtl_item ==
                    reinterpret_cast<nudisplaylistitem_s *>(reinterpret_cast<u8 *>(mgr->mtlbuff.addr) + j * 0x80)) {
                mgr->mtl_buffers_used[j] = 0;
                break;
            }
        }

        {
            sp->nmtls--;
            if (sp->nmtls == 0) {
                // Unlink empty sortpri.
                if (mgr->sort_list != sp) {
                    nusortpri_s *cur = mgr->sort_list;
                    while (cur->sys_next != sp)
                        cur = cur->sys_next;
                    cur->sys_next = sp->sys_next;
                } else {
                    mgr->sort_list = sp->sys_next;
                }
                --mgr->nused_sort_pris;
                for (i32 j = sp_idx; j < mgr->dyn_mtl_dlist.nsort_pris - 1; ++j) {
                    mgr->dyn_mtl_dlist.sort_pris[j] = mgr->dyn_mtl_dlist.sort_pris[j + 1];
                }
                --mgr->dyn_mtl_dlist.nsort_pris;
            }

            // Compact material slots and recycle the removed list into the free tail.
            NUDISPLAYLIST *removed_dl = mgr->dyn_mtl_dlist.dlist_mtls[idx];
            for (i32 j = idx; j < static_cast<i32>(mgr->dyn_mtl_dlist.nmtls) - 1; ++j) {
                mgr->dyn_mtl_dlist.mtls[j] = mgr->dyn_mtl_dlist.mtls[j + 1];
                mgr->dyn_mtl_dlist.dlist_mtls[j] = mgr->dyn_mtl_dlist.dlist_mtls[j + 1];
                mgr->material_used[j] = mgr->material_used[j + 1];
            }
            mgr->dyn_mtl_dlist.dlist_mtls[mgr->dyn_mtl_dlist.nmtls - 1] = removed_dl;
            mgr->material_used[mgr->dyn_mtl_dlist.nmtls - 1] = 0;

            {
                for (i32 i = 0; i < static_cast<i32>(mgr->dyn_mtl_dlist.nmtls); ++i) {
                    if (mgr->material_used[i] != 0) {
                        mgr->dyn_mtl_dlist.mtls[i]->display_list->mtl_id = i;
                    }
                }
            }
            {
                u32 total = 0;
                for (i32 i = 0; i < mgr->dyn_mtl_dlist.nsort_pris; ++i) {
                    total += mgr->dyn_mtl_dlist.sort_pris[i].nmtls;
                }
                for (i32 i = 0; i < mgr->dyn_mtl_dlist.nsort_pris; ++i) {
                    nusortpri_s *sp = &mgr->dyn_mtl_dlist.sort_pris[i];
                    if (sp->nmtls == 0)
                        continue;
                    for (u32 k = 0; k < total; ++k) {
                        if (MtlSortKey(mgr->dyn_mtl_dlist.mtls[k]) == sp->sort_pri) {
                            sp->mtl_first = static_cast<u16>(k);
                            break;
                        }
                    }
                }
            }
        }

        mtl->display_list = nullptr;
    }
    mgr->ndel_materials = 0;

    // ── Addition pass ──
    for (i32 ni = 0; ni < mgr->nnew_materials; ++ni) {
        NUMTL *mtl = mgr->new_materials[ni];

        // Already present? bump refcount.
        const i32 idx = DynMtlFind(mtl);
        if (idx != -1) {
            ++mgr->material_used[idx];
            continue;
        }

        // Find free slot.
        i32 pos = -1;
        for (i32 j = 0; j < static_cast<i32>(mgr->dyn_mtl_dlist.nmtls); ++j) {
            if (mgr->material_used[j] == 0) {
                pos = j;
                break;
            }
        }
        if (pos < 0)
            continue;

        // Find free stream buffer.
        i32 buf_off = -0x80;
        for (i32 j = 0; j < static_cast<i32>(mgr->dyn_mtl_dlist.nmtls); ++j) {
            if (mgr->mtl_buffers_used[j] == 0) {
                mgr->mtl_buffers_used[j] = 1;
                buf_off = j * 0x80;
                break;
            }
        }

        i32 key = MtlSortKey(mtl);
        i32 sp_idx = -1;
        if (mgr->dyn_mtl_dlist.nsort_pris > 0) {
            for (i32 j = 0; j < mgr->dyn_mtl_dlist.nsort_pris; ++j) {
                if (key == mgr->dyn_mtl_dlist.sort_pris[j].sort_pri)
                    sp_idx = j;
            }
        }

        // Insertion position — before first used material with key >= ours.
        i32 insert = pos;
        for (i32 j = 0; j < static_cast<i32>(mgr->dyn_mtl_dlist.nmtls); ++j) {
            if (mgr->material_used[j] != 0) {
                if (key <= MtlSortKey(mgr->dyn_mtl_dlist.mtls[j])) {
                    insert = j;
                    break;
                }
            }
        }

        // Make room: shift [insert .. nmtls-2] right, tail goes to insert.
        NUDISPLAYLIST *old_tail = mgr->dyn_mtl_dlist.dlist_mtls[mgr->dyn_mtl_dlist.nmtls - 1];
        for (i32 j = static_cast<i32>(mgr->dyn_mtl_dlist.nmtls) - 2; j >= insert; --j) {
            mgr->dyn_mtl_dlist.mtls[j + 1] = mgr->dyn_mtl_dlist.mtls[j];
            mgr->dyn_mtl_dlist.dlist_mtls[j + 1] = mgr->dyn_mtl_dlist.dlist_mtls[j];
            mgr->material_used[j + 1] = mgr->material_used[j];
        }
        mgr->dyn_mtl_dlist.dlist_mtls[insert] = old_tail;
        mgr->material_used[insert] = 0;

        {
            for (i32 i = 0; i < static_cast<i32>(mgr->dyn_mtl_dlist.nmtls); ++i) {
                if (mgr->material_used[i] != 0) {
                    mgr->dyn_mtl_dlist.mtls[i]->display_list->mtl_id = i;
                }
            }
        }
        mgr->material_used[insert] = 1;

        NUDISPLAYLIST *dl = mgr->dyn_mtl_dlist.dlist_mtls[insert];
        mtl->display_list = dl;

        VARIPTR buf_ptr{reinterpret_cast<void *>(reinterpret_cast<u8 *>(mgr->mtlbuff.addr) + buf_off)};
        nudisplaylistitem_s *items = NuDisplayListCreateMtlDlist(dl->mtl_item, mtl, &buf_ptr, mgr->mtlbuffend);

        mgr->dyn_mtl_dlist.mtls[insert] = mtl;
        dl->dyn_geom = items + 6;
        dl->dlist = &mgr->dyn_mtl_dlist;
        dl->mtl_item = items;
        dl->mtl_id = insert;

        nusortpri_s *sp;
        if (sp_idx == -1) {
            sp = &mgr->dyn_mtl_dlist.sort_pris[mgr->dyn_mtl_dlist.nsort_pris];
            sp->items = items;
            sp->field_18 = mgr->field_4a8;
            sp->sort_pri = key;
            sp->nmtls = 0;

            nusortpri_s *prev = nullptr;
            nusortpri_s *cur = mgr->sort_list;
            if (cur == nullptr) {
                mgr->sort_list = sp;
                sp->sys_next = nullptr;
            } else {
                while (cur != nullptr && key > cur->sort_pri) {
                    prev = cur;
                    cur = cur->sys_next;
                }
                if (prev == nullptr) {
                    sp->sys_next = mgr->sort_list;
                    mgr->sort_list = sp;
                } else {
                    sp->sys_next = cur;
                    prev->sys_next = sp;
                }
            }
            ++mgr->nused_sort_pris;
            ++mgr->dyn_mtl_dlist.nsort_pris;
            sp->nmtls = 1;
        } else {
            sp = &mgr->dyn_mtl_dlist.sort_pris[sp_idx];
            if (__builtin_expect(insert != 0 && MtlSortKey(mgr->dyn_mtl_dlist.mtls[insert - 1]) == key, 0)) {
                // Splice after previous material's block.
                auto *anchor = mgr->dyn_mtl_dlist.mtls[insert - 1]->display_list->mtl_item;
                auto *follow = static_cast<nudisplaylistitem_s *>(anchor[7].next);
                items[7].type = kItemType_Link;
                items[7].id = kItemId_Next;
                items[7].next = follow;
                anchor[7].next = items;
            } else {
                // Head of chain.
                items[7].type = kItemType_Link;
                items[7].id = kItemId_Next;
                items[7].next = sp->items;
                sp->items = items;
            }
            ++sp->nmtls;
        }

        {
            u32 total = 0;
            for (i32 i = 0; i < mgr->dyn_mtl_dlist.nsort_pris; ++i) {
                total += mgr->dyn_mtl_dlist.sort_pris[i].nmtls;
            }
            for (i32 i = 0; i < mgr->dyn_mtl_dlist.nsort_pris; ++i) {
                nusortpri_s *sp = &mgr->dyn_mtl_dlist.sort_pris[i];
                if (sp->nmtls == 0)
                    continue;
                for (u32 k = 0; k < total; ++k) {
                    if (MtlSortKey(mgr->dyn_mtl_dlist.mtls[k]) == sp->sort_pri) {
                        sp->mtl_first = static_cast<u16>(k);
                        break;
                    }
                }
            }
        }
    }
    mgr->nnew_materials = 0;
}

extern "C" {
    void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count) {
        global_dlist_manager.max_fx = count;
        global_dlist_manager.fx_used = buffer->u8_ptr;
        buffer->addr += count;
        buffer->addr = ALIGN(buffer->addr, 4);
        global_dlist_manager.fx_sort_pris = (NUSORTPRI *)buffer->void_ptr;
        buffer->addr += count * sizeof(NUSORTPRI);
        buffer->addr = ALIGN(buffer->addr, 16);
        global_dlist_manager.fx_items = buffer->void_ptr;
        buffer->addr += count * 0x20;
        for (i32 i = 0; i < count; ++i) {
            global_dlist_manager.fx_used[i] = 0;
            NUDISPLAYLISTITEM *terminator = (NUDISPLAYLISTITEM *)global_dlist_manager.fx_items + i * 2 + 1;
            terminator->type = 0x84;
            terminator->next = NULL;
            terminator->id = 4;
        }
    }

    i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer) {
        i32 index = -1;
        for (i32 i = 0; i < global_dlist_manager.max_fx; ++i) {
            if (!global_dlist_manager.fx_used[i]) {
                index = i;
                break;
            }
        }
        if (index == -1)
            return 0;
        NUDISPLAYLISTITEM *item = (NUDISPLAYLISTITEM *)((u8 *)global_dlist_manager.fx_items + index * 0x20);
        global_dlist_manager.fx_used[index] = 1;
        NUSORTPRI *record = &global_dlist_manager.fx_sort_pris[index];
        record->display_scene = NULL;
        record->sort_pri = priority + (layer << 17);
        record->items = item;
        record->field_18 = 0;
        record->nmtls = 0;
        NUSORTPRI *previous = NULL;
        NUSORTPRI *current = global_dlist_manager.sort_list;
        if (current) {
            while (current && record->sort_pri > current->sort_pri) {
                previous = current;
                current = current->sys_next;
            }
            if (previous) {
                record->sys_next = current;
                previous->sys_next = record;
            } else {
                record->sys_next = global_dlist_manager.sort_list;
                global_dlist_manager.sort_list = record;
            }
        } else {
            global_dlist_manager.sort_list = record;
            record->sys_next = NULL;
        }
        ++global_dlist_manager.nused_sort_pris;
        DisplayListCreateFxItemPS(item, type);
        return index + 1;
    }

    void NuDisplayListDestroyFx(i32 handle) {
        i32 index = handle - 1;
        global_dlist_manager.fx_used[index] = 0;
        NUSORTPRI *record = &global_dlist_manager.fx_sort_pris[index];
        NUSORTPRI *previous = global_dlist_manager.sort_list;
        if (record == previous) {
            global_dlist_manager.sort_list = record->sys_next;
        } else {
            while (previous->sys_next != record)
                previous = previous->sys_next;
            previous->sys_next = record->sys_next;
        }
        --global_dlist_manager.nused_sort_pris;
        DisplayListDestroyFxItemPS((u8 *)global_dlist_manager.fx_items + index * 0x20);
    }

    void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode) {
        void *item = (u8 *)global_dlist_manager.fx_items + (handle - 1) * 0x20;
        DisplayListSetFxItemParamPS(item, parameter, value, mode);
    }

extern "C" void NuDisplayListDraw2D(void) {
}

extern "C" void NuDisplayListCaptureBegin(void) {
    i32 request = capture_dlist;
    if (request) {
        nudlist_debug_level = request - 1;
        capture_dlist = 0;
        do_capture = 1;
        capture_fh = NuFileOpen("dlist.htm", NUFILE_WRITE);
        NuHtmlBegin((void *)(isize)capture_fh);
        NuHtmlWrite("<body bgcolor=#C0C0C0>");
    }
}

extern "C" void NuDisplayListCaptureEnd(void) {
    if (do_capture) {
        NuHtmlBanner();
        NuHtmlEnd();
        NuFileClose(capture_fh);
        do_capture = 0;
    }
}

// NuDisplaySceneAdd @ 0x2e9e30
extern "C" void NuDisplaySceneAdd(NUDLDLISTSCENE *scene) {
    NuThreadCriticalSectionBegin(global_dlist_manager.loading_critical_section);

    global_dlist_manager.dlists[global_dlist_manager.ndisplay_lists++] = scene;
    DisplayListBeforeFrameReset(scene);

    if (scene->nsort_pris > 0) {
        NUSORTPRI *sort_list = global_dlist_manager.sort_list;
        i32 used_count = global_dlist_manager.nused_sort_pris;
        i32 renderplane = numtl_renderplane;
        i32 i;
        for (i = 0; i < scene->nsort_pris; ++i) {
            NUSORTPRI *sort_pri = &scene->sort_pris[i];
            if (renderplane != 0) {
                i32 *key = &sort_pri->sort_pri;
                *key += renderplane << 17;
            }

            if (sort_list == nullptr) {
                sort_pri->sys_next = nullptr;
                sort_list = sort_pri;
            } else {
                const i32 key = sort_pri->sort_pri;
                NUSORTPRI *previous = nullptr;
                NUSORTPRI *current = sort_list;
                while (current != nullptr && key > current->sort_pri) {
                    previous = current;
                    current = current->sys_next;
                }
                if (previous == nullptr) {
                    sort_pri->sys_next = sort_list;
                    sort_list = sort_pri;
                } else {
                    sort_pri->sys_next = current;
                    previous->sys_next = sort_pri;
                }
            }
            ++used_count;
        }
        global_dlist_manager.sort_list = sort_list;
        global_dlist_manager.nused_sort_pris = used_count;
    }

    if (scene->material_animations != nullptr) {
        scene->material_animations->next = global_dlist_manager.mtlanim_list;
        global_dlist_manager.mtlanim_list = scene->material_animations;
    }
    scene->flags &= 0xef;
    scene->render_buffer &= 0xdf;
    scene->alpha_values = nullptr;
    NuDisplaySceneAddPS(scene);

    NuThreadCriticalSectionEnd(global_dlist_manager.loading_critical_section);
}

// NuDisplaySceneDestroy @ 0x2e9fd0
extern "C" void NuDisplaySceneDestroy(NUDLDLISTSCENE *scene) {
    if (scene == nullptr) {
        return;
    }

    NuThreadCriticalSectionBegin(global_dlist_manager.loading_critical_section);
    NuDisplaySceneDestroyPS(scene);

    const i32 ndisplay_lists = global_dlist_manager.ndisplay_lists;
    for (i32 i = 0;; ++i) {
        if (i >= ndisplay_lists) {
            // Destroying a scene that was never added hangs, as in the original.
            for (;;) {
            }
        }
        if (global_dlist_manager.dlists[i] == scene) {
            break;
        }
    }

    const i32 nsort_pris = scene->nsort_pris;
    if (nsort_pris > 0) {
        NUSORTPRI *sort_list = global_dlist_manager.sort_list;
        const i32 nused_sort_pris = global_dlist_manager.nused_sort_pris;
        for (i32 i = 0; i < nsort_pris; ++i) {
            NUSORTPRI *sort_pri = &scene->sort_pris[i];
            if (sort_list == sort_pri) {
                sort_list = sort_pri->sys_next;
                continue;
            }

            NUSORTPRI *previous = sort_list;
            while (previous->sys_next != sort_pri) {
                previous = previous->sys_next;
            }
            previous->sys_next = sort_pri->sys_next;
        }
        global_dlist_manager.sort_list = sort_list;
        global_dlist_manager.nused_sort_pris = nused_sort_pris - nsort_pris;
    }

    if (scene->material_animations != nullptr) {
        NUMTLANIMSET *node = global_dlist_manager.mtlanim_list;
        if (node == scene->material_animations) {
            global_dlist_manager.mtlanim_list = scene->material_animations->next;
        } else {
            while (node->next != scene->material_animations) {
                node = node->next;
            }
            node->next = scene->material_animations->next;
        }
    }

    i32 removed = 0;
    for (i32 i = 0; i < global_dlist_manager.ndisplay_lists; ++i) {
        global_dlist_manager.dlists[i - removed] = global_dlist_manager.dlists[i];
        if (global_dlist_manager.dlists[i] == scene) {
            ++removed;
        }
    }
    if (removed != 0) {
        --global_dlist_manager.ndisplay_lists;
    }

    NuThreadCriticalSectionEnd(global_dlist_manager.loading_critical_section);
}

    NUGSCN *NuGHGRelocate(NUGSCN *scene, VARIPTR *destination) {
        u8 *new_base = destination->u8_ptr;
        u8 *old_base = *reinterpret_cast<u8 **>(reinterpret_cast<u8 *>(scene) + 0x1d4);
        u32 size = *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(scene) + 0x1d8);
        i32 delta = new_base - old_base;
        if (delta == 0) {
            return scene;
        }
        if (old_base == NULL || size == 0) {
            return NULL;
        }

        if (scene->display_list != NULL) {
            NuDisplaySceneDestroy(reinterpret_cast<NUDLDLISTSCENE *>(scene->display_list));
        }
        NuTexAnimRemoveList(scene->texture_anims);
        NuGHGPreRelocateFixupPS(scene);

        u32 remaining = size;
        if (delta > 0) {
            u32 count = static_cast<u32>(delta) < remaining ? static_cast<u32>(delta) : remaining;
            u8 *source = old_base + remaining - count;
            u8 *target = new_base + remaining - count;
            do {
                count = static_cast<u32>(delta) < remaining ? static_cast<u32>(delta) : remaining;
                memcpy(target, source, count);
                target -= count;
                source -= count;
                remaining -= count;
            } while (remaining != 0);
        } else {
            u32 step = static_cast<u32>(-delta);
            u8 *source = old_base;
            u8 *target = new_base;
            do {
                u32 count = step < remaining ? step : remaining;
                memcpy(target, source, count);
                target += count;
                source += count;
                remaining -= count;
            } while (remaining != 0);
        }

        NUGSCN *moved_scene = reinterpret_cast<NUGSCN *>(reinterpret_cast<u8 *>(scene) + delta);
        *reinterpret_cast<u8 **>(reinterpret_cast<u8 *>(moved_scene) + 0x1d4) += delta;

        u8 *table = new_base + 0x18;
        table += *reinterpret_cast<i32 *>(table);
        i32 pointer_count = *reinterpret_cast<i32 *>(table);
        for (i32 i = 0; i < pointer_count; ++i) {
            u8 *entry = table + 4 + i * 4;
            u32 *pointer = reinterpret_cast<u32 *>(entry + *reinterpret_cast<i32 *>(entry));
            if (*pointer != 0) {
                *pointer += delta;
            }
        }

        nutexanim_s *anims = static_cast<nutexanim_s *>(moved_scene->texture_anims);
        i32 count = moved_scene->num_texture_anims;
        if (count != 0) {
            for (i32 i = 0; i < count; ++i) {
                anims[i].material = reinterpret_cast<numtl_s *>(reinterpret_cast<u8 *>(anims[i].material) + delta);
                anims[i].env = reinterpret_cast<nutexanimenv_s *>(reinterpret_cast<u8 *>(anims[i].env) + delta);
            }
            for (i32 i = 0; i < count - 1; ++i) {
                anims[i].next = reinterpret_cast<nutexanim_s *>(reinterpret_cast<u8 *>(anims[i].next) + delta);
                anims[i + 1].previous =
                    reinterpret_cast<nutexanim_s *>(reinterpret_cast<u8 *>(anims[i + 1].previous) + delta);
            }
        }

        NuTexAnimAddList(anims);
        moved_scene->additional_scenes = NULL;
        moved_scene->rendered_additional_scene_count = 0;
        if (moved_scene->display_list != NULL) {
            NuDisplaySceneAdd(reinterpret_cast<NUDLDLISTSCENE *>(moved_scene->display_list));
        }
        NuGHGPostRelocateFixupPS(moved_scene, delta);
        return moved_scene;
    }

}

void xxxNuDisplayListUpdateSpecial(nuhspecial_s *special) {
    NUDLDLISTSCENE *scene = special->scene->display_list;
    NUDISPLAYSPECIAL *display = special->display_special;
    NUMTX_ALIGNED16 matrix = *NuSpecialGetDrawMtx(special);
    NUVEC corners[8] = {
        {display->bounds_min.x, display->bounds_min.y, display->bounds_min.z},
        {display->bounds_max.x, display->bounds_max.y, display->bounds_max.z},
        {display->bounds_min.x, display->bounds_min.y, display->bounds_max.z},
        {display->bounds_max.x, display->bounds_max.y, display->bounds_min.z},
        {display->bounds_min.x, display->bounds_max.y, display->bounds_min.z},
        {display->bounds_min.x, display->bounds_max.y, display->bounds_max.z},
        {display->bounds_max.x, display->bounds_min.y, display->bounds_min.z},
        {display->bounds_max.x, display->bounds_min.y, display->bounds_max.z},
    };
    NuVecMtxTransform(&corners[0], &corners[0], &matrix);
    NuVecMtxTransform(&corners[1], &corners[1], &matrix);
    NuVecMtxTransform(&corners[2], &corners[2], &matrix);
    NuVecMtxTransform(&corners[3], &corners[3], &matrix);
    NuVecMtxTransform(&corners[4], &corners[4], &matrix);
    NuVecMtxTransform(&corners[5], &corners[5], &matrix);
    NuVecMtxTransform(&corners[6], &corners[6], &matrix);
    NuVecMtxTransform(&corners[7], &corners[7], &matrix);

    NUVEC minimum;
    NUVEC maximum;
    NuVecMin(&minimum, &corners[0], &corners[1]);
    NuVecMin(&minimum, &minimum, &corners[2]);
    NuVecMin(&minimum, &minimum, &corners[3]);
    NuVecMin(&minimum, &minimum, &corners[4]);
    NuVecMin(&minimum, &minimum, &corners[5]);
    NuVecMin(&minimum, &minimum, &corners[6]);
    NuVecMin(&minimum, &minimum, &corners[7]);
    NuVecMax(&maximum, &corners[0], &corners[1]);
    NuVecMax(&maximum, &maximum, &corners[2]);
    NuVecMax(&maximum, &maximum, &corners[3]);
    NuVecMax(&maximum, &maximum, &corners[4]);
    NuVecMax(&maximum, &maximum, &corners[5]);
    NuVecMax(&maximum, &maximum, &corners[6]);
    NuVecMax(&maximum, &maximum, &corners[7]);

    if ((scene->render_buffer & NUDL_SCENE_RENDER_FLAG_CENTER_EXTENT_BOUNDS) == 0) {
        NUCLIPBOUNDS *bounds = &scene->clip_bounds[display->instance_ix];
        bounds->center = minimum;
        bounds->extent = maximum;
        // Retail copies uninitialized local W components in min/max mode.
        // They are not clipping coordinates; retain the destination padding,
        // as the active NuDisplayListUpdateSpecial implementation does.
    } else {
        NUVEC extent;
        extent.y = (maximum.y - minimum.y) * 0.5f;
        extent.x = (maximum.x - minimum.x) * 0.5f;
        extent.z = (maximum.z - minimum.z) * 0.5f;
        const f32 radius = NuFsqrt(extent.x + extent.y + extent.z);
        NUCLIPBOUNDS *bounds = &scene->clip_bounds[display->instance_ix];
        bounds->center.x = (maximum.x + minimum.x) * 0.5f;
        bounds->center.y = (maximum.y + minimum.y) * 0.5f;
        bounds->center.z = (maximum.z + minimum.z) * 0.5f;
        bounds->center_w = radius;
        bounds->extent = extent;
    }
    DisplayListUpdateSpecialTransformPS(special, &matrix);
}

extern "C" {
extern "C" void NuDisplayListUpdateSpecial(nuhspecial_s *special) {
    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    NUDLDLISTSCENE *display_scene = reinterpret_cast<NUDLDLISTSCENE *>(special->scene->display_list);
    NUMTX draw_matrix;
    draw_matrix = *NuSpecialGetDrawMtx(special);

    NUVEC4 local_center;
    NUVEC half_extent;
    half_extent.x = (display->bounds_max.x - display->bounds_min.x) * 0.5f;
    half_extent.y = (display->bounds_max.y - display->bounds_min.y) * 0.5f;
    half_extent.z = (display->bounds_max.z - display->bounds_min.z) * 0.5f;

    local_center.x = (display->bounds_max.x + display->bounds_min.x) * 0.5f;
    local_center.y = (display->bounds_max.y + display->bounds_min.y) * 0.5f;
    local_center.z = (display->bounds_max.z + display->bounds_min.z) * 0.5f;

    NUVEC world_extent;
    world_extent.x = NuFabs(draw_matrix.m00) * half_extent.x + NuFabs(draw_matrix.m10) * half_extent.y +
                     NuFabs(draw_matrix.m20) * half_extent.z;
    world_extent.y = NuFabs(draw_matrix.m01) * half_extent.x + NuFabs(draw_matrix.m11) * half_extent.y +
                     NuFabs(draw_matrix.m21) * half_extent.z;
    world_extent.z = NuFabs(draw_matrix.m02) * half_extent.x + NuFabs(draw_matrix.m12) * half_extent.y +
                     NuFabs(draw_matrix.m22) * half_extent.z;

    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&local_center), reinterpret_cast<NUVEC *>(&local_center), &draw_matrix);
    local_center.w = NuFsqrt(world_extent.x + world_extent.y + world_extent.z);

    if ((display_scene->render_buffer & NUDL_SCENE_RENDER_FLAG_CENTER_EXTENT_BOUNDS) != 0) {
        NUVEC4 *clip_bounds = reinterpret_cast<NUVEC4 *>(display_scene->clip_bounds);
        const i32 bounds_index = display->instance_ix * 2;
        clip_bounds[bounds_index] = local_center;
        clip_bounds[bounds_index + 1].x = world_extent.x;
        clip_bounds[bounds_index + 1].y = world_extent.y;
        clip_bounds[bounds_index + 1].z = world_extent.z;
    } else {
        NUVEC4 *clip_bounds = reinterpret_cast<NUVEC4 *>(display_scene->clip_bounds);
        const i32 bounds_index = display->instance_ix * 2;
        clip_bounds[bounds_index].x = local_center.x - world_extent.x;
        clip_bounds[bounds_index].y = local_center.y - world_extent.y;
        clip_bounds[bounds_index].z = local_center.z - world_extent.z;
        clip_bounds[bounds_index + 1].x = local_center.x + world_extent.x;
        clip_bounds[bounds_index + 1].y = local_center.y + world_extent.y;
        clip_bounds[bounds_index + 1].z = local_center.z + world_extent.z;
    }

    DisplayListUpdateSpecialTransformPS(special, &draw_matrix);
}

extern "C" void NuSpecialUpdate(nuhspecial_s *special) {
    if (special == NULL) {
        return;
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    if (display == NULL || special->scene == NULL) {
        return;
    }

    NUDLDLISTSCENE *display_scene = reinterpret_cast<NUDLDLISTSCENE *>(special->scene->display_list);
    if (display_scene == NULL ||
        (display_scene->instance_visibility_enabled & NUDL_SCENE_INSTANCE_VISIBILITY_ENABLED) == 0) {
        return;
    }

    display->flags |= NUDISPLAYSPECIAL_FLAG_MATRIX_UPDATED;
    NuDisplayListUpdateSpecial(special);
}

extern "C" void NuSpecialSetDrawPos(nuhspecial_s *special, NUVEC *pos) {
    if (special == NULL || special->scene == NULL) {
        return;
    }
    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        NUMTX *matrix = static_cast<NUMTX *>(legacy->instance);
        matrix->m30 = pos->x;
        matrix->m31 = pos->y;
        matrix->m32 = pos->z;
    } else {
        NUMTX *matrix = &special->display_special->draw_mtx;
        matrix->m30 = pos->x;
        matrix->m31 = pos->y;
        matrix->m32 = pos->z;
        if ((special->scene->display_list->instance_visibility_enabled & 1) != 0) {
            NuDisplayListUpdateSpecial(special);
            special->display_special->flags |= NUDISPLAYSPECIAL_FLAG_MATRIX_UPDATED;
        }
    }
}

    void NuSpecialSetDrawMtx(void *special, NUMTX *mtx) {
        NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
        if (handle == NULL || handle->scene == NULL) {
            return;
        }
        NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
        if (legacy != NULL) {
            *reinterpret_cast<NUMTX *>(legacy->instance) = *mtx;
            return;
        }
        static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special)->draw_mtx = *mtx;
        if ((handle->scene->display_list->instance_visibility_enabled & 1) != 0) {
            NuDisplayListUpdateSpecial(static_cast<nuhspecial_s *>(special));
            static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special)->flags |= 0x400;
        }
    }

// ──────────────────────────────────────────────────────────────────────────────
// End-of-frame
// ──────────────────────────────────────────────────────────────────────────────

extern "C" void NuDisplayListSwapBuffersEndFrame(void) {
    NUDLIST_MANAGER *mgr = &global_dlist_manager;

    NuThreadCriticalSectionBegin(mgr->loading_critical_section);
    DisplayListLinkDynamicMtls();
    DisplayListSwapBuffersPS();
    DisplayListMaterialClipUpdate(&mgr->dyn_mtl_dlist);

    for (i32 i = 0; i < mgr->ndisplay_lists; ++i) {
        nudisplayscene_s *sc = mgr->dlists[i];
        u8 flags = sc->flags;

        if (__builtin_expect((flags & 4) != 0, 1)) {
            // Clip-word diff: 2 bits per object, 16 objects per u32 word.
            u8 buf = NuDisplaySceneCurrentBuffer(sc);
            const u16 *counts = sc->clip_counts;
            const u8 *cur_words = sc->clip_used[buf];
            const u32 *other_words = reinterpret_cast<const u32 *>(sc->clip_used[buf ^ 1]);
            i32 words = (sc->nclip_objects + 15) / 16;
            i32 obj = 0;

            for (i32 w = 0; w < words; ++w, cur_words += 4) {
                if (*reinterpret_cast<const u32 *>(cur_words) == other_words[w]) {
                    obj += 16;
                    continue;
                }
                for (i32 b = 0; b < 4; ++b) {
                    i32 byte_val = cur_words[b];
                    for (i32 shift = 0; shift != 8; shift += 2) {
                        i32 *indices = sc->clip_objects[obj].indices;
                        switch ((byte_val >> shift) & 3) {
                            case 0:
                                for (i32 j = 0; j < counts[obj]; ++j) {
                                    NUDISPLAYLISTITEM *item = &sc->items[indices[j]];
                                    item->id = kItemId_Cnt;
                                    item[-1].id = kItemId_Cnt;
                                }
                                break;
                            case 1:
                                for (i32 j = 0; j < counts[obj]; ++j) {
                                    NUDISPLAYLISTITEM *item = &sc->items[indices[j]];
                                    item->id = kItemId_Call;
                                    item[-1].id = kItemId_Call;
                                }
                                break;
                            default:
                                for (i32 j = 0; j < counts[obj]; ++j) {
                                    NUDISPLAYLISTITEM *item = &sc->items[indices[j]];
                                    item->id = kItemId_Call;
                                    item[-1].id = kItemId_Call;
                                }
                                break;
                        }
                        if (++obj >= sc->nclip_objects) {
                            goto clip_done;
                        }
                    }
                }
            }
        clip_done:

            // Alpha fade — patch alpha into every 0x82 geom item.
            if (__builtin_expect(sc->alpha_values && sc->nclip_objects > 0, 0)) {
                for (i32 o = 0; o < sc->nclip_objects; ++o) {
                    i32 *indices = sc->clip_objects[o].indices;
                    u16 cnt = sc->clip_counts[o];
                    for (i32 j = 0; j < cnt; ++j) {
                        auto *item = &sc->items[indices[j]];
                        if (__builtin_expect(item->id != kItemId_Cnt && item->type == 0x82, 0)) {
                            DisplayListSetAlphaPS(item - 1, item, sc->alpha_values[o]);
                            cnt = sc->clip_counts[o];
                        }
                    }
                }
            }
            flags = sc->flags;
        }
        if (__builtin_expect((flags & 6) != 0, 0)) {
            DisplayListMaterialClipUpdate(sc);
        }
    }

    // Publish front → safe render scenes.
    for (i32 s = 0; s < 24; ++s) {
        mgr->safe_render_scenes[s] = mgr->front_render_scenes[s];
        mgr->front_render_scenes[s] = nullptr;
    }

    // Per-sortpri render-state walk.
    // The display-list walk invalidates the cached IDs; other cache fields
    // are populated when their corresponding state is first emitted.
    nurndrstate_s tmp_state;
    tmp_state.mtl = nullptr;
    tmp_state.tex_id = -1;
    tmp_state.konst_id = -1;
    tmp_state.global_id = -1;
    tmp_state.lights_id = -1;
    tmp_state.camera_id = -1;
    tmp_state.fog_id = -1;
    tmp_state.reflection_id = -1;
    nuglobalrndrstate_s *last_local = nullptr;

    auto linkSceneGeometry = [](NUDISPLAYLIST *dl) {
        if (dl->scene_first[dl->scene_buffer] != dl->scene_next) {
            dl->dyn_geom->next = dl->scene_first[dl->scene_buffer];
            dl->scene_next->type = kItemType_Link;
            dl->scene_next->next = dl->dyn_geom + 1;
            dl->scene_next->id = kItemId_Next;
        } else {
            dl->dyn_geom->next = dl->dyn_geom + 1;
        }
    };

    auto copyMaterialGeometry = [&](NUDISPLAYLIST *dl, nudisplaylistitem_s *first, nudisplayscene_s *sc) {
        *dl->dyn_geom = *first;
        if (__builtin_expect((sc->flags & 4) == 0, 0)) {
            dl->mtl_last->next = dl->mtl_item->next;
        } else if (dl->scene_first[dl->scene_buffer] != dl->scene_next) {
            dl->mtl_last->next = dl->scene_first[dl->scene_buffer];
            dl->scene_next->type = kItemType_Link;
            dl->scene_next->next = dl->dyn_geom + 1;
            dl->scene_next->id = kItemId_Next;
        }
        dl->mtl_last = first;
    };

    for (nusortpri_s *sp = mgr->sort_list; sp; sp = sp->sys_next) {
        nudisplayscene_s *sc = sp->display_scene;
        if (!sc) {
            RndrStateUpdateFx(&tmp_state, sp->items);
            continue;
        }
        if ((sc->flags & 6) == 0 || sp->nmtls == 0)
            continue;

        for (i32 m = 0; m < sp->nmtls; ++m) {
            i32 mi = sp->mtl_first + m;
            NUDISPLAYLIST *dl = sc->dlist_mtls[mi];
            if (__builtin_expect(dl->mtl_item->id != kItemId_Cnt, 0))
                continue;

            NUMTL *mtl = sc->mtls[mi];
            nuglobalrndrstate_s *ls = sc->local_state;

            if (__builtin_expect(ls && (sc->flags & 4) != 0, 1)) {
                if (dl->mtl_last != dl->first) {
                    DisplayListUpdateRenderState(dl, ls);
                    last_local = sc->local_state;
                } else if (ls != last_local) {
                    DisplayListUpdateRenderState(dl, ls);
                    last_local = sc->local_state;
                }
            } else {
                last_local = nullptr;
            }
            if (__builtin_expect(dl->mtl_last == dl->first, 0)) {
                linkSceneGeometry(dl);
            } else {
                copyMaterialGeometry(dl, dl->first, sc);
            }

            RndrStateUpdate(&tmp_state, mtl, dl->mtl_item);

            // Visibility cull — hide materials that fail the scene's flag test.
            u16 scene_flags = *reinterpret_cast<const u16 *>(&sc->flags);
            if (__builtin_expect((scene_flags & 0x1fe0) != 0 &&
                ((static_cast<u8>(scene_flags >> 5) & static_cast<u8>(MTL_ATTRIB_DWORD1(mtl) >> 0xe)) != 0), 1)) {
                dl->mtl_item->id = kItemId_Next;
                i32 mid = dl->mtl_id;
                i32 word = (mid >= 0) ? mid >> 3 : (mid + 7) >> 3;
                i32 bit = mid % 8;
                u8 *mu = sc->mtl_used[NuDisplaySceneCurrentBuffer(sc)];
                mu[word] &= static_cast<u8>(~(1U << (static_cast<u32>(bit) & 31)));
            }
        }
    }

    for (i32 i = 0; i < mgr->ndisplay_lists; ++i) {
        nudisplayscene_s *sc = mgr->dlists[i];
        if (sc->local_state) {
            RndrStateResetGlobalState(sc->local_state);
        }
    }

    NuThreadCriticalSectionEnd(mgr->loading_critical_section);
}

extern "C" void NuDisplayListSwapBuffersBeginFrame(void) {
    NuThreadCriticalSectionBegin(global_dlist_manager.loading_critical_section);

    // Flip the dynamic-material scene's render buffer.
    u32 flip = (~global_dlist_manager.dyn_mtl_dlist.render_buffer) & 0x80;
    global_dlist_manager.dyn_mtl_dlist.render_buffer &= 0x7f;
    global_dlist_manager.dyn_mtl_dlist.render_buffer |= flip;
    DisplayListBeforeFrameFlip(&global_dlist_manager.dyn_mtl_dlist);

    for (i32 i = 0; i < global_dlist_manager.ndisplay_lists; ++i) {
        nudisplayscene_s *sc = global_dlist_manager.dlists[i];
        u32 nv = (~static_cast<u32>(static_cast<u8>(sc->render_buffer))) & 0xffffff80;
        sc->render_buffer &= 0x7f;
        sc->render_buffer |= nv;
        DisplayListBeforeFrameFlip(sc);
        if (sc->gscene) {
            *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(sc->gscene) + 0x44) = 0;
        }
    }

    global_dlist_manager.nrender_scenes = 0;
    for (nusortpri_s *sp = global_dlist_manager.sort_list; sp; sp = sp->sys_next) {
        sp->flags &= 0xfd;
    }

    RndrStateResetSharedGlobalState();
    NuThreadCriticalSectionEnd(global_dlist_manager.loading_critical_section);
}

}

void NuDisplayListEndScene(void) {
    NuDisplayListBeginCriticalSection();
    for (i32 i = 0; i < global_dlist_manager.ndisplay_lists; ++i) {
        NUDLDLISTSCENE *scene = global_dlist_manager.dlists[i];
        if (scene->flags & (NUDL_SCENE_FLAG_CLIP_MATERIALS | NUDL_SCENE_FLAG_CLIPPING))
            scene->flags |= NUDL_SCENE_FLAG_END_SCENE;
    }
    if (global_dlist_manager.dyn_mtl_dlist.flags & NUDL_SCENE_FLAG_CLIP_MATERIALS)
        global_dlist_manager.dyn_mtl_dlist.flags |= NUDL_SCENE_FLAG_END_SCENE;
    NuDisplayListEndCriticalSection();
}

extern "C" {
    void NuDisplayListAnimateMtls(f32 frame_time) {
        static f32 sinetime = 0.0f;
        sinetime += frame_time;

        NuDisplayListBeginCriticalSection();
        for (NUMTLANIMSET *set = global_dlist_manager.mtlanim_list; set != NULL; set = set->next) {
            NUDLDLISTSCENE *scene = set->scene;
            for (i32 index = 0; index < set->material_count; ++index) {
                NUMTL *material = scene->mtls[set->material_indices[index]];
                if (material->disable_u_animation == 1 || material->disable_v_animation == 1) {
                    continue;
                }

                for (u32 layer = 0; layer < 4; ++layer) {
                    if (material->shader_desc.tex_anim_data[layer] == -1) {
                        continue;
                    }

                    NUTEXANIMDATA &animation = material->shader_desc.tex_anim_desc[layer];
                    f32 u = 0.0f;
                    f32 v = 0.0f;

                    switch (animation.u_mode) {
                        case NUTEXANIM_MODE_LINEAR:
                            material->shader_desc.tex_anim_offsets[layer][0] += frame_time * animation.u_frequency;
                            material->shader_desc.tex_anim_offsets[layer][0] -=
                                NuFloor(material->shader_desc.tex_anim_offsets[layer][0]);
                            u = material->shader_desc.tex_anim_offsets[layer][0];
                            break;
                        case NUTEXANIM_MODE_SINE:
                            u = NU_SIN_LUT(static_cast<i32>(animation.u_frequency * sinetime * 65536.0f)) *
                                animation.u_amplitude;
                            break;
                        case NUTEXANIM_MODE_COSINE:
                            u = NU_COS_LUT(static_cast<i32>(animation.u_frequency * sinetime * 65536.0f)) *
                                animation.u_amplitude;
                            break;
                        default:
                            break;
                    }

                    switch (animation.v_mode) {
                        case NUTEXANIM_MODE_LINEAR:
                            material->shader_desc.tex_anim_offsets[layer][1] += frame_time * animation.v_frequency;
                            material->shader_desc.tex_anim_offsets[layer][1] -=
                                NuFloor(material->shader_desc.tex_anim_offsets[layer][1]);
                            v = material->shader_desc.tex_anim_offsets[layer][1];
                            break;
                        case NUTEXANIM_MODE_SINE:
                            v = NU_SIN_LUT(static_cast<i32>(animation.v_frequency * sinetime * 65536.0f)) *
                                animation.v_amplitude;
                            break;
                        case NUTEXANIM_MODE_COSINE:
                            v = NU_COS_LUT(static_cast<i32>(animation.v_frequency * sinetime * 65536.0f)) *
                                animation.v_amplitude;
                            break;
                        default:
                            break;
                    }

                    NuMtlSetUVOffsetPS(material, layer, u, v);
                }
            }
        }
        NuDisplayListEndCriticalSection();
    }

    void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last) {
        list->mtl_last->next = first;
        NUDISPLAYLISTITEM *continuation = list->dyn_geom + 1;
        last->type = 0x8d;
        last->next = continuation;
        last->id = 1;
        list->mtl_last = last;
    }

    void NuDisplayListClipSpecials(i32 enabled) {
        clip_special_objects = enabled;
    }

extern "C" void NuDisplayListBurstRndrSpecial(nuhspecial_s *handle, u32 count, NUMTX *matrices, i32 clip) {
    u16 visible[1024];
    if (handle == NULL)
        return;
    NUDISPLAYSPECIAL *special = handle->display_special;
    u32 visible_count;
    if (clip == 0) {
        visible_count = count;
    } else {
        visible_count = 0;
        for (u32 i = 0; i != count; ++i) {
            i32 result = NuCameraClipTestExtents(reinterpret_cast<NUVEC *>(&special->bounds_min),
                                                 reinterpret_cast<NUVEC *>(&special->bounds_max), &matrices[i], 0, 0);
            if (static_cast<u8>(result) != 0)
                visible[visible_count++] = static_cast<u16>(i | (result << 8));
        }
    }
    if (visible_count == 0)
        return;
    NUCLIPOBJECT *object = special->clip_objects;
    f32 *range = special->clip_range;
    while (!(*range <= 0) && object->nmaterials == 0) {
        ++range;
        ++object;
    }
    NUDLDLISTSCENE *scene = handle->scene->display_list;
    scene->flags |= 2;
    for (u32 material = 0; material < static_cast<u32>(object->nmaterials); ++material) {
        NUDISPLAYLIST *list = scene->mtls[object->material_ids[material]]->display_list;
        scene->mtl_used[NuDisplaySceneCurrentBuffer(scene)][list->mtl_id / 8] |= 1 << (list->mtl_id % 8);
        DisplayListUpdateRenderState(list, &render_state);
        VARIPTR *buffer = NuDisplayListLinkItems(list, visible_count * 2);
        NUDISPLAYLISTITEM *geometry = &scene->items[object->indices[material]];
        if (geometry->type != 0x8f) {
            void *geometry_data = geometry->next;
            if (clip == 0) {
                for (u32 i = 0; i != visible_count; ++i) {
                    NUMTX *matrix = &matrices[i];
                    void *transform = DisplayListCreateGeomTransformPS(buffer, matrix, scene->mtls[list->mtl_id],
                                                                       geometry_data, NULL);
                    list->items->id = 3;
                    list->items->next = transform;
                    list->items->type = 0x8c;
                    NUDISPLAYLISTITEM *first = list->items++;
                    list->items->id = 3;
                    list->items->next = geometry_data;
                    list->items->type = 0x82;
                    NUDISPLAYLISTITEM *last = list->items++;
                    DisplayListSetAlphaPS(first, last, 1);
                }
            } else {
                for (u32 i = 0; i != visible_count; ++i) {
                    NUMTX *matrix = &matrices[static_cast<u8>(visible[i])];
                    void *transform = DisplayListCreateGeomTransformPS(buffer, matrix, scene->mtls[list->mtl_id],
                                                                       geometry_data, NULL);
                    list->items->next = transform;
                    list->items->type = 0x8c;
                    list->items->id = 3;
                    NUDISPLAYLISTITEM *first = list->items++;
                    list->items->next = geometry_data;
                    list->items->id = 3;
                    list->items->type = 0x82;
                    NUDISPLAYLISTITEM *last = list->items++;
                    DisplayListSetAlphaPS(first, last, 1);
                }
            }
        } else {
            for (u32 i = 0; i != visible_count; ++i) {
                NUMTX *matrix = &matrices[clip != 0 ? static_cast<u8>(visible[i]) : i];
                void *transform = DisplayListCreateFaceonTransformPS(buffer, matrix, scene->mtls[list->mtl_id],
                                                                     scene->items[object->indices[material]].next);
                void *faceon =
                    NuDisplayListPrepareFaceonPS(buffer, scene->items[object->indices[material]].next, matrix);
                list->items->next = transform;
                list->items->id = 3;
                list->items->type = 0x90;
                NUDISPLAYLISTITEM *first = list->items++;
                list->items->type = 0x8f;
                list->items->next = faceon;
                list->items->id = 3;
                NUDISPLAYLISTITEM *last = list->items++;
                DisplayListSetAlphaPS(first, last, 1);
            }
        }
    }
}

    void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip) {
        NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
        if (handle == NULL || handle->scene == NULL) {
            return;
        }

        NuSpecialLegacyRuntimeLayout *runtime = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
        if (runtime != NULL && runtime->instance != NULL) {
            return;
        }
        if (handle->display_special == NULL) {
            return;
        }
        NuDisplayListBurstRndrSpecial(special, count, matrices, clip);
    }

}

void DisplayListPrintItem(nudisplaylistitem_s *item, i32 index, i32 type_count, i32 *types, i32 file_handle) {
    char text[256];
    char detail[256];
    text[0] = '\0';
    if (file_handle != 0) {
        if (item->id == 0)
            strcpy(text, "<font color = \"#00a000\">");
        else
            strcpy(text, "<font color = \"#0000a0\">");
    }
    sprintf(detail, "%d: ", index);
    NuStrCat(text, detail);
    switch (item->type) {
        case 0x80:
            strcpy(detail, "MTL              ");
            break;
        case 0x81:
            strcpy(detail, "TXTRLOAD         ");
            break;
        case 0x82:
            strcpy(detail, "GEOMCALL         ");
            break;
        case 0x83:
            strcpy(detail, "MTXLOAD          ");
            break;
        case 0x84:
            strcpy(detail, "TERMINATE        ");
            break;
        case 0x85:
            strcpy(detail, "MTL_CLIP         ");
            break;
        case 0x86:
            strcpy(detail, "CLUT             ");
            break;
        case 0x87:
            strcpy(detail, "DUMMY            ");
            break;
        case 0x88:
            strcpy(detail, "DLIST            ");
            break;
        case 0x8b:
            strcpy(detail, "DYNAMIC GEOMETRY ");
            break;
        case 0x8c:
            strcpy(detail, "TRANSFORM_REF    ");
            break;
        case 0x8d:
            strcpy(detail, "NEXT             ");
            break;
        case 0x8e:
            strcpy(detail, "END ITEM         ");
            break;
        case 0x8f:
            strcpy(detail, "FACEON           ");
            break;
        case 0x90:
            strcpy(detail, "FACEON_TRANSFORM ");
            break;
        case 0x93:
            strcpy(detail, "GEOMCALL2D       ");
            break;
        case 0x94:
            strcpy(detail, "LIGHTS           ");
            break;
        case 0x95:
            strcpy(detail, "DEBUG GEOMCALL   ");
            break;
        case 0x97:
            strcpy(detail, "LINECALL2D       ");
            break;
        case 0x9a:
            strcpy(detail, "CAMERA           ");
            break;
        case 0x9b:
            strcpy(detail, "FX SCREEN COPY   ");
            break;
        case 0x9c:
            strcpy(detail, "FX SUN FLARE     ");
            break;
        case 0x9d:
            strcpy(detail, "FX SCREEN FILTER ");
            break;
        case 0x9e:
            strcpy(detail, "FX SPEED BLUR    ");
            break;
        case 0x9f:
            strcpy(detail, "LINEBUFFER       ");
            break;
        case 0xa0:
            strcpy(detail, "GEOM CLIPPED     ");
            break;
        case 0xad:
            strcpy(detail, "INST_SURF_GEOM ITEM ");
            break;
        case 0xb0:
            strcpy(detail, "LIGHTMAP ITEM    ");
            break;
        default:
            if (DisplayListDebugPS(item, detail) != 0)
                strcpy(detail, "ERROR: Unknown item.");
            break;
    }
    NuStrCat(text, detail);
    sprintf(detail, "(%x)", static_cast<u32>(reinterpret_cast<usize>(item->next)));
    NuStrCat(text, detail);
    bool selected = type_count == 0;
    auto item_type = item->type;
    for (i32 type_index = 0; type_index < type_count; ++type_index) {
        if (types[type_index] != item_type) {
            continue;
        }
        selected = true;
        break;
    }
    if (selected && nudlist_debug_level > 0) {
        if (file_handle != 0) {
            NuHtmlWrite(text);
            NuHtmlWrite("</br>\n");
            if (item->id == 0)
                NuHtmlWrite("<font color = \"#000000\">");
        }
    }
    if (file_handle != 0)
        DisplayListPrintItemPS(item, file_handle);
}

extern "C" {
    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

    void NuDisplaySceneDebug(NUDLDLISTSCENE *scene, i32 flags, i32 depth, i32 *state) {
        if (scene == NULL) {
            return;
        }

        if ((flags & 3) == 0) {
            NUDISPLAYLISTITEM *item = scene->items;
            if (item == NULL) {
                return;
            }
            const i32 terminator = (flags & 2) ? 0x84 : 0x8e;
            i32 item_index = 0;
            if (!(flags & 1)) {
                do {
                    DisplayListPrintItem(item, item_index, depth, state, 0);
                    ++item;
                    ++item_index;
                } while (item->type != terminator);
            } else {
                do {
                    DisplayListPrintItem(item, item_index, depth, state, 0);
                    if (item->id == 1) {
                        item = static_cast<NUDISPLAYLISTITEM *>(item->next);
                    } else {
                        ++item;
                    }
                    ++item_index;
                } while (item->type != terminator);
            }
            DisplayListPrintItem(item, item_index, depth, state, 0);
        } else {
            for (i32 sort_index = 0; sort_index < scene->nsort_pris; ++sort_index) {
                NUDISPLAYLISTITEM *item = scene->sort_pris[sort_index].items;
                if (item == NULL) {
                    continue;
                }
                i32 item_index = 0;
                if (!(flags & 1)) {
                    do {
                        DisplayListPrintItem(item, item_index, depth, state, 0);
                        ++item;
                        ++item_index;
                    } while (item->type != 0x84);
                } else {
                    do {
                        DisplayListPrintItem(item, item_index, depth, state, 0);
                        if (item->id == 1) {
                            item = static_cast<NUDISPLAYLISTITEM *>(item->next);
                        } else {
                            ++item;
                        }
                        ++item_index;
                    } while (item->type != 0x84);
                }
                DisplayListPrintItem(item, item_index, depth, state, 0);
            }
        }
    }

// Debug capture
// ──────────────────────────────────────────────────────────────────────────────
// Debug capture
extern "C" void NuDisplayListDebugToFile(NUDISPLAYLISTITEM *item, i32 file) {
    NuHtmlWrite("<font face=courier new>\n");
    if (item) {
        i32 index = 0;
        do {
            DisplayListPrintItem(item, index, 0, nullptr, file);
            if (item->id == kItemId_Next)
                item = (NUDISPLAYLISTITEM *)item->next;
            else
                ++item;
            ++index;
        } while (item->type != kItemType_Terminator);
        DisplayListPrintItem(item, index, 0, nullptr, file);
    }
}

// ──────────────────────────────────────────────────────────────────────────────

extern "C" void NuDisplayListCaptureSortPriority(nusortpri_s *sort_pri) {
    if (!do_capture) {
        return;
    }

    const char *name = nullptr;
    if (sort_pri->display_scene) {
        name = sort_pri->display_scene->name;
    }
    if (name == nullptr) {
        NuHtmlHeading1("Display Scene : UNKNOWN (sort: %d)", sort_pri->sort_pri);
    } else {
        NuHtmlHeading1("Display Scene : %s (sort: %d)", name, sort_pri->sort_pri);
    }
    NuHtmlWrite("<font face=courier new>\n");

    const auto fh = capture_fh;
    nudisplaylistitem_s *item = sort_pri->items;
    if (item == nullptr) {
        return;
    }

    i32 idx = 0;
    while (true) {
        DisplayListPrintItem(item, idx, 0, nullptr, fh);
        if (item->id == kItemId_Next) {
            item = static_cast<nudisplaylistitem_s *>(item->next);
        } else {
            ++item;
        }
        ++idx;
        if (item->type == kItemType_Terminator) {
            break;
        }
    }
    DisplayListPrintItem(item, idx, 0, nullptr, fh);
}

extern "C" SAGA_HOST_WEAK void NuDisplayListDrawRenderScene(i32 render_scene_id) {
    NUDLIST_MANAGER *mgr = &global_dlist_manager;

    NuThreadCriticalSectionBegin(mgr->loading_critical_section);
    nudisplaylistrenderscene_s *rs = mgr->safe_render_scenes[render_scene_id];
    if (rs) {
        const i32 nsort_pris = rs->nsort_pris;
        for (i32 i = 0; i < nsort_pris; ++i) {
            nusortpri_s *sp = rs->sort_pris[i];
            NuDisplayListCaptureSortPriority(sp);
            NuDisplayListDrawItems(sp->items);
            // Callbacks can update the slot; the original reloads it before
            // drawing the next priority or the 2D tail.
            rs = mgr->safe_render_scenes[render_scene_id];
        }
        NuDisplayListDrawItems(&rs->render_2d_first);
        mgr->safe_render_scenes[render_scene_id] = nullptr;
    }
    NuThreadCriticalSectionEnd(mgr->loading_critical_section);
}

}

void NuDynamicLight::renderShadowMap(i32 index, nuframebuffer_s *) {
    RenderSet &set = render_sets[index];
    if (set.display_lists[0].mtl_last == set.display_lists[0].first &&
        set.display_lists[1].mtl_last == set.display_lists[1].first && set.geometry_count == 0)
        return;
    refreshShadowTransform(set);
    NuDisplayListSetItemTable(1);
    NUMTX matrix;
    NUMTX bias = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5f, 0, 0, 0, 0.5f, 1};
    NuMtxMulH(&matrix, &set.warp, &bias);
    NuRenderContextSetViewProj(&matrix, &numtx_identity);
    if (set.display_lists[0].mtl_last != set.display_lists[0].first) {
        NUDISPLAYLISTITEM *item = set.display_lists[0].mtl_last;
        item->type = 0x84;
        item->id = 4;
        item->next = NULL;
        NuRenderContextSetZFunc_inline(0);
        memcpy(g_renderContext_kTint, nuvec4_one, sizeof(g_renderContext_kTint));
        NuShaderManagerSetfv(0x44, nuvec4_one);
        NuDisplayListDrawItems(set.display_lists[0].first);
        set.display_lists[0].mtl_last = set.display_lists[0].first;
    }
    if (set.display_lists[1].mtl_last != set.display_lists[1].first) {
        NUDISPLAYLISTITEM *item = set.display_lists[1].mtl_last;
        item->type = 0x84;
        item->id = 4;
        item->next = NULL;
        NuRenderContextSetZFunc_inline(0);
        memcpy(g_renderContext_kTint, nuvec4_one, sizeof(g_renderContext_kTint));
        NuShaderManagerSetfv(0x44, nuvec4_one);
        NuDisplayListDrawItems(set.display_lists[1].first);
        set.display_lists[1].mtl_last = set.display_lists[1].first;
    }
    for (i32 i = 0; i < set.geometry_count; ++i)
        NuDisplayListDrawRenderScene(set.reserved_33c[i]);
    NuDisplayListSetItemTable(0);
}

void NuDeferredFilterGen::render() {
    nuframebuffer_s *output = static_cast<nuframebuffer_s *>(portOutFramebuffer.get());
    NuProxyBuffer *normal = static_cast<NuProxyBuffer *>(portNormalBuffer.get());
    NuProxyBuffer *color = static_cast<NuProxyBuffer *>(portColorBuffer.get());
    NuProxyBuffer *depth_rt = static_cast<NuProxyBuffer *>(portDepthRTBuffer.get());
    NuProxyBuffer *depth = static_cast<NuProxyBuffer *>(portDepthBuffer.get());
    NuPostResolve(color);
    NuPostResolve(normal);
    NuPostResolve(depth_rt);
    NuPostResolve(depth);
    nueffecttex_s *destination = NuFramebufferGetAttachedTex(output, 0, NULL, NULL);
    f32 last_sample = static_cast<f32>(sample_count) - 1.0f;
    bool first = true;
    for (i32 i = 0; i < dynamic_light_count; ++i) {
        NuDynamicLight *light = dynamic_lights[i];
        if (light->reserved_7bc == 0)
            continue;
        NUMTX view, projection;
        memcpy(&view, g_renderContext_view, sizeof(view));
        memcpy(&projection, g_renderContext_projection, sizeof(projection));
        for (i32 shadow = 0; shadow < light->active_render_set_count; ++shadow) {
            // The original selects consecutive texture members beginning at 0x30.
            nueffecttex_s *shadow_texture = textures[shadow + 2];
            NuFramebufferAttachTex2D(shadow_fbos[shadow], 4, shadow_texture, 0);
            NuFramebufferBind(shadow_fbos[shadow]);
            i32 width, height;
            NuEffectTexGetDimension(shadow_texture, 0, &width, &height);
            NuRenderContextSetViewport(0, 0, width, height);
            NuFramebufferClear(0x300, 0xffffffff);
            light->renderShadowMap(shadow, shadow_fbos[shadow]);
            NuFramebufferResolveAll(true);
        }
        NuRenderContextSetViewProj(&view, &projection);
        NuFramebufferBind(light_fbo);
        if (first)
            NuFramebufferClear(0x900, 0x00ff0000);
        i32 type = light->parameter_5;
        nushaderprogram_s *program = programs[type == 0 || type == 1 ? 0 : 1];
        if (!first) {
            g_boundShader = 0;
            glUseProgram(0);
            g_currentShaderProgram = NULL;
        }
        {
            nushaderprogram_s *bound = program;
            g_boundShader = bound != NULL ? bound->program : 0;
            glUseProgram(g_boundShader);
            g_currentShaderProgram = bound;
        }
        light->bindShaderResources(program);
        PostBlurDrawQuad();
        NuFramebufferResolve(0, false);
        first = false;
    }
    copy(textures[0], 1, textures[0], 0, copyTexProgram, NULL);
    blur7x7Loopback(textures[0], 1, textures[0], 2, 2, sample_count - 2, true, 1.0f, blur7x7Program);
    NuFramebufferBind(output);
    i32 width = NuFramebufferGetWidth(output);
    i32 height = NuFramebufferGetHeight(output);
    NuRenderContextSetViewport(0, 0, width, height);
    {
        nushaderprogram_s *bound = programs[2];
        g_boundShader = bound != NULL ? bound->program : 0;
        glUseProgram(g_boundShader);
        g_currentShaderProgram = bound;
    }
    f32 params[4] = {parameters[1], last_sample, parameters[2], parameters[3]};
    PostBlurSetVertexParam(programs[2], 0x80a0, params, 4);
    PostBlurDrawQuad();
    color->kind = 0;
    color->texture = destination;
    color->resolved = false;
    color->enabled = true;
    portColorBuffer.set(color);
}

extern "C" {
// ──────────────────────────────────────────────────────────────────────────────
// Per-material reset
// ──────────────────────────────────────────────────────────────────────────────

extern "C" void NuDisplayListReset(nudisplaylist_s *dl) {
    nurndrstate_s *st = dl->state;
    st->mtl = nullptr;
    st->tex_id = -1;
    st->global_id = -1;
    st->lights_id = -1;
    st->camera_id = -1;
    st->fog_id = -1;
    st->konst_id = -1;
    st->reflection_id = -1;
    dl->mtl_last = dl->first;
}

// ──────────────────────────────────────────────────────────────────────────────
// Render-scene queueing
// ──────────────────────────────────────────────────────────────────────────────

extern "C" i32 NuDisplayListAddRenderScene(void) {
    NUDLIST_MANAGER *mgr = &global_dlist_manager;
    VARIPTR *buf = display_list_buffer;

    nusortpri_s **pris = reinterpret_cast<nusortpri_s **>((buf->addr + 0xfU) & ~0xfU);
    buf->addr = reinterpret_cast<usize>(pris + mgr->nused_sort_pris + 1);

    i32 count = 0;
    for (nusortpri_s *sp = mgr->sort_list; sp; sp = sp->sys_next) {
        nudisplayscene_s *sc = sp->display_scene;
        if (__builtin_expect(sc == nullptr, 0)) {
            // FX sortpri — carry over unless already captured this frame.
            if ((*reinterpret_cast<u8 *>(&sp->flags) & 2) == 0) {
                pris[count++] = sp;
                *reinterpret_cast<u8 *>(&sp->flags) |= 2;
            }
        } else if (__builtin_expect((sc->flags & NUDL_SCENE_FLAG_NEEDS_BUILD) == 0, 1)) {
            if ((*reinterpret_cast<u8 *>(&sp->flags) & 2) == 0 && (sc->flags & 6) != 0) {
                pris[count++] = sp;
                *reinterpret_cast<u8 *>(&sp->flags) |= 2;
            }
        } else if (__builtin_expect((sc->flags & 2) != 0, 1)) {
            // Rebuild this sortpri's material chains into the stream buffer.
            nusortpri_s *new_sp = reinterpret_cast<nusortpri_s *>((buf->addr + 3U) & ~0x3U);
            nudisplaylistitem_s *area = reinterpret_cast<nudisplaylistitem_s *>(new_sp + 1);
            buf->addr = reinterpret_cast<usize>(new_sp + 1);

            new_sp->display_scene = sc;
            new_sp->nmtls = sp->nmtls;
            new_sp->mtl_first = sp->mtl_first;
            new_sp->sort_pri = sp->sort_pri;
            new_sp->items = area;

            i32 slot = 0;
            for (i32 m = 0; m < sp->nmtls; ++m) {
                NUDISPLAYLIST *dl = sp->display_scene->dlist_mtls[sp->mtl_first + m];
                if (dl->mtl_last == dl->first)
                    continue;

                if (dl->mtl_item[1].type == kItemType_Mtl) {
                    new_sp->items[slot] = dl->mtl_item[1];
                } else {
                    new_sp->items[slot] = dl->mtl_item[3];
                }
                new_sp->items[slot + 1] = *dl->first;
                slot += 2;
                dl->mtl_last->next = &new_sp->items[slot];
                dl->mtl_last = dl->first;
                ResetRenderStateCache(dl->state);
            }
            new_sp->items[slot].id = kItemId_Ret;
            new_sp->items[slot].next = nullptr;
            new_sp->items[slot].type = kItemType_Terminator;
            buf->addr += static_cast<usize>(slot + 1) * kItemSize;

            pris[count++] = new_sp;
            new_sp->flags |= 2;
        }
    }

    if (count == 0 && mgr->dlist_2d.mtl_last == mgr->dlist_2d.first) {
        return -1;
    }

    auto *rs = reinterpret_cast<nudisplaylistrenderscene_s *>((buf->addr + 3U) & ~0x3U);
    mgr->front_render_scenes[mgr->nrender_scenes] = rs;
    buf->addr = reinterpret_cast<usize>(rs + 1);
    rs->nsort_pris = count;
    rs->sort_pris = pris;

    if (mgr->dlist_2d.mtl_last == mgr->dlist_2d.first) {
        rs->render_2d_first.type = kItemType_Terminator;
        rs->render_2d_first.id = kItemId_Ret;
        rs->render_2d_first.next = nullptr;
    } else {
        mgr->dlist_2d.mtl_last->type = kItemType_Terminator;
        mgr->dlist_2d.mtl_last->id = kItemId_Ret;
        mgr->dlist_2d.mtl_last->next = nullptr;
        rs->render_2d_first = *mgr->dlist_2d.first;
        NuDisplayListReset(&mgr->dlist_2d);
    }

    return mgr->nrender_scenes++;
}

    void NuDisplayListDrawAll(void) {
        NuDisplayListCaptureBegin();
        if (global_dlist_manager.nrender_scenes == 0)
            NuDisplayListAddRenderScene();
        for (i32 i = 0; i < global_dlist_manager.nrender_scenes; ++i)
            NuDisplayListDrawRenderScene(i);
        NuDisplayListDraw2D();
        NuDisplayListCaptureEnd();
    }

extern "C" void NuRndrEndScene(void) {
    auto *scn = &currentScene;

    scn->render_scene_id = NuDisplayListAddRenderScene();
    i32 has_dynamic_light = scn->unknown_38;

    // If a dynamic light was attached to this scene, hand it to the light
    // manager now that the render-scene id is known.
    if (has_dynamic_light != 0 && scn->render_scene_id != -1) {
        if (NuDynamicLightIsEnabled(scn->unknown_3c)) {
            NuDynamicLightAddRenderScene(static_cast<NuDynamicLight *>(scn->unknown_3c), scn->unknown_40,
                                         scn->render_scene_id);
        } else {
            scn->unknown_38 = 0;
        }
        scn->render_scene_id = -1;
    }

    // Queue this scene for the render thread. processRenderScenes drains the
    // array and resets the count at the end of the frame.
    sceneParameters[sceneParametersCount++] = *scn;
}

extern "C" void NuRndrEndSceneEx(i32) {
    NuRndrEndScene();
}

}

// NuIOS_SetCullMode is transcribed in android/nuiosdl_gl.cpp (original 0x29c110).

static u16 NuLgtArcHalf(f32 value) {
    union {
        f32 value;
        u32 bits;
    } conversion = {value};
    i32 sign = conversion.bits >> 31;
    i32 exponent = (conversion.bits >> 23) & 0xff;
    i32 mantissa = conversion.bits & 0x7fffff;
    // Preserve the original truncation and exponent clamp, including subnormals.
    exponent -= 112;
    if (exponent < 0)
        exponent = 0;
    if (exponent > 31)
        exponent = 31;
    return (sign << 15) | ((exponent & 31) << 10) | (mantissa >> 13);
}

static inline void NuLgtArcVertex(NUVEC4 *position, i32 colour, f32 u, f32 v) {
    g_NuPrim_StreamBufferPtr->u32_ptr[3] =
        g_NuPrim_NeedsOverbrightening ? colour : ((colour >> 1) & 0x7f7f7f) | (colour & 0xff000000);
    if (g_NuPrim_NeedsHalfUVs) {
        ((u16 *)g_NuPrim_StreamBufferPtr->u8_ptr)[8] = NuLgtArcHalf(u);
        ((u16 *)g_NuPrim_StreamBufferPtr->u8_ptr)[9] = NuLgtArcHalf(v);
    } else {
        g_NuPrim_StreamBufferPtr->f32_ptr[4] = u;
        g_NuPrim_StreamBufferPtr->f32_ptr[5] = v;
    }
    NuPrim2DAddXYZ(position->x, position->y, position->z);
}

void NuLgtArcLaserDraw(i32 paused) {
    f32 v0 = NuLgtArcMtl[0].v0, v1 = NuLgtArcMtl[0].v1;
    if (paused)
        NuLgtArcLaserCnt = NuLgtArcLaserOldCnt;
    if (!NuLgtArcMtl[0].material || NuLgtArcLaserCnt == 0) {
        NuLgtArcLaserOldCnt = 0;
        return;
    }
    NUVEC4 *vertices = (NUVEC4 *)NuScratchAlloc32(0xc0);
    NUMTX *clip = NuCameraGetVPCSMtx();
    if (!NuRndrBeginSceneEx(-1, -2, 0)) {
        NuScratchRelease();
        return;
    }
    ++NuPrimCSPos;
    NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_PS2);
    NuPrim2DBegin(0, 5, NuLgtArcMtl[0].material);
    NULGTARCLASER *laser = NuLgtArcLaserData;
    for (i32 i = 0; i < NuLgtArcLaserCnt; ++i, ++laser) {
        vertices[4].x = laser->start.x;
        vertices[4].y = laser->start.y;
        vertices[4].z = laser->start.z;
        vertices[4].w = 1.0f;
        NuVec4MtxTransformVU0(&vertices[4], &vertices[4], clip);
        vertices[5].x = laser->end.x;
        vertices[5].y = laser->end.y;
        vertices[5].z = laser->end.z;
        vertices[5].w = 1.0f;
        NuVec4MtxTransformVU0(&vertices[5], &vertices[5], clip);
        if (__builtin_expect(!(vertices[4].w >= 0.5f) && !(vertices[5].w >= 0.5f), 0))
            continue;
        if (vertices[4].w < 0.5f) {
            f32 numerator = vertices[5].w - 0.5f;
            f32 denominator = vertices[5].w - vertices[4].w;
            vertices[6].x = (laser->start.x - laser->end.x) * numerator / denominator + laser->end.x;
            vertices[6].y = (laser->start.y - laser->end.y) * numerator / denominator + laser->end.y;
            vertices[6].z = (laser->start.z - laser->end.z) * numerator / denominator + laser->end.z;
        } else {
            vertices[6].x = laser->start.x;
            vertices[6].y = laser->start.y;
            vertices[6].z = laser->start.z;
        }
        vertices[6].w = 1.0f;
        f32 dx = vertices[6].x - laser->end.x;
        f32 dy = vertices[6].y - laser->end.y;
        f32 dz = vertices[6].z - laser->end.z;
        sqrt((double)(dx * dx + dy * dy + dz * dz));
        if (__builtin_expect(vertices[5].w < 0.5f, 1)) {
            f32 denominator = vertices[4].w - vertices[5].w;
            f32 numerator = vertices[4].w - 0.5f;
            vertices[7].x = (laser->end.x - laser->start.x) * numerator / denominator + laser->start.x;
            vertices[7].y = (laser->end.y - laser->start.y) * numerator / denominator + laser->start.y;
            vertices[7].z = (laser->end.z - laser->start.z) * numerator / denominator + laser->start.z;
        } else {
            vertices[7].x = laser->end.x;
            vertices[7].y = laser->end.y;
            vertices[7].z = laser->end.z;
        }
        vertices[7].w = 1.0f;
        NuVec4MtxTransformVU0(&vertices[4], &vertices[6], clip);
        NuVec4ScaleXYZVU0(&vertices[4], &vertices[4], 1.0f / vertices[4].w);
        NuVec4MtxTransformVU0(&vertices[5], &vertices[7], clip);
        NuVec4ScaleXYZVU0(&vertices[5], &vertices[5], 1.0f / vertices[5].w);
        NUVEC perpendicular = {(vertices[5].y - vertices[4].y) * ((f32)nurndr_pixel_width / 240.0f),
                               vertices[4].x - vertices[5].x, 0.0f};
        NuVecNorm(&perpendicular, &perpendicular);
        perpendicular.x *= (((f32)nurndr_pixel_width * 150.0f) / 240.0f) * laser->width;
        perpendicular.y *= laser->width * 150.0f;
        vertices[7].x -= vertices[6].x;
        vertices[7].y -= vertices[6].y;
        vertices[7].z -= vertices[6].z;
        vertices[7].w -= vertices[6].w;
        f32 length =
            NuFsqrt(vertices[7].x * vertices[7].x + vertices[7].y * vertices[7].y + vertices[7].z * vertices[7].z);
        if (__builtin_expect(!(length > 0.0f), 0))
            continue;
        vertices[10].x = laser->bend.x;
        vertices[10].y = laser->bend.y;
        vertices[10].z = laser->bend.z;
        f32 factor =
            vertices[10].x * vertices[10].x + vertices[10].y * vertices[10].y + vertices[10].z * vertices[10].z;
        if (factor > 0.0f)
            factor = laser->bend_amount / NuFsqrt(factor);
        vertices[10].x *= factor;
        vertices[10].y *= factor;
        vertices[10].z *= factor;
        NuLgtSeed = laser->seed;
        f32 step = laser->segment_length / length;
        step = 1.0f < step ? 1.0f : step;
        f32 u = NuLgtArcMtl[0].u0, u1 = NuLgtArcMtl[0].u1;
        vertices[4].w = 1.0f;
        vertices[4].x = vertices[6].x;
        vertices[4].y = vertices[6].y;
        vertices[4].z = vertices[6].z;
        NuVec4MtxTransformVU0(&vertices[4], &vertices[4], clip);
        NuVec4ScaleXYZVU0(&vertices[4], &vertices[4], 1.0f / vertices[4].w);
        vertices[1].x = vertices[3].x = vertices[4].x;
        vertices[1].y = vertices[3].y = vertices[4].y;
        vertices[1].z = vertices[3].z = vertices[4].z;
        f32 progress = 0.0f;
        auto laser_wobble = laser->wobble;
        do {
            f32 next = progress + step;
            if (next >= 1.0f) {
                vertices[5].x = vertices[6].x + vertices[7].x;
                vertices[5].y = vertices[6].y + vertices[7].y;
                vertices[5].z = vertices[6].z + vertices[7].z;
                u = NuLgtArcMtl[0].u1 + (1.0f - progress) * (NuLgtArcMtl[0].u0 - NuLgtArcMtl[0].u1) / step;
            } else {
                i32 angle = ((i32)(32768.0f * next) >> 1) & 0x7fff;
                vertices[5].x = next * vertices[7].x + vertices[6].x + vertices[10].x * NuTrigTable[angle];
                vertices[5].y = next * vertices[7].y + vertices[6].y + vertices[10].y * NuTrigTable[angle];
                vertices[5].z = next * vertices[7].z + vertices[6].z + vertices[10].z * NuTrigTable[angle];
            }
            vertices[5].w = 1.0f;
            NuVec4MtxTransformVU0(&vertices[5], &vertices[5], clip);
            f32 reciprocal = 1.0f / vertices[5].w;
            NuVec4ScaleXYZVU0(&vertices[5], &vertices[5], reciprocal);
            f32 jitter_x = 0.0f, jitter_y = 0.0f;
            if (1.0f - 1.5f * step > progress && NuLgtSeed != 0) {
                f32 random = (f32)((NuLgtRand() & 0xff) - 128);
                jitter_x = perpendicular.x * random * laser_wobble * reciprocal;
                jitter_y = perpendicular.y * random * laser_wobble * reciprocal;
            }
            vertices[0].x = vertices[5].x - perpendicular.x * reciprocal + jitter_x;
            vertices[0].y = vertices[5].y - perpendicular.y * reciprocal + jitter_y;
            vertices[0].z = vertices[5].z;
            vertices[2].x = perpendicular.x * reciprocal + vertices[5].x + jitter_x;
            vertices[2].y = perpendicular.y * reciprocal + vertices[5].y + jitter_y;
            vertices[2].z = vertices[5].z;
            if (!(vertices[0].x < -50.0f && vertices[1].x < 50.0f) &&
                !(vertices[0].x > 700.0f && vertices[1].x > 700.0f) &&
                !(vertices[0].y < -50.0f && vertices[1].y < -50.0f) &&
                !(vertices[0].y > 530.0f && vertices[1].y > 530.0f)) {
                NuLgtArcVertex(&vertices[0], laser->colour, u, v0);
                NuLgtArcVertex(&vertices[1], laser->colour, u1, v0);
                NuLgtArcVertex(&vertices[2], laser->colour, u, v1);
                NuLgtArcVertex(&vertices[1], laser->colour, u1, v0);
                NuLgtArcVertex(&vertices[3], laser->colour, u1, v1);
                NuLgtArcVertex(&vertices[2], laser->colour, u, v1);
            }
            vertices[1].x = vertices[0].x;
            vertices[1].y = vertices[0].y;
            vertices[1].z = vertices[0].z;
            vertices[3].x = vertices[2].x;
            vertices[3].y = vertices[2].y;
            vertices[3].z = vertices[2].z;
            progress = next;
        } while (1.0f > progress);
    }
    NuPrim2DEnd();
    NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[--NuPrimCSPos]);
    NuRndrEndSceneEx(0);
    NuLgtArcLaserOldCnt = NuLgtArcLaserCnt;
    NuLgtArcLaserCnt = 0;
    if (!paused)
        ++NuLgtArcLaserFrame;
    NuScratchRelease();
}

void DisplayListCreateDynMtlList(variptr_u *buffer, variptr_u buffer_end) {
    NUDLIST_MANAGER *manager = &global_dlist_manager;
    NUDLDLISTSCENE *scene = &manager->dyn_mtl_dlist;

    NuDisplayListCreate(scene, buffer, buffer_end, 0x400, 0x80, 0, 0, 0x80, 0, 0);
    scene->nsort_pris = 0;
    scene->name = const_cast<char *>("Dynamic Material Display Scene");

    NUDISPLAYLIST **display_lists = scene->dlist_mtls;
    NUDISPLAYLISTITEM *material_item = scene->items;
    for (i32 i = 0; i < 0x80; ++i) {
        display_lists[i]->mtl_item = material_item;
        display_lists[i]->dyn_geom = material_item + 6;
        display_lists[i]->dlist = scene;
        display_lists[i]->mtl_id = i;
        material_item += 8;
    }

    manager->nnew_materials = 0;
    manager->ndel_materials = 0;
    manager->new_materials = reinterpret_cast<NUMTL **>(ALIGN(buffer->addr, 0x10));
    manager->del_materials = manager->new_materials + 0x80;
    manager->material_used = reinterpret_cast<u8 *>(manager->new_materials + 0x100);
    manager->mtl_buffers_used = reinterpret_cast<u8 *>(manager->new_materials + 0x120);
    buffer->addr = reinterpret_cast<usize>(manager->new_materials + 0x140);
    memset(manager->material_used, 0, 0x80);
    memset(manager->mtl_buffers_used, 0, 0x80);

    manager->mtlbuff.addr = ALIGN(buffer->addr, 0x10);
    manager->mtlbuffend.addr = manager->mtlbuff.addr + 0x4000;
    *buffer = manager->mtlbuffend;

    NUDISPLAYLIST *list = &manager->dlist_2d;
    list->first->id = 1;
    list->first->type = 0x8d;
    list->first->next = nullptr;
    list->mtl_last = list->first;
    list->state = reinterpret_cast<NURNDRSTATE *>(ALIGN(buffer->addr, 4));
    buffer->addr = reinterpret_cast<usize>(list->state + 1);
    NuDisplayListReset(list);
    scene->flags |= NUDL_SCENE_FLAG_NEEDS_BUILD;
}

__attribute__((optimize("O2"))) NuDynamicLight *NuDynamicLight::clone(variptr_u *arena, variptr_u) {
    NuDynamicLight *copy = reinterpret_cast<NuDynamicLight *>((arena->addr + 31) & ~usize(31));
    arena->addr = reinterpret_cast<usize>(copy) + ((sizeof(NuDynamicLight) + 31) & ~usize(31));
    RenderSet *destination_ptr = copy->render_sets;
    const RenderSet *source_ptr = render_sets;
    do {
        RenderSet &destination = *destination_ptr;
        const RenderSet &source = *source_ptr;
        destination.view = source.view;
        destination.projection = source.projection;
        destination.warp = source.warp;
        destination.shadow_transform = source.shadow_transform;
        memcpy(destination.reserved_100, source.reserved_100, sizeof(source.reserved_100));
        destination.warp_factor = source.warp_factor;
        CloneLightVector(destination.corners[0], source.corners[0]);
        CloneLightVector(destination.corners[1], source.corners[1]);
        CloneLightVector(destination.corners[2], source.corners[2]);
        CloneLightVector(destination.corners[3], source.corners[3]);
        CloneLightVector(destination.corners[4], source.corners[4]);
        CloneLightVector(destination.corners[5], source.corners[5]);
        CloneLightVector(destination.corners[6], source.corners[6]);
        CloneLightVector(destination.corners[7], source.corners[7]);
        CloneLightVector(destination.shadow_planes[0], source.shadow_planes[0]);
        CloneLightVector(destination.shadow_planes[1], source.shadow_planes[1]);
        CloneLightVector(destination.shadow_planes[2], source.shadow_planes[2]);
        CloneLightVector(destination.shadow_planes[3], source.shadow_planes[3]);
        CloneLightVector(destination.shadow_planes[4], source.shadow_planes[4]);
        CloneLightVector(destination.shadow_planes[5], source.shadow_planes[5]);
        CloneLightVector(destination.shadow_planes[6], source.shadow_planes[6]);
        CloneLightVector(destination.shadow_planes[7], source.shadow_planes[7]);
        CloneLightVector(destination.shadow_planes[8], source.shadow_planes[8]);
        CloneLightVector(destination.shadow_planes[9], source.shadow_planes[9]);
        CloneLightVector(destination.shadow_planes[10], source.shadow_planes[10]);
        CloneLightVector(destination.shadow_planes[11], source.shadow_planes[11]);
        destination.shadow_plane_count = source.shadow_plane_count;
        CloneLightVector(destination.far_plane, source.far_plane);
        CloneLightVector(destination.capsule_center, source.capsule_center);
        CloneLightVector(destination.capsule_end, source.capsule_end);
        destination.capsule_radius = source.capsule_radius;
        destination.display_lists[0] = source.display_lists[0];
        destination.display_lists[1] = source.display_lists[1];
        {
            nurndrstate_s *second = source.render_states[1];
            nurndrstate_s *first = source.render_states[0];
            destination.render_states[1] = second;
            destination.render_states[0] = first;
        }
        {
            NUDISPLAYLISTITEM *second = source.list_items[1];
            NUDISPLAYLISTITEM *first = source.list_items[0];
            destination.list_items[1] = second;
            destination.list_items[0] = first;
        }
        {
            NUDISPLAYLISTITEM *second = source.scene_cursor[1];
            NUDISPLAYLISTITEM *first = source.scene_cursor[0];
            destination.scene_cursor[1] = second;
            destination.scene_cursor[0] = first;
        }
        {
            NUDISPLAYLISTITEM *second = source.scene_first[1];
            NUDISPLAYLISTITEM *first = source.scene_first[0];
            destination.scene_first[1] = second;
            destination.scene_first[0] = first;
        }
        {
            NUDISPLAYLISTITEM *first = source.scene_end[0];
            NUDISPLAYLISTITEM *second = source.scene_end[1];
            destination.scene_end[1] = second;
            destination.scene_end[0] = first;
        }
        destination.reserved_33c[0] = source.reserved_33c[0];
        destination.reserved_33c[1] = source.reserved_33c[1];
        destination.reserved_33c[2] = source.reserved_33c[2];
        destination.reserved_33c[3] = source.reserved_33c[3];
        destination.reserved_33c[4] = source.reserved_33c[4];
        destination.reserved_33c[5] = source.reserved_33c[5];
        destination.reserved_33c[6] = source.reserved_33c[6];
        destination.reserved_33c[7] = source.reserved_33c[7];
        destination.geometry_count = source.geometry_count;
        ++source_ptr;
        ++destination_ptr;
    } while (destination_ptr != copy->render_sets + 2);
    copy->reserved_6c0[0] = reserved_6c0[0];
    copy->reserved_6c0[1] = reserved_6c0[1];
    copy->reserved_6c0[2] = reserved_6c0[2];
    copy->render_set_capacity = render_set_capacity;
    copy->active_render_set_count = active_render_set_count;
    copy->reserved_6d4[0] = reserved_6d4[0];
    copy->reserved_6d4[1] = reserved_6d4[1];
    copy->reserved_6d4[2] = reserved_6d4[2];
    copy->reserved_6d4[3] = reserved_6d4[3];
    copy->reserved_6d4[4] = reserved_6d4[4];
    copy->reserved_6d4[5] = reserved_6d4[5];
    copy->reserved_6d4[6] = reserved_6d4[6];
    copy->reserved_6d4[7] = reserved_6d4[7];
    copy->reserved_6d4[8] = reserved_6d4[8];
    copy->reserved_6d4[9] = reserved_6d4[9];
    copy->reserved_6d4[10] = reserved_6d4[10];
    copy->reserved_6d4[11] = reserved_6d4[11];
    copy->reserved_6d4[12] = reserved_6d4[12];
    copy->reserved_6d4[13] = reserved_6d4[13];
    copy->reserved_6d4[14] = reserved_6d4[14];
    copy->reserved_6d4[15] = reserved_6d4[15];
    copy->reserved_6d4[16] = reserved_6d4[16];
    copy->reserved_6d4[17] = reserved_6d4[17];
    copy->reserved_6d4[18] = reserved_6d4[18];
    copy->reserved_6d4[19] = reserved_6d4[19];
    copy->direction.x = direction.x;
    copy->direction.y = direction.y;
    copy->direction.z = direction.z;
    copy->reserved_730 = reserved_730;
    copy->view = view;
    copy->projection = projection;
    copy->parameter_4 = parameter_4;
    copy->parameter_5 = parameter_5;
    copy->reserved_7bc = reserved_7bc;
    copy->reserved_7c0[0] = reserved_7c0[0];
    copy->reserved_7c0[1] = reserved_7c0[1];
    CloneLightVector(copy->reserved_7c8, reserved_7c8);
    copy->used_on_specials = used_on_specials;
    for (i32 i = 0; i < render_set_capacity; ++i) {
        RenderSet &set = render_sets[i];
        set.geometry_count = 0;
        NuDisplayListReset(&set.display_lists[0]);
        NUDISPLAYLISTITEM *item0 = set.display_lists[0].first;
        item0->type = 0x8d;
        item0->next = NULL;
        item0->id = 1;
        NuDisplayListReset(&set.display_lists[1]);
        NUDISPLAYLISTITEM *item1 = set.display_lists[1].first;
        item1->type = 0x8d;
        item1->id = 1;
        item1->next = NULL;
    }
    return copy;
}

extern "C" {
    NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end) {
        return light->clone(arena, end);
    }

}

void NuDynamicLight::resetGeometry() {
    for (i32 i = 0; i < render_set_capacity; ++i) {
        RenderSet &set = render_sets[i];
        set.geometry_count = 0;
        NuDisplayListReset(&set.display_lists[0]);
        NUDISPLAYLISTITEM *item0 = set.display_lists[0].first;
        item0->type = 0x8d;
        item0->next = NULL;
        item0->id = 1;
        NuDisplayListReset(&set.display_lists[1]);
        NUDISPLAYLISTITEM *item1 = set.display_lists[1].first;
        item1->type = 0x8d;
        item1->id = 1;
        item1->next = NULL;
    }
}

void NuDeferredFilterGen::resetAll() {
    for (i32 i = 0; i < dynamic_light_count; ++i) {
        dynamic_lights[i]->resetGeometry();
    }
    dynamic_light_count = 0;
    deferred_geometry_count = 0;
}

extern "C" {
    void NuDynamicLightResetGeometry(NuDynamicLight *light) {
        light->resetGeometry();
    }

}

NuDynamicLight::RenderSet::RenderSet() {
    parameter_100 = 0.01f;
    geometry_count = 0;
    shadow_plane_count = 0;
    parameter_104 = 0.001f;
    warp_factor = 1.0f;
    for (i32 i = 0; i < 2; ++i) {
        list_items[i] = dlistItemPool.allocate();
        render_states[i] = rndrStatePool.allocate();
        memset(&display_lists[i], 0, sizeof(display_lists[i]));
        NUDISPLAYLISTITEM *item = list_items[i];
        display_lists[i].first = item;
        item->type = 0x8d;
        item->next = NULL;
        item->id = 1;
        display_lists[i].mtl_last = display_lists[i].first;
        display_lists[i].state = render_states[i];
        NuDisplayListReset(&display_lists[i]);
    }
}

NuDynamicLight::NuDynamicLight() {
    reserved_7bc = 0;
    render_set_capacity = 2;
    active_render_set_count = 0;
    parameter_4 = 0;
    parameter_5 = 0;
    used_on_specials = 0;
    reserved_7c0[1] = 4.0f;
    reserved_7c8.x = 0.25f;
    reserved_7c0[0] = 0.8f;
    reserved_7c8.y = 0.0f;
    reserved_7c8.z = 100.0f;
    reserved_7c8.w = 110.0f;
}

NuDynamicLight *NuDynamicLight::create() {
    NuDynamicLight *light = dynamicLightPool.allocate();
    new (light) NuDynamicLight;
    return light;
}

extern "C" {
    NuDynamicLight *NuDynamicLightCreate() {
        return NuDynamicLight::create();
    }

}

// Original 0x2f1180: reset every material-list head and make all clipping
// objects visible in the current frame's two-bit visibility map.
void NuDisplaySceneUnclip(NUDLDLISTSCENE *scene) {
    for (i32 i = 0; i < static_cast<i32>(scene->nmtls); ++i) {
        scene->dlist_mtls[i]->mtl_item->id = 0;
    }
    for (i32 i = 0; i < scene->nclip_objects; ++i) {
        scene->clip_used[NuDisplaySceneCurrentBuffer(scene)][i >> 2] |= static_cast<u8>(1u << (2 * (i & 3)));
    }
}

extern "C" {
// Original 0x2f11f0: reset the scene's per-object near/far clip ranges.
extern "C" void NuInvalidateClipRanges(NUDLDLISTSCENE *scene) {
    for (i32 index = 0; index < scene->nclip_objects; ++index) {
        if (scene->lod_ranges[index] != 0.0f) {
            scene->lod_ranges[index] = FLT_MAX;
        }
        scene->far_clip_ranges[index] = FLT_MAX;
    }
}

extern "C" void NuDisplayListExecute(nudisplaylistitem_s *item, const nudl_handler_fn *item_table) {
    // `item_table` points at the entry for type 0x80.
    for (;;) {
        switch (item->id) {
            case kItemId_Next:
                item = static_cast<nudisplaylistitem_s *>(item->next);
                break;
            case kItemId_Cnt:
                ++item;
                break;
            case kItemId_Call: {
                auto handler = item_table[static_cast<u32>(item->type) - kItemType_Mtl];
                if (handler)
                    handler(item->next);
                ++item;
                break;
            }
            default:
                // RET (and any unexpected id >=2 other than CALL) terminates.
                return;
        }
    }
}

}

void DefaultMtl(NUMTL *mtl) {
    // The attribs bytes (0x40-0x44 of NUMTL; attribs lives at 0x40 as a flat
    // 8-byte bitfield word) are set with direct byte/word ops in the original,
    // and diffuse r/g/b (0x54/0x58/0x5c) + opacity (0x70) default to 1.0f.
    f32 one = 1.0f;
    u8 *b = (u8 *)&mtl->attribs;

    u8 v41 = b[1];
    v41 &= 0x0f;
    b[4] |= 0x10;
    u8 v40 = b[0];
    v40 &= 0xc0;
    v41 |= 0x20;
    v40 |= 0x10;
    b[1] = v41;
    u8 v42 = b[2];
    v42 &= 0x88;
    b[0] = v40;
    v42 |= 0x16;
    b[2] = v42;

    mtl->diffuse_color.r = one;
    mtl->diffuse_color.g = one;
    mtl->diffuse_color.b = one;
    mtl->opacity = one;

    u16 w42 = *(u16 *)&b[2];
    w42 &= 0x807f;
    w42 |= 0x3f80;
    *(u16 *)&b[2] = w42;
}

void NuMtlUpdate(NUMTL *mtl) {
    NuMtlUpdatePS(mtl);
    mtl->version++;
}

extern "C" {
extern "C" NUDLDLISTSCENE *NuDisplaySceneClone(NUDLDLISTSCENE *source, VARIPTR *buffer, VARIPTR /*buffer_end*/) {
    NuThreadCriticalSectionBegin(global_dlist_manager.loading_critical_section);
    NUDLDLISTSCENE *scene = CloneSceneAllocate<NUDLDLISTSCENE>(buffer, 1);
    *scene = *source;
    scene->items = CloneSceneAllocate<NUDISPLAYLISTITEM>(buffer, source->nitems);
    memcpy(scene->items, source->items, sizeof(NUDISPLAYLISTITEM) * source->nitems);
    const i32 clip_pairs = ((source->nclip_objects + 7) / 8) << 1;
    u32 clip_bytes = static_cast<u32>(clip_pairs / 16 * 16) + 16;
    const usize clip_base = (buffer->addr + 15) & ~static_cast<usize>(15);
    scene->clip_used[0] = reinterpret_cast<u8 *>(clip_base);
    scene->clip_used[1] = reinterpret_cast<u8 *>(clip_base + static_cast<usize>(clip_bytes));
    buffer->addr = reinterpret_cast<usize>(scene->clip_used[1]) + clip_bytes;
    memset(reinterpret_cast<void *>(clip_base), 0, clip_bytes);
    memset(scene->clip_used[1], 0, clip_bytes);
    u32 mtl_bytes = (((source->nmtls + 7) >> 7) + 1) * 16;
    const usize mtl_base = (buffer->addr + 15) & ~static_cast<usize>(15);
    scene->mtl_used[0] = reinterpret_cast<u8 *>(mtl_base);
    scene->mtl_used[1] = reinterpret_cast<u8 *>(mtl_base + mtl_bytes);
    buffer->addr = reinterpret_cast<usize>(scene->mtl_used[1]) + mtl_bytes;
    memset(reinterpret_cast<void *>(mtl_base), 0, mtl_bytes);
    memset(scene->mtl_used[1], 0, mtl_bytes);
    NUDISPLAYLIST *lists = reinterpret_cast<NUDISPLAYLIST *>((buffer->addr + 3) & ~static_cast<usize>(3));
    NUDISPLAYLIST *first_list = lists;
    scene->dlist_mtls = reinterpret_cast<NUDISPLAYLIST **>(lists + source->nmtls);
    NURNDRSTATE *states = reinterpret_cast<NURNDRSTATE *>(scene->dlist_mtls + source->nmtls);
    const usize states_size = sizeof(NURNDRSTATE) * source->nmtls;
    buffer->addr = reinterpret_cast<usize>(states) + states_size;
    memset(states, 0, states_size);
    const usize heads_size = sizeof(NUDISPLAYLISTITEM) * source->nmtls;
    NUDISPLAYLISTITEM *heads = reinterpret_cast<NUDISPLAYLISTITEM *>((buffer->addr + 3) & ~static_cast<usize>(3));
    buffer->addr = reinterpret_cast<usize>(heads) + heads_size;
    memset(heads, 0, heads_size);
    for (i32 i = 0; i < static_cast<i32>(source->nmtls); ++i, ++lists, ++states, ++heads) {
        scene->dlist_mtls[i] = lists;
        memcpy(scene->dlist_mtls[i], source->dlist_mtls[i], sizeof(NUDISPLAYLIST));
        scene->dlist_mtls[i]->dlist = scene;
        scene->dlist_mtls[i]->state = states;
        scene->dlist_mtls[i]->mtl_item = scene->items + (source->dlist_mtls[i]->mtl_item - source->items);
        scene->dlist_mtls[i]->dyn_geom = scene->items + (source->dlist_mtls[i]->dyn_geom - source->items);
        scene->dlist_mtls[i]->first = heads;
        scene->dlist_mtls[i]->mtl_last = scene->dlist_mtls[i]->first;
        const usize scene_first_base = (buffer->addr + 15) & ~static_cast<usize>(15);
        scene->dlist_mtls[i]->scene_first[0] = reinterpret_cast<NUDISPLAYLISTITEM *>(scene_first_base);
        buffer->addr = scene_first_base;
        buffer->addr += sizeof(NUDISPLAYLISTITEM) * static_cast<usize>(source->dlist_mtls[i]->nscene_items);
        scene->dlist_mtls[i]->scene_first[1] = reinterpret_cast<NUDISPLAYLISTITEM *>(buffer->addr);
        buffer->addr += sizeof(NUDISPLAYLISTITEM) * static_cast<usize>(source->dlist_mtls[i]->nscene_items);
        NUDISPLAYLISTITEM *first = scene->dlist_mtls[i]->first;
        first->type = 0x8d;
        first->next = NULL;
        first->id = 1;
    }
    const i32 nmtls = source->nmtls;
    NUMTL *materials = reinterpret_cast<NUMTL *>((buffer->addr + 3) & ~static_cast<usize>(3));
    NUMTL **mtl_ptrs = reinterpret_cast<NUMTL **>(materials + nmtls);
    scene->mtls = mtl_ptrs;
    buffer->addr = reinterpret_cast<usize>(mtl_ptrs + nmtls);
    for (i32 i = 0; i < static_cast<i32>(source->nmtls); ++i, ++materials) {
        scene->mtls[i] = materials;
        memcpy(scene->mtls[i], source->mtls[i], sizeof(NUMTL));
        scene->mtls[i]->display_list = reinterpret_cast<NUDISPLAYLIST *>(
            reinterpret_cast<u8 *>(first_list) +
            ((reinterpret_cast<usize>(source->mtls[i]->display_list) - reinterpret_cast<usize>(source->dlist_mtls[0])) &
             ~usize(3)));
        NuMtlUpdate(scene->mtls[i]);
    }
    NUSORTPRI *sort_pris = reinterpret_cast<NUSORTPRI *>((buffer->addr + 3) & ~static_cast<usize>(3));
    scene->sort_pris = sort_pris;
    buffer->addr = reinterpret_cast<usize>(sort_pris + source->nsort_pris);
    memcpy(scene->sort_pris, source->sort_pris, sizeof(NUSORTPRI) * source->nsort_pris);
    for (i32 i = 0; i < source->nsort_pris; ++i) {
        scene->sort_pris[i].display_scene = scene;
        scene->sort_pris[i].items = scene->items + (source->sort_pris[i].items - source->items);
        scene->sort_pris[i].dlist_next = &scene->sort_pris[i];
    }
    auto source_nitems = source->nitems;
    for (i32 i = 0; i < source_nitems; ++i) {
        NUDISPLAYLISTITEM *item = &scene->items[i];
        if (item->type == 0x80) {
            for (i32 j = 0; j < static_cast<i32>(source->nmtls); ++j) {
                if (source->items[i].next == source->mtls[j]) {
                    item->next = scene->mtls[j];
                    break;
                }
            }
        } else if (item->type == 0x85) {
            item->next = scene->items + (static_cast<NUDISPLAYLISTITEM *>(source->items[i].next) - source->items);
        }
    }
    if (source->material_animations != NULL) {
        scene->material_animations = CloneSceneAllocate<NUMTLANIMSET>(buffer, 1);
        *scene->material_animations = *source->material_animations;
        scene->material_animations->next = global_dlist_manager.mtlanim_list;
        scene->material_animations->scene = scene;
        global_dlist_manager.mtlanim_list = scene->material_animations;
    } else {
        scene->material_animations = NULL;
    }
    scene->local_state = CloneSceneAllocate<NUGLOBALRNDRSTATE>(buffer, 1);
    memset(scene->local_state, 0, sizeof(NUGLOBALRNDRSTATE));
    NuDisplaySceneClonePS(source, scene, buffer);
    global_dlist_manager.dlists[global_dlist_manager.ndisplay_lists++] = scene;
    DisplayListBeforeFrameReset(scene);
    if (scene->nsort_pris > 0) {
        NUSORTPRI *sort_list = global_dlist_manager.sort_list;
        NUSORTPRI *sort = scene->sort_pris;
        i32 render_plane = numtl_renderplane;
        i32 render_plane_offset = render_plane * 0x20000;
        i32 used_sort_pris = global_dlist_manager.nused_sort_pris;
        for (i32 i = 0; i < scene->nsort_pris; ++i, ++sort) {
            sort->sort_pri &= 0x1ffff;
            if (render_plane != 0)
                sort->sort_pri += render_plane_offset;
            if (sort_list == NULL) {
                sort->sys_next = NULL;
                sort_list = sort;
            } else {
                const i32 key = sort->sort_pri;
                NUSORTPRI *previous = NULL;
                NUSORTPRI *current = sort_list;
                while (current != NULL && key > current->sort_pri) {
                    previous = current;
                    current = current->sys_next;
                }
                if (previous != NULL) {
                    sort->sys_next = current;
                    previous->sys_next = sort;
                } else {
                    sort->sys_next = sort_list;
                    sort_list = sort;
                }
            }
            ++used_sort_pris;
        }
        global_dlist_manager.sort_list = sort_list;
        global_dlist_manager.nused_sort_pris = used_sort_pris;
    }
    scene->flags &= 0xef;
    scene->render_buffer |= 0x20;
    NuDisplaySceneAddPS(scene);
    NuThreadCriticalSectionEnd(global_dlist_manager.loading_critical_section);
    return scene;
}

}

void RndrMaskScreen(i32 texture, f32 clear_x, f32 clear_y, f32 clear_width, f32 clear_height, f32 mask_x, f32 mask_y,
                    f32 mask_width, f32 mask_height, i32 coordinate_mode) {
    pAlphaMask->tex_id = static_cast<i16>(texture);
    NuMtlUpdate(pAlphaMask);
    NuRndrBeginScene(-1);
    NUPRIMSCALEMODE coordinates = kPRIM_COORD_LOOKUP[coordinate_mode];
    ++NuPrimCSPos;
    NuPrimSetCoordinateSystem(coordinates);
    NuPrim2DBegin(1, 7, pZClearMaterial);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(clear_x, clear_y, 0.0f);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(clear_x + clear_width, clear_y, 0.0f);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(clear_x, clear_y + clear_height, 0.0f);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(clear_x + clear_width, clear_y + clear_height, 0.0f);
    NuPrim2DEnd();
    --NuPrimCSPos;
    NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    ++NuPrimCSPos;
    NuPrimSetCoordinateSystem(coordinates);
    NuPrim2DBegin(1, 7, pAlphaMask);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(mask_x, mask_y, 1.0f);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(1.0f, 0.0f);
    NuPrim2DAddXYZ(mask_x + mask_width, mask_y, 1.0f);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(0.0f, 1.0f);
    NuPrim2DAddXYZ(mask_x, mask_y + mask_height, 1.0f);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(1.0f, 1.0f);
    NuPrim2DAddXYZ(mask_x + mask_width, mask_y + mask_height, 1.0f);
    NuPrim2DEnd();
    --NuPrimCSPos;
    NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    NuRndrEndScene();
}

extern "C" {
extern "C" i32 NuSpecialForceToAlpha(nuhspecial_s *special) {
    if (special->scene == NULL) {
        return 0;
    }
    NUDLDLISTSCENE *scene = special->scene->display_list;
    NUCLIPOBJECT *object = special->display_special->clip_objects;
    if (object->nmaterials <= 0) {
        return 0;
    }
    NUMTL *material = scene->mtls[object->material_ids[0]];
    if (material == NULL) {
        return 0;
    }
    i32 index = 0;
    do {
        do {
            material->attribs.alpha_mode = NUMTL_ALPHA_MODE_ALPHA;
            NuMtlUpdate(material);
            material = material->next;
        } while (material != NULL);
        ++index;
        if (object->nmaterials <= index) {
            break;
        }
        material = scene->mtls[object->material_ids[index]];
    } while (material != NULL);
    return 1;
}

}

NUMTL *NuMtlCreate(i32 count) {
    const i32 render_plane = numtl_renderplane;
    NUMTL *mtl = NULL;

    for (i32 i = 0; i < count; i++) {
        NUMTL *next = mtl;
        mtl = NULL;

        for (i32 j = 0; j < max_materials; j++) {
            if (!material_list[j].is_used && material_list[j].display_list == NULL) {
                mtl = &material_list[j];
                break;
            }
        }

        memset(mtl, 0, sizeof(NUMTL));

        DefaultMtl(mtl);

        mtl->is_used = true;
        mtl->unknown_0_4 = true;
        mtl->renderplane = render_plane;
        mtl->attribs.unknown_6_128 = true;

        mtl->next = next;
    }

    NuMtlCreatePS(mtl, 0);

    return mtl;
}

extern "C" {
extern "C" void NuTimeBarInitEx(VARIPTR *buffer, VARIPTR unused_buffer) {
    std::memset(NuTimeBar_SetList, 0, sizeof(NuTimeBar_SetList));
    CreateTimeBar(buffer, NULL, 7);
    NuTimeBarCreateSetEx(buffer, unused_buffer, NULL);

    NuTimeBar_FrameOutMtl = NuMtlCreate(1);
    u8 *attributes = reinterpret_cast<u8 *>(&NuTimeBar_FrameOutMtl->attribs);
    attributes[0] = static_cast<u8>((attributes[0] & 0xf0) | 2);
    NuMtlUpdate(NuTimeBar_FrameOutMtl);
    NuTimeBar_Initialised = 1;
}

extern "C" void NuTimeBarInit(void) {
    VARIPTR unused = {};
    NuTimeBarInitEx(NULL, unused);
}

}

i32 NuQFntReadPS(VUFNT *font, i32 tex_id, i32 flags, i32 render_plane, VARIPTR *buf, VARIPTR buf_end) {
    NUMTL *mtl;
    NUSHADERMTLDESC shader_desc;
    VUFNT_ANDROID *platform_font;

    mtl = NuMtlCreate(1);
    font->mtl = mtl;

    mtl->diffuse_color.r = 1.0f;
    mtl->diffuse_color.g = 1.0f;
    mtl->diffuse_color.b = 1.0f;

    mtl->opacity = 1.0f;

    mtl->attribs.alpha_mode = 1;
    mtl->attribs.unknown_2_4 = 1;
    mtl->tex_id = tex_id;
    mtl->attribs.unknown_1_1_2 = 1;
    mtl->attribs.unknown_1_4_8 = 1;
    mtl->attribs.cull_mode = 2;
    mtl->attribs.z_mode = 3;

    memset(&shader_desc, 0, sizeof(NUSHADERMTLDESC));

    shader_desc.diffuse_map_tex_id[0] = tex_id;
    shader_desc.byte4 = 0x10;
    shader_desc.flags = 0x1000;
    shader_desc.diffuse_color[0] = RGBA_TO_NUCOLOUR32(0xff, 0xff, 0xff, 0xff);

    shader_desc.vtx_desc.has_position = 1;
    shader_desc.vtx_desc.has_diffuse = 1;
    shader_desc.vtx_desc.tex_coord_mode = 1;
    shader_desc.vtx_desc.has_no_transform = 1;

    shader_desc.unknown_24 = 1.0f;

    NuMtlSetShaderDescPS(mtl, &shader_desc);
    NuMtlUpdate(font->mtl);

    font->platform_data = (VUFNT_ANDROID *)ALIGN(buf->addr, 0x10);
    buf->addr = ALIGN(buf->addr, 0x10) + sizeof(VUFNT_ANDROID);

    platform_font = (VUFNT_ANDROID *)font->platform_data;
    font->x_scale = &platform_font->x_scale;
    font->y_scale = &platform_font->y_scale;
    font->color_abgr = &platform_font->colour;

    font->hdr = NULL;

    return 0;
}

extern "C" {
extern "C" NUMTL *NuMtlCreateEx(i32 count, i32 render_plane) {
    NUMTL *mtl = NULL;
    for (i32 i = 0; i < count; ++i) {
        NUMTL *next = mtl;
        mtl = NULL;
        for (i32 j = 0; j < max_materials; ++j) {
            if (!material_list[j].is_used && material_list[j].display_list == NULL) {
                mtl = &material_list[j];
                break;
            }
        }
        memset(mtl, 0, sizeof(NUMTL));
        DefaultMtl(mtl);
        mtl->is_used = true;
        mtl->unknown_0_4 = true;
        mtl->renderplane = render_plane;
        mtl->attribs.unknown_6_128 = true;
        mtl->next = next;
    }
    NuMtlCreatePS(mtl, 0);
    return mtl;
}

// original 0x2f24a0 — same allocation sweep as NuMtlCreate, but WITHOUT
// the attribs byte6 bit7 set (asm has no `orb $0x80,0x46` here). Each material
// is queued for dynamic display-list creation and the platform pass runs with
// is_3d=1 (no has_no_transform bit). Byte6 clear keeps NuMtlUpdatePS on the
// RetrieveShaderVariant path instead of the vtx_desc bit2 poke path.
extern "C" NUMTL *NuMtlCreate3D(i32 count) {
    i32 i;
    i32 j;
    const i32 render_plane = numtl_renderplane;
    NUMTL *mtl = NULL;

    for (i = 0; i < count; i++) {
        NUMTL *next = mtl;
        mtl = NULL;

        for (j = 0; j < max_materials; j++) {
            if (!material_list[j].is_used && material_list[j].display_list == NULL) {
                mtl = &material_list[j];
                break;
            }
        }

        memset(mtl, 0, sizeof(NUMTL));

        DefaultMtl(mtl);

        mtl->is_used = true;
        mtl->unknown_0_4 = true;
        mtl->renderplane = render_plane;
        // NB: no unknown_6_128 here — original Create3D leaves byte6 clear.
        NuDisplayListCreateMtl(mtl);

        mtl->next = next;
    }

    NuMtlCreatePS(mtl, 1);

    return mtl;
}

}

void NuMtlInitEx(VARIPTR *buf, i32 mtl_count) {
    NuMtlInitExPS(buf);
    max_materials = mtl_count;
    material_list = (NUMTL *)ALIGN(buf->addr, 0x10);
    buf->addr = (usize)material_list + mtl_count * sizeof(NUMTL);
    memset(material_list, 0, mtl_count * sizeof(NUMTL));
    numtl_defaultmtl3d = NuMtlCreate3D(1);
    numtl_defaultmtl2d = NuMtlCreate(1);

    NUSHADERMTLDESC desc2d;
    memset(&desc2d, 0, sizeof(desc2d));
    desc2d.byte4 |= 0x10;
    desc2d.flags = 0x1000;
    desc2d.diffuse_color[0] = 0xffffffff;
    desc2d.unknown_24 = 1.0f;
    desc2d.vtx_desc.has_diffuse = 1;
    desc2d.vtx_desc.has_position = 1;
    desc2d.vtx_desc.has_no_transform = 1;
    NUSHADERMTLDESC desc3d;
    memset(&desc3d, 0, sizeof(desc3d));
    desc3d.byte4 |= 0x10;
    desc3d.flags = 0x1000;
    desc3d.diffuse_color[0] = 0xffffffff;
    desc3d.unknown_24 = 1.0f;
    desc3d.vtx_desc.has_position = 1;
    desc3d.vtx_desc.has_diffuse = 1;
    NuMtlSetShaderDescPS(numtl_defaultmtl2d, &desc2d);
    NuMtlSetShaderDescPS(numtl_defaultmtl3d, &desc3d);

    char package_path[512];
    i32 package = AndroidOBBUtils::LookupPackagePath(package_path, NuFileDeviceAndroidOBBType::MAIN);
    if (package != 1) {
        if (package == 2) {
            if (g_apkFileDevice == NULL) {
                NuFile::InitData init = {};
                g_apkFileDevice = new NuFileDeviceAndroidAPK("apk:", init);
                NuDatSet(NuDatOpen(package_path, buf, NULL));
            }
        }
    } else {
        NuDatSet(NuDatOpen(package_path, buf, NULL));
    }
    package = AndroidOBBUtils::LookupPackagePath(package_path, NuFileDeviceAndroidOBBType::PATCH);
    if (package != 1) {
        if (package == 2) {
            if (g_apkFileDevice == NULL) {
                NuFile::InitData init = {};
                g_apkFileDevice = new NuFileDeviceAndroidAPK("apk:", init);
                NuDatSet(NuDatOpen(package_path, buf, NULL));
            }
        }
    } else {
        NuDatSet(NuDatOpen(package_path, buf, NULL));
    }
    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/numtl_gen.c", 0xd5);
    NuTexInitExPS(buf);
    numtl_defaultmtl2d->attribs.alpha_mode = 1;
    NuMtlUpdate(numtl_defaultmtl2d);
    NuMtlUpdate(numtl_defaultmtl3d);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/numtl_gen.c", 0xee);
}

static void OccluderSetInitMaterials() {
    OccluderSet::ms_pZOnlyMtl3D = NuMtlCreate3D(1);
    OccluderSet::ms_pZOnlyMtl2D = NuMtlCreate(1);
    OccluderSet::ms_pAlphaMtl2D = NuMtlCreate(1);
    NUSHADERMTLDESC desc2d;
    memset(&desc2d, 0, sizeof(desc2d));
    desc2d.flags = 0x1000;
    desc2d.vtx_desc.has_position = 1;
    desc2d.vtx_desc.has_no_transform = 1;
    NuMtlSetShaderDescPS(OccluderSet::ms_pZOnlyMtl2D, &desc2d);
    OccluderSet::ms_pZOnlyMtl2D->attribs.cull_mode = 2;
    OccluderSet::ms_pZOnlyMtl2D->attribs.alpha_mode = 13;
    OccluderSet::ms_pZOnlyMtl2D->attribs.z_mode = 0;
    OccluderSet::ms_pZOnlyMtl2D->attribs.unknown_2_8 = 0;
    NuMtlUpdate(OccluderSet::ms_pZOnlyMtl2D);
    NUSHADERMTLDESC desc3d;
    memset(&desc3d, 0, sizeof(desc3d));
    desc3d.flags = 0x1000;
    desc3d.vtx_desc.has_position = 1;
    NuMtlSetShaderDescPS(OccluderSet::ms_pZOnlyMtl3D, &desc3d);
    OccluderSet::ms_pZOnlyMtl3D->attribs.cull_mode = 2;
    OccluderSet::ms_pZOnlyMtl3D->attribs.alpha_mode = 13;
    OccluderSet::ms_pZOnlyMtl3D->attribs.z_mode = 0;
    OccluderSet::ms_pZOnlyMtl3D->attribs.unknown_2_8 = 0;
    NuMtlUpdate(OccluderSet::ms_pZOnlyMtl3D);
    OccluderSet::ms_pAlphaMtl2D->attribs.cull_mode = 2;
    OccluderSet::ms_pAlphaMtl2D->attribs.alpha_mode = 1;
    OccluderSet::ms_pAlphaMtl2D->attribs.z_mode = 1;
    NuMtlUpdate(OccluderSet::ms_pAlphaMtl2D);
}

void OccluderSet::Init(u32 max_occluders, VARIPTR *buffer, VARIPTR buffer_end) {
    capacity = max_occluders;
    count = 0;
    occluders = reinterpret_cast<OccluderRecord *>((buffer->addr + 15) & ~(usize)15);
    indices = reinterpret_cast<u32 *>(occluders + max_occluders);
    buffer->addr = reinterpret_cast<usize>(indices + max_occluders);
    if (ms_pZOnlyMtl3D == NULL)
        OccluderSetInitMaterials();
}

void OcclusionManager::Init(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end) {
    if (capacity != 0) {
        sets[0].Init(capacity, buffer, buffer_end);
        sets[1].Init(capacity, buffer, buffer_end);
        building_set = &sets[0];
        current_set = &sets[1];
        unknown_158 = 0.3f;
        unknown_15c = -1.0f;
        initialized = true;
        enabled = true;
    }
}

extern "C" {
    void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end) {
        g_OcclusionManager.Init(capacity, buffer, buffer_end);
    }

extern "C" NUMTL *NuMtlCreateEx3D(i32 count, i32 render_plane) {
    NUMTL *mtl = NULL;
    for (i32 i = 0; i < count; ++i) {
        NUMTL *next = mtl;
        mtl = NULL;
        for (i32 j = 0; j < max_materials; ++j) {
            if (!material_list[j].is_used && material_list[j].display_list == NULL) {
                mtl = &material_list[j];
                break;
            }
        }
        memset(mtl, 0, sizeof(NUMTL));
        DefaultMtl(mtl);
        mtl->is_used = true;
        mtl->unknown_0_4 = true;
        mtl->renderplane = render_plane;
        NuDisplayListCreateMtl(mtl);
        mtl->next = next;
    }
    NuMtlCreatePS(mtl, 1);
    return mtl;
}

extern "C" NUMTL *NuMtlCreateBuff(i32, VARIPTR *buffer) {
    NUMTL *material = NuMtlAllocateBuff(buffer);
    material->attribs.unknown_6_128 = true;
    NuMtlCreatePS(material, 0);
    return material;
}

extern "C" NUMTL *NuMtlCreateBuff3D(i32, VARIPTR *buffer) {
    NUMTL *material = NuMtlAllocateBuff(buffer);
    NuDisplayListCreateMtl(material);
    NuMtlCreatePS(material, 1);
    return material;
}

extern "C" void NuMtlAnimateSetSpeedScale(f32 speed_scale) {
    mtl_animation_speed_scale = speed_scale;
}

extern "C" i32 NuMtlSpecialSetUV(nuhspecial_s *special, f32 u, f32 v) {
    if (special->scene == NULL) {
        return 0;
    }
    NUDLDLISTSCENE *scene = special->scene->display_list;
    NUCLIPOBJECT *object = special->display_special->clip_objects;
    for (i32 i = 0; i < object->nmaterials; ++i) {
        NUMTL *material = scene->mtls[object->material_ids[i]];
        if (material == NULL) {
            break;
        }
        do {
            NuMtlSetUVOffsetPS(material, 0, u, v);
            material = material->next;
        } while (material != NULL);
    }
    return 0;
}

extern "C" i32 NuMtlSetCurrentRenderPlane(i32 render_plane) {
    i32 previous_render_plane = numtl_renderplane;
    numtl_renderplane = render_plane;
    return previous_render_plane;
}

}

void NuRndrInitGeneric() {
    const i32 render_plane = NuMtlSetCurrentRenderPlane(22);
    if (!pZClearMaterial) {
        pZClearMaterial = NuMtlCreate(1);
        pZClearMaterial->attribs.z_mode = 2;
        pZClearMaterial->attribs.alpha_mode = 2;
        pZClearMaterial->attribs.alpha_test = 1;
        NuMtlUpdate(pZClearMaterial);
    }
    if (!pAlphaMask) {
        pAlphaMask = NuMtlCreate(1);
        pAlphaMask->attribs.alpha_fail = 0;
        pAlphaMask->attribs.z_mode = 2;
        pAlphaMask->attribs.alpha_test = 6;
        pAlphaMask->attribs.alpha_ref = 0;
        pAlphaMask->attribs.alpha_mode = 2;
    }
    NuMtlSetCurrentRenderPlane(render_plane);
    NuVpResetRegions();
}

VUFNT *NuQFntDuplicate(VUFNT *font, i32 flags, i32 render_plane, VARIPTR *buf, VARIPTR *buf_end) {
    VUFNT *duplicate = reinterpret_cast<VUFNT *>(ALIGN(buf->addr, 0x10));
    buf->void_ptr = duplicate;
    buf->addr += font->size;

    if (buf->addr < buf_end->addr) {
        memmove(duplicate, font, font->size);
        duplicate->flags |= 1;
        RemapAddr(duplicate, font, reinterpret_cast<void **>(&duplicate->glyphs));
        RemapAddr(duplicate, font, reinterpret_cast<void **>(&duplicate->unicode_map));
    }

    i32 previous_render_plane = NuMtlSetCurrentRenderPlane(render_plane);
    NUMTL *material = (flags & 8) != 0 ? NuMtlCreate3D(1) : NuMtlCreate(1);
    duplicate->mtl = material;
    material->attribs = font->mtl->attribs;
    material->tex_id = font->mtl->tex_id;
    material->sort_pri = font->mtl->sort_pri;
    material->attribs.unknown_2_1_2 = 2;
    if ((flags & 8) != 0) {
        material->attribs.unknown_6_128 = 0;
    }
    if ((flags & 0x40) != 0) {
        material->attribs.z_mode = 0;
    }
    NuMtlUpdate(material);
    NuMtlSetCurrentRenderPlane(previous_render_plane);
    return duplicate;
}

extern "C" {
extern "C" void NuMtlAnimateSetMask(i32 mask) {
    mtl_animation_mask = mask;
}

extern "C" void NuMtlInitOverride(i32 count, VARIPTR *buffer, VARIPTR *end) {
    g_overrideList = NuLstCreateBuff(count, sizeof(MTL_OVERRIDE_RECORD), buffer, *end, 16);
}

extern "C" void NuMtlRegisterForOverride(NUMTL *original, NUVERTEXDESCRIPTOR *descriptors, i32 count) {
    for (i32 i = 0; i < count; ++i) {
        NUVERTEXDESCRIPTOR descriptor = descriptors[i];
        if (FindOverrideMaterial(original, descriptor) == NULL) {
            MTL_OVERRIDE_RECORD *entry = reinterpret_cast<MTL_OVERRIDE_RECORD *>(NuLstAlloc(g_overrideList));
            entry->material = NuMtlCreate3D(1);
            entry->original = original;
            entry->vertex_descriptor = descriptors[i];
            *entry->material = *original;
            entry->material->shader_desc.vtx_desc = descriptors[i];
            entry->material->shader_desc.shader_variant_id = 0;
            entry->material->shader_desc.shader_id = 0;
            NuMtlUpdate(entry->material);
        }
    }
}

extern "C" NUMTL *NuMtlFindVariantMtl(NUMTL *source, NUMTL *original) {
    NULNKHDR *node;
    MTL_OVERRIDE_RECORD *entry;

    for (node = NuLstGetNext(g_overrideList, NULL); node != NULL; node = NuLstGetNext(g_overrideList, node)) {
        entry = reinterpret_cast<MTL_OVERRIDE_RECORD *>(node);
        if (original == entry->original &&
            entry->material->shader_desc.vtx_desc.flags == source->shader_desc.vtx_desc.flags) {
            return entry->material;
        }
    }

    NuMtlRegisterForOverride(original, &source->shader_desc.vtx_desc, 1);

    const NUVERTEXDESCRIPTOR descriptor = source->shader_desc.vtx_desc;
    for (node = NuLstGetNext(g_overrideList, NULL); node != NULL; node = NuLstGetNext(g_overrideList, node)) {
        entry = reinterpret_cast<MTL_OVERRIDE_RECORD *>(node);
        if (entry->original == original && entry->material->shader_desc.vtx_desc.flags == descriptor.flags) {
            return entry->material;
        }
    }
    return NULL;
}

    i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values) {
        (void)skinned;
        if (__builtin_expect(special_handle == NULL || mtx == NULL, 0))
            return 0;
        NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special_handle);
        NuSpecialBoundsDisplayLayout *special = static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special);
        NUMTX wind_matrix;
        if (special->wind_scale != 0) {
            WindShear(&wind_matrix, mtx, special->wind_scale, special->wind_speed);
            mtx = &wind_matrix;
        }
        i32 clip_state = nuspecial_clip_state;
        NUDLDLISTSCENE *scene = reinterpret_cast<NUDLDLISTSCENE *>(handle->scene->display_list);
        if (clip_state == -1)
            clip_state = NuCameraClipTestExtents(&special->min, &special->max, mtx, 0.0f, 0);

        i32 shadow_clip = 0;
        i32 previous_shadow_results = 0;
        if ((scene->visibility_flags[special->instance_ix] & 0x20) != 0 && NuSpecialHasActiveShadowLights() != 0) {
            previous_shadow_results = NuSpecialHaveShadowClipTestResults();
            NUVEC minimum, maximum;
            NuVecMtxTransform(&minimum, &special->min, mtx);
            NuVecMtxTransform(&maximum, &special->max, mtx);
            shadow_clip = NuSpecialClipTestShadowLights(&minimum, &maximum, 0);
        }
        if (shadow_clip == 0 && clip_state == 0)
            return 0;

        NUVEC center;
        NuVecAdd(&center, &special->min, &special->max);
        NuVecScale(&center, &center, 0.5f);
        NuVecMtxTransform(&center, &center, mtx);
        f32 distance_sqr = NuCameraDistSqr(&center);
        distance_sqr = !(distance_sqr < 0.0f) ? distance_sqr : 0.0f;

        NUCLIPOBJECT *clip_object = special->clip_objects;
        if (special->clip_range != NULL && special->clip_range[0] != 0.0f) {
            i32 lod = 0;
            if (nurndr_force_lod == 0) {
                while (distance_sqr < special->clip_range[lod])
                    ++lod;
            } else {
                do {
                    ++lod;
                } while (special->clip_range[lod] != 0.0f && lod != nurndr_force_lod);
            }
            clip_object += lod;
        }

        f32 distance_alpha = 1.0f;
        if (__builtin_expect(scene->fade_ranges != NULL && (scene->visibility_flags[special->instance_ix] & 0x40) != 0, 0)) {
            const f32 fade_end = scene->fade_ranges[special->instance_ix * 2 + 1];
            const f32 fade_start = scene->fade_ranges[special->instance_ix * 2];
            if (__builtin_expect(fade_end <= fade_start, 0)) {
                if (distance_sqr <= fade_end * fade_end)
                    return 0;
                distance_alpha = (NuFsqrt(distance_sqr) - fade_end) / (fade_start - fade_end);
                if (distance_alpha > 1.0f)
                    distance_alpha = 1.0f;
            } else if (__builtin_expect(!(distance_sqr <= fade_start * fade_start), 0)) {
                distance_alpha = (fade_end - NuFsqrt(distance_sqr)) / (fade_end - fade_start);
                distance_alpha = distance_alpha >= 0.0f ? distance_alpha : 0.0f;
            }
            if (distance_alpha == 0.0f)
                return 0;
        }

        void *transform_packet = NULL;
        for (u32 i = 0; i < static_cast<u32>(clip_object->nmaterials); ++i) {
            NUMTL *material = scene->mtls[clip_object->material_ids[i]];
            NUMTL *replacement = NULL;
            if (__builtin_expect(nurndr_forced_mtl_table != NULL && material->unknown_9a[0] != 0, 0))
                replacement = nurndr_forced_mtl_table[material->unknown_9a[0]];
            else
                replacement = nurndr_forced_mtl;
            if (__builtin_expect(replacement != NULL, 1)) {
                if (material->shader_desc.vtx_desc.flags != replacement->shader_desc.vtx_desc.flags)
                    material = NuMtlFindVariantMtl(material, replacement);
                else
                    material = replacement;
            }
            for (; material != NULL; material = material->next) {
                NUDISPLAYLIST *list = material->display_list;
                if (list == NULL)
                    continue;
                NUDISPLAYLISTITEM *geometry = scene->items + clip_object->indices[i];
                if (clip_state != 0) {
                    NUDLDLISTSCENE *material_scene = list->dlist;
                    material_scene->flags |= NUDL_SCENE_FLAG_CLIP_MATERIALS;
                    const i32 used_material = list->mtl_id;
                    u8 *used = material_scene->mtl_used[NuDisplaySceneCurrentBuffer(material_scene)];
                    used[used_material >> 3] |= static_cast<u8>(1U << (used_material & 7));
                    RndrStateSetConstAlphaTint(nuspecial_const_alpha_enabled, nuspecial_const_tint_enabled,
                                               nuspecial_const_alpha, &nuspecial_const_tint, material);
                    DisplayListUpdateRenderState(list, &render_state);
                    const i32 shadow_caster =
                        (scene->visibility_flags[special->instance_ix] & 0x20) != 0 && nuspecial_reflection == 0;
                    if (geometry->type == 0x8f) {
                        VARIPTR *buffer = NuDisplayListLinkItems(list, 2);
                        void *faceon = DisplayListCreateFaceonTransformPS(buffer, mtx, list->dlist->mtls[list->mtl_id],
                                                                          geometry->next);
                        NUDISPLAYLISTITEM *first = list->items;
                        first->type = 0x90;
                        first->id = 3;
                        first->next = faceon;
                        list->items = first + 1;
                        void *packet = NuDisplayListPrepareFaceonPS(buffer, geometry->next, mtx);
                        NUDISPLAYLISTITEM *last = list->items;
                        last->type = 0x8f;
                        last->id = 3;
                        last->next = packet;
                        list->items = last + 1;
                        DisplayListSetAlphaPS(first, last, distance_alpha);
                    } else if (skin_matrices != NULL) {
                        NUDISPLAYLISTITEM *first_and_last[3];
                        DisplayListProcessSkin(list, geometry, first_and_last, mtx, &transform_packet,
                                               skin_matrices, blend_values, shadow_caster);
                    } else if (static_cast<i32>(material->shader_desc.flags) < 0 && clip_object->indices[i] > 1 &&
                               (geometry[-2].type == 0xae || geometry[-2].type == 0xb0 || geometry[-2].type == 0xaf)) {
                        NUDISPLAYLISTITEM *first_and_last[3];
                        DisplayListProcessLightmapped(list, geometry, first_and_last, mtx, &transform_packet,
                                                      distance_alpha);
                    } else {
                        VARIPTR *buffer = NuDisplayListLinkItems(list, 2);
                        void *packet = DisplayListCreateGeomTransformPS(buffer, mtx, list->dlist->mtls[list->mtl_id],
                                                                        geometry->next, transform_packet);
                        NUDISPLAYLISTITEM *first = list->items;
                        first->id = 3;
                        first->type = 0x8c;
                        first->next = packet;
                        list->items = first + 1;
                        NUDISPLAYLISTITEM *last = list->items;
                        last->type = 0x82;
                        last->id = 3;
                        last->next = geometry->next;
                        list->items = last + 1;
                        transform_packet = packet;
                        DisplayListSetAlphaPS(first, last, distance_alpha);
                        DisplayListSetShadowCasterFlagPS(first, last, shadow_caster);
                    }
                }

                if (shadow_clip != 0 && nuspecial_reflection == 0) {
                    for (i32 light_index = 0; light_index < NuSpecialGetActiveShadowLights(); ++light_index) {
                        NuDynamicLight *light = static_cast<NuDynamicLight *>(NuSpecialGetShadowLight(light_index));
                        const i32 render_set_count = NuDynamicLightGetActiveRenderSetCount(light);
                        const u32 visible_sets = static_cast<u32>(NuSpecialGetShadowClipTestResult(light_index));
                        for (i32 render_set = 0; render_set < render_set_count; ++render_set) {
                            if ((visible_sets & (1 << render_set)) == 0)
                                continue;
                            NUDISPLAYLIST *shadow_list = NuDynamicLightGetDList(light, render_set, material);
                            shadow_list->dlist = scene;
                            NuDisplayListLinkItems(shadow_list, 1);
                            NUDISPLAYLISTITEM *item = shadow_list->items;
                            item->type = 0x80;
                            item->id = 3;
                            item->next = material;
                            shadow_list->items = item + 1;
                            DisplayListUpdateRenderStateShadow(shadow_list, &render_state.state);
                            geometry = scene->items + clip_object->indices[i];
                            if (geometry->type == 0x8f) {
                                VARIPTR *buffer = NuDisplayListLinkItems(shadow_list, 2);
                                void *faceon = DisplayListCreateFaceonTransformPS(
                                    buffer, mtx, shadow_list->dlist->mtls[shadow_list->mtl_id], geometry->next);
                                NUDISPLAYLISTITEM *first = shadow_list->items;
                                first->type = 0x90;
                                first->id = 3;
                                first->next = faceon;
                                shadow_list->items = first + 1;
                                void *packet = NuDisplayListPrepareFaceonPS(buffer, geometry->next, mtx);
                                NUDISPLAYLISTITEM *last = shadow_list->items;
                                last->type = 0x8f;
                                last->id = 3;
                                last->next = packet;
                                shadow_list->items = last + 1;
                                DisplayListSetAlphaPS(first, last, distance_alpha);
                            } else if (skin_matrices != NULL) {
                                NUDISPLAYLISTITEM *first_and_last[3];
                                DisplayListProcessSkin(shadow_list, geometry, first_and_last, mtx,
                                                       &transform_packet, skin_matrices, blend_values, 1);
                            } else if (static_cast<i32>(material->shader_desc.flags) < 0 &&
                                       clip_object->indices[i] > 1 &&
                                       (geometry[-2].type == 0xae || geometry[-2].type == 0xb0 ||
                                        geometry[-2].type == 0xaf)) {
                                NUDISPLAYLISTITEM *first_and_last[3];
                                DisplayListProcessLightmapped(shadow_list, geometry, first_and_last, mtx,
                                                              &transform_packet, distance_alpha);
                            } else {
                                VARIPTR *buffer = NuDisplayListLinkItems(shadow_list, 2);
                                void *packet = DisplayListCreateGeomTransformPS(
                                    buffer, mtx, shadow_list->dlist->mtls[shadow_list->mtl_id], geometry->next,
                                    transform_packet);
                                NUDISPLAYLISTITEM *first = shadow_list->items;
                                first->type = 0x8c;
                                first->id = 3;
                                first->next = packet;
                                shadow_list->items = first + 1;
                                NUDISPLAYLISTITEM *last = shadow_list->items;
                                last->type = 0x82;
                                last->id = 3;
                                last->next = geometry->next;
                                shadow_list->items = last + 1;
                                transform_packet = packet;
                                DisplayListSetAlphaPS(first, last, distance_alpha);
                                DisplayListSetShadowCasterFlagPS(first, last, 1);
                            }
                        }
                    }
                }
            }
        }
        RndrStateSetConstAlphaTint(0, 0, 0.0f, NULL, NULL);
        if (previous_shadow_results == 0)
            NuSpecialClearShadowClipTestResults();
        return clip_state;
    }

    static void NuDisplaySceneRndrBody(NUDLDLISTSCENE *scene) {

        u8 visibility_context[0x2800];
        RndrStateCopyGlobalState(scene->local_state);
        NuVisibilityResult *visibility =
            static_cast<NuVisibilityResult *>(NuVisiEvaluate(scene->gscene, visibility_context));
        if (currentScene.unknown_3c != NULL) {
            if (visibility == NULL || (visibility->state & 1) == 0) {
                i32 instance = 0;
                for (i32 clip = 0; clip < scene->nclip_objects; ++instance) {
                    if ((scene->visibility_flags[instance] & NUDL_INSTANCE_FLAG_CASTS_SHADOW) != 0) {
                        NUCLIPBOUNDS &bounds = scene->clip_bounds[instance];
                        if (scene->lod_ranges[clip] != 0.0f)
                            clip = DisplaySceneSelectLod(scene, clip, DisplaySceneDistanceSqrToCamera(bounds));
                        const i32 result = NuDynamicLightTestShadowExtrusionExtent(
                            static_cast<NuDynamicLight *>(currentScene.unknown_3c), &bounds.center, &bounds.extent,
                            currentScene.unknown_40);
                        if (result != 0)
                            DisplaySceneSetClipResult(scene, clip, result);
                    }
                    clip = DisplaySceneNextInstance(scene, clip);
                }
            }
        } else if (visibility == NULL || (visibility->state & 1) == 0) {
            scene->alpha_values = NULL;
            if (scene->fade_ranges != NULL) {
                VARIPTR *buffer = NuDisplayListGetBuffer();
                scene->alpha_values = buffer->f32_ptr;
                buffer->addr += static_cast<usize>(scene->nclip_objects) * sizeof(f32);
            }
            i32 instance = 0;
            for (i32 clip = 0; clip < scene->nclip_objects; ++instance) {
                const u8 flags = scene->visibility_flags[instance];
                bool visible = visibility == NULL || visibility->instance_tree_bits == NULL ||
                               (visibility->instance_tree_bits[instance >> 3] & (1 << (instance & 7))) != 0;
                if (visibility != NULL && visibility->portal_marker != NULL && portals_enabled != 0) {
                    visible = (flags & NUDL_INSTANCE_FLAG_NO_VISIBILITY_TEST) != 0 ||
                              (visibility->portal_bits[instance >> 3] & (1 << (instance & 7))) != 0;
                }
                if (visible && (flags & NUDL_INSTANCE_FLAG_VISIBLE) != 0) {
                    NUCLIPBOUNDS &bounds = scene->clip_bounds[instance];
                    const f32 distance_sqr = DisplaySceneDistanceSqrToCamera(bounds);
                    clip = DisplaySceneSelectLod(scene, clip, distance_sqr);
                    const f32 far_clip = DisplaySceneFarClip(scene, instance, clip);
                    const i32 result = NuCameraClipTestExtentsAxisAligned(&bounds.center, &bounds.extent, far_clip);
                    if (result != 0) {
                        if (scene->alpha_values != NULL)
                            scene->alpha_values[clip] = DisplaySceneDistanceFade(scene, instance, distance_sqr);
                        DisplaySceneSetClipResult(scene, clip, result);
                    }
                }
                clip = DisplaySceneNextInstance(scene, clip);
            }
        }
        if (scene->nclip_objects != 0)
            scene->flags |= NUDL_SCENE_FLAG_CLIPPING;
        DisplayListGenerateTransforms(scene);

        NUGSCN temporary_scene;
        NUGSCN *gscene = scene->gscene;
        if (gscene == NULL) {
            temporary_scene.display_list = scene;
            gscene = &temporary_scene;
        }
        const bool portal_filter =
            portal_special_objects != 0 && visibility->portal_marker != NULL && portals_enabled != 0;
        if ((scene->instance_visibility_enabled & NUDL_SCENE_INSTANCE_VISIBILITY_ENABLED) == 0 &&
            noscenespecials == 0) {
            nuhspecial_s handle = {gscene, NULL, NULL};
            for (i32 index = 0; index < scene->nspecials; ++index) {
                NUDISPLAYSPECIAL *special = &static_cast<NUDISPLAYSPECIAL *>(scene->specials)[index];
                if ((special->flags & NUDISPLAYSPECIAL_FLAG_VISIBLE) == 0)
                    continue;
                handle.display_special = special;
                const i32 instance = special->instance_ix;
                if (portal_filter && (scene->visibility_flags[instance] & NUDL_INSTANCE_FLAG_NO_VISIBILITY_TEST) == 0 &&
                    (visibility->portal_bits[instance >> 3] & (1 << (instance & 7))) == 0)
                    continue;
                NUMTX *draw_mtx = special->draw_mtx_ptr;
                if (draw_mtx != reinterpret_cast<NUMTX *>(-1) && draw_mtx != NULL)
                    NuDisplayListRndrSpecial(&handle, draw_mtx, 0, NULL, NULL);
                else
                    NuDisplayListRndrSpecial(&handle, &special->draw_mtx, 0, NULL, NULL);
            }
        }
    }

    void NuDisplaySceneRndr(void *display_scene) {
        NUDLDLISTSCENE *scene = static_cast<NUDLDLISTSCENE *>(display_scene);
        if ((scene->flags & NUDL_SCENE_FLAG_CLIPPING) != 0)
            return;
        NuDisplaySceneRndrBody(scene);
    }

extern "C" void NuGScnRndr(NUGSCN *scene) {
    if (scene->additional_scenes != NULL && scene->rendered_additional_scene_count > 0) {
        NuDisplaySceneRndr(scene->additional_scenes[scene->rendered_additional_scene_count - 1]->display_list);
    } else {
        NuDisplaySceneRndr(scene->display_list);
    }
    ++scene->rendered_additional_scene_count;
}

    void DisplaySceneRndrSpecials(NUDLDLISTSCENE *scene, i32, void *visibility_context) {
        NuVisibilityResult *visibility =
            static_cast<NuVisibilityResult *>(NuVisiEvaluate(scene->gscene, &visibility_context));
        i32 portal_filter = 0;
        if (visibility != NULL && portal_special_objects != 0 && visibility->portal_marker != NULL)
            portal_filter = portals_enabled != 0;
        void *shadow_pass = currentScene.unknown_3c;

        if ((scene->instance_visibility_enabled & NUDL_SCENE_INSTANCE_VISIBILITY_ENABLED) != 0 ||
            noscenespecials != 0) {
            return;
        }

        NUGSCN temporary_scene;
        NUGSCN *gscene = scene->gscene;
        if (gscene == NULL) {
            temporary_scene.display_list = scene;
            gscene = &temporary_scene;
        }

        nuhspecial_s special_handle;
        special_handle.scene = gscene;
        for (i32 index = 0; index < scene->nspecials; ++index) {
            NUDISPLAYSPECIAL *special = &static_cast<NUDISPLAYSPECIAL *>(scene->specials)[index];
            if ((special->flags & NUDISPLAYSPECIAL_FLAG_VISIBLE) == 0) {
                continue;
            }
            special_handle.display_special = special;
            if (shadow_pass && (scene->visibility_flags[special->instance_ix] & NUDL_INSTANCE_FLAG_CASTS_SHADOW) == 0) {
                continue;
            }
            if (portal_filter &&
                __builtin_expect(
                    (scene->visibility_flags[special->instance_ix] & NUDL_INSTANCE_FLAG_NO_VISIBILITY_TEST) == 0, 1) &&
                (visibility->portal_bits[special->instance_ix >> 3] & (1U << (special->instance_ix & 7))) == 0) {
                continue;
            }
            NUMTX *draw_mtx = special->draw_mtx_ptr;
            if (draw_mtx == reinterpret_cast<NUMTX *>(-1)) {
                draw_mtx = NULL;
            }
            if (draw_mtx == NULL) {
                NuDisplayListRndrSpecial(&special_handle, &special->draw_mtx, 0, NULL, NULL);
            } else {
                NuDisplayListRndrSpecial(&special_handle, draw_mtx, 0, NULL, NULL);
            }
        }
    }

}

void NuDynamicLight::addShadowCasterScene(nugscn_s *scene) {
    if (render_set_capacity > 0) {
        RenderSet *current = render_sets;
        RenderSet *end = render_sets + render_set_capacity;
        do {
            RenderSet &set = *current;
            set.scene_cursor[0] = NULL;
            set.scene_first[0] = NULL;
            set.scene_end[0] = NULL;
            set.scene_cursor[1] = NULL;
            set.scene_first[1] = NULL;
            set.scene_end[1] = NULL;
        } while (++current != end);
    }
    NUDLDLISTSCENE *dl = scene->display_list;
    i32 instance = 0;
    for (i32 lod = 0; lod < dl->nclip_objects; ++instance, ++lod) {
        if ((dl->visibility_flags[instance] & 0x21) == 0x21) {
            f32 dx = global_camera.mtx.m30 - dl->clip_bounds[instance].center.x;
            f32 dy = global_camera.mtx.m31 - dl->clip_bounds[instance].center.y;
            f32 dz = global_camera.mtx.m32 - dl->clip_bounds[instance].center.z;
            f32 distance = dx * dx + dy * dy + dz * dz;
            if (dl->lod_ranges[lod] != 0.0f) {
                while (dl->lod_ranges[lod] > distance)
                    ++lod;
            }
            NUCLIPOBJECT *object = &dl->clip_objects[lod];
            for (i32 i = 0; i < render_set_capacity; ++i) {
                RenderSet &set = render_sets[i];
                NUPORTALBOX &box = scene->portal_boxes[instance];
                f32 sx = set.capsule_end.x - set.capsule_center.x;
                f32 sy = set.capsule_end.y - set.capsule_center.y;
                f32 sz = set.capsule_end.z - set.capsule_center.z;
                f32 radius = set.capsule_radius + box.first_w;
                f32 ox = box.first.x - set.capsule_center.x;
                f32 length = sy * sy + sx * sx + sz * sz;
                f32 oy = box.first.y - set.capsule_center.y;
                f32 oz = box.first.z - set.capsule_center.z;
                f32 dot = sx * ox + sy * oy + sz * oz;
                f32 projection = (1.0f / length) * dot;
                f32 positive = (0.0f < projection ? 1.0f : 0.0f) * projection;
                f32 t = (positive <= 1.0f ? 1.0f : 0.0f) * positive + (1.0f < positive ? 1.0f : 0.0f);
                f32 distance = (ox * ox + oy * oy + oz * oz) + (length * t) * t - (dot + dot) * t;
                if (__builtin_expect(distance > radius * radius, 0))
                    continue;
                VuVec minimum, maximum;
                minimum.x = box.first.x - box.second.x;
                minimum.z = box.first.z - box.second.z;
                minimum.y = box.first.y - box.second.y;
                maximum.x = box.first.x + box.second.x;
                maximum.y = box.first.y + box.second.y;
                maximum.z = box.first.z + box.second.z;
                if (!!!testShadowExtrusion(minimum, maximum, i)) {
                    continue;
                }
                for (i32 j = 0; j < object->nmaterials; ++j) {
                    NUMTL *material = scene->mtls[object->material_ids[j]];
                    NUDISPLAYLIST *list =
                        material->shader_desc.unknown_1b4 ? &set.display_lists[1] : &set.display_lists[0];
                    i32 channel = list - set.display_lists;
                    if ((usize)set.scene_cursor[channel] >= (usize)set.scene_end[channel]) {
                        NUDISPLAYLISTITEM *cursor =
                            (NUDISPLAYLISTITEM *)((display_list_buffer->addr + 15) & ~(usize)15);
                        set.scene_cursor[channel] = cursor;
                        display_list_buffer->addr = (usize)(cursor + 49);
                        if (set.scene_end[channel]) {
                            set.scene_end[channel]->next = cursor;
                            set.scene_end[channel]->id = 1;
                            set.scene_end[channel]->type = 0x8d;
                        } else
                            set.scene_first[channel] = cursor;
                        set.scene_end[channel] = cursor + 48;
                        material = scene->mtls[object->material_ids[j]];
                    }
                    set.scene_cursor[channel][0].id = 3;
                    set.scene_cursor[channel][0].type = 0x80;
                    set.scene_cursor[channel][0].next = material;
                    set.scene_cursor[channel][1] = dl->items[object->indices[j] - 1];
                    set.scene_cursor[channel][2] = dl->items[object->indices[j]];
                    set.scene_cursor[channel][1].id = 3;
                    set.scene_cursor[channel][2].id = 3;
                    set.scene_cursor[channel] += 3;
                }
            }
        }
        while (dl->lod_ranges[lod] != 0.0f)
            ++lod;
    }
    DisplaySceneRndrSpecials(dl, 0, NULL);
    for (i32 i = 0; i < render_set_capacity; ++i) {
        RenderSet &set = render_sets[i];
        if (set.scene_first[0]) {
            NUDISPLAYLIST *list = &set.display_lists[0];
            NuDisplayListLinkItems(list, 1);
            NUMTL *material = scene->mtls[0];
            list->items->id = 3;
            list->items->next = material;
            list->items->type = 0x80;
            ++list->items;
            NuDisplayListLinkList(list, set.scene_first[0], set.scene_cursor[0]);
        }
        if (set.scene_first[1]) {
            NUDISPLAYLIST *list = &set.display_lists[1];
            NuDisplayListLinkItems(list, 1);
            NUMTL *material = scene->mtls[0];
            list->items->type = 0x80;
            list->items->next = material;
            list->items->id = 3;
            ++list->items;
            NuDisplayListLinkList(list, set.scene_first[1], set.scene_cursor[1]);
        }
    }
}

extern "C" {
    void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene) {
        light->addShadowCasterScene(scene);
    }

    i32 NuSpecialDrawWith(void *special, NUMTX *mtx) {
        NuSpecialHandleLayout *handle = static_cast<NuSpecialHandleLayout *>(special);
        if (handle->scene == NULL) {
            return 0;
        }
        NUMTX combined;
        if (handle->display_special != NULL) {
            NuMtxMul(&combined, static_cast<NUMTX *>(handle->display_special), mtx);
            return NuDisplayListRndrSpecial(reinterpret_cast<nuhspecial_s *>(special), &combined, 0, NULL, NULL);
        }
        NuMtxMul(&combined, static_cast<NUMTX *>(handle->special), mtx);
        return 0;
    }

    i32 NuSpecialDrawSmoothSkinDwa(void *special, NUMTX *skin_matrices, NUMTX *world_matrix,
                                   DEFORMERWEIGHTSARRAY *blend_values) {
        NuSpecialHandleLayout *handle = static_cast<NuSpecialHandleLayout *>(special);
        if (handle->scene == NULL || handle->display_special == NULL) {
            return 0;
        }
        return NuDisplayListRndrSpecial(reinterpret_cast<nuhspecial_s *>(special), world_matrix, 2, skin_matrices,
                                        blend_values);
    }

    i32 NuSpecialDrawSmoothSkin(void *special, NUMTX *skin_matrices, NUMTX *world_matrix) {
        NuSpecialHandleLayout *handle = static_cast<NuSpecialHandleLayout *>(special);
        if (handle->scene == NULL || handle->display_special == NULL) {
            return 0;
        }
        return NuDisplayListRndrSpecial(reinterpret_cast<nuhspecial_s *>(special), world_matrix, 2, skin_matrices,
                                        NULL);
    }

    i32 NuSpecialDrawAt(void *special, NUMTX *mtx) {
        NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
        if (handle == NULL || handle->scene == NULL || handle->display_special == NULL) {
            return 0;
        }
        return NuDisplayListRndrSpecial(reinterpret_cast<nuhspecial_s *>(special), mtx, 0, NULL, NULL);
    }

    // Original @0x2f56a0. Draw rigid hierarchy pieces at their evaluated joint
    // matrices, then build skin matrices for the smooth hierarchy pieces.
    i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32) {
        NUMTX_ALIGNED16 draw_matrix; // The original realigns the frame for this matrix.
        i32 clip_state = nuspecial_clip_state;
        if (clip_state == -1) {
            clip_state = NuCameraClipHGobj(reinterpret_cast<nugscn_s *>(object), world_matrix, joint_matrices);
        }

        i32 shadow_clip = 0;
        if (NuSpecialHasActiveShadowLights() != 0) {
            NUVEC bounds_min;
            NUVEC bounds_max;
            NuVecMtxTransform(&bounds_min, &object->bounds_min, world_matrix);
            NuVecMtxTransform(&bounds_max, &object->bounds_max, world_matrix);
            shadow_clip = NuSpecialClipTestShadowLights(&bounds_min, &bounds_max, 1);
        }
        if ((clip_state | shadow_clip) == 0) {
            NuSpecialClearShadowClipTestResults();
            return 0;
        }

        const i32 previous_clip_state = NuSpecialSetClipping(1, clip_state);
        i32 drawn = 0;

        for (i32 render_index = 0; render_index < render_count; ++render_index) {
            i32 part_index;
            if (render_indices != NULL) {
                part_index = render_indices[render_index];
                if (part_index < 0 || part_index >= object->render_count)
                    continue;
            } else {
                part_index = 0;
                render_count = 1;
            }
            NUMTX *skin_matrices = NULL;
            if (object->render_parts[part_index].smooth_skin_special != NULL ||
                object->render_parts[part_index].alternate_smooth_skin_special != NULL) {
                display_list_buffer->addr = ALIGN(ALIGN(display_list_buffer->addr, 0x10), 0x80);
                skin_matrices = reinterpret_cast<NUMTX *>(display_list_buffer->addr);
                display_list_buffer->addr += static_cast<usize>(object->joint_count) * sizeof(NUMTX);
            }

            void *blend_value = blend_values != NULL ? blend_values[render_index] : NULL;

            if (object->render_parts[part_index].rigid_specials != NULL) {
                for (i32 joint_index = 0; joint_index < object->joint_count; ++joint_index) {
                    if (object->render_parts[part_index].rigid_specials[joint_index] != NULL) {

                        NuMtxMulVU0(&draw_matrix, &joint_matrices[joint_index], world_matrix);
                        drawn |=
                            NuSpecialDrawAt(object->render_parts[part_index].rigid_specials[joint_index], &draw_matrix);
                    }
                }
            }

            if (object->render_parts[part_index].alternate_rigid_specials != NULL) {
                for (i32 joint_index = 0; joint_index < object->joint_count; ++joint_index) {
                    if (object->render_parts[part_index].alternate_rigid_specials[joint_index] != NULL) {
                        NuMtxMulVU0(&draw_matrix, &joint_matrices[joint_index], world_matrix);
                        drawn |= NuSpecialDrawAt(object->render_parts[part_index].alternate_rigid_specials[joint_index],
                                                 &draw_matrix);
                    }
                }
            }

            if (object->render_parts[part_index].smooth_skin_special != NULL ||
                object->render_parts[part_index].alternate_smooth_skin_special != NULL) {
                const i32 joint_count = object->joint_count;
                NUMTX *inverse_bind_matrix = object->inverse_bind_matrices;
                for (i32 joint_index = 0; joint_index < joint_count; ++joint_index) {
                    NuMtxMulH(&skin_matrices[joint_index], &inverse_bind_matrix[joint_index],
                              &joint_matrices[joint_index]);
                }
                if (object->render_parts[part_index].smooth_skin_special != NULL) {
                    drawn |=
                        NuSpecialDrawSmoothSkinDwa(object->render_parts[part_index].smooth_skin_special, skin_matrices,
                                                   world_matrix, static_cast<DEFORMERWEIGHTSARRAY *>(blend_value));
                }
                if (object->render_parts[part_index].alternate_smooth_skin_special != NULL) {
                    drawn |= NuSpecialDrawSmoothSkinDwa(object->render_parts[part_index].alternate_smooth_skin_special,
                                                        skin_matrices, world_matrix,
                                                        static_cast<DEFORMERWEIGHTSARRAY *>(blend_value));
                }
            }
        }

        NuSpecialSetClipping(1, previous_clip_state);
        NuSpecialClearShadowClipTestResults();
        return drawn;
    }

extern "C" NUMTL *NuMtlFindVariantMtlFromDesc(NUMTL *original, NUVERTEXDESCRIPTOR descriptor) {
    return FindOverrideMaterial(original, descriptor, original);
}

extern "C" void NuMtlDestroy(NUMTL *mtl) {
    mtl->is_used = false;
    NuDisplayListDestroyMtl(mtl);
    NULNKHDR *node = NuLstGetNext(g_overrideList, NULL);
    while (node != NULL) {
        MTL_OVERRIDE_RECORD *entry = reinterpret_cast<MTL_OVERRIDE_RECORD *>(node);
        node = NuLstGetNext(g_overrideList, node);
        if (entry->original == mtl) {
            entry->material->is_used = false;
            NuDisplayListDestroyMtl(entry->material);
            NuLstFree(reinterpret_cast<NULNKHDR *>(entry));
        }
    }
}

}

void NuShaderMtlDescInit(NUSHADERMTLDESC *desc) {
    i32 i;

    if (desc == NULL) {
        return;
    }

    memset(desc, 0, sizeof(NUSHADERMTLDESC));

    desc->unknown_24 = 1.0f;
    desc->byte4 |= 0x10;

    desc->flags = 0x1000;

    desc->diffuse_color[0] = RGBA_TO_NUCOLOUR32(0xff, 0x80, 0x80, 0xff);

    for (i = 0; i < 4; i++) {
        desc->tex_anim_data[i] = -1;
    }

    desc->vtx_desc.has_position = 1;
    desc->vtx_desc.has_diffuse = 1;
}

void NuTexInitEx(VARIPTR *buf, i32 max_tex_count) {
    max_textures = max_tex_count;

    texture_list = (NUNATIVETEX **)ALIGN(buf->addr, 0x4);
    buf->addr = (usize)texture_list + max_tex_count * sizeof(NUNATIVETEX *);
    memset(texture_list, 0, max_tex_count * sizeof(NUNATIVETEX *));

    texture_order = (i32 *)ALIGN(buf->addr, 0x4);
    buf->addr = (usize)texture_order + max_tex_count * sizeof(i32);
    memset(texture_order, 0, max_tex_count * sizeof(i32));

    gTextureLoadCount = 0;
}

i32 NuTexCreateNative(NUNATIVETEX *tex, bool is_pvrtc) {
    i32 i;

    if (tex == NULL) {
        return 0;
    }

    criticalSection.Lock();

    for (i32 i = 0; i < max_textures; i++) {
        if (texture_list[i] == NULL) {
            texture_list[i] = tex;
            texture_order[i] = gTextureLoadCount++;

            criticalSection.Unlock();

            NuTexCreatePS(tex, is_pvrtc);

            return i + 1;
        }
    }

    criticalSection.Unlock();

    return 0;
}

NUQFNT *NuQFntReadBuffer(VARIPTR *font, VARIPTR *buf, VARIPTR buf_end) {
    VARIPTR base;
    VARIPTR texture;
    VARIPTR relocation_table;
    VARIPTR relocation_entry;
    VARIPTR pointer;
    VARIPTR target;
    i32 texture_size;
    i32 relocation_table_end;
    u32 relocation_count;
    u32 i;
    i32 width __attribute__((aligned(16)));
    i32 height __attribute__((aligned(16)));
    u32 gl_texture;
    VUFNT *result;
    NUNATIVETEX *native_texture;

    base = *font;
    if (base.void_ptr == NULL)
        return NULL;

    relocation_table.addr = base.addr + *(i32 *)base.void_ptr;
    relocation_count = *(i32 *)relocation_table.void_ptr;
    relocation_entry.addr = relocation_table.addr + 4;

    struct Relocation {
        i32 pointer;
        i32 target;
    };
    Relocation *relocations = NULL;
    if (relocation_count != 0) {
        relocations = (Relocation *)__builtin_alloca(relocation_count * sizeof(Relocation));
        for (i = 0; i < relocation_count; i++, relocation_entry.addr += 4) {
            pointer.addr = relocation_entry.addr + *(i32 *)relocation_entry.void_ptr;
            relocations[i].pointer = pointer.addr - font->addr;
            if (*(i32 *)pointer.void_ptr == 0) {
                relocations[i].target = 0;
            } else {
                target.addr = pointer.addr + *(i32 *)pointer.void_ptr;
                relocations[i].target = target.addr - font->addr;
                *(usize *)pointer.void_ptr = target.addr;
            }
        }
    }

    texture.addr = ((usize *)base.void_ptr)[2];
    texture_size = relocation_table.addr - texture.addr;
    relocation_table_end = relocation_entry.addr - base.addr;
    const i32 texture_offset = texture.addr - base.addr;
    const i32 relocation_offset = relocation_table.addr - base.addr;
    gl_texture = NuIOS_CreateGLTexFromPlatformInMemory(texture.void_ptr, &width, &height, false);
    if (g_buttonsFont != 0) {
        width /= 2;
        height /= 2;
    }

    g_buttonsFont = 0;
    base = *font;
    texture.addr = base.addr + texture_offset;
    relocation_table.addr = base.addr + relocation_offset;
    memmove(texture.void_ptr, relocation_table.void_ptr, relocation_table_end - relocation_offset);
    base = *font;
    *(i32 *)base.void_ptr = texture_offset;
    texture.addr = base.addr + texture_offset;

    if (relocation_count != 0) {
        relocation_table.addr = texture.addr;
        *(i32 *)relocation_table.void_ptr = relocation_count;
        relocation_entry.addr = relocation_table.addr + 4;
        for (i = 0; i < relocation_count; i++, relocation_entry.addr += 4) {
            if (relocations[i].pointer != 0) {
                *(i32 *)relocation_entry.void_ptr = relocations[i].pointer - (relocation_entry.addr - base.addr);
                *(i32 *)(base.addr + relocations[i].pointer) = relocations[i].target - relocations[i].pointer;
            } else {
                *(i32 *)relocation_entry.void_ptr = 0;
            }
        }
    }

    buf->addr -= texture_size;
    memset(buf->void_ptr, 0, texture_size);
    result = *(VUFNT **)NuPtrBlockFix(font->void_ptr);

    native_texture = (NUNATIVETEX *)ALIGN(buf->addr, 4);
    buf->addr = (usize)(native_texture + 1);
    native_texture->width = width;
    native_texture->height = height;
    native_texture->image_data = NULL;
    native_texture->size = 0;
    native_texture->platform.gl_tex = gl_texture;
    i32 texture_id = NuTexCreateNative(native_texture, true);
    NuQFntReadPS(result, texture_id, 4, 0, buf, buf_end);
    return result;
}

__attribute__((optimize("O2"))) void NuQFntInit(VARIPTR *buf, VARIPTR buf_end) {
    VARIPTR sysfont_ptr;

    if (nuqfnt_init != 0) {
        return;
    }

    nuqfnt_init = 1;
    sysfont_ptr.u8_ptr = sysfont;
    system_qfont = NuQFntReadBuffer(&sysfont_ptr, buf, buf_end);

    if (system_qfont != NULL) {
        NuQFntSetICGap(system_qfont, 1.0f);
    }
}

__attribute__((optimize("O2"))) NUQFNT *NuQFntRead(char *filepath, VARIPTR *buf, VARIPTR buf_end) {
    char *platform;
    char *ext;
    VARIPTR aligned;
    i32 bytes_read;
    char fixed_path[1204];

    if (NuPlatform::Get()->GetCurrentPlatform() != ANDROID_PVRTC_PLATFORM) {
        // ORIG_BUG: While `fixed_path` is 1204 bytes, its size is passed as 1024.
        NuStrFixExtPlatform(fixed_path, filepath, g_fontExtension, 1024, "MOB");
    } else {
        NuStrFixExtPlatform(fixed_path, filepath, "fnt", 1024, "IOS");
    }

    aligned.addr = ALIGN(buf->addr, 0x20);
    buf->addr = aligned.addr;

    g_datfileMode = 1;

    bytes_read = NuFileLoadBufferVP(fixed_path, buf, &buf_end);

    if (bytes_read == 0) {
        return NULL;
    }

    g_buttonsFont = NuStrIStr(fixed_path, "BUTTONS_MOB.ANDROID") != 0 || NuStrIStr(fixed_path, "BUTTONS_IOS.FNT") != 0;

    return NuQFntReadBuffer(&aligned, buf, buf_end);
}

i32 NuTexReserveNative(NUNATIVETEX *texture, i32 tex_id) {
    if (texture == NULL)
        return 0;
    if (tex_id != 0) {
        texture_list[tex_id - 1] = texture;
        return tex_id;
    }
    for (i32 i = 0; i < max_textures; ++i) {
        if (texture_list[i] == NULL) {
            texture_list[i] = texture;
            return i + 1;
        }
    }
    return 0;
}

i32 NuTexWidth(i32 tex_id) {
    return texture_list[tex_id - 1]->width;
}

i32 NuTexHeight(i32 tex_id) {
    return texture_list[tex_id - 1]->height;
}

void DrawAlphaImage(i32 rows, i32 cols, numtl_s *material, i32 use_pixel_offsets, NuBloomParameters *parameters) {
    f32 inv_col = 1.0f / (cols - 1);
    f32 inv_row = 1.0f / (rows - 1);
    f32 x0 = 0.0f * inv_row * 2.0f - 1.0f;
    f32 y_start = 0.0f * inv_col * 2.0f - 1.0f;
    f32 x1 = inv_row * 2.0f - 1.0f;
    f32 step_x = inv_row * 2.0f;
    f32 step_y = inv_col * 2.0f;
    f32 near_scale = parameters->near_scale * 128.0f;
    f32 far_scale = parameters->far_scale * 128.0f;
    f32 near_angle = parameters->near_angle / 180.0f;
    f32 far_angle = parameters->far_angle / 180.0f;
    f32 scale_delta = far_scale - near_scale;
    f32 angle_delta = far_angle - near_angle;
    NUVPREGION saved_region = g_NuVpRegion;
    NuVpResetRegions();

    NUCAMERA camera;
    NuCameraGet(&camera);
    u32 clear_screen;
    memcpy(&clear_screen, &parameters->unknown_18, sizeof(clear_screen));
    if (clear_screen != 0)
        ClearScreen();
    if (parameters->directional)
        NuVecNorm(&parameters->direction, &parameters->direction);

    static f32 pixelOffsetX;
    static f32 pixelOffsetY;
    if (use_pixel_offsets) {
        pixelOffsetX = 0.5f / static_cast<f32>(NuTexWidth(material->tex_id));
        pixelOffsetY = 0.5f / static_cast<f32>(NuTexHeight(material->tex_id));
    }

    static i32 first = 1;
    static f32 camFov;
    static f32 adjacent;
    static i32 row, col;
    if (first) {
        camFov = camera.fov;
        adjacent = 1.0f / NU_TAN_LUT((i32)(camera.fov * 0.5f * 10430.3779296875f));
        first = 0;
    }
    if (camera.fov != camFov) {
        adjacent = 1.0f / NU_TAN_LUT((i32)(camera.fov * 0.5f * 10430.3779296875f));
        camera.fov = camFov;
    }
    camera.mtx.m30 = 0.0f;
    camera.mtx.m31 = 0.0f;
    camera.mtx.m32 = 0.0f;
    camera.mtx.m33 = 1.0f;

    ++NuPrimCSPos;
    NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_NORMALISED);
    for (row = 0; row < rows - 1; ++row) {
        const i32 cache_row = row;
        NuPrim2DBegin(1, 7, material);
        f32 y0 = y_start;
        for (col = 0; col < cols; ++col) {
            NUVEC direction;
            const f32 direction_y = ((y0 + 1.0f) - 1.0f) * -camera.aspect;
            direction.x = (x0 + 1.0f) - 1.0f;
            direction.y = direction_y;
            direction.z = adjacent;
            NuVecNorm(&direction, &direction);
            NuVecMtxTransform(&direction, &direction, &camera.mtx);
            NuVecNorm(&direction, &direction);

            f32 brightness = direction.y;
            u32 colour;
            if (!(cache_row > 0)) {
                brightness = brightness * 0.5f + 0.5f;
                if (brightness <= near_angle)
                    brightness = near_scale * parameters->intensity;
                else if (brightness >= far_angle)
                    brightness = far_scale * parameters->intensity;
                else
                    brightness =
                        ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity;
                if (parameters->directional) {
                    f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;
                    if (angle < parameters->direction_near_angle) {
                        brightness += 128.0f * parameters->direction_far_scale;
                    } else if (!(angle > parameters->direction_far_angle)) {
                        f32 blend;
                        if (parameters->direction_bias == 1.0f)
                            blend = 1.0f - (angle - parameters->direction_near_angle) /
                                               (parameters->direction_far_angle - parameters->direction_near_angle);
                        else
                            blend = 1.0f -
                                    NuPowFast((angle - parameters->direction_near_angle) /
                                                  (parameters->direction_far_angle - parameters->direction_near_angle),
                                              parameters->direction_bias);
                        brightness +=
                            128.0f * (blend * (parameters->direction_far_scale - parameters->direction_near_scale) +
                                      parameters->direction_near_scale);
                    }
                }
                colour = (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 24) | 0x808080;
                NuRndrPrimSetColour(colour);
            } else {
                colour = (static_cast<u32>(cacheValues[col]) << 24) | 0x808080;
                NuRndrPrimSetColour(colour);
            }
            AlphaImagePrimUV(static_cast<f32>(row) * inv_row + pixelOffsetX,
                             (static_cast<f32>(col) * inv_col + pixelOffsetY) * 0.9f);
            NuPrim2DAddXYZ(x0, y0, 0.0f);

            direction.x = (x1 + 1.0f) - 1.0f;
            direction.y = direction_y;
            direction.z = adjacent;
            NuVecNorm(&direction, &direction);
            NuVecMtxTransform(&direction, &direction, &camera.mtx);
            NuVecNorm(&direction, &direction);
            brightness = direction.y * 0.5f + 0.5f;
            if (brightness <= near_angle)
                brightness = near_scale * parameters->intensity;
            else if (brightness >= far_angle)
                brightness = far_scale * parameters->intensity;
            else
                brightness =
                    ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity;
            if (parameters->directional) {
                f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;
                if (angle < parameters->direction_near_angle) {
                    brightness += 128.0f * parameters->direction_far_scale;
                } else if (!(angle > parameters->direction_far_angle)) {
                    f32 blend;
                    if (parameters->direction_bias == 1.0f)
                        blend = 1.0f - (angle - parameters->direction_near_angle) /
                                           (parameters->direction_far_angle - parameters->direction_near_angle);
                    else
                        blend =
                            1.0f - NuPowFast((angle - parameters->direction_near_angle) /
                                                 (parameters->direction_far_angle - parameters->direction_near_angle),
                                             parameters->direction_bias);
                    brightness +=
                        128.0f * (blend * (parameters->direction_far_scale - parameters->direction_near_scale) +
                                  parameters->direction_near_scale);
                }
            }
            const u8 alpha = static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)));
            cacheValues[col] = alpha;
            colour = (static_cast<u32>(alpha) << 24) | 0x808080;
            NuRndrPrimSetColour(colour);
            AlphaImagePrimUV(static_cast<f32>(row + 1) * inv_row + pixelOffsetX,
                             (static_cast<f32>(col) * inv_col + pixelOffsetY) * 0.9f);
            NuPrim2DAddXYZ(x1, y0, 0.0f);
            y0 += step_y;
        }
        NuPrim2DEnd();
        x0 += step_x;
        x1 += step_x;
    }
    --NuPrimCSPos;
    NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    g_NuVpRegion = saved_region;
}

__attribute__((optimize("O2"))) void NuQFntPrintCharW(NUQFNT *font, u16 *text, u32 flags) {
    VUFNT *vufnt = static_cast<VUFNT *>(font);
    VUFNT_ANDROID *platform = vufnt->platform_data;
    f32 y = platform->y;
    f32 z = platform->z;
    f32 x = platform->x;
    if (text != NULL) {

        f32 inverse_texture_width = 1.0f;
        f32 inverse_texture_height = 1.0f;
        if (vufnt->mtl->tex_id > 0) {
            inverse_texture_width = 1.0f / static_cast<f32>(NuTexWidth(vufnt->mtl->tex_id));
            inverse_texture_height = 1.0f / static_cast<f32>(NuTexHeight(vufnt->mtl->tex_id));
        }

        f32 height = vufnt->height * *vufnt->y_scale;
        f32 space_width;
        if (nuqfnt_space_width != 0.0f)
            space_width = nuqfnt_space_width * *vufnt->x_scale;
        else
            space_width = vufnt->space_width * *vufnt->x_scale;
        NuPrimCSPos++;
        NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_PS2);
        const u32 is_3d = flags & 4;
        if (is_3d)
            NuPrim3DBegin(0, 7, vufnt->mtl, &vufnt->platform_data->mtx);
        else
            NuPrim2DBegin(4, 7, vufnt->mtl);

        i32 colour = platform->colour;
        u16 character = *text++;
        if (character != 0) {
            do {
                VUFNTCHAR *glyph = &vufnt->glyphs[character];
                f32 glyph_width = character == 0x20 ? space_width : glyph->width * *vufnt->x_scale;
                f32 advance = glyph_width;
                f32 top, bottom;
                if (is_3d) {
                    top = y - height;
                    bottom = y;
                } else {
                    top = y;
                    bottom = y + height;
                }
                f32 left = x;
                if ((flags & 1) != 0 && static_cast<u16>(character - 0x30) <= 9) {
                    const i16 zero_code = static_cast<i16>(NuQFntEncodeUnicodeChar(font, 0x30));
                    VUFNTCHAR *zero = &vufnt->glyphs[zero_code];
                    f32 digit_width = zero->width * *vufnt->x_scale;
                    left = (digit_width - glyph_width) + x;
                    advance = digit_width;
                }

                if (character != 0x20) {
                    f32 right = left + glyph_width;
                    if (is_3d) {
                        const f32 u0 = inverse_texture_width * glyph->x;
                        const f32 v0 = inverse_texture_height * glyph->y;
                        const f32 u1 = (glyph->width + glyph->x) * inverse_texture_width;
                        const f32 v1 = (glyph->y + vufnt->height) * inverse_texture_height;
                        NuRndrPrimSetColour(colour);
                        NuRndrPrimUV(u0, v1);
                        NuRndrPrimPosition(left, top, z);
                        NuRndrPrimSetColour(colour);
                        NuRndrPrimUV(u1, v1);
                        NuRndrPrimPosition(right, top, z);
                        NuRndrPrimSetColour(colour);
                        NuRndrPrimUV(u1, v0);
                        NuRndrPrimPosition(right, bottom, z);
                        NuRndrPrimSetColour(colour);
                        NuRndrPrimUV(u1, v0);
                        NuRndrPrimPosition(right, bottom, z);
                        NuRndrPrimSetColour(colour);
                        NuRndrPrimUV(u0, v0);
                        NuRndrPrimPosition(left, bottom, z);
                        NuRndrPrimSetColour(colour);
                        NuRndrPrimUV(u0, v1);
                        NuRndrPrimPosition(left, top, z);
                    } else {
                        NuRndrPrimSetColour(colour);
                        const f32 v0 = inverse_texture_height * glyph->y;
                        const f32 u0 = inverse_texture_width * glyph->x;
                        NuRndrPrimUV(u0, v0);
                        NuPrim2DAddXYZ(left, top, 0.0f);
                        NuRndrPrimSetColour(colour);
                        const f32 v1 = (glyph->y + vufnt->height) * inverse_texture_height;
                        const f32 u1 = (glyph->x + glyph->width) * inverse_texture_width;
                        NuRndrPrimUV(u1, v1);
                        NuPrim2DAddXYZ(right, bottom, 0.0f);
                    }
                }
                x = x + vufnt->ic_gap * *vufnt->x_scale + advance;
            } while ((character = *text++) != 0);
        }

        if (is_3d)
            NuPrim3DEnd();
        else
            NuPrim2DEnd();
        NuPrimCSPos--;
        NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    }
}

void NuQFntPrintRSW(RNDRSTREAM *, NUQFNT *font, u16 *text, u32 flags) {
    NuQFntPrintCharW(font, text, flags);
}

void NuQFntPrintU(NUQFNT *font, char *text) {
    if (font == NULL)
        font = system_qfont;
    if (font != NULL) {
        if (text != NULL) {
            u16 encoded[1024];
            NuQFntUTF8toQCode(font, text, encoded);
            NuQFntPrintRSW(NULL, font, encoded, NuQFntMode);
        }
    }
}

extern "C" {
    void NuQFntPrint3DU(NUQFNT *font, char *text) {
        NuQFntPushPrintMode(4);
        NuQFntPrintU(font, text);
        NuQFntPopPrintMode();
    }

    void NuQFntPrint2dU(NUQFNT *font, char *text) {
        NuQFntPushPrintMode(2);
        NuQFntPrintU(font, text);
        NuQFntPopPrintMode();
    }

}

void OcclusionManager::RenderStats() const {
    if (!initialized || !enabled || NuRndrDoingScreenGrab != 0)
        return;
    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntSet(system_qfont);
    NuQFntSetScale(system_qfont, 0.7f, 0.7f);
    NuQFntSetPointSize(system_qfont, 0.7f, 0.7f);
    NuQFntMove2d(system_qfont, 112.0f, 112.0f, 0.0f);
    NuQFntSetColour2d(system_qfont, 0x80000000);
    char text[256];
    sprintf(text, "Occluders : %2d, Queries : %2d, Occluded : %2d, Visible : %2d", static_cast<i32>(current_set->count),
            static_cast<i32>(unknown_164), static_cast<i32>(unknown_160), static_cast<i32>(unknown_164 - unknown_160));
    NuQFntPrint2dU(system_qfont, text);
    f32 width = NuQFntLenScale();
    f32 height = NuQFntHeightScale();
    NuQFntMove2d(system_qfont, 112.0f - width, 112.0f - height, 0.0f);
    NuQFntSetColour2d(system_qfont, 0xff00ffff);
    NuQFntPrint2dU(system_qfont, text);
    NuQFntPopCoordinateSystem();
    NuQFntPopPrintMode();
}

extern "C" {
    void NuOcclusionManagerRenderStats(void) {
        g_OcclusionManager.RenderStats();
    }

    void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...) {
        char text[1024];
        NuQFntPushPrintMode(2);
        va_list arguments;
        va_start(arguments, format);
        vsprintf(text, format, arguments);
        va_end(arguments);
        i32 width = static_cast<i32>(NuQFntPrintLenU(font, text));
        if (alignment == 0x20) {
            x -= width;
        } else if (__builtin_expect(alignment == 0x40, 1)) {
            x -= width / 2;
        }
        NuQFntMove(font, static_cast<f32>(x), static_cast<f32>(y), 0.0f);
        NuQFntPrintU(font, text);
        NuQFntPopPrintMode();
    }

extern "C" void NuTimeBarSetRenderHorizontal(i32 set) {
    static i32 tmppeak_reset = 1;
    if (set == -1 && NuTimeBar_EngineEnabled == 0)
        return;

    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntSet(system_qfont);
    NuQFntSetColour(system_qfont, 0xe0e0e0e0);
    NuQFntSetScale(system_qfont, 0.85f, 0.85f);

    const f32 vertical_scale = 120.0f / static_cast<f32>(PS2_VREZ_H);
    TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
    f32 name_width = 0.0f;
    for (i32 slot = 0; slot < timebar->slot_count; ++slot) {
        if (timebar->slot_names[slot] != NULL) {
            f32 width = NuQFntPrintLenU(system_qfont, const_cast<char *>(timebar->slot_names[slot]));
            if (width > name_width)
                name_width = width;
        }
    }
    NuQFntPopPrintMode();
    NuQFntPopCoordinateSystem();
    name_width /= static_cast<f32>(PS2_VREZ_W << 4);

    for (i32 slot = 0; slot < timebar->slot_count; ++slot) {
        u32 &maximum = reinterpret_cast<u32 *>(timebar->field_14)[slot];
        u32 &recent_peak = reinterpret_cast<u32 *>(timebar->field_18)[slot];
        u32 value = static_cast<u32>(timebar->accumulators[timebar->toggle_flags[slot]][slot]);
        if (value > maximum)
            maximum = value;
        if (tmppeak_reset == 0)
            recent_peak = 0;
        value = static_cast<u32>(timebar->accumulators[timebar->toggle_flags[slot]][slot]);
        if (value > recent_peak)
            recent_peak = value;

        if (recent_peak != 0) {
            const f32 left = name_width + 0.045f;
            const f32 top = static_cast<f32>(slot + 1) * 0.025f * 2.0f + 0.15f;
            const f32 right = left + timebar_unsigned_float(recent_peak) * 60.0f / 1000000.0f * vertical_scale;
            const i32 x = static_cast<i32>(left * (PS2_VREZ_W << 4));
            const i32 y = static_cast<i32>(top * (PS2_VREZ_H << 4));
            const i32 width = static_cast<i32>((right - left) * (PS2_VREZ_W << 4));
            const i32 height = static_cast<i32>((top + 0.025f - top) * (PS2_VREZ_H << 4));
            NuRndrRect2di(x, y, width, height, timebar->colours[slot], NULL);

            NuQFntPushPrintMode(2);
            NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
            NuQFntSet(system_qfont);
            NuQFntSetColour(system_qfont, 0xe0e0e0e0);
            NuQFntSetScale(system_qfont, 0.85f, 0.85f);
            const i32 text_y = y + height - static_cast<i32>(NuQFntHeight(system_qfont) * 0.25f);
            if (timebar->slot_names[slot] != NULL)
                NuQFntPrintEx(system_qfont, static_cast<i32>(0.025f * (PS2_VREZ_W << 4)), text_y, 0x10,
                              timebar->slot_names[slot]);
            if (timebar->field_18[slot] != 0)
                NuQFntPrintEx(system_qfont, x, text_y, 0x10, "%i",
                              static_cast<i32>(timebar_unsigned_float(static_cast<u32>(timebar->field_18[slot])) *
                                               15734.0f / 1000000.0f));
            if (timebar->field_14[slot] != 0)
                NuQFntPrintEx(system_qfont, static_cast<i32>(0.975f * (PS2_VREZ_W << 4)), text_y, 0x20, "%i",
                              static_cast<i32>(timebar_unsigned_float(static_cast<u32>(timebar->field_14[slot])) *
                                               15734.0f / 1000000.0f));
            NuQFntPopPrintMode();
            NuQFntPopCoordinateSystem();
        }
        if (timebar->field_28 != 0)
            NuTimeBarSlotReset(set, slot);
        if (NuTimeBar_PeakReset != 0)
            timebar->field_14[slot] = 0;
    }
    NuTimeBar_PeakReset = 0;
    --tmppeak_reset;
    if (tmppeak_reset < 0)
        tmppeak_reset = 30;
}

extern "C" void NuTimeBarSetRender(i32 set) {
    static i32 tmppeak_reset = 1;
    // Like the horizontal renderer, active rendering requires a created set.
    TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
    f32 gpu_frames = 0.0f;
    if (set == -1) {
        if (__builtin_expect(NuTimeBar_GpuFrameOutEnabled != 0, 0)) {
            i32 buffer = timebar->toggle_flags[1];
            u32 gpu_time = static_cast<u32>(timebar->accumulators[buffer][1]);
            gpu_frames = timebar_unsigned_float(gpu_time) * nuapi.fps / 1000000.0f;
        }
        if (NuTimeBar_EngineEnabled == 0 && gpu_frames <= 1.0f) {
            return;
        }
    }

    NuRndrBeginScene(-1);
    if (set >= 0 || (set == -1 && NuTimeBar_EngineEnabled != 0)) {
        const f32 vertical_scale = 120.0f / static_cast<f32>(PS2_VREZ_H);

        for (i32 column_index = 1; column_index <= timebar->slot_count; ++column_index) {
            const i32 slot = column_index - 1;
            i32 buffer = timebar->toggle_flags[slot];
            u32 value = static_cast<u32>(timebar->accumulators[buffer][slot]);
            u32 &maximum = reinterpret_cast<u32 *>(timebar->field_14)[slot];
            u32 &recent_peak = reinterpret_cast<u32 *>(timebar->field_18)[slot];
            if (value > maximum) {
                maximum = value;
            }
            if (tmppeak_reset == 0) {
                recent_peak = 0;
            }
            value = static_cast<u32>(timebar->accumulators[timebar->toggle_flags[slot]][slot]);
            if (value > recent_peak) {
                recent_peak = value;
            }

            if (recent_peak != 0) {
                const f32 left = static_cast<f32>(column_index) * 0.025f * 3.0f;
                const f32 top = 0.05f;
                const f32 bottom = top + timebar_unsigned_float(recent_peak) * 60.0f / 1000000.0f * vertical_scale;
                const i32 y = static_cast<i32>(top * (PS2_VREZ_H << 4));
                const i32 x = static_cast<i32>(left * (PS2_VREZ_W << 4));
                const i32 width = static_cast<i32>((left + 0.025f - left) * (PS2_VREZ_W << 4));
                const i32 height = static_cast<i32>((bottom - top) * (PS2_VREZ_H << 4));
                NuRndrRect2di(x, y, width, height, timebar->colours[slot], NULL);

                NuQFntPushPrintMode(2);
                NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
                NuQFntSet(system_qfont);
                NuQFntSetScale(system_qfont, 1.0f, 1.0f);
                NuQFntSetColour(system_qfont, 0xe0e0e0e0);

                const i32 text_width = PS2_VREZ_W;
                NuQFntBaseline(system_qfont);
                if (timebar->field_18[slot] != 0) {
                    i32 value = static_cast<i32>(timebar_unsigned_float(static_cast<u32>(timebar->field_18[slot])) *
                                                 15734.0f / 1000000.0f);
                    i32 text_y = static_cast<i32>(static_cast<f32>(PS2_VREZ_H) * 0.05f) * 16;
                    if (value > 9999)
                        value = 9999;
                    text_y += static_cast<i32>(NuQFntHeight(system_qfont)) * (value > 999 ? 5 : 4);
                    const f32 column = static_cast<f32>(text_width) * 0.025f;
                    // Retail stacks three digits bottom-up, plus a fourth for
                    // values >= 1000; the final height query occurs either way.
                    for (i32 digit = 0; digit < 3; ++digit) {
                        text_y -= static_cast<i32>(NuQFntHeight(system_qfont));
                        const i32 text_x =
                            (static_cast<i32>(column) * (column_index * 3) + static_cast<i32>(column * 0.5f)) * 16;
                        NuQFntPrintEx(system_qfont, text_x, text_y, 0x40, "%d", value % 10);
                        value /= 10;
                    }
                    const f32 last_height = NuQFntHeight(system_qfont);
                    if (value != 0) {
                        text_y -= static_cast<i32>(last_height);
                        const i32 text_x =
                            (static_cast<i32>(column) * (column_index * 3) + static_cast<i32>(column * 0.5f)) * 16;
                        NuQFntPrintEx(system_qfont, text_x, text_y, 0x40, "%d", value % 10);
                    }
                }
                if (timebar->field_14[slot] != 0) {
                    const f32 column = static_cast<f32>(PS2_VREZ_W) * 0.025f;
                    const i32 text_x =
                        (static_cast<i32>(column) * (column_index * 3) + static_cast<i32>(column * 0.5f)) * 16;
                    i32 text_y = PS2_VREZ_H * 16;
                    text_y -= static_cast<i32>(NuQFntHeight(system_qfont)) * 2;
                    text_y += static_cast<i32>(NuQFntBaseline(system_qfont));
                    if ((slot & 1) != 0)
                        text_y -= 320;
                    NuQFntPrintEx(system_qfont, text_x, text_y, 0x40, "%d",
                                  static_cast<i32>(timebar_unsigned_float(static_cast<u32>(timebar->field_14[slot])) *
                                                   15734.0f / 1000000.0f));
                }
                if (timebar->slot_names[slot] != NULL) {
                    const f32 column = static_cast<f32>(PS2_VREZ_W) * 0.025f;
                    const i32 text_x =
                        (static_cast<i32>(column) * (column_index * 3) + static_cast<i32>(column * 0.5f)) * 16;
                    i32 text_y = PS2_VREZ_H * 16;
                    text_y -= static_cast<i32>(NuQFntHeight(system_qfont));
                    text_y += static_cast<i32>(NuQFntBaseline(system_qfont));
                    text_y -= (slot & 1) != 0 ? 640 : 320;
                    NuQFntPrintEx(system_qfont, text_x, text_y, 0x40, timebar->slot_names[slot]);
                }
                NuQFntPopPrintMode();
                NuQFntPopCoordinateSystem();
            }

            if (timebar->field_28 != 0) {
                NuTimeBarSlotReset(set, slot);
            }
            if (NuTimeBar_PeakReset != 0) {
                timebar->field_14[slot] = 0;
            }
        }

        NuTimeBar_PeakReset = 0;
        --tmppeak_reset;
        if (tmppeak_reset < 0) {
            tmppeak_reset = 30;
        }
    }

    if (gpu_frames > 1.0f) {
        NuRndrRect2di(0, 0, 0x3c0, 0xe00, 0x80000064, NuTimeBar_FrameOutMtl);
        NuRndrRect2di(0x2440, 0, 0x3c0, 0xe00, 0x80000064, NuTimeBar_FrameOutMtl);
    }
    NuRndrEndScene();
}

    void NuQFntPrintV(NUQFNT *font, const char *format, va_list arguments) {
        char text[1024];
        vsprintf(text, format, arguments);
        NuQFntPrintU(font, text);
    }

}

void NuQFntPrintW(NUQFNT *font, u16 *text) {
    if (font == NULL)
        font = system_qfont;
    if (font != NULL)
        NuQFntPrintRSW(NULL, font, text, NuQFntMode);
}

extern "C" {
    void NuQFntPrint3DW(NUQFNT *font, u16 *text) {
        NuQFntPushPrintMode(4);
        NuQFntPrintW(font, text);
        NuQFntPopPrintMode();
    }

    void NuQFntPrint2dW(NUQFNT *font, u16 *text) {
        NuQFntPushPrintMode(2);
        NuQFntPrintW(font, text);
        NuQFntPopPrintMode();
    }

}

f32 NuQFntPrintJustifiedRSW(RNDRSTREAM *stream, void *font_ptr, u16 *text, f32 x, f32 y, f32 z, f32 sx, f32 sy,
                            f32 width, f32 line_spacing, u32 colour, NUMTX *mtx) {
    VUFNT *font = static_cast<VUFNT *>(font_ptr);
    f32 saved_space_width = nuqfnt_space_width;
    u16 encoded_space[2] __attribute__((aligned(16))) = {0x20, 0};
    f32 saved_ic_gap = font->ic_gap;
    f32 space_width = saved_space_width == 0.0f ? font->space_width : saved_space_width;

    NuQFntSetColourRS(stream, font, colour);
    NuQFntSetScaleRS(stream, font, sx, sy);
    NuQFntSetSpaceWidth(font, space_width);
    f32 printed_space_width = NuQFntPrintLenW(font, encoded_space);
    if (mtx != NULL)
        NuQFntSetMtx(font, mtx);
    f32 line_height = NuQFntHeight(font);
    const u16 hyphen = NuQFntEncodeUnicodeChar(font, '-');
    i32 line_number = 0;
    do {
        NuQFntSetICGap(font, saved_ic_gap);
        NuQFntSetScaleRS(stream, font, sx, sy);
        u16 line[256] __attribute__((aligned(16)));
        u16 *next = text;
        i32 words = 0, punctuation_spaces = 0;
        f32 words_width = 0.0f;
        for (;;) {
            u16 *word_start = next;
            i32 length = 0, extra_spaces = 0;
            while (*next != 0 && *next != hyphen) {
                if (*next != 0x20) {
                    line[length] = *next;
                    line[length + 1] = 0;
                    if (NuQFntPrintLenW(font, line) > width) {
                        line[length] = 0;
                        break;
                    }
                    ++length;
                } else {
                    const i32 following = next[1];
                    if (following != '?' && following != '!' && following != '.' && following != ',')
                        break;
                    ++extra_spaces;
                }
                ++next;
            }
            // A hyphen terminates a word segment but belongs to that segment.
            if (*next == hyphen)
                line[length++] = *next++;
            while (*next == 0x20)
                ++next;
            line[length] = 0;
            const f32 word_width = NuQFntPrintLenW(font, line);
            if (words == 0) {
                words_width = word_width + words_width;
            } else {
                const f32 combined_word_width = word_width + words_width;
                if (!(width * justify_squash >=
                      (words + punctuation_spaces + extra_spaces) * printed_space_width + combined_word_width)) {
                    next = word_start;
                    break;
                }
                words_width = combined_word_width;
            }
            ++words;
            punctuation_spaces += extra_spaces;
            if (*next == 0)
                break;
        }

        i32 length = 0;
        u16 *copy_cursor = text;
        while (copy_cursor < next) {
            if (*copy_cursor != 0x20) {
                line[length++] = *copy_cursor++;
            } else {
                line[length++] = 0x20;
                do {
                    ++copy_cursor;
                } while (*copy_cursor == 0x20);
            }
        }
        line[length] = 0;
        const f32 spaces = words - 1 + punctuation_spaces;
        f32 scale;
        if (*next == 0) {
            const f32 ratio = width / (printed_space_width * spaces + words_width);
            scale = 1.0f < ratio ? 1.0f : ratio;
        } else if (words == 1) {
            scale = width / words_width;
        } else {
            const f32 ratio = width / (printed_space_width * spaces + words_width);
            scale = justify_stretch < ratio ? justify_stretch : ratio;
        }
        NuQFntSetScaleRS(stream, font, sx * scale, sy);
        if (*next == 0) {
            NuQFntSetSpaceWidth(font, space_width * scale);
        } else {
            if (words == 1)
                NuQFntSetSpaceWidth(font, space_width);
            else
                NuQFntSetSpaceWidth(font, (space_width * (width - words_width * scale)) /
                                              (scale * printed_space_width * spaces));
        }
        NuQFntMoveRS(stream, font, x, y + line_number * line_height * line_spacing, z);
        NuQFntPrintRSW(stream, font, line, mtx == NULL ? 0 : 4);
        ++line_number;
        text = next;
    } while (*text != 0);

    NuQFntSetSpaceWidth(font, saved_space_width);
    NuQFntSetICGap(font, saved_ic_gap);
    return line_number * line_height * line_spacing;
}

f32 NuQFntPrintJustifiedW(NUQFNT *font, u16 *text, f32 x, f32 y, f32 z, f32 sx, f32 sy, f32 width, f32 line_spacing,
                          u32 colour, NUMTX *mtx) {
    if (font == NULL)
        font = system_qfont;
    if (font == NULL)
        return 0.0f;
    return NuQFntPrintJustifiedRSW(NULL, font, text, x, y, z, sx, sy, width, line_spacing, colour, mtx);
}

extern "C" {
    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

    VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *) {
        DECOMP_ASSERT(offsetof(NUFNT, height) == 0x0c, "NUFNT height offset");
        DECOMP_ASSERT(offsetof(NUFNT, baseline) == 0x14, "NUFNT baseline offset");
        DECOMP_ASSERT(offsetof(NUFNT, ic_gap) == 0x16, "NUFNT inter-character gap offset");
        DECOMP_ASSERT(offsetof(NUFNT, mtl) == 0x18, "NUFNT material offset");
        DECOMP_ASSERT(offsetof(NUFNT, glyph_count) == 0x1c, "NUFNT glyph count offset");
        DECOMP_ASSERT(offsetof(NUFNT, character_map) == 0x20, "NUFNT character map offset");
        DECOMP_ASSERT(offsetof(NUFNT, glyphs) == 0x120, "NUFNT glyphs offset");
        DECOMP_ASSERT(sizeof(NUFNTCHAR) == 0x14, "NUFNTCHAR stride");
        DECOMP_ASSERT(sizeof(VUFNTHDR) == 0x20, "VUFNTHDR size");
        DECOMP_ASSERT(offsetof(VUFNT, legacy_font) == 0x28, "VUFNT legacy font offset");

        f32 texture_width = NuTexWidth(font->mtl->tex_id);
        f32 texture_height = NuTexHeight(font->mtl->tex_id);
        VUFNT *result = reinterpret_cast<VUFNT *>(ALIGN(buf->addr, 16));
        buf->addr = reinterpret_cast<usize>(result) + sizeof(VUFNT) + sizeof(VUFNT_ANDROID);
        memset(result, 0, sizeof(VUFNT) + sizeof(VUFNT_ANDROID));
        result->glyph_count = font->glyph_count;
        result->platform_data = reinterpret_cast<VUFNT_ANDROID *>(result + 1);
        result->height = font->height;
        result->baseline = font->baseline;
        result->ic_gap = font->ic_gap;
        result->hdr = static_cast<VUFNTHDR *>(buf->void_ptr);
        result->legacy_font = font;

        result->space_width = font->glyphs[0].u1 * texture_width - font->glyphs[0].u0 * texture_width;
        *buf->f32_ptr++ = result->ic_gap;
        *buf->f32_ptr++ = result->height;
        *buf->f32_ptr++ = 1.0f / texture_width;
        *buf->f32_ptr++ = 1.0f / texture_height;
        *buf->f32_ptr++ = 0.0f;
        *buf->f32_ptr++ = 0.0f;
        *buf->f32_ptr++ = result->space_width;
        *buf->f32_ptr++ = 0.0f;
        result->x_scale = buf->f32_ptr;
        *buf->f32_ptr++ = 1.0f;
        result->y_scale = buf->f32_ptr;
        *buf->f32_ptr++ = 0.5f;

        result->glyphs = static_cast<VUFNTCHAR *>(buf->void_ptr);
        for (i32 i = 0; i < (MIN(font->glyph_count, 256)); i++) {
            *buf->f32_ptr++ = font->glyphs[i].u0 * texture_width;
            *buf->f32_ptr++ = font->glyphs[i].v0 * texture_height;
            *buf->f32_ptr++ = font->glyphs[i].u1 * texture_width - font->glyphs[i].u0 * texture_width;
        }
        result->character_map = buf->u8_ptr;
        for (i32 i = 0; i < 256; i++) {
            i32 index = font->character_map[i];
            if (index == 0xff)
                index = 0;
            if (i == ' ')
                index = font->glyph_count;
            *buf->u8_ptr++ = index;
        }

        result->mtl = reinterpret_cast<NUMTL *>(ALIGN(buf->addr, 16));
        buf->addr = ALIGN(buf->addr, 16) + sizeof(NUMTL);
        memcpy(result->mtl, font->mtl, sizeof(NUMTL));
        NUMTL *material = result->mtl;
        material->attribs.unknown_2_1_2 = 2;
        material->attribs.uv_mode = (flags & 8) == 0;
        material->attribs.alpha_mode = (flags & 2) == 0;
        if ((flags & 0x40) != 0)
            material->attribs.z_mode = 0;
        else if ((flags & 0x20) != 0)
            material->attribs.z_mode = 1;
        else
            material->attribs.z_mode = 3;
        if ((flags & 4) != 0)
            material->attribs.filter_mode = 1;
        else if ((flags & 0x10) != 0)
            material->attribs.filter_mode = 2;
        else
            material->attribs.filter_mode = 0;
        material->sort_pri = static_cast<i16>(render_options >> 16);
        NuMtlUpdate(material);
        result->platform_data->x = 0.0f;
        result->platform_data->y = 0.0f;
        result->platform_data->z = 0.0f;
        return result;
    }

}

NUNATIVETEX *NuTexGetNative(i32 tex_id) {
    if (tex_id > 0) {
        return texture_list[tex_id - 1];
    }

    return NULL;
}

void NuTexAddReference(i32 tex_id, NUGSCN *) {
    NUNATIVETEX *tex;

    tex = NuTexGetNative(tex_id);
    if (tex != NULL) {
        tex->ref_count++;
    }
}

void NuTexRemoveReference(i32 tex_id, NUGSCN *) {
    NUNATIVETEX *tex;

    tex = NuTexGetNative(tex_id);
    if (tex != NULL) {
        tex->ref_count--;
    }
}

i32 NuGScnRestoreTID(nugscn_s *scene, i32 tid) {
    if (tid == 0) {
        return -1;
    }

    extern i32 NuTexGetUnresolvedTextureTIDPS();
    if (tid == NuTexGetUnresolvedTextureTIDPS()) {
        NuTexRemoveReference(tid, scene);
        return -1;
    }

    NUNATIVETEX *texture = NuTexGetNative(tid);
    for (i32 i = 0; i != scene->ntextures; ++i) {
        NUNATIVETEX *scene_texture = scene->textures[i];
        bool checksum_matches = true;
        for (i32 byte = 0; byte < 16; ++byte) {
            if (texture->checksum[byte] != scene_texture->checksum[byte]) {
                checksum_matches = false;
                break;
            }
        }
        if (checksum_matches && scene->texture_ids[i] == tid) {
            if (scene_texture->ref_count < 0) {
                NuTexRemoveReference(tid, scene);
                return i | 0x4000;
            }
            return i;
        }
    }

    return -1;
}

void NuGScnRestoreTIDs(nugscn_s *scene) {
    for (i32 i = 0; i < scene->nummtl; ++i) {
        if (scene->ntextures == 0) {
            continue;
        }

        NUMTL *material = scene->mtls[i];
        material->tex_id = (i16)NuGScnRestoreTID(scene, material->tex_id);

        NUSHADERMTLDESC &shader = material->shader_desc;
        shader.specular_map_tid = NuGScnRestoreTID(scene, shader.specular_map_tid);
        shader.lightmap_tex_id[0] = NuGScnRestoreTID(scene, shader.lightmap_tex_id[0]);
        shader.normal_map_tid = NuGScnRestoreTID(scene, shader.normal_map_tid);
        shader.lightmap_tex_id[1] = NuGScnRestoreTID(scene, shader.lightmap_tex_id[1]);
        shader.envmap_cubic_tid = NuGScnRestoreTID(scene, shader.envmap_cubic_tid);
        shader.unknown_198 = NuGScnRestoreTID(scene, shader.unknown_198);
        shader.shine_map_ps2_tid = NuGScnRestoreTID(scene, shader.shine_map_ps2_tid);
        for (i32 layer = 0; layer < 4; ++layer) {
            shader.diffuse_map_tex_id[layer] = NuGScnRestoreTID(scene, shader.diffuse_map_tex_id[layer]);
        }
        shader.vtf_height_map_tid = NuGScnRestoreTID(scene, shader.vtf_height_map_tid);
        shader.vtf_normal_map_tid = NuGScnRestoreTID(scene, shader.vtf_normal_map_tid);
        shader.field_1e4 = NuGScnRestoreTID(scene, shader.field_1e4);
        shader.field_1e8 = NuGScnRestoreTID(scene, shader.field_1e8);
    }

    NuGScnRestoreTIDsPS(scene);
}

i32 NuTexGetRefCount(i32 tex_id) {
    NUNATIVETEX *tex;

    tex = NuTexGetNative(tex_id);
    if (tex != NULL) {
        return tex->ref_count;
    }

    return 0;
}

void NuTexCleartid(i32 tex_id) {
    if (tex_id != 0) {
        if (NuTexGetRefCount(tex_id) != -1) {
            NuEffectTexGetEffectFromNative(tex_id);
            NuTexDestroyPS(texture_list[tex_id - 1]);
        }
        texture_list[tex_id - 1] = NULL;
    }
}

extern "C" {
extern "C" i32 NuTexResolveReference(NUGSCN *scene, i32 tex_id) {
    if ((tex_id & 0x4000) == 0) {
        return scene->texture_ids[tex_id];
    }
    if (__builtin_expect(max_textures == 0, 0)) {
        return 0;
    }

    const i32 reference_index = tex_id & 0x3fff;
    i32 newest_texture_id = 0;
    i32 newest_texture_order = 0;

    for (u32 index = 0; index != static_cast<u32>(max_textures); ++index) {
        NUNATIVETEX *texture = texture_list[index];
        if (texture == NULL || texture->ref_count < 0) {
            continue;
        }

        NUNATIVETEX *reference = scene->textures[reference_index];
        bool checksum_matches = true;
        for (i32 byte = 0; byte < 16; ++byte) {
            if (texture->checksum[byte] != reference->checksum[byte]) {
                checksum_matches = false;
                break;
            }
        }
        if (checksum_matches && static_cast<u32>(texture_order[index]) > static_cast<u32>(newest_texture_order)) {
            newest_texture_id = index + 1;
            newest_texture_order = texture_order[index];
        }
    }

    if (newest_texture_id != 0) {
        NuTexAddReference(newest_texture_id, scene);
        scene->texture_ids[reference_index] = newest_texture_id;
    }
    return newest_texture_id;
}

}

i32 NuGScnFixupTID(nugscn_s *scene, i32 tid) {
    if (tid == -1) {
        return 0;
    }
    if ((tid & 0x4000) == 0) {
        return scene->texture_ids[tid];
    }

    extern i32 NuTexGetUnresolvedTextureTIDPS();
    i32 resolved_tid = NuTexResolveReference(scene, tid);
    if (resolved_tid == 0) {
        resolved_tid = NuTexGetUnresolvedTextureTIDPS();
        NuTexAddReference(resolved_tid, scene);
    }
    return resolved_tid;
}

void NuGScnFixupTIDs(nugscn_s *scene) {
    for (i32 i = 0; i < scene->nummtl; ++i) {
        NUMTL *material = scene->mtls[i];
        material->tex_id = (i16)NuGScnFixupTID(scene, material->tex_id);

        NUSHADERMTLDESC &shader = material->shader_desc;
        shader.specular_map_tid = NuGScnFixupTID(scene, shader.specular_map_tid);
        shader.lightmap_tex_id[0] = NuGScnFixupTID(scene, shader.lightmap_tex_id[0]);
        shader.normal_map_tid = NuGScnFixupTID(scene, shader.normal_map_tid);
        shader.lightmap_tex_id[1] = NuGScnFixupTID(scene, shader.lightmap_tex_id[1]);
        shader.envmap_cubic_tid = NuGScnFixupTID(scene, shader.envmap_cubic_tid);
        shader.unknown_198 = NuGScnFixupTID(scene, shader.unknown_198);
        shader.shine_map_ps2_tid = NuGScnFixupTID(scene, shader.shine_map_ps2_tid);
        for (i32 layer = 0; layer < 4; ++layer) {
            shader.diffuse_map_tex_id[layer] = NuGScnFixupTID(scene, shader.diffuse_map_tex_id[layer]);
        }
        shader.vtf_height_map_tid = NuGScnFixupTID(scene, shader.vtf_height_map_tid);
        shader.vtf_normal_map_tid = NuGScnFixupTID(scene, shader.vtf_normal_map_tid);
        shader.field_1e4 = NuGScnFixupTID(scene, shader.field_1e4);
        shader.field_1e8 = NuGScnFixupTID(scene, shader.field_1e8);

        NUMTL *updated_material = scene->mtls[i];
        reinterpret_cast<u8 *>(updated_material)[0x46] &= 0xbf;
        NuMtlUpdate(updated_material);
    }

    NuGScnFixupTIDsPS(scene);
}

static NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path, char *scene_data) {
    NUGSCN *scene = reinterpret_cast<NUGSCN *>(scene_data);
    if (__builtin_expect(scene == NULL, 0)) {
        char converted_path[1033];
        NuFileExtConvert(converted_path, path);
        NUFILE file = NuFileOpen(converted_path, NUFILE_READ);
        if (file == 0) {
            return NULL;
        }

        i32 file_size = (i32)NuFileOpenSize(file);
        buf->addr = ALIGN(buf->addr, 0x20);
        i32 uploaded = NuGScnUploadGfxDataFromFilePS(buf, *buf_end, (i32)file);
        scene = (NUGSCN *)ALIGN(buf->addr, 0x20);
        buf->addr = (usize)((char *)scene + file_size - uploaded);
        NuFileRead(file, scene, file_size - uploaded);
        NuFileClose(file);
    }

    const i32 version = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(scene) + 8);
    void **fixed = (void **)NuPtrBlockFix((char *)scene + 0x18);
    NUGSCN *fixed_scene = (NUGSCN *)*fixed;
    if (fixed_scene != NULL) {
        if (__builtin_expect(*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(fixed_scene) + 0x168) > 0, 1)) {
            if (version < 0 || version > 2) {
                NuSevereWarning("Your Character version is %d, whereas the Engine only accepts Character versions "
                                "from %d to %d",
                                version, 0, 2);
            }
        } else if (version < 0 || version > 2) {
            NuSevereWarning("Your Scene version is %d, whereas the Engine only accepts Scene versions from %d to %d",
                            version, 0, 2);
        }
        if (version < 2) {
            *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(fixed_scene) + 0x1f4) = 1;
        }
        NuGScnCreatePS(fixed_scene, buf, buf_end);

        for (i32 i = 0; i < fixed_scene->ntextures; ++i) {
            if (fixed_scene->textures[i]->ref_count < 0) {
                fixed_scene->texture_ids[i] = 0xabcdabcd;
            } else {
                fixed_scene->texture_ids[i] = NuTexCreateNative(fixed_scene->textures[i], true);
            }
        }
        NuGScnFixupTIDs(fixed_scene);
        nutexanim_s *animations = static_cast<nutexanim_s *>(fixed_scene->texture_anims);
        u32 animation_texture_count = 0;
        for (i32 i = 0; i < fixed_scene->num_texture_anims; ++i) {
            u32 end = reinterpret_cast<usize>(animations[i].texture_ids) + animations[i].texture_count;
            if (animation_texture_count < end)
                animation_texture_count = end;
        }
        for (i32 i = 0; i < animation_texture_count; ++i) {
            fixed_scene->texture_anim_ids[i] = fixed_scene->texture_ids[fixed_scene->texture_anim_ids[i]];
        }
        for (i32 i = 0; i < fixed_scene->num_texture_anims; ++i) {
            nutexanim_s *anim = &static_cast<nutexanim_s *>(fixed_scene->texture_anims)[i];
            anim->flags = 0;
            anim->next = NULL;
            anim->previous = NULL;
            anim->texture_ids = fixed_scene->texture_anim_ids + reinterpret_cast<usize>(anim->texture_ids);
            anim->material = fixed_scene->mtls[reinterpret_cast<usize>(anim->material)];
            nutexanimprog_s *program = NuTexAnimProgFind(anim->program_name);
            anim->env = NuTexAnimEnvCreate(buf, anim->material, anim->texture_ids, program);
        }
        for (i32 i = 0; i < fixed_scene->num_texture_anims - 1; ++i) {
            animations[i].next = &animations[i + 1];
            animations[i + 1].previous = &animations[i];
        }
        NuTexAnimAddList(animations);
        if (fixed_scene->display_list != NULL) {
            NuDisplaySceneAdd(reinterpret_cast<NUDLDLISTSCENE *>(fixed_scene->display_list));
        }
        NuGScnFixupPS(fixed_scene);
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(fixed_scene) + 0x40) = 0;
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(fixed_scene) + 0x44) = 0;
    }
    return fixed_scene;
}

extern "C" {
    NUGSCN *NuGHGFixup(NUGSCN *scene, void *) {
        return NuReadGraphicsData(NULL, NULL, NULL, reinterpret_cast<char *>(scene));
    }

    void NuGScnReadFromMemory(NUGSCN *scene) {
        NuReadGraphicsData(NULL, NULL, NULL, reinterpret_cast<char *>(scene));
    }

    nuhgobj_s *NuGHGRead(char *path, VARIPTR *buf, VARIPTR buf_end) {
        nuapi.loading_hgobj = 1;
        nuhgobj_s *object = reinterpret_cast<nuhgobj_s *>(NuReadGraphicsData(buf, &buf_end, path, NULL));
        if (object != NULL && nuapi.force_shadows_on_characters != 0) {
            for (i32 i = 0; i != object->display_list->nspecials; ++i) {
                object->display_list->visibility_flags[i] |= 0x20;
            }
        }
        nuapi.loading_hgobj = 0;
        return object;
    }

    NUGSCN *NuGScnReadForMultiRender(VARIPTR *buf, VARIPTR buf_end, char *path, i32 render_count) {
        NUGSCN *scene = NuReadGraphicsData(buf, &buf_end, path, NULL);
        if (scene != NULL && render_count > 1) {
            NUGSCN **additional_scenes = reinterpret_cast<NUGSCN **>(ALIGN(buf->addr, 32));
            buf->addr = reinterpret_cast<usize>(additional_scenes + render_count);
            i32 index = 0;
            for (; index < render_count - 1; ++index) {
                additional_scenes[index] = reinterpret_cast<NUGSCN *>(ALIGN(buf->addr, 32));
                buf->addr = ALIGN(buf->addr, 32) + sizeof(NUGSCN);
                memcpy(additional_scenes[index], scene, sizeof(NUGSCN));
                additional_scenes[index]->display_list = NuDisplaySceneClone(scene->display_list, buf, buf_end);
                additional_scenes[index]->display_list->gscene = additional_scenes[index];
            }
            additional_scenes[index] = NULL;
            scene->additional_scenes = additional_scenes;
        }
        return scene;
    }

    NUGSCN *NuGScnRead(VARIPTR *buf, VARIPTR buf_end, char *path) {
        RemoveDirectionalMaps = 1;
        RemoveNormalMaps = 1;
        NUGSCN *scene = NuReadGraphicsData(buf, &buf_end, path, NULL);
        RemoveNormalMaps = 0;
        RemoveDirectionalMaps = 0;
        return scene;
    }

extern "C" i32 NuTexGenTexture(NUNATIVETEX *tex) {
    for (i32 i = 0; i < max_textures; ++i) {
        if (texture_list[i] == NULL) {
            texture_list[i] = tex;
            return i + 1;
        }
    }
    return 0;
}

}

void NuTexRemap(i32, i32) {
}

void NuChecksumAsHex(u8 *checksum, char *out) {
    i32 i;
    char hex_digits[] = "0123456789abcdef";

    for (i = 0; i < 16; i++) {
        u8 check_digit = checksum[i];
        i32 least_sig_digit = check_digit >> 4;

        out[i * 2] = hex_digits[least_sig_digit];
        out[i * 2 + 1] = hex_digits[(u8)(check_digit - (least_sig_digit << 4))];
    }

    out[32] = '\0';
}

void NuTexHiresFilename(i32 tex_id, char *filename) {
    NUNATIVETEX *tex;
    char checksum_hex[33];

    tex = NuTexGetNative(tex_id);

    NuStrCpy(filename, "c:\\temp\\stream\\textures\\");
    NuChecksumAsHex(tex->checksum, checksum_hex);
    NuStrCat(filename, checksum_hex);
    NuStrCat(filename, ".tex");
}

i32 NuTexSwapHires(i32 tex_id_lo, i32 tex_id_hi) {
    return 0;
}

void NuTexLoadHires(i32 tex_id) {
    char hires_path[2048];
    i32 tex_id_hi;

    NuTexHiresFilename(tex_id, hires_path);
    tex_id_hi = NuTexRead(hires_path, NULL, VARIPTR{});
    NuTexSwapHires(tex_id, tex_id_hi);
}

void NuTexUnloadHires(i32 tex_id) {
}

extern "C" {
extern "C" void NuTexDestroy(i32 tex_id) {
    NuTexUnloadHires(tex_id);
    if (NuTexGetRefCount(tex_id) > 0) {
        NuTexRemoveReference(tex_id, NULL);
        return;
    }
    NuTexDestroyPS(texture_list[tex_id - 1]);
    texture_list[tex_id - 1] = NULL;
}

    i32 NuWindLoad(NUWIND *wind, i32 index, char *name, VARIPTR *buffer, VARIPTR buffer_end) {
        if (wind != NULL) {
            if ((u32)index < 8) {
                if (wind->unk0[index] >= 0) {
                    NuTexDestroy(wind->unk0[index]);
                }
                i32 texture = NuTexRead(name, buffer, buffer_end);
                if (texture != 0) {
                    wind->unk0[index] = texture;
                    return texture;
                }
                wind->unk0[index] = -1;
                return -1;
            }
        }
        return -1;
    }

    void NuWindUnload(NUWIND *wind, i32 index) {
        if (wind != NULL && wind->unk0[index] >= 0) {
            NuTexDestroy(wind->unk0[index]);
            wind->unk0[index] = -1;
        }
    }

extern "C" void NuGScnRemove(nugscn_s *scene) {
    if (scene->texture_anims != nullptr) {
        NuTexAnimRemoveList(scene->texture_anims);
    }

    NuGScnRestoreTIDs(scene);
    for (i32 i = 0; i < scene->ntextures; ++i) {
        if (scene->textures[i]->ref_count >= 0) {
            NuTexDestroy(scene->texture_ids[i]);
        }
    }

    if (scene->additional_scenes != nullptr) {
        for (i32 i = 0; scene->additional_scenes[i] != nullptr; ++i) {
            NuDisplaySceneDestroy(scene->additional_scenes[i]->display_list);
        }
    }
    NuDisplaySceneDestroy(scene->display_list);
    NuGScnDestroyPS(scene);
}

    // Original @0x2fb930. Hierarchies are backed by graphics scenes, so their
    // destruction follows the ordinary scene-removal path.
    void NuHGobjDestroy(nuhgobj_s *object) {
        NuGScnRemove(reinterpret_cast<nugscn_s *>(object));
    }

}

__attribute__((optimize("O2"))) NUQFNT *NuQFntLoadPtr(char *path, char *texture_path, i32, i32, VARIPTR *buf, VARIPTR *buf_end) {
    VUFNT *font = static_cast<VUFNT *>(NuQFntRead(path, buf, *buf_end));
    if (font != NULL)
        return font;

    VARIPTR saved_buffer = *buf;
    char filename[1024];
    buf->addr = ALIGN(buf->addr, 16);
    sprintf(filename, "%s.qfn", path);
    i32 size = NuFileLoadBuffer(filename, buf->void_ptr, buf_end->addr - buf->addr);
    if (size != 0) {
        VUFNT *loaded_font = static_cast<VUFNT *>(buf->void_ptr);
        RemapAddr(loaded_font, NULL, reinterpret_cast<void **>(&loaded_font->glyphs));
        RemapAddr(loaded_font, NULL, reinterpret_cast<void **>(&loaded_font->unicode_map));
        buf->addr = ALIGN(buf->addr + size, 16);
        loaded_font->platform_data = static_cast<VUFNT_ANDROID *>(buf->void_ptr);
        buf->addr += sizeof(VUFNT_ANDROID);
        loaded_font->x_scale = &loaded_font->platform_data->x_scale;
        loaded_font->color_abgr = &loaded_font->platform_data->colour;
        loaded_font->y_scale = &loaded_font->platform_data->y_scale;

        if (texture_path != NULL)
            strcpy(filename, texture_path);
        else
            strcpy(filename, path);
        i32 texture_id = NuTexRead(filename, buf, *buf_end);
        if (texture_id != 0) {
            NUMTL *material = NuMtlCreate(1);
            loaded_font->mtl = material;
            if (material != NULL) {
                material->diffuse_color.r = 1.0f;
                material->diffuse_color.g = 1.0f;
                material->diffuse_color.b = 1.0f;
                material->opacity = 1.0f;
                material->attribs.cull_mode = 2;
                material->attribs.z_mode = 3;
                material->attribs.unknown_2_4 = 1;
                material->attribs.alpha_mode = NUMTL_ALPHA_MODE_ALPHA;
                material->tex_id = texture_id;
                NuMtlUpdate(material);
                return loaded_font;
            }
            NuTexDestroy(texture_id);
        }
    }
    *buf = saved_buffer;
    return font;
}

extern "C" {
    void NuQFntDestroy(VUFNT *font) {
        if (font != NULL) {
            NUMTL *material = font->mtl;
            i32 texture_id = material->tex_id;
            NuMtlDestroy(material);
            if ((font->flags & 1) == 0) {
                NuTexDestroy(texture_id);
            }
        }
    }

}

nutexmanager_s *NuTexGetManager() {
    return g_texman;
}

void NuTexManagerInit(VARIPTR *buf, VARIPTR) {
    g_texman = reinterpret_cast<nutexmanager_s *>(ALIGN(buf->addr, 0x10));
    buf->addr = reinterpret_cast<usize>(g_texman + 1);
}

void MapToGrid(NUVEC *grid, NUVEC *fraction, i32 *column, i32 *row, NUVEC *position, nutexmanager_s *manager) {
    f32 columns = static_cast<f32>(manager->grid_columns);
    f32 rows = static_cast<f32>(manager->grid_rows);
    f32 x = (position->x - manager->grid_centre.x + manager->grid_width * 0.5f) * (columns / manager->grid_width);
    f32 z = (position->z - manager->grid_centre.z + manager->grid_depth * 0.5f) * (rows / manager->grid_depth);
    grid->y = 0.0f;
    grid->x = x;
    grid->z = z;
    if (grid->x < 0.0f) {
        grid->x = 0.0f;
    }
    if (grid->z < 0.0f) {
        grid->z = 0.0f;
    }
    if (grid->x > columns) {
        grid->x = columns - 0.001f;
    }
    if (grid->z > rows) {
        grid->z = rows - 0.001f;
    }
    fraction->x = NuFloor(grid->x);
    fraction->y = 0.0f;
    fraction->z = NuFloor(grid->z);
    *column = static_cast<i32>(fraction->x);
    *row = static_cast<i32>(fraction->z);
    fraction->x = grid->x - fraction->x;
    fraction->z = grid->z - fraction->z;
}

void NuTexManagerStream(nugscn_s *) {
    streamOff = 1;
}

void NuTexCreateFramebufferCopy(void *buffer, void *buffer_end) {
    NuEffectTexLockVP(buffer, buffer_end);
    nuframebuffer_s *framebuffer = NuFramebufferGetObject(1);
    i32 width = NuFramebufferGetWidth(framebuffer);
    i32 height = NuFramebufferGetHeight(framebuffer);
    nueffecttex_s *texture = NuEffectTexCreate2D(width, height, 1, 1, 1);
    NuEffectTexUnlockVP();
    NuEffectTexMapNative(texture);
}

