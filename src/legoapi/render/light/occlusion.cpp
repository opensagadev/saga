#include "legoapi/legoapi_types.h"
#include "decomp.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuprim.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
struct nuvisiboxtree_s;
struct nuvisiboxtreenode_s;
OcclusionManager g_OcclusionManager;
NUMTL *OccluderSet::ms_pZOnlyMtl3D;
NUMTL *OccluderSet::ms_pZOnlyMtl2D;
NUMTL *OccluderSet::ms_pAlphaMtl2D;

void OccluderSet::AddOccluder(const NUVEC *a, const NUVEC *b, const NUVEC *c, const NUVEC *d) {
    if (count < capacity) {
        queries_prepared = false;
        occluders[count].vertices[0] = NUVEC4{a->x, a->y, a->z, 1.0f};
        occluders[count].vertices[1] = NUVEC4{b->x, b->y, b->z, 1.0f};
        occluders[count].vertices[2] = NUVEC4{c->x, c->y, c->z, 1.0f};
        occluders[count].vertices[3] = NUVEC4{d->x, d->y, d->z, 1.0f};
        ++count;
    }
}

void OccluderSet::Clear() {
    count = 0;
    queries_prepared = false;
}

void OccluderSet::Init(u32 max_occluders, VARIPTR *buffer, VARIPTR buffer_end) {
    capacity = max_occluders;
    count = 0;
    occluders = reinterpret_cast<OccluderRecord *>((buffer->addr + 15) & ~(usize)15);
    indices = reinterpret_cast<u32 *>(occluders + max_occluders);
    buffer->addr = reinterpret_cast<usize>(indices + max_occluders);
    if (ms_pZOnlyMtl3D == NULL) {
        ms_pZOnlyMtl3D = NuMtlCreate3D(1);
        ms_pZOnlyMtl2D = NuMtlCreate(1);
        ms_pAlphaMtl2D = NuMtlCreate(1);
        NUSHADERMTLDESC desc2d;
        memset(&desc2d, 0, sizeof(desc2d));
        desc2d.flags = 0x1000;
        desc2d.vtx_desc.has_position = 1;
        desc2d.vtx_desc.has_no_transform = 1;
        NuMtlSetShaderDescPS(ms_pZOnlyMtl2D, &desc2d);
        ms_pZOnlyMtl2D->attribs.alpha_mode = 13;
        ms_pZOnlyMtl2D->attribs.cull_mode = 2;
        ms_pZOnlyMtl2D->attribs.z_mode = 0;
        ms_pZOnlyMtl2D->attribs.unknown_2_8 = 0;
        NuMtlUpdate(ms_pZOnlyMtl2D);
        NUSHADERMTLDESC desc3d;
        memset(&desc3d, 0, sizeof(desc3d));
        desc3d.flags = 0x1000;
        desc3d.vtx_desc.has_position = 1;
        NuMtlSetShaderDescPS(ms_pZOnlyMtl3D, &desc3d);
        ms_pZOnlyMtl3D->attribs.alpha_mode = 13;
        ms_pZOnlyMtl3D->attribs.cull_mode = 2;
        ms_pZOnlyMtl3D->attribs.z_mode = 0;
        ms_pZOnlyMtl3D->attribs.unknown_2_8 = 0;
        NuMtlUpdate(ms_pZOnlyMtl3D);
        ms_pAlphaMtl2D->attribs.alpha_mode = 1;
        ms_pAlphaMtl2D->attribs.cull_mode = 2;
        ms_pAlphaMtl2D->attribs.z_mode = 1;
        NuMtlUpdate(ms_pAlphaMtl2D);
    }
}

bool OccluderSet::IsOccludedOBB(nuvec_s const *minimum, nuvec_s const *maximum, numtx_s const *matrix) {
    if (!queries_prepared) return false;
    NUVEC4 corners[8] = {
        {minimum->x, maximum->y, minimum->z, 1.0f},
        {minimum->x, maximum->y, maximum->z, 1.0f},
        {maximum->x, maximum->y, maximum->z, 1.0f},
        {maximum->x, maximum->y, minimum->z, 1.0f},
        {minimum->x, minimum->y, minimum->z, 1.0f},
        {minimum->x, minimum->y, maximum->z, 1.0f},
        {maximum->x, minimum->y, maximum->z, 1.0f},
        {maximum->x, minimum->y, minimum->z, 1.0f}
    };
    NUMTX transform;
    if (matrix) NuMtxMulH(&transform, const_cast<NUMTX *>(matrix), &projection_matrix);
    else transform = projection_matrix;
    float min_x = 1000000.0f, min_y = 1000000.0f, min_depth = 1000000.0f;
    float max_x = -1000000.0f, max_y = -1000000.0f;
    for (u32 i = 0; i < 8; ++i) {
        NUVEC4 &v = corners[i];
        NuVec4MtxTransform(&v, reinterpret_cast<NUVEC *>(&v), &transform);
        if (v.w <= 0.0f) return false;
        v.x /= v.w;
        v.y /= v.w;
        v.z /= v.w;
        min_x = v.x < min_x ? v.x : min_x;
        min_y = v.y < min_y ? v.y : min_y;
        max_x = v.x > max_x ? v.x : max_x;
        max_y = v.y > max_y ? v.y : max_y;
        min_depth = v.w < min_depth ? v.w : min_depth;
    }
    i32 limit = static_cast<i32>(count) < 100 ? static_cast<i32>(count) : 100;
    for (i32 i = 0; i != limit; ++i) {
        if (indices[i] == 0xffffffffu) continue;
        OccluderRecord &record = occluders[indices[i]];
        if (record.depth > min_depth - 2.0f) return false;
        if (min_x > record.max_x || min_y > record.max_y ||
            record.min_x > max_x || record.min_y > max_y) continue;
        NUVEC4 *v = record.transformed;
        float winding = (v[2].x - v[0].x) * (v[1].y - v[0].y) -
                        (v[1].x - v[0].x) * (v[2].y - v[0].y);
        bool inside = true;
        for (i32 edge = 0; edge < 4 && inside; ++edge) {
            i32 start = winding > 0.0f ? edge : 3 - edge;
            i32 end = winding > 0.0f ? (edge + 1) & 3 : (6 - edge) & 3;
            NUVEC4 normal = {v[end].y - v[start].y, -(v[end].x - v[start].x), 0.0f, 0.0f};
            NuVecNorm(reinterpret_cast<NUVEC *>(&normal), reinterpret_cast<NUVEC *>(&normal));
            normal.z = normal.w = 0.0f;
            for (u32 corner = 0; corner < 8; ++corner) {
                float distance = (corners[corner].x - v[start].x) * normal.x +
                                 (corners[corner].y - v[start].y) * normal.y + 0.0f;
                if (distance < 0.01f) {
                    inside = false;
                    break;
                }
            }
        }
        if (inside) return true;
    }
    return false;
}

bool OccluderSet::IsOccludedSphere(nuvec_s const *center, float radius) {
    if (!queries_prepared) return false;
    NUVEC4 projected;
    NuVec4MtxTransform(&projected, const_cast<NUVEC *>(center), &projection_matrix);
    if (radius > projected.w) return false;
    projected.x /= projected.w;
    projected.y /= projected.w;
    projected.z /= projected.w;
    float screen_radius = radius / projected.w;
    i32 limit = static_cast<i32>(count) < 100 ? static_cast<i32>(count) : 100;
    for (i32 i = 0; i != limit; ++i) {
        if (indices[i] == 0xffffffffu) continue;
        OccluderRecord &record = occluders[indices[i]];
        if (record.depth > projected.w - radius - 2.0f) return false;
        if (projected.x - screen_radius > record.max_x ||
            projected.y - screen_radius > record.max_y ||
            record.min_x > projected.x + screen_radius ||
            record.min_y > projected.y + screen_radius) continue;
        NUVEC4 *v = record.transformed;
        float winding = (v[1].y - v[0].y) * (v[2].x - v[0].x) -
                        (v[1].x - v[0].x) * (v[2].y - v[0].y);
        bool inside = true;
        for (i32 edge = 0; edge < 4; ++edge) {
            i32 start = winding > 0.0f ? edge : 3 - edge;
            i32 end = winding > 0.0f ? (edge + 1) & 3 : (6 - edge) & 3;
            NUVEC4 normal = {v[end].y - v[start].y, -(v[end].x - v[start].x), 0.0f, 0.0f};
            NuVecNorm(reinterpret_cast<NUVEC *>(&normal), reinterpret_cast<NUVEC *>(&normal));
            normal.z = normal.w = 0.0f;
            float distance = (projected.x - v[start].x) * normal.x +
                             (projected.y - v[start].y) * normal.y + 0.0f - screen_radius;
            if (distance < 0.0f) {
                inside = false;
                break;
            }
        }
        if (inside) return true;
    }
    return false;
}

OccluderSet::OccluderSet() : occluders(NULL), indices(NULL), capacity(0), count(0) {
}

void OccluderSet::OnCameraSet() {
    queries_prepared = false;
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
            record.min_x = v.x < record.min_x ? v.x : record.min_x;
            record.max_x = v.x > record.max_x ? v.x : record.max_x;
            record.min_y = v.y < record.min_y ? v.y : record.min_y;
            record.max_y = v.y > record.max_y ? v.y : record.max_y;
            record.min_depth = v.w < record.min_depth ? v.w : record.min_depth;
            record.depth = v.w > record.depth ? v.w : record.depth;
        }
        if (valid && (record.depth < 0.0f || record.min_depth < 0.0f ||
                      record.max_x < -1.0f || record.min_x > 1.0f ||
                      record.max_y < -1.0f || record.min_y > 1.0f)) {
            valid = false;
        }
        indices[index] = valid ? index : 0xffffffffu;
    }
    qsort(indices, count, sizeof(*indices), SortByDepth);
    queries_prepared = true;
}

void OccluderSet::RenderOccluders(bool depth_only) const {
    const u32 triangle_indices[6] = {0, 1, 2, 2, 3, 0};
    if (queries_prepared) {
        ++NuPrimCSPos;
        NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_NORMALISED);
        NuPrim2DBegin(0, 5, depth_only ? ms_pZOnlyMtl2D : NULL);
        for (u32 i = 0; i < count; ++i) {
            if (indices[i] == 0xffffffffu) continue;
            const OccluderRecord &record = occluders[indices[i]];
            if (record.min_depth < 0.0f || record.depth < 0.0f) continue;
            u32 colour = static_cast<u32>(record.vertices[0].x * record.vertices[0].z) | 0xff000000u;
            for (u32 vertex = 0; vertex < 6; ++vertex) {
                u32 packed = g_NuPrim_NeedsOverbrightening ? colour :
                             ((colour >> 1) & 0x007f7f7fu) | 0xff000000u;
                *reinterpret_cast<u32 *>(g_NuPrim_StreamBufferPtr->addr + 12) = packed;
                const NUVEC4 &v = record.transformed[triangle_indices[vertex]];
                NuPrim2DAddXYZ(v.x, -v.y, v.z);
            }
        }
        NuPrim2DEnd();
        --NuPrimCSPos;
        NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    } else {
        NuPrim3DBegin(0, 5, depth_only ? ms_pZOnlyMtl3D : NULL, NULL);
        for (u32 i = 0; i < count; ++i) {
            const OccluderRecord &record = occluders[i];
            u32 colour = static_cast<u32>(record.vertices[0].x * record.vertices[0].z) | 0xff000000u;
            for (u32 vertex = 0; vertex < 6; ++vertex) {
                u32 packed = g_NuPrim_NeedsOverbrightening ? colour :
                             ((colour >> 1) & 0x007f7f7fu) | 0xff000000u;
                *reinterpret_cast<u32 *>(g_NuPrim_StreamBufferPtr->addr + 12) = packed;
                const NUVEC4 &v = record.vertices[triangle_indices[vertex]];
                *reinterpret_cast<NUVEC *>(g_NuPrim_StreamBufferPtr->addr) = NUVEC{v.x, v.y, v.z};
                g_NuPrim_StreamBufferPtr->addr += 24;
            }
        }
        g_NuPrim_VertexCount += count * 6;
        NuPrim3DEnd();
    }
}

i32 OccluderSet::SortByDepth(void const *left, void const *right) {
    i32 a = *static_cast<const i32 *>(left);
    i32 b = *static_cast<const i32 *>(right);
    if (a == -1) return 1;
    if (b == -1) return -1;
    OccluderRecord *records = g_OcclusionManager.current_set->occluders;
    return records[b].depth > records[a].depth ? -1 : 1;
}

OccluderSet::~OccluderSet() {
}

void OcclusionManager::AddOccluder(nuvec_s const *center, float radius) {
    if (!initialized || !enabled) return;
    NUMTX *view = NuCameraGetViewMtx();
    NUVEC right = {view->m00 * radius, view->m10 * radius, view->m20 * radius};
    NUVEC up = {view->m01 * radius, view->m11 * radius, view->m21 * radius};
    NUVEC forward = {view->m02 * radius, view->m12 * radius, radius * view->m22};
    NUVEC4 a = {((center->x + up.x) - right.x) - forward.x,
                ((center->y + up.y) - right.y) - forward.y,
                ((center->z + up.z) - right.z) - forward.z, 1.0f};
    NUVEC4 b = {((center->x + up.x) + right.x) + forward.x,
                ((center->y + up.y) + right.y) + forward.y,
                ((center->z + up.z) + right.z) + forward.z, 1.0f};
    NUVEC4 c = {(right.x + (center->x - up.x)) + forward.x,
                (right.y + (center->y - up.y)) + forward.y,
                (right.z + (center->z - up.z)) + forward.z, 1.0f};
    NUVEC4 d = {((center->x - up.x) - right.x) - forward.x,
                ((center->y - up.y) - right.y) - forward.y,
                ((center->z - up.z) - right.z) - forward.z, 1.0f};
    AddOccluder(reinterpret_cast<NUVEC *>(&a), reinterpret_cast<NUVEC *>(&b),
                reinterpret_cast<NUVEC *>(&c), reinterpret_cast<NUVEC *>(&d));
}

void OcclusionManager::AddOccluder(nuvec_s const *minimum, nuvec_s const *maximum, numtx_s const *matrix) {
    if (!initialized || !enabled) return;
    NUVEC4 corners[8] = {
        {minimum->x, maximum->y, minimum->z, 1.0f},
        {maximum->x, maximum->y, maximum->z, 1.0f},
        {maximum->x, minimum->y, maximum->z, 1.0f},
        {minimum->x, minimum->y, minimum->z, 1.0f},
        {minimum->x, maximum->y, maximum->z, 1.0f},
        {maximum->x, maximum->y, minimum->z, 1.0f},
        {maximum->x, minimum->y, minimum->z, 1.0f},
        {minimum->x, minimum->y, maximum->z, 1.0f}
    };
    for (u32 i = 0; i < 8; ++i) {
        NUVEC *vertex = reinterpret_cast<NUVEC *>(&corners[i]);
        NuVecMtxTransform(vertex, vertex, const_cast<NUMTX *>(matrix));
    }
    AddOccluder(reinterpret_cast<NUVEC *>(&corners[0]), reinterpret_cast<NUVEC *>(&corners[1]),
                reinterpret_cast<NUVEC *>(&corners[2]), reinterpret_cast<NUVEC *>(&corners[3]));
    AddOccluder(reinterpret_cast<NUVEC *>(&corners[4]), reinterpret_cast<NUVEC *>(&corners[5]),
                reinterpret_cast<NUVEC *>(&corners[6]), reinterpret_cast<NUVEC *>(&corners[7]));
}

void OcclusionManager::AddOccluder(nuvec_s const *a, nuvec_s const *b, nuvec_s const *c, nuvec_s const *d) {
    if (!initialized || !enabled || building_set->count >= building_set->capacity) return;
    if (unknown_158 > 0.0f || unknown_15c > 0.0f) {
        NUVEC4 projected[4];
        NuVec4MtxTransform(&projected[0], const_cast<NUVEC *>(a), NuCameraGetVPMtx());
        NuVec4MtxTransform(&projected[1], const_cast<NUVEC *>(b), NuCameraGetVPMtx());
        NuVec4MtxTransform(&projected[2], const_cast<NUVEC *>(c), NuCameraGetVPMtx());
        if (unknown_158 > 0.0f) {
            NUVEC ab = {projected[1].x - projected[0].x, projected[1].y - projected[0].y,
                        projected[1].z - projected[0].z};
            NUVEC ac = {projected[2].x - projected[0].x, projected[2].y - projected[0].y,
                        projected[2].z - projected[0].z};
            NUVEC normal = {ac.z * ab.y - ac.y * ab.z, ab.z * ac.x - ac.z * ab.x,
                            ab.x * ac.y - ab.y * ac.x};
            NuVecNorm(&normal, &normal);
            if (unknown_158 > fabsf(normal.z)) return;
        }
        NuVec4MtxTransform(&projected[3], const_cast<NUVEC *>(d), NuCameraGetVPMtx());
        float min_x = 1000000.0f, min_y = 1000000.0f;
        float max_x = -1000000.0f, max_y = -1000000.0f;
        for (u32 i = 0; i < 4; ++i) {
            NUVEC4 &v = projected[i];
            if (v.w < 0.001f) return;
            v.x /= v.w;
            v.y /= v.w;
            v.z /= v.w;
            min_x = v.x < min_x ? v.x : min_x;
            min_y = v.y < min_y ? v.y : min_y;
            max_x = v.x > max_x ? v.x : max_x;
            max_y = v.y > max_y ? v.y : max_y;
        }
        if (unknown_15c > 0.0f && unknown_15c > (max_x - min_x) * (max_y - min_y) * 0.25f) return;
    }
    building_set->AddOccluder(a, b, c, d);
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

void OcclusionManager::EndFrame() {
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

bool OcclusionManager::IsOccludedOBB(nuvec_s const *minimum, nuvec_s const *maximum, numtx_s const *matrix) {
    if (!initialized || !enabled) return false;
    if (!current_set->queries_prepared) {
        NUMTX *projection = NuCameraGetVPMtx();
        NUMTX *view = NuCameraGetViewMtx();
        current_set->PrepareForQueries(view, projection);
    }
    ++unknown_164;
    bool occluded = current_set->IsOccludedOBB(minimum, maximum, matrix);
    if (occluded) ++unknown_160;
    return occluded;
}

bool OcclusionManager::IsOccludedSphere(nuvec_s const *center, float radius) {
    if (!initialized || !enabled) return false;
    if (!current_set->queries_prepared) {
        NUMTX *projection = NuCameraGetVPMtx();
        NUMTX *view = NuCameraGetViewMtx();
        current_set->PrepareForQueries(view, projection);
    }
    ++unknown_164;
    bool occluded = current_set->IsOccludedSphere(center, radius);
    if (occluded) ++unknown_160;
    return occluded;
}

OcclusionManager::OcclusionManager() : initialized(false), enabled(true), building_set(NULL), current_set(NULL),
    unknown_158(0.3f), unknown_15c(-1.0f) {
}

void OcclusionManager::OnCameraSet() {
    if (initialized && enabled) current_set->OnCameraSet();
}

void OcclusionManager::RenderStats() const {
}

void OcclusionManager::RenderZPass() const {
    if (initialized && enabled) current_set->RenderOccluders(true);
}

void OcclusionManager::SetEnabled(bool value) {
    enabled = value;
}

OcclusionManager::~OcclusionManager() {
}

static __used__ void BoxTreeRndrRec(nuvisiboxtree_s *, unsigned char *, nuvisiboxtreenode_s *, int, float, nugscn_s *) {
}
