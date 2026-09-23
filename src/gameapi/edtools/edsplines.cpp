#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"
#include <new>

extern eduiiattr_s EdLevelAttr;
extern ClassEditor theClassEditor;
extern MemoryManager theMemoryManager;
i32 EdTerrRay(VuVec &, VuVec &);

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

void *KnotHelper::GetNextObject(void *current) {
    if (current == NULL) {
        SplineObject *spline = theSplineHelper.first_object;
        if (spline == NULL)
            return NULL;
        do {
            if (spline->knots.count != 0)
                return spline->knots.first;
            spline = spline->next;
        } while (spline != NULL);
        return NULL;
    }

    SplineKnot *knot = static_cast<SplineKnot *>(current);
    SplineKnot *next = knot->next;
    if (next == NULL) {
        SplineObject *spline = knot->spline->next;
        if (spline != NULL) {
            do {
                if (spline->knots.count != 0)
                    return spline->knots.first;
                spline = spline->next;
            } while (spline != NULL);
        }
    }
    return next;
}

i32 KnotHelper::GetNumObjects() {
    i32 count = 0;
    for (SplineObject *spline = theSplineHelper.first_object; spline != NULL; spline = spline->next)
        count += spline->knots.count;
    return count;
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
}

i32 SplineTool::Process(EdInputContext &) {
    return 0;
}

void SplineTool::Render() {
}

void SplineHelper::AddMenuItems(eduimenu_s *menu) {
    eduiMenuAddItem(menu, eduiItemSeparatorCreate(0, &EdLevelAttr));
    eduiMenuAddItem(menu,
                    eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdSplineSmoothKnot, const_cast<char *>("Smooth Knot")));
    eduiMenuAddItem(
        menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdSplineSmoothSpline, const_cast<char *>("Smooth Spline")));
    eduiMenuAddItem(
        menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdSplineReverseSpline, const_cast<char *>("Reverse Spline")));
    eduiMenuAddItem(
        menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdSplineReGenPoints, const_cast<char *>("Re-Gen Points")));
    eduiMenuAddItem(menu, eduiItemCheckCreate(0, &EdLevelAttr, auto_generate_points, 0, cbEdSplineAutoGenPoints,
                                              const_cast<char *>("Auto-Gen Points")));
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

SplineObject *SplineHelper::Find(char *name) {
    for (SplineObject *spline = first_object; spline != NULL; spline = spline->next) {
        if (NuStrICmp(name, spline->name) == 0)
            return spline;
    }
    return NULL;
}

i32 SplineHelper::Find(char *substring, SplineObject **results, i32 capacity) {
    i32 count = 0;
    for (SplineObject *spline = first_object; spline != NULL && count < capacity; spline = spline->next) {
        if (NuStrIStr(spline->name, substring) != NULL)
            results[count++] = spline;
    }
    return count;
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
    for (SplineObject *spline = first_object; spline != NULL; spline = spline->next)
        spline->GenPoints();
}

void SplineHelper::PreLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void SplineHelper::Process(void *object, EdInputContext &input) {
    SplineObject *spline = static_cast<SplineObject *>(object);
    if (input.GetHold(3) != 0.0f)
        return;
    if (spline->points.block_count != 0)
        return;
    spline->GenPoints();
}

void SplineHelper::Render(void *object, i32 flags) {
    static_cast<SplineObject *>(object)->Draw(flags, flags == 0, flags, 0.0f);
}

void SplineHelper::SerialiseObject(EdStream &, void *) {
    STUBBED();
}

void SplineHelper::cbEdSplineAutoGenPoints(eduimenu_s *, eduiitem_s *item, u32) {
    theSplineHelper.auto_generate_points ^= 1;
    item->highlighted = theSplineHelper.auto_generate_points & 1;
}

void SplineHelper::cbEdSplineReGenPoints(eduimenu_s *, eduiitem_s *, u32) {
    for (ClassObjectListEntry *selected = theClassEditor.selected_objects.first; selected != NULL;
         selected = selected->next) {
        if (selected->ed_class == theSplineHelper.object_class)
            static_cast<SplineObject *>(selected->object)->GenPoints();
    }
}

void SplineHelper::cbEdSplineReverseSpline(eduimenu_s *, eduiitem_s *, u32) {
    for (ClassObjectListEntry *selected = theClassEditor.selected_objects.first; selected != NULL;
         selected = selected->next) {
        if (selected->ed_class == theSplineHelper.object_class) {
            SplineObject *spline = static_cast<SplineObject *>(selected->object);
            spline->ReverseKnots();
            if (theSplineHelper.auto_generate_points)
                spline->GenPoints();
        }
    }
}

void SplineHelper::cbEdSplineSmoothKnot(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void SplineHelper::cbEdSplineSmoothSpline(eduimenu_s *, eduiitem_s *, u32) {
    for (ClassObjectListEntry *selected = theClassEditor.selected_objects.first; selected != NULL;
         selected = selected->next) {
        if (selected->ed_class == theSplineHelper.object_class) {
            SplineObject *spline = static_cast<SplineObject *>(selected->object);
            spline->SmoothKnots();
            if (theSplineHelper.auto_generate_points)
                spline->GenPoints();
        }
    }
}

void SplineObject::Clone() {
    STUBBED();
}

void SplineObject::Draw(i32, i32, i32, float) {
    STUBBED();
}

void SplineObject::DropPoint(VuVec &point) {
    if (drop != 0) {
        VuVec start = point;
        point.y -= 10.0f;
        VuVec direction{0.0f, -1000.0f, 0.0f, 1.0f};
        if (EdTerrRay(start, direction) != 0)
            point = start;
    }
}

void SplineObject::GenBezierPoints() {
    STUBBED();
}

void SplineObject::GenLinearPoints() {
    points.Clear();
    for (SplineKnot *knot = knots.first; knot != NULL; knot = knot->next) {
        VuVec point = knot->position;
        DropPoint(point);
        point.y += height;
        points.AddPoint(point);
    }
    if (closed != 0 && knots.first != NULL) {
        VuVec point = knots.first->position;
        DropPoint(point);
        point.y += height;
        points.AddPoint(point);
    }
}

void SplineObject::GenPoints() {
    if (0.1f > step)
        GenLinearPoints();
    else
        GenBezierPoints();
}

void SplineObject::ReverseKnots() {
    STUBBED();
}

void SplineObject::SmoothKnots() {
    for (SplineKnot *knot = knots.first; knot != NULL; knot = knot->next)
        knot->Smooth();
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

void SplinePointList::AddPoint(VuVec &point) {
    SplinePointBlock *block = first;
    while (block != NULL && block->point_count >= block->capacity)
        block = block->next;
    if (block == NULL) {
        block = new (theMemoryManager.AllocPool(sizeof(SplinePointBlock), 1)) SplinePointBlock();
        if (block == NULL)
            return;
        block->points[block->point_count++] = point;
        block->next = NULL;
        block->previous = last;
        if (last != NULL)
            last->next = block;
        last = block;
        if (first == NULL)
            first = block;
        ++block_count;
        return;
    }
    block->points[block->point_count++] = point;
}

void SplinePointList::Clear() {
    SplinePointBlock *block = first;
    while (block != NULL) {
        SplinePointBlock *next = block->next;
        if (block->capacity == 16) {
            if (next == NULL)
                last = block->previous;
            else
                next->previous = block->previous;
            if (block->previous == NULL)
                first = next;
            else
                block->previous->next = next;
            block->next = NULL;
            block->previous = NULL;
            --block_count;
            delete block;
        } else {
            block->point_count = 0;
        }
        block = next;
    }
}

void SplinePointList::Draw() {
    for (SplinePointBlock *block = first; block != NULL; block = block->next)
        block->Draw();
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
    for (i32 index = 0; index < point_count; ++index)
        EdDrawLineCross(points[index], 0.5f, -1);
}

SplinePointBlock::SplinePointBlock() {
    next = NULL;
    previous = NULL;
    capacity = 16;
    points = static_cast<VuVec *>(theMemoryManager.AllocPool(16 * sizeof(VuVec), 1));
}

SplinePointBlock::SplinePointBlock(i32 requested_capacity) {
    next = NULL;
    previous = NULL;
    capacity = requested_capacity;
    points = static_cast<VuVec *>(theMemoryManager.AllocPool(requested_capacity * sizeof(VuVec), 1));
}

SplinePointBlock::~SplinePointBlock() {
    if (points != NULL)
        theMemoryManager.FreePool(points, capacity * sizeof(VuVec));
}

inline void SplinePointBlock::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(SplinePointBlock));
}
