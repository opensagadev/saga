#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/misc/utilities.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nucore/nustring.h"
#include <new>

extern eduiiattr_s EdLevelAttr;
extern ClassEditor theClassEditor;
extern MemoryManager theMemoryManager;
extern LevelEditor theLevelEditor;
extern numtl_s *edLevel3dMtl;
i32 EdTerrRay(VuVec &, VuVec &);
void EdDrawBegin(i32);
void EdDrawEnd();
void EdDrawLineCircleY(VuVec const &, f32, i32, i32);
void DrawBezierLine(VuVec &, VuVec &, VuVec &, VuVec &, numtl_s *, i32);
f32 BezierLineLength(VuVec &, VuVec &, VuVec &, VuVec &);
i32 BezierLinePos(VuVec &, VuVec &, VuVec &, VuVec &, VuVec &, f32);

template <typename Ref>
static Ref *make_spline_reference(EdClass *object_class, char *type, char *name, i32 offset, i32 size, i32 attributes,
                                  EdControl *control, bool add_to_class = true) {
    Ref *reference = new (theMemoryManager.AllocPool(sizeof(Ref), 1)) Ref();
    static_cast<EdRef &>(*reference) = EdRef(type, name, offset, size, attributes, control, 0);
    if (add_to_class)
        object_class->AddType(reference);
    return reference;
}

static EdStringControl *make_spline_string_control() {
    return new (theMemoryManager.AllocPool(sizeof(EdStringControl), 1)) EdStringControl();
}

static EdVectorControl *make_spline_vector_control() {
    return new (theMemoryManager.AllocPool(sizeof(EdVectorControl), 1)) EdVectorControl();
}

static EdEnumControl *make_spline_yes_no_control() {
    EdEnumControl *control = new (theMemoryManager.AllocPool(sizeof(EdEnumControl), 1)) EdEnumControl();
    control->items = EdEnumControl::YesNoItems;
    return control;
}

static EdFloatControl *make_spline_float_control(f32 minimum, f32 maximum) {
    EdFloatControl *control = new (theMemoryManager.AllocPool(sizeof(EdFloatControl), 1)) EdFloatControl();
    control->value_type = EdType_Float;
    control->format = const_cast<char *>("%.2f");
    control->minimum = minimum;
    control->maximum = maximum;
    return control;
}

f32 SplineLength(nugspline_s *spline, i32 closed) {
    if (spline == NULL || spline->length < 2)
        return 0.0f;
    i32 segment_count = closed != 0 ? spline->length : spline->length - 1;
    f32 length = 0.0f;
    NUVEC *previous = spline->pts;
    for (i32 segment = 1; segment <= segment_count; ++segment) {
        NUVEC *next = &spline->pts[segment == spline->length ? 0 : segment];
        NUVEC difference;
        NuVecSub(&difference, next, previous);
        length += NuVecMag(&difference);
        previous = next;
    }
    return length;
}

f32 KnotHelper::DistanceToObject(VuVec &origin, VuVec &direction, void *object, EdRef **reference) {
    SplineKnot *knot = static_cast<SplineKnot *>(object);
    f32 distance = LineToPointDistance(origin, direction, knot->position, NULL);
    EdRef *nearest = position_ref;
    f32 in_distance = LineToPointDistance(origin, direction, knot->in_tangent, NULL);
    if (in_distance < distance) {
        distance = in_distance;
        nearest = in_tangent_ref;
    }
    f32 out_distance = LineToPointDistance(origin, direction, knot->out_tangent, NULL);
    if (out_distance < distance) {
        distance = out_distance;
        nearest = out_tangent_ref;
    }
    distance -= 0.25f * EdManipulator::Scale;
    if (reference != NULL)
        *reference = nearest;
    return distance < 0.0f ? 0.0f : distance;
}

void *KnotHelper::CreateObject(void *, i32, i32) {
    SplineObject *spline = NULL;
    SplineKnot *selected_knot = NULL;
    for (ClassObjectListEntry *selected = theClassEditor.selected_objects.first; selected != NULL;
         selected = selected->next) {
        if (spline == NULL && selected->ed_class == theSplineHelper.object_class)
            spline = static_cast<SplineObject *>(selected->object);
        if (selected_knot == NULL && selected->ed_class == object_class)
            selected_knot = static_cast<SplineKnot *>(selected->object);
        if (spline != NULL && selected_knot != NULL)
            break;
    }
    if (spline == NULL) {
        spline = static_cast<SplineObject *>(theSplineHelper.CreateObject(NULL, 0, 0));
        theClassEditor.MakeUniqueName("NewSpline", spline->name, sizeof(spline->name));
    }
    SplineKnot *knot = static_cast<SplineKnot *>(theMemoryManager.AllocPool(sizeof(SplineKnot), 1));
    knot->spline = spline;
    knot->led_file = Placeable::CurrentLedFile;
    if (selected_knot == NULL) {
        knot->next = NULL;
        knot->previous = spline->knots.last;
        if (spline->knots.last != NULL)
            spline->knots.last->next = knot;
        else
            spline->knots.first = knot;
        spline->knots.last = knot;
    } else {
        knot->next = selected_knot->next;
        knot->previous = selected_knot;
        if (selected_knot->next == NULL)
            spline->knots.last = knot;
        else
            selected_knot->next->previous = knot;
        selected_knot->next = knot;
    }
    ++spline->knots.count;
    knot->Smooth();
    return knot;
}

void KnotHelper::DestroyObject(void *object, i32) {
    theLevelEditor.destroying_objects = 1;
    SplineKnot *knot = static_cast<SplineKnot *>(object);
    SplineObject *spline = knot->spline;
    if (knot->previous != NULL)
        knot->previous->next = knot->next;
    else
        spline->knots.first = knot->next;
    if (knot->next != NULL)
        knot->next->previous = knot->previous;
    else
        spline->knots.last = knot->previous;
    knot->next = NULL;
    knot->previous = NULL;
    --spline->knots.count;
    theMemoryManager.FreePool(knot, sizeof(SplineKnot));
    if (spline->knots.count == 0)
        theSplineHelper.DestroyObject(spline, 0);
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

void KnotHelper::Process(void *object, EdInputContext &) {
    SplineKnot *knot = static_cast<SplineKnot *>(object);
    if (!theClassEditor.IsSelectedObject(knot->spline, NULL)) {
        ClassObject spline_object = {theSplineHelper.object_class, knot->spline, NULL};
        theClassEditor.SelectObject(spline_object, 1);
    }
}

void KnotHelper::Render(void *object, i32) {
    SplineKnot *knot = static_cast<SplineKnot *>(object);
    EdDrawBegin(0);
    EdDrawLineCircleY(knot->position, 0.25f * EdManipulator::Scale, static_cast<i32>(0x80808080), 8);
    EdDrawLineCircleY(knot->in_tangent, 0.25f * EdManipulator::Scale, static_cast<i32>(0x80800000), 8);
    EdDrawLineCircleY(knot->out_tangent, 0.25f * EdManipulator::Scale, static_cast<i32>(0x80000080), 8);
    EdDrawLineSegment(knot->position, knot->in_tangent, -1);
    EdDrawLineSegment(knot->position, knot->out_tangent, -1);
    EdDrawEnd();
}

void SplineKnot::Smooth() {
    VuVec before = previous != NULL ? previous->position : position;
    VuVec after = next != NULL ? next->position : position;
    if (previous == NULL && next == NULL) {
        in_tangent = VuVec{0.0f, 0.0f, 0.0f, 0.0f};
        out_tangent = in_tangent;
        return;
    }
    VuVec delta{(after.x - before.x) * 0.1f, (after.y - before.y) * 0.1f, (after.z - before.z) * 0.1f, 0.0f};
    in_tangent = VuVec{position.x - delta.x, position.y - delta.y, position.z - delta.z, 0.0f};
    out_tangent = VuVec{position.x + delta.x, position.y + delta.y, position.z + delta.z, 0.0f};
    if (spline != NULL) {
        spline->DropPoint(out_tangent);
        spline->DropPoint(in_tangent);
    }
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

void SplineHelper::ClearLevel(i32 level) {
    if (level == -1)
        return;
    for (SplineObject *spline = first_object; spline != NULL;) {
        SplineObject *next = spline->next;
        if (spline->led_file == level)
            DestroyObject(spline, 0);
        spline = next;
    }
}

void *SplineHelper::CreateObject(void *, i32, i32) {
    SplineObject *spline = new (theMemoryManager.AllocPool(sizeof(SplineObject), 1)) SplineObject();
    spline->previous = last_object;
    if (last_object != NULL)
        last_object->next = spline;
    else
        first_object = spline;
    last_object = spline;
    ++object_count;
    return spline;
}

void SplineHelper::DestroyObject(void *object, i32) {
    theLevelEditor.destroying_objects = 1;
    SplineObject *spline = static_cast<SplineObject *>(object);
    for (SplineObject *current = first_object; current != NULL; current = current->next) {
        if (current != spline || current->knots.count != 0)
            continue;
        if (current->previous != NULL)
            current->previous->next = current->next;
        else
            first_object = current->next;
        if (current->next != NULL)
            current->next->previous = current->previous;
        else
            last_object = current->previous;
        current->next = NULL;
        current->previous = NULL;
        --object_count;
        delete current;
        return;
    }
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
    EdClass *spline_class =
        theRegistry.RegisterClass(const_cast<char *>("Spline"), reinterpret_cast<EdClassInterface *>(this), 0x08400100);
    make_spline_reference<EdRef>(spline_class, const_cast<char *>("String"), const_cast<char *>("Spline"), 0x0c, 0x20,
                                 2, make_spline_string_control());
    make_spline_reference<EdRef>(spline_class, const_cast<char *>("Short"), const_cast<char *>("LEDFile"), 0x44, 0,
                                 0x10000100, NULL);
    make_spline_reference<EdRefSpline>(spline_class, const_cast<char *>("Float"), const_cast<char *>("Height"),
                                       static_cast<i32>(0x80000002), 0, 0, make_spline_float_control(-1.0e9f, 1.0e9f));
    make_spline_reference<EdRefSpline>(spline_class, const_cast<char *>("Float"), const_cast<char *>("Step"),
                                       static_cast<i32>(0x80000001), 0, 0, make_spline_float_control(0.0f, 1000.0f));
    make_spline_reference<EdRefSpline>(spline_class, const_cast<char *>("Int"), const_cast<char *>("Drop"),
                                       static_cast<i32>(0x80000003), 0, 0, make_spline_yes_no_control());
    make_spline_reference<EdRefSpline>(spline_class, const_cast<char *>("Int"), const_cast<char *>("Closed"),
                                       static_cast<i32>(0x80000004), 0, 0, make_spline_yes_no_control());

    EdClass *knot_class = theRegistry.RegisterClass(const_cast<char *>("Knot"),
                                                    reinterpret_cast<EdClassInterface *>(&theKnotHelper), 0x18400000);
    EdRef *position =
        make_spline_reference<EdRefKnot>(knot_class, const_cast<char *>("VuVec"), const_cast<char *>("Pos"),
                                         static_cast<i32>(0x80000003), 0, 8, make_spline_vector_control(), false);
    EdRef *in_tangent =
        make_spline_reference<EdRefKnot>(knot_class, const_cast<char *>("VuVec"), const_cast<char *>("In"),
                                         static_cast<i32>(0x80000004), 0, 8, make_spline_vector_control(), false);
    EdRef *out_tangent =
        make_spline_reference<EdRefKnot>(knot_class, const_cast<char *>("VuVec"), const_cast<char *>("Out"),
                                         static_cast<i32>(0x80000005), 0, 8, make_spline_vector_control(), false);
    theKnotHelper.position_ref = position;
    theKnotHelper.in_tangent_ref = in_tangent;
    theKnotHelper.out_tangent_ref = out_tangent;
    make_spline_reference<EdRefKnot>(knot_class, const_cast<char *>("String"), const_cast<char *>("Spline"),
                                     static_cast<i32>(0x80000001), 0x20, 0x10000002, make_spline_string_control());
    make_spline_reference<EdRef>(knot_class, const_cast<char *>("Short"), const_cast<char *>("LEDFile"), 0x44, 0,
                                 0x10000100, NULL);
    knot_class->AddType(position);
    knot_class->AddType(in_tangent);
    knot_class->AddType(out_tangent);
    make_spline_reference<EdRefKnot>(knot_class, const_cast<char *>("Float"), const_cast<char *>("Radius"),
                                     static_cast<i32>(0x80000002), 0, 0x10000040, NULL);
}

void SplineHelper::PostLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    for (SplineObject *spline = first_object; spline != NULL; spline = spline->next)
        spline->GenPoints();
}

void SplineHelper::PreLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
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

void SplineHelper::SerialiseObject(EdStream &stream, void *object) {
    SplineObject *spline = static_cast<SplineObject *>(object);
    i32 knot_count;
    if (stream.mode == 2) {
        knot_count = spline->knots.count;
        stream.SerialiseBuffer(&knot_count, sizeof(knot_count), 1);
        for (SplineKnot *knot = spline->knots.first; knot != NULL; knot = knot->next)
            theKnotHelper.object_class->SerialiseObject(stream, knot);
    }
    if (stream.mode == 1) {
        stream.SerialiseBuffer(&knot_count, sizeof(knot_count), 1);
        for (i32 index = 0; index < knot_count; ++index) {
            SplineKnot *knot = static_cast<SplineKnot *>(theMemoryManager.AllocPool(sizeof(SplineKnot), 1));
            knot->next = NULL;
            knot->previous = NULL;
            knot->led_file = Placeable::CurrentLedFile;
            theKnotHelper.object_class->SerialiseObject(stream, knot);
            knot->spline = spline;
            knot->previous = spline->knots.last;
            if (spline->knots.last != NULL)
                spline->knots.last->next = knot;
            else
                spline->knots.first = knot;
            spline->knots.last = knot;
            ++spline->knots.count;
        }
    }
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
    SplineObject *changed_splines[63];
    i32 changed_count = 0;
    for (ClassObjectListEntry *selected = theClassEditor.selected_objects.first; selected != NULL;
         selected = selected->next) {
        if (selected->ed_class != theKnotHelper.object_class)
            continue;
        SplineKnot *knot = static_cast<SplineKnot *>(selected->object);
        knot->Smooth();
        SplineObject *spline = knot->spline;
        if (!theSplineHelper.auto_generate_points || spline == NULL)
            continue;
        i32 index = 0;
        while (index < changed_count && changed_splines[index] != spline)
            ++index;
        if (index == changed_count)
            changed_splines[changed_count++] = spline;
    }
    for (i32 index = 0; index < changed_count; ++index)
        changed_splines[index]->GenPoints();
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

inline SplineObject::SplineObject()
    : next(NULL), previous(NULL), knots{}, points{}, led_file(Placeable::CurrentLedFile) {
}

SplineObject::~SplineObject() {
    while (points.first != NULL) {
        SplinePointBlock *block = points.first;
        points.first = block->next;
        delete block;
    }
    points.last = NULL;
    points.block_count = 0;
    while (knots.first != NULL) {
        SplineKnot *knot = knots.first;
        knots.first = knot->next;
        theMemoryManager.FreePool(knot, sizeof(SplineKnot));
    }
    knots.last = NULL;
    knots.count = 0;
}

inline void SplineObject::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(SplineObject));
}

SplineObject *SplineObject::Clone() {
    SplineObject *clone = new (theMemoryManager.AllocPool(sizeof(SplineObject), 1)) SplineObject();
    NuStrCpy(clone->name, name);
    for (SplineKnot *source = knots.first; source != NULL; source = source->next) {
        SplineKnot *knot = static_cast<SplineKnot *>(theMemoryManager.AllocPool(sizeof(SplineKnot), 1));
        knot->next = NULL;
        knot->previous = clone->knots.last;
        knot->spline = clone;
        knot->led_file = Placeable::CurrentLedFile;
        if (clone->knots.last != NULL)
            clone->knots.last->next = knot;
        else
            clone->knots.first = knot;
        clone->knots.last = knot;
        ++clone->knots.count;
        knot->position = source->position;
        knot->in_tangent = source->in_tangent;
        knot->out_tangent = source->out_tangent;
    }
    clone->step = step;
    clone->height = height;
    clone->drop = drop;
    clone->closed = closed;
    GenPoints();
    return clone;
}

void SplineObject::Draw(i32, i32 colour_mode, i32 straight, float show_points) {
    EdDrawBegin(0);
    i32 colour = colour_mode == 0 ? static_cast<i32>(0x80808080) : static_cast<i32>(0x80008080);
    for (SplineKnot *knot = knots.first; knot != NULL && knot->next != NULL; knot = knot->next) {
        SplineKnot *next = knot->next;
        if (straight != 0 || step < 0.1f)
            EdDrawLineSegment(knot->position, next->position, colour);
        else
            DrawBezierLine(knot->position, knot->out_tangent, next->position, next->in_tangent, edLevel3dMtl, colour);
    }
    if (show_points != 0.0f && step > 0.1f)
        points.Draw();
    EdDrawEnd();
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
    points.Clear();
    SplineKnot *start = knots.first;
    if (start == NULL)
        return;
    SplineKnot *end = start->next;
    bool closing = false;
    f32 distance = 0.0f;
    while (end != NULL) {
        f32 length = BezierLineLength(start->position, start->out_tangent, end->position, end->in_tangent);
        for (; distance < length; distance += step) {
            VuVec point;
            BezierLinePos(point, start->position, start->out_tangent, end->position, end->in_tangent, distance);
            DropPoint(point);
            point.y += height;
            points.AddPoint(point);
        }
        distance -= length;
        if (closing)
            return;
        start = end;
        end = end->next;
        if (end == NULL && closed != 0) {
            closing = true;
            end = knots.first;
        }
    }
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
    SplineKnot *knot = knots.last;
    knots.first = NULL;
    knots.last = NULL;
    knots.count = 0;
    while (knot != NULL) {
        SplineKnot *prior = knot->previous;
        knot->next = NULL;
        knot->previous = knots.last;
        if (knots.last != NULL)
            knots.last->next = knot;
        else
            knots.first = knot;
        knots.last = knot;
        ++knots.count;
        knot = prior;
    }
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
