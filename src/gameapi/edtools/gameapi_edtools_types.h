#ifndef GAMEAPI_EDTOOLS_TYPES_H
#define GAMEAPI_EDTOOLS_TYPES_H
#pragma once

#include "decomp.h"
#include "nu2api/nucore/fixed_width.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/nuvec.h"
#include "gameapi/edtools/EdObjectNotifier.h"
#include <string.h>

struct ClassObjectList;
struct EdBitControl;
struct EdClass;
struct EdClassInterface;
struct EdClassObjectNameControl;
struct EdColourControl;
struct EdControl;
struct EdDefunctList;
struct EdEnumControl;
struct EdFileInputStream;
struct EdFileOutputStream;
struct EdInputContext;
struct EdInputStream;
struct EdManMove;
struct EdManRotate;
struct EdManScale;
struct EdManipulator;
struct EdMatrixControl;
struct EdMember;
struct EdObjectNotifier;
struct EdOutputStream;
struct EdRef;
struct EdRefKnot;
struct EdRefPlaceable;
struct EdRefSpecialObject;
struct EdRefSpline;
struct EdRegistry;
struct EdSfxNameControl;
struct EdSpecialObjectControl;
struct EdStream;
struct EdString;
struct EdStringControl;
struct EdSubSystem;
struct EdSystem;
struct EdType;
struct EdVectorControl;
struct EditorSettings;
struct KnotHelper;
struct MemoryBuffer;
struct SplineHelper;
struct SplineKnot;
struct SplineKnotList;
struct SplineObject;
struct SplinePointBlock;
struct SplinePointList;
struct SplineTool;
struct VuMtx;
struct VuVec;
struct burnset_s;
struct eduiiattr_s;
struct eduiitem_s;
struct eduimenu_s;
struct nucamera_s;
struct nugscn_s;
struct nugspline_s;
struct nupad_s;
struct nuvec_s;
struct part_typedesc_s;
union variptr_u;

struct ClassObjectList;
struct EdMember {
    void *object;
    EdRef *reference;
};
struct EdSubSystem {
    virtual ~EdSubSystem();
    virtual void SubInitialise(variptr_u &, variptr_u &, i32);
    virtual void SubReset();
    virtual void SubProcess(float);
    virtual void SubRender();

    EdSubSystem *next;
    EdSubSystem *previous;
};
struct MemoryBuffer;
struct VuMtx;
struct VuVec;
struct burnout_s {
    i32 active;
    NUVEC position;
    float field_10, field_14, field_18, field_1c, field_20;
};
struct burn_parameters_s {
    i32 field_00;
    float field_04, field_08, field_0c, field_10, field_14;
    i32 field_18;
    float field_1c, field_20, field_24;
    i32 field_28;
    float field_2c, field_30, field_34, field_38, field_3c;
    float field_40, field_44, field_48, field_4c, field_50;
};
struct burnset_s {
    burn_parameters_s parameters;
    burn_parameters_s parameters_copy;
    i32 field_a8, field_ac, field_b0, field_b4;
    float field_b8, field_bc, field_c0, field_c4, field_c8, field_cc;
    burnout_s burnouts[32];
    i32 active_count;
    i32 selected_index;
    float field_558, field_55c;
    i32 field_560;
};
struct eduiiattr_s;
struct eduiitem_s;
struct eduimenu_s;
struct nucamera_s;
struct nugscn_s;
struct nugspline_s;
struct nupad_s;
struct nuvec_s;
union variptr_u;

struct edanim_param_s {
    i32 instance_id;
    i32 effect_count;
    i32 sound_count;
    i32 field_00c;
    i32 field_010;
    float field_014;
    float field_018;
    char effect_names[8][16];
    i32 effect_ids[8];
    i32 effect_intervals[8];
    i32 effect_flags[8];
    float effect_positions[8][3];
    i16 effect_angles[8];
    i16 effect_angle_ranges[8];
    float field_17c;
    char sound_names[8][0x10];
    i32 sound_ids[8];
    i32 sound_flags[8];
    float sound_values[8];
    float sound_positions[8][3];
    i32 platform_id;
    float bounce_impulse;
    float bounce_spring;
    float bounce_damping;
    i8 page;
    u8 reserved_2d1[3];
};
static_assert(sizeof(edanim_param_s) == 0x2d4, "edanim_param_s size");

struct EdClass {
    char *name;
    i32 flags;
    EdRef *members;
    EdRef *last_member;
    i32 member_count;
    EdClassInterface *interface;

    void AddType(EdRef *);
    void CopyObject(void *, void *);
    i32 FindMember(EdMember *, void *, i32, i32);
    void *FindObject(char *);
    EdRef *FindTypeRef(char *, i32);
    EdRef *FindTypeRef(i32, i32);
    i32 GetStreamClasses(EdStream &, i32 *, i32 &, i32);
    void Serialise(EdStream &, i32 *);
    void SerialiseObject(EdStream &, void *);
    void SerialiseObject(EdStream &, void *, EdClass *, EdRegistry *);
    i32 SerialiseObjectHeader(EdStream &, void *);
};
struct EdClassInterfaceVTable {
    void (*destroy)(EdClassInterface *);
    void (*delete_object)(EdClassInterface *);
    void (*clear_level)(EdClassInterface *, i32);
    void (*flush)(EdClassInterface *);
    i32 (*get_num_objects)(EdClassInterface *);
    void *(*get_next_object)(EdClassInterface *, void *);
    void *(*get_next_filtered_object)(EdClassInterface *, void *, i32 (*)(void *));
    void *(*create_object)(EdClassInterface *, void *, i32, i32);
    void (*destroy_object)(EdClassInterface *, void *, i32);
    void (*defunct_object)(EdClassInterface *, void *);
    void (*revive_object)(EdClassInterface *, void *);
    void (*set_object_guid)(EdClassInterface *, void *, i32);
    i32 (*get_object_guid)(EdClassInterface *, void *);
    i32 (*get_constructor_data)(EdClassInterface *, void *, void *, i32);
    void (*construct)(EdClassInterface *, void *, void *);
    void (*process)(EdClassInterface *, void *, EdInputContext &);
    void (*render)(EdClassInterface *, void *, i32);
    void (*enter_editor)(EdClassInterface *);
    void (*exit_editor)(EdClassInterface *);
    void (*enter_level)(EdClassInterface *);
    void (*exit_level)(EdClassInterface *);
    void (*update_lists)(EdClassInterface *, MemoryBuffer *, MemoryBuffer *);
    void (*pre_load_initialisation)(EdClassInterface *, MemoryBuffer *, MemoryBuffer *);
    void (*post_load_initialisation)(EdClassInterface *, MemoryBuffer *, MemoryBuffer *);
    void (*pre_save_initialisation)(EdClassInterface *);
    void (*post_save_initialisation)(EdClassInterface *);
    void (*serialise_object)(EdClassInterface *, EdStream &, void *);
    f32 (*distance_to_ray)(EdClassInterface *, VuVec &, VuVec &, void *, EdRef **);
    f32 (*distance_to_point)(EdClassInterface *, VuVec &, void *, EdRef **);
    void (*add_menu_items)(EdClassInterface *, eduimenu_s *);
    void (*import)(EdClassInterface *);
};
DECOMP_ASSERT(offsetof(EdClassInterfaceVTable, get_next_object) == 0x14, "EdClassInterface next-object slot");
DECOMP_ASSERT(offsetof(EdClassInterfaceVTable, serialise_object) == 0x68, "EdClassInterface serialise slot");
struct EdClassInterface {
    EdClassInterfaceVTable *vtable;
    EdClass *object_class;

    void ClearLevel(i32);
    void DefunctObject(void *);
    void ReviveObject(void *);
    void SetObjectGuid(void *, i32);
    i32 GetObjectGuid(void *);
    i32 GetConstructorData(void *, void *, i32);
    void Construct(void *, void *);
    void Process(void *, EdInputContext &);
    void Render(void *, i32);
    void EnterEditor();
    void ExitEditor();
    void EnterLevel();
    void ExitLevel();
    void UpdateLists(MemoryBuffer *, MemoryBuffer *);
    void PreLoadInitialisation(MemoryBuffer *, MemoryBuffer *);
    void PostLoadInitialisation(MemoryBuffer *, MemoryBuffer *);
    void PreSaveInitialisation();
    void PostSaveInitialisation();
    void SerialiseObject(EdStream &, void *);
    void AddMenuItems(eduimenu_s *);
    void Import();
    f32 DistanceToObject(VuVec &, VuVec &, void *, EdRef **);
    f32 DistanceToObject(VuVec &, void *, EdRef **);
    void *GetNextObject(void *, i32 (*)(void *));
};
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdClassInterface) == 0x8, "EdClassInterface size");
struct EdControl {
    static EdInputContext *Input;
    virtual ~EdControl();
    static void operator delete(void *);
    virtual void Refresh();
    virtual void Process(EdInputContext &);
    virtual void Render();
    virtual void AddMenuItem(eduimenu_s *, EdRef *, void *);
    virtual void SetMenuItemAttr(i32, eduiitem_s *, eduiiattr_s *, eduiiattr_s *);

    eduiitem_s *item;
    EdRef *reference;
    void *object;

    i32 SelectSubObject();
    static void cbSelected(eduimenu_s *, eduiitem_s *, u32);
};
struct EdColourControl : EdControl {
    EdColourControl();
    void AddMenuItem(eduimenu_s *, EdRef *, void *) override;
    void Refresh() override;
    static void cbButton(eduimenu_s *, eduiitem_s *, u32);
    static void cbChanged(eduimenu_s *, eduiitem_s *, u32);
    static void cbColourSelected(eduimenu_s *, eduiitem_s *, u32);
};
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdColourControl) == 0x10, "EdColourControl 32-bit size");
template <typename T> struct EdValueControl : EdControl {
    i32 value_type;
    char *format;
    T minimum;
    T maximum;

    ~EdValueControl() override;
    static void operator delete(void *);
    void AddMenuItem(eduimenu_s *, EdRef *, void *) override;
    void Refresh() override;
    static void cbButton(eduimenu_s *, eduiitem_s *, u32);
    static void cbChanged(eduimenu_s *, eduiitem_s *, u32);
    static f32 MouseScale;
};
struct EdFloatControl : EdValueControl<f32> {
    ~EdFloatControl() override;
    static void operator delete(void *);
};
template <> void EdValueControl<f32>::AddMenuItem(eduimenu_s *, EdRef *, void *);
template <> EdValueControl<f32>::~EdValueControl();
template <> void EdValueControl<f32>::operator delete(void *);
template <> void EdValueControl<f32>::Refresh();
template <> void EdValueControl<f32>::cbButton(eduimenu_s *, eduiitem_s *, u32);
template <> void EdValueControl<f32>::cbChanged(eduimenu_s *, eduiitem_s *, u32);
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdFloatControl) == 0x20, "EdFloatControl 32-bit size");
struct EdDefunctListEntry {
    EdDefunctListEntry *next;
    EdDefunctListEntry *previous;
    EdClass *object_class;
    void *object;

    EdDefunctListEntry() : next(nullptr), previous(nullptr) {
    }
};
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdControl) == 0x10, "EdControl 32-bit size");
struct EdDefunctList {
    EdDefunctListEntry *first;
    EdDefunctListEntry *last;
    i32 count;

    void ReviveAll(i32);
};
struct EdEnumControl : EdControl {
    struct Item {
        char *name;
        i32 value;
    };
    Item *items;

    void AddMenuItem(eduimenu_s *, EdRef *, void *) override;
    char *GetEnumString(i32);
    i32 GetEnumValue(char *);
    void Refresh() override;
    static void cbButton(eduimenu_s *, eduiitem_s *, u32);
    static void cbChanged(eduimenu_s *, eduiitem_s *, u32);
    static void cbSelectItem(eduimenu_s *, eduiitem_s *, u32);
    static Item OpenClosedItems[];
    static Item OnOffItems[];
    static Item YesNoItems[];
};
struct EdBitControl : EdEnumControl {
    u32 bit_mask;

    void AddMenuItem(eduimenu_s *, EdRef *, void *) override;
    void Refresh() override;
    static void cbButton(eduimenu_s *, eduiitem_s *, u32);
    static void cbChanged(eduimenu_s *, eduiitem_s *, u32);
    static void cbSelectItem(eduimenu_s *, eduiitem_s *, u32);
};
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdBitControl) == 0x18, "EdBitControl 32-bit size");
struct MemoryBuffer {
    variptr_u *position;
    variptr_u *end;
    u32 used;
    u32 remaining;

    void *Allocate(usize size) {
        if (size >= end->addr - position->addr) {
            return NULL;
        }
        char *allocation = (char *)ALIGN(position->addr, 16);
        position->char_ptr = allocation + size;
        memset(allocation, 0, size);
        used += size;
        remaining -= size;
        return allocation;
    }
};
struct EdStream {
    virtual ~EdStream() {
    }
    virtual i32 Eat(i32, i32) = 0;
    virtual i32 SerialiseBuffer(void *, i32, i32) = 0;
    virtual i32 SerialiseString(char *, i32) = 0;
    virtual i32 SerialiseString(char **) = 0;
    virtual i32 SerialiseString(char **, i32) = 0;
    virtual char const *BeginBlock(char const *) = 0;
    virtual void EndBlock() = 0;

    i32 version;
    i32 mode;
    i32 swap_endianness;
    i32 unknown_10;
    MemoryBuffer *memory_buffer;
    MemoryBuffer *secondary_buffer;
    i32 flags;

    EdStream();
    EdStream(MemoryBuffer *);
    EdStream(MemoryBuffer *, MemoryBuffer *);
};
struct EdInputStream : EdStream {
    EdInputStream() : EdStream() {
    }
    EdInputStream(MemoryBuffer *buffer, MemoryBuffer *secondary) : EdStream(buffer, secondary) {
    }
    virtual ~EdInputStream() {
    }
    virtual i32 SerialiseString(char **);
    virtual i32 SerialiseString(char **, i32);
    virtual i32 SerialiseString(char *, i32);
};
struct EdOutputStream : EdStream {
    virtual ~EdOutputStream() {
    }
    virtual i32 SerialiseString(char **);
    virtual i32 SerialiseString(char **, i32);
    virtual i32 SerialiseString(char *, i32);
};
struct EdFileInputStream : EdInputStream {
    EdFileInputStream() : EdInputStream() {
        mode = 1;
        block_count = 0;
        name_length = 0;
        pending = 0;
        file = 0;
    }
    EdFileInputStream(MemoryBuffer *buffer, MemoryBuffer *secondary) : EdInputStream(buffer, secondary) {
        mode = 1;
        block_count = 0;
        name_length = 0;
        pending = 0;
        file = 0;
    }
    struct Block {
        i32 position;
        i32 size;
        i32 name_offset;
    };
    Block blocks[8];
    i32 block_count;
    char block_names[256];
    i32 name_length;
    Block pending_block;
    i32 pending;
    i32 file;

    virtual ~EdFileInputStream() {
    }
    virtual char const *BeginBlock(char const *);
    virtual i32 Eat(i32, i32);
    virtual void EndBlock();
    void Open(i32, i32);
    virtual i32 SerialiseBuffer(void *, i32, i32);
};
struct EdFileOutputStream : EdOutputStream {
    i32 block_positions[8];
    i32 block_count;
    i32 file;

    EdFileOutputStream() {
        mode = 2;
        block_count = 0;
        file = 0;
    }

    virtual ~EdFileOutputStream() {
    }
    virtual char const *BeginBlock(char const *);
    virtual i32 Eat(i32, i32);
    virtual void EndBlock();
    void Open(i32, i32);
    virtual i32 SerialiseBuffer(void *, i32, i32);
};
DECOMP_ASSERT(sizeof(MemoryBuffer) == 0x10, "MemoryBuffer size");
DECOMP_ASSERT(sizeof(EdStream) == 0x20, "EdStream size");
DECOMP_ASSERT(offsetof(EdStream, version) == 0x04, "EdStream version offset");
DECOMP_ASSERT(offsetof(EdStream, mode) == 0x08, "EdStream mode offset");
DECOMP_ASSERT(offsetof(EdStream, swap_endianness) == 0x0c, "EdStream endian offset");
DECOMP_ASSERT(offsetof(EdStream, memory_buffer) == 0x14, "EdStream memory buffer offset");
DECOMP_ASSERT(offsetof(EdStream, secondary_buffer) == 0x18, "EdStream secondary buffer offset");
DECOMP_ASSERT(offsetof(EdStream, flags) == 0x1c, "EdStream flags offset");
DECOMP_ASSERT(sizeof(EdFileInputStream::Block) == 0x0c, "EdFileInputStream block size");
DECOMP_ASSERT(offsetof(EdFileInputStream, block_count) == 0x80, "EdFileInputStream block count offset");
DECOMP_ASSERT(offsetof(EdFileInputStream, name_length) == 0x184, "EdFileInputStream name length offset");
DECOMP_ASSERT(offsetof(EdFileInputStream, pending_block) == 0x188, "EdFileInputStream pending block offset");
DECOMP_ASSERT(offsetof(EdFileInputStream, file) == 0x198, "EdFileInputStream file offset");
DECOMP_ASSERT(sizeof(EdFileInputStream) == 0x19c, "EdFileInputStream size");
DECOMP_ASSERT(offsetof(EdFileOutputStream, block_count) == 0x40, "EdFileOutputStream block count offset");
DECOMP_ASSERT(offsetof(EdFileOutputStream, file) == 0x44, "EdFileOutputStream file offset");
DECOMP_ASSERT(sizeof(EdFileOutputStream) == 0x48, "EdFileOutputStream size");
struct EdInputContext {
    u8 reserved_00[0x40];
    nupad_s *pad;
    f32 delta_time;
    f32 current_time;
    f32 repeat_window;
    u8 held[40];
    u8 pressed[40];
    u8 released[40];
    u8 repeated[40];
    u8 cleared[40];
    f32 values[40];
    f32 repeat_times[40];

    void Clear(i32);
    EdInputContext();
    f32 Get(i32);
    f32 GetHold(i32);
    f32 GetPress(i32);
    f32 GetRelease(i32);
    f32 GetRepeat(i32);
    void Set(i32, float, float);
    void Update(nucamera_s *, nupad_s *, float, bool);
};
struct EdManipulator {
    i32 selected_attribute;
    u8 reserved_0x08[0x64];
    static f32 Scale;
    static i32 AxisColour[8];

    void DrawAxis(VuVec &, VuMtx *);
    void DrawRotator(VuVec &);
    void GetAxisLocators(VuVec &, VuVec *, VuMtx *);
    virtual i32 Process(EdInputContext &, ClassObjectList &);
    virtual void Render(ClassObjectList &);
    i32 SelectAxis(EdInputContext &, VuVec &, VuVec &, VuVec &, VuMtx *);
    i32 SelectRotator(EdInputContext &, VuVec &, VuVec &);
};
struct EdManMove : EdManipulator {
    EdManMove();
    i32 Process(EdInputContext &, ClassObjectList &) override;
    void Render(ClassObjectList &) override;
};
struct EdManRotate : EdManipulator {
    EdManRotate();
    i32 Process(EdInputContext &, ClassObjectList &) override;
    void Render(ClassObjectList &) override;
    i32 RotateItem(EdInputContext &, ClassObjectList &, i32, i32);
};
struct EdManScale : EdManipulator {
    EdManScale();
    i32 Process(EdInputContext &, ClassObjectList &) override;
    void Render(ClassObjectList &) override;
};
struct EdMatrixControl : EdControl {
    eduiitem_s *components[9];
    void AddMenuItem(eduimenu_s *, EdRef *, void *) override;
    void Destroy();
    EdMatrixControl();
    ~EdMatrixControl() override;
    static void operator delete(void *);
    void Refresh() override;
    void SetMenuItemAttr(i32, eduiitem_s *, eduiiattr_s *, eduiiattr_s *) override;
    static void cbButton(eduimenu_s *, eduiitem_s *, u32);
    static void cbChanged(eduimenu_s *, eduiitem_s *, u32);
    static void cbSelected(eduimenu_s *, eduiitem_s *, u32);
};
struct EdRef {
    virtual void *GetMemberObject(void *);
    virtual void GetMemberData(void *, i32, void *, i32);
    virtual void SetMemberData(void *, i32, void *, i32, i16 *);

    EdRef *next;
    EdRef *previous;
    i32 type_id;
    char *name;
    i32 member_offset;
    i32 size;
    i32 attributes;
    EdControl *control;
    i32 replication_group;

    void CheckType(i32);
    EdRef() : next(NULL), previous(NULL) {
    }
    EdRef(char *, char *, i32, i32, i32, EdControl *, i32);
    i32 GetAttributeData(void *, i32, i32, void *, i32);
    i32 GetTypeSize(i32, i32);
    void Serialise(EdStream &, i32 *);
    i32 SetAttributeData(void *, i32, i32, void *, i32);
};
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdRef) == 0x28, "EdRef 32-bit size");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(EdRef, type_id) == 0xc, "EdRef::type_id 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(EdRef, member_offset) == 0x14, "EdRef::member_offset 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(EdRef, attributes) == 0x1c, "EdRef::attributes 32-bit offset");
struct EdRefKnot : EdRef {
    void GetMemberData(void *, i32, void *, i32);
    void SetMemberData(void *, i32, void *, i32, i16 *);
};
struct EdRefPlaceable : EdRef {
    void GetMemberData(void *, i32, void *, i32);
    void SetMemberData(void *, i32, void *, i32, i16 *);
};
struct EdRefSpecialObject : EdRef {
    void GetMemberData(void *, i32, void *, i32);
    void SetMemberData(void *, i32, void *, i32, i16 *);
};
struct EdRefSpline : EdRef {
    void GetMemberData(void *, i32, void *, i32);
    void SetMemberData(void *, i32, void *, i32, i16 *);
};
struct EdRegistry {
    struct NameMapping {
        char *source;
        char *destination;
    };

    i32 initialised;
    EdType *types;
    EdClass *classes;
    NameMapping *mappings;
    EdObjectNotifier **notifiers;
    i32 (*create_object_guid)();
    i32 type_capacity;
    i32 type_count;
    i32 class_capacity;
    i32 class_count;
    i32 mapping_capacity;
    i32 object_count;
    i32 notifier_capacity;
    i32 notifier_count;
    EdDefunctList defunct_objects;

    i32 AddMapping(char *, char *);
    void AddObjectNotifier(EdObjectNotifier *);
    void ClassIFaceProcess(EdClass *, void *, EdInputContext &);
    void ClassIFaceProcess(i32, void *, EdInputContext &);
    void ClassIFaceRender(EdClass *, void *, i32);
    void ClassIFaceRender(i32, void *, i32);
    void *CreateObject(EdClassInterface *, void *, i32, i32, i32);
    void DefunctObject(EdClassInterface *, void *, i32, i32);
    void DestroyObject(EdClassInterface *, void *, i32, i32);
    void Flush();
    EdClass *GetClass(char *);
    EdClass *GetClass(i32);
    i32 GetClassId(EdClass *);
    i32 GetClassId(char *);
    void GetStreamClassMapping(EdStream &, i32 *, i32 &, i32);
    EdType *GetType(char *);
    EdType *GetType(i32);
    i32 GetTypeId(char *);
    void Initialise(variptr_u &, variptr_u &, i32, i32, i32, i32);
    char *MapName(char *);
    void NotifyCreateObject(void *, EdClass *, void *, i32, i32, i32);
    void NotifyDefunctObject(void *, EdClass *, i32);
    void NotifyDestroyObject(void *, EdClass *, i32, i32);
    void NotifyReviveObject(void *, EdClass *, i32);
    void RegisterBaseTypes();
    EdClass *RegisterClass(char *, EdClassInterface *, i32);
    i32 RegisterType(char *, i32, void (*)(EdStream &, void *, i32));
    void Serialise(EdStream &);
    void SerialiseObjects(EdStream &, EdRegistry *);
};
struct EdSpecialObjectControl : EdControl {
    eduimenu_s *menu;
    EdSpecialObjectControl();
    void AddMenuItem(eduimenu_s *, EdRef *, void *) override;
    void Process(EdInputContext &) override;
    void Render() override;
    static void cbButton(eduimenu_s *, eduiitem_s *, u32);
    static void cbChanged(eduimenu_s *, eduiitem_s *, u32);
    static void cbSelectObject(eduimenu_s *, eduiitem_s *, u32);
};
struct EdString {
    char *data;

    void Set(char const *);
    ~EdString();
};
DECOMP_ASSERT(sizeof(EdString) == 4, "EdString size");
DECOMP_ASSERT(offsetof(EdString, data) == 0, "EdString data offset");
struct EdStringControl : EdControl {
    void AddMenuItem(eduimenu_s *, EdRef *, void *) override;
    EdStringControl();
    ~EdStringControl() override;
    static void operator delete(void *);
    void GetVal(char *, i32);
    void Refresh() override;
    void SetVal(char const *);
    static void cbChanged(eduimenu_s *, eduiitem_s *, u32);
    static void cbPress(eduimenu_s *, eduiitem_s *, u32);
};
struct EdSfxNameControl : EdStringControl {
    EdSfxNameControl();
    void AddMenuItem(eduimenu_s *, EdRef *, void *) override;
    static void cbButton(eduimenu_s *, eduiitem_s *, u32);
    static void cbChanged(eduimenu_s *, eduiitem_s *, u32);
    static void cbSelectSfx(eduimenu_s *, eduiitem_s *, u32);
};
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdSfxNameControl) == 0x10, "EdSfxNameControl ABI");
struct EdClassObjectNameControl : EdStringControl {
    EdClass *selected_class;
    void *selected_object;
    EdRef *selected_reference;

    EdClassObjectNameControl();
    ~EdClassObjectNameControl() override;
    static void operator delete(void *);
    void AddMenuItem(eduimenu_s *, EdRef *, void *) override;
    void Process(EdInputContext &) override;
    void Render() override;
    static void cbButton(eduimenu_s *, eduiitem_s *, u32);
    static void cbChanged(eduimenu_s *, eduiitem_s *, u32);
    static void cbSelectClass(eduimenu_s *, eduiitem_s *, u32);
    static void cbSelectObject(eduimenu_s *, eduiitem_s *, u32);
};
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdClassObjectNameControl) == 0x1c,
              "EdClassObjectNameControl original size");
struct EdSystem {
    EdSubSystem *first_subsystem;
    EdSubSystem *last_subsystem;
    i32 subsystem_count;

    void Initalise(variptr_u &, variptr_u &, i32);
    void Process(float);
    void RegisterSubSystem(EdSubSystem *);
    void Render();
    void Reset();
};
extern EdSystem theEdSystem;
struct EdType {
    char *name;
    i32 size;
    void (*serialise)(EdStream &, void *, i32);

    void Serialise(EdStream &);
};

extern i32 EdType_Char;
extern i32 EdType_Short;
extern i32 EdType_Int;
extern i32 EdType_Float;
extern i32 EdType_VuVec;
extern i32 EdType_VuMtx;
extern i32 EdType_Enumeration;
extern i32 EdType_String;
extern i32 EdType_Colour3;
extern i32 EdType_NuHSpecial;
extern i32 EdType_NuVec;
extern i32 EdType_NuMtx;
extern EdRegistry theRegistry;

DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdClass) == 0x18, "EdClass 32-bit size");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdType) == 0xc, "EdType 32-bit size");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(EdType, serialise) == 0x8, "EdType::serialise 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdMember) == 0x8, "EdMember 32-bit size");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdRegistry) == 0x44, "EdRegistry 32-bit size");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdObjectNotifier) == 4, "EdObjectNotifier 32-bit size");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdDefunctListEntry) == 0x10, "EdDefunctListEntry 32-bit size");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EdDefunctList) == 0xc, "EdDefunctList 32-bit size");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(EdRegistry, defunct_objects) == 0x38,
              "EdRegistry::defunct_objects 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(EdRegistry, notifier_count) == 0x34,
              "EdRegistry::notifier_count 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(EdRegistry, types) == 0x4, "EdRegistry::types 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(EdRegistry, classes) == 0x8, "EdRegistry::classes 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(EdRegistry, type_count) == 0x1c, "EdRegistry::type_count 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(EdRegistry, class_count) == 0x24,
              "EdRegistry::class_count 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(EdRegistry, object_count) == 0x2c,
              "EdRegistry::object_count 32-bit offset");
struct EdVectorControl : EdControl {
    eduiitem_s *components[3];
    void AddMenuItem(eduimenu_s *, EdRef *, void *) override;
    void Destroy();
    EdVectorControl();
    ~EdVectorControl() override;
    static void operator delete(void *);
    void Refresh() override;
    static void cbButton(eduimenu_s *, eduiitem_s *, u32);
    static void cbChanged(eduimenu_s *, eduiitem_s *, u32);
    static void cbSelected(eduimenu_s *, eduiitem_s *, u32);
};
struct EditorSettings {
    virtual ~EditorSettings() {
    }
    f32 cursor_radius;
    i32 snap_terrain;

    void AddMenuItems(eduimenu_s *);
    EditorSettings();
    void Serialise(EdStream &);
};
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(EditorSettings) == 0xc, "EditorSettings 32-bit size");
struct EdClassInterfaceVTableObject {
    void *offset_to_top;
    void *type_info;
    EdClassInterfaceVTable methods;
};
extern const EdClassInterfaceVTableObject splineHelperVTable asm("_ZTV12SplineHelper");
extern const EdClassInterfaceVTableObject knotHelperVTable asm("_ZTV10KnotHelper");
struct KnotHelper {
    void *vtable;
    EdClass *object_class;
    EdRef *position_ref;
    EdRef *in_tangent_ref;
    EdRef *out_tangent_ref;

    KnotHelper()
        : vtable(const_cast<EdClassInterfaceVTable *>(&knotHelperVTable.methods)), object_class(NULL),
          position_ref(NULL), in_tangent_ref(NULL), out_tangent_ref(NULL) {
    }
    ~KnotHelper();
    void Flush();
    void *CreateObject(void *, i32, i32);
    void DestroyObject(void *, i32);
    f32 DistanceToObject(VuVec &, VuVec &, void *, EdRef **);
    void *GetNextObject(void *);
    i32 GetNumObjects();
    void Process(void *, EdInputContext &);
    void Render(void *, i32);
};
struct SplineHelper {
    void *vtable;
    EdClass *object_class;
    SplineObject *first_object;
    SplineObject *last_object;
    i32 object_count;
    i32 auto_generate_points;

    SplineHelper()
        : vtable(const_cast<EdClassInterfaceVTable *>(&splineHelperVTable.methods)), object_class(NULL),
          first_object(NULL), last_object(NULL), object_count(0), auto_generate_points(0) {
    }
    ~SplineHelper();
    void Flush();
    void AddMenuItems(eduimenu_s *);
    void ClearLevel(i32);
    void *CreateObject(void *, i32, i32);
    void DestroyObject(void *, i32);
    SplineObject *Find(char *);
    i32 Find(char *, SplineObject **, i32);
    void *GetNextObject(void *);
    i32 GetNumObjects();
    void Initialise();
    void PostLoadInitialisation(MemoryBuffer *, MemoryBuffer *);
    void PreLoadInitialisation(MemoryBuffer *, MemoryBuffer *);
    void Process(void *, EdInputContext &);
    void Render(void *, i32);
    void SerialiseObject(EdStream &, void *);
    static void cbEdSplineAutoGenPoints(eduimenu_s *, eduiitem_s *, u32);
    static void cbEdSplineReGenPoints(eduimenu_s *, eduiitem_s *, u32);
    static void cbEdSplineReverseSpline(eduimenu_s *, eduiitem_s *, u32);
    static void cbEdSplineSmoothKnot(eduimenu_s *, eduiitem_s *, u32);
    static void cbEdSplineSmoothSpline(eduimenu_s *, eduiitem_s *, u32);
};
struct SplineKnot {
    SplineKnot *next;
    SplineKnot *previous;
    VuVec position;
    VuVec in_tangent;
    VuVec out_tangent;
    SplineObject *spline;
    i16 led_file;
    u16 reserved_3e;

    void Smooth();
};
struct SplineKnotList {
    SplineKnot *first;
    SplineKnot *last;
    i32 count;

    i32 GetPoint(i32, VuVec &);
};
struct SplinePointBlock {
    SplinePointBlock *next;
    SplinePointBlock *previous;
    i32 capacity;
    i32 point_count;
    VuVec *points;

    void Draw();
    SplinePointBlock();
    SplinePointBlock(i32);
    virtual ~SplinePointBlock();
    static void operator delete(void *);
};
struct SplinePointList {
    SplinePointBlock *first;
    SplinePointBlock *last;
    i32 block_count;

    void AddPoint(VuVec &);
    void Clear();
    void Draw();
    i32 GetNumPoints();
    i32 GetPoint(i32, VuVec &);
};
struct SplineObject {
    virtual ~SplineObject();
    SplineObject *next;
    SplineObject *previous;
    char name[32];
    SplineKnotList knots;
    SplinePointList points;
    i16 led_file;
    u16 reserved_46;
    f32 step;
    f32 height;
    i32 drop;
    i32 closed;

    SplineObject();
    static void operator delete(void *);
    SplineObject *Clone();
    void Draw(i32, i32, i32, float);
    void DropPoint(VuVec &);
    void GenBezierPoints();
    void GenLinearPoints();
    void GenPoints();
    void ReverseKnots();
    void SmoothKnots();
};
DECOMP_ASSERT(sizeof(SplineKnot) == 0x40, "SplineKnot size");
DECOMP_ASSERT(offsetof(SplineKnot, in_tangent) == 0x18, "SplineKnot incoming tangent offset");
DECOMP_ASSERT(offsetof(SplineKnot, out_tangent) == 0x28, "SplineKnot outgoing tangent offset");
DECOMP_ASSERT(offsetof(SplineKnot, spline) == 0x38, "SplineKnot owning spline offset");
DECOMP_ASSERT(sizeof(SplineObject) == 0x58, "SplineObject size");
DECOMP_ASSERT(offsetof(SplineObject, points) == 0x38, "SplineObject points offset");
DECOMP_ASSERT(offsetof(SplineObject, step) == 0x48, "SplineObject step offset");
DECOMP_ASSERT(offsetof(SplineHelper, auto_generate_points) == 0x14, "SplineHelper automatic generation offset");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(SplineHelper) == 0x18, "SplineHelper size");
DECOMP_ASSERT(sizeof(KnotHelper) == 0x14, "KnotHelper size");
extern SplineHelper theSplineHelper;
extern KnotHelper theKnotHelper;
#endif // GAMEAPI_EDTOOLS_TYPES_H
