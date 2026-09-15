#include "decomp.h"
#include "gameapi_edtools_types.h"

void SplineLength(nugspline_s *, i32) {
    STUBBED();
}

void KnotHelper::DistanceToObject(VuVec &, VuVec &, void *, EdRef **) {
    STUBBED();
}

void KnotHelper::CreateObject(void *, i32, i32) {
    STUBBED();
}

void KnotHelper::DestroyObject(void *, i32) {
    STUBBED();
}

void KnotHelper::GetNextObject(void *) {
    STUBBED();
}

void KnotHelper::GetNumObjects() {
    STUBBED();
}

void KnotHelper::Process(void *, EdInputContext &) {
    STUBBED();
}

void KnotHelper::Render(void *, i32) {
    STUBBED();
}

void SplineKnot::Smooth() {
    STUBBED();
}

void SplineTool::Initialise(variptr_u &, variptr_u &, i32) {
    STUBBED();
}

void SplineTool::Process(EdInputContext &) {
    STUBBED();
}

void SplineTool::Render() {
    STUBBED();
}

void SplineHelper::AddMenuItems(eduimenu_s *) {
    STUBBED();
}

void SplineHelper::ClearLevel(i32) {
    STUBBED();
}

void SplineHelper::CreateObject(void *, i32, i32) {
    STUBBED();
}

void SplineHelper::DestroyObject(void *, i32) {
    STUBBED();
}

void SplineHelper::Find(char *) {
    STUBBED();
}

void SplineHelper::Find(char *, SplineObject **, i32) {
    STUBBED();
}

void *SplineHelper::GetNextObject(void *current) {
    return current != NULL ? static_cast<SplineObject *>(current)->next : first_object;
}

int SplineHelper::GetNumObjects() {
    return object_count;
}

void SplineHelper::Initialise() {
    STUBBED();
}

void SplineHelper::PostLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void SplineHelper::PreLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void SplineHelper::Process(void *, EdInputContext &) {
    STUBBED();
}

void SplineHelper::Render(void *, i32) {
    STUBBED();
}

void SplineHelper::SerialiseObject(EdStream &, void *) {
    STUBBED();
}

void SplineHelper::cbEdSplineAutoGenPoints(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void SplineHelper::cbEdSplineReGenPoints(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void SplineHelper::cbEdSplineReverseSpline(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void SplineHelper::cbEdSplineSmoothKnot(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void SplineHelper::cbEdSplineSmoothSpline(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void SplineObject::Clone() {
    STUBBED();
}

void SplineObject::Draw(i32, i32, i32, float) {
    STUBBED();
}

void SplineObject::DropPoint(VuVec &) {
    STUBBED();
}

void SplineObject::GenBezierPoints() {
    STUBBED();
}

void SplineObject::GenLinearPoints() {
    STUBBED();
}

void SplineObject::GenPoints() {
    STUBBED();
}

void SplineObject::ReverseKnots() {
    STUBBED();
}

void SplineObject::SmoothKnots() {
    STUBBED();
}

i32 SplineKnotList::GetPoint(i32 index, VuVec &point) {
    SplineKnot *knot = first;
    while (knot != NULL && index != 0) {
        knot = knot->next;
        index--;
    }

    if (knot == NULL) {
        return false;
    }

    point = knot->position;
    return true;
}

void SplinePointList::AddPoint(VuVec &) {
    STUBBED();
}

void SplinePointList::Clear() {
    STUBBED();
}

void SplinePointList::Draw() {
    STUBBED();
}

i32 SplinePointList::GetNumPoints() {
    i32 point_count = 0;
    for (SplinePointBlock *block = first; block != NULL; block = block->next) {
        point_count = block->point_count;
    }
    return point_count;
}

i32 SplinePointList::GetPoint(i32 index, VuVec &point) {
    SplinePointBlock *block = first;
    while (block != NULL && index >= block->point_count) {
        block = block->next;
    }

    if (block == NULL) {
        return false;
    }

    point = block->points[index];
    return true;
}

void SplinePointBlock::Draw() {
    STUBBED();
}

SplinePointBlock::SplinePointBlock() {
    STUBBED();
}

SplinePointBlock::SplinePointBlock(i32) {
    STUBBED();
}

SplinePointBlock::~SplinePointBlock() {
}
