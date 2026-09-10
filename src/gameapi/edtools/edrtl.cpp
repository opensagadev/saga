#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edfile.h"

burnset_s *edrtl_edit_burnset;
static NUVEC pcpos;

i32 edrtlBurnoutSave(char *filename, burnset_s *set) {
    i32 i;
    i32 version = 5;
    i32 count = 0;
    if (!set)
        return 0;
    for (i = 0; i < 32; ++i) {
        if (set->burnouts[i].active)
            ++count;
    }
    EdFileSetMedia(1);
    if (EdFileOpen(filename, static_cast<NUFILEMODE>(1))) {
        EdFileSetReadWrongEndianess(1);
        EdFileWriteInt(version);
        EdFileWriteInt(count);
        EdFileWriteFloat(set->parameters.field_24);
        EdFileWriteFloat(set->parameters.field_14);
        EdFileWriteFloat(set->parameters.field_1c);
        EdFileWriteFloat(set->field_b8);
        EdFileWriteFloat(set->field_bc);
        EdFileWriteFloat(set->field_c0);
        EdFileWriteFloat(set->field_c4);
        EdFileWriteFloat(set->field_c8);
        EdFileWriteFloat(set->field_cc);
        for (i = 0; i < 32; ++i) {
            if (set->burnouts[i].active) {
                EdFileWriteNuVec(&set->burnouts[i].position);
                EdFileWriteFloat(set->burnouts[i].field_10);
                EdFileWriteFloat(set->burnouts[i].field_14);
                EdFileWriteFloat(set->burnouts[i].field_18);
                EdFileWriteFloat(set->burnouts[i].field_1c);
                EdFileWriteFloat(set->burnouts[i].field_20);
            }
        }
        EdFileWriteFloat(set->parameters.field_04);
        EdFileWriteFloat(set->parameters.field_08);
        EdFileWriteFloat(set->parameters.field_0c);
        EdFileWriteFloat(set->parameters.field_10);
        EdFileWriteFloat(set->parameters.field_20);
        EdFileWriteInt(set->parameters.field_28);
        EdFileWriteFloat(set->parameters.field_2c);
        EdFileWriteFloat(set->parameters.field_30);
        EdFileWriteFloat(set->parameters.field_34);
        EdFileWriteFloat(set->parameters.field_38);
        EdFileWriteFloat(set->parameters.field_3c);
        EdFileWriteFloat(set->parameters.field_40);
        EdFileWriteFloat(set->parameters.field_44);
        EdFileWriteFloat(set->parameters.field_48);
        EdFileClose();
        EdFileSetReadWrongEndianess(0);
        return 1;
    }
    return 0;
}
void edrtlDetermineNearestBurn(float distance, burnset_s *set);
void edrtlInitBurnset(burnset_s *set);
i32 edrtlBurnoutLoadSet(char *filename, burnset_s *set);

extern "C" burnset_s *edrtlBurnoutLoad(char *filename, VARIPTR *buffer) {
    buffer->addr = (buffer->addr + 3) & ~static_cast<usize>(3);
    burnset_s *set = static_cast<burnset_s *>(buffer->void_ptr);
    edrtlInitBurnset(set);
    set->field_560 = edrtlBurnoutLoadSet(filename, set);
    edrtl_edit_burnset = set;
    buffer->char_ptr += sizeof(burnset_s);
    return set;
}

i32 edrtlBurnoutLoadSet(char *filename, burnset_s *set) {
    i32 max_version = 5;
    if (!set)
        return 0;
    EdFileSetMedia(1);
    if (EdFileOpen(filename, static_cast<NUFILEMODE>(0))) {
        EdFileSetReadWrongEndianess(1);
        i32 version = EdFileReadInt();
        if (version > max_version) {
            EdFileSetReadWrongEndianess(0);
            EdFileClose();
            return 0;
        }
        i32 count = EdFileReadInt();
        set->parameters.field_24 = EdFileReadFloat();
        set->parameters.field_14 = EdFileReadFloat();
        set->parameters.field_1c = EdFileReadFloat();
        if (version > 1) {
            set->field_b8 = EdFileReadFloat();
            set->field_bc = EdFileReadFloat();
            set->field_c0 = EdFileReadFloat();
            set->field_c4 = EdFileReadFloat();
            if (version == 2)
                set->field_c4 = set->field_c4 - 1.0f;
            set->field_c8 = EdFileReadFloat();
            set->field_cc = EdFileReadFloat();
        } else {
            set->field_b8 = 1.0f;
            set->field_bc = 4.0f;
            set->field_c0 = 0.2f;
            set->field_c4 = 0.5f;
            set->field_c8 = 1.9f;
            set->field_cc = 0.0f;
        }
        if (set->active_count + count > 32)
            count = 32 - set->active_count;
        i32 slot = 0;
        for (i32 i = 0; i < count; ++i) {
            while (set->burnouts[slot].active && slot < 32)
                ++slot;
            if (slot < 32) {
                set->burnouts[slot].active = 1;
                EdFileReadNuVec(&set->burnouts[slot].position);
                set->burnouts[slot].field_10 = EdFileReadFloat();
                set->burnouts[slot].field_14 = EdFileReadFloat();
                set->burnouts[slot].field_18 = EdFileReadFloat();
                set->burnouts[slot].field_1c = EdFileReadFloat();
                set->burnouts[slot].field_20 = EdFileReadFloat();
                set->active_count = set->active_count + 1;
            }
        }
        if (version > 4) {
            set->parameters.field_04 = EdFileReadFloat();
            set->parameters.field_08 = EdFileReadFloat();
            set->parameters.field_0c = EdFileReadFloat();
            set->parameters.field_10 = EdFileReadFloat();
            set->parameters.field_20 = EdFileReadFloat();
            set->parameters.field_28 = EdFileReadInt();
            set->parameters.field_2c = EdFileReadFloat();
            set->parameters.field_30 = EdFileReadFloat();
            set->parameters.field_34 = EdFileReadFloat();
            set->parameters.field_38 = EdFileReadFloat();
            set->parameters.field_3c = EdFileReadFloat();
            set->parameters.field_40 = EdFileReadFloat();
            set->parameters.field_44 = EdFileReadFloat();
            set->parameters.field_48 = EdFileReadFloat();
        }
        EdFileSetReadWrongEndianess(0);
        EdFileClose();
        set->selected_index = -1;
        edrtlDetermineNearestBurn(1.0f, set);
        set->field_b4 = 1;
        return 1;
    }
    return 0;
}

void edrtlDetermineNearestBurn(float distance, burnset_s *set) {
    NUVEC delta;
    float distance_squared;
    if (set->selected_index != -1) {
        NuVecSub(&delta, &pcpos, &set->burnouts[set->selected_index].position);
        distance_squared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (distance_squared == 0.0f)
            return;
    }
    set->selected_index = -1;
    for (i32 i = 0; i < 32; ++i) {
        if (set->burnouts[i].active) {
            NuVecSub(&delta, &pcpos, &set->burnouts[i].position);
            distance_squared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
            if (distance < 0.0f || distance_squared < distance) {
                set->selected_index = i;
                distance = distance_squared;
            }
        }
    }
}

void edrtlResetBurnset(burnset_s *burnset) {
    if (burnset == NULL)
        return;
    for (i32 i = 0; i < 32; ++i) {
        burnset->burnouts[i].active = 0;
    }
    burnset->active_count = 0;
    burnset->selected_index = -1;
}

i32 edrtlAddBurnout(nuvec_s *position) {
    i32 index = 0;
    if (edrtl_edit_burnset && edrtl_edit_burnset->active_count < 32) {
        while (edrtl_edit_burnset->burnouts[index].active)
            ++index;
        edrtl_edit_burnset->burnouts[index].active = 1;
        edrtl_edit_burnset->burnouts[index].position = *position;
        edrtl_edit_burnset->burnouts[index].field_1c = edrtl_edit_burnset->field_558;
        edrtl_edit_burnset->burnouts[index].field_20 = edrtl_edit_burnset->field_55c;
        edrtl_edit_burnset->burnouts[index].field_10 = edrtl_edit_burnset->parameters.field_24;
        edrtl_edit_burnset->burnouts[index].field_14 = edrtl_edit_burnset->parameters.field_14;
        edrtl_edit_burnset->burnouts[index].field_18 = edrtl_edit_burnset->parameters.field_1c;
        ++edrtl_edit_burnset->active_count;
        return index;
    }
    return -1;
}

void edrtlPlaceBurnout(i32 index, nuvec_s *position) {
    if (!edrtl_edit_burnset || !edrtl_edit_burnset->burnouts[index].active)
        return;
    edrtl_edit_burnset->burnouts[index].position = *position;
}

void edrtlRemoveBurnout(i32 index) {
    if (!edrtl_edit_burnset || !edrtl_edit_burnset->burnouts[index].active)
        return;
    edrtl_edit_burnset->burnouts[index].active = 0;
    --edrtl_edit_burnset->active_count;
    edrtl_edit_burnset->selected_index = -1;
}

void edrtlInitBurnset(burnset_s *set) {
    set->parameters.field_00 = 1;
    set->parameters.field_04 = 0.0f;
    set->parameters.field_08 = 0.0f;
    set->parameters.field_0c = 180.0f;
    set->parameters.field_10 = 1.0f;
    set->parameters.field_14 = 0.0f;
    set->parameters.field_18 = 0;
    set->parameters.field_1c = 0.0f;
    set->parameters.field_20 = 4.0f;
    set->parameters.field_24 = 0.0f;
    set->parameters.field_28 = 0;
    set->parameters.field_2c = 0.577f;
    set->parameters.field_30 = 0.577f;
    set->parameters.field_34 = 0.577f;
    set->parameters.field_38 = 0.0f;
    set->parameters.field_3c = 1.0f;
    set->parameters.field_40 = 180.0f;
    set->parameters.field_44 = 0.0f;
    set->parameters.field_48 = 1.0f;
    set->parameters.field_4c = 0.0f;
    set->parameters.field_50 = 0.0f;
    set->parameters_copy = set->parameters;
    set->field_a8 = 0;
    set->field_ac = 0;
    set->field_b0 = 0;
    set->field_b4 = 1;
    set->field_b8 = 1.0f;
    set->field_bc = 4.0f;
    set->field_c0 = 0.2f;
    set->field_c4 = 0.5f;
    set->field_c8 = 1.9f;
    set->field_cc = 0.0f;
    set->active_count = 0;
    set->selected_index = -1;
    set->field_558 = 1.0f;
    set->field_55c = 0.2f;
    set->field_560 = 0;
    for (i32 i = 0; i < 32; ++i)
        set->burnouts[i].active = 0;
}
