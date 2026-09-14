#include "decomp.h"
#include <string.h>
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/gizmos/trigger/giztimer.h"
#include "legoapi/gizmos/trigger/gizrandom.h"
#include "legoapi/gizmos/trigger/gizspecial.h"

static VARIPTR *load_buff;
static VARIPTR *load_endbuff;
static FLOWBOX_s *load_flowbox;
static i32 load_conditiontype;
static u8 load_conditionParam;
static char load_gizmoname[32];
static GIZFLOW_s *load_gizflow;
static i32 load_gizmotype;
static i32 load_numgizmos;
static u16 load_g_flags;
static i8 load_t_randomTime;
static float load_t_time;
static i32 load_r_noutputs;
static i32 load_r_outputChance[8];
static i32 load_nflowboxes;
static char load_name[32];
extern GIZACTIONDEFN_s *gizactiondefs;

static void xGizmoType(nufpar_s *parser) {
    NuFParGetWord(parser);
    if (parser->word_buf != NULL)
        load_gizmotype = GizmoGetTypeIDByName(load_gizflow->gizmo_sys, parser->word_buf);
}
static void xGizmoName(nufpar_s *parser) {
    NuFParGetWord(parser);
    if (parser->word_buf != NULL)
        NuStrNCpy(load_gizmoname, parser->word_buf, 32);
}
static void xStartInvis(nufpar_s *) {
    load_g_flags |= 1;
}
static void xEndInvis(nufpar_s *) {
    load_g_flags |= 2;
}
static void xReverse(nufpar_s *) {
    load_g_flags |= 4;
}
static void xEndDeact(nufpar_s *) {
    load_g_flags |= 8;
}
static void xOutputOnly(nufpar_s *) {
    load_g_flags = 0x10;
}
static void xNotStoryMode(nufpar_s *) {
    load_g_flags |= 0x20;
}
static void xNotFreeplay(nufpar_s *) {
    load_g_flags |= 0x40;
}
static void xReverseInvis(nufpar_s *) {
    load_g_flags |= 0x80;
}
static void xGizRandomTime(nufpar_s *) {
    load_t_randomTime = 1;
}
static void xGizTimer(nufpar_s *parser) {
    load_t_time = NuFParGetFloat(parser);
}
static void xRand_NumOutputs(nufpar_s *parser) {
    load_r_noutputs = NuFParGetInt(parser);
}
static void xRand_OutputChance(nufpar_s *parser) {
    i32 index = NuFParGetInt(parser);
    load_r_outputChance[index] = NuFParGetInt(parser);
}
static NUFPCOMJMP cfgtab_Gizmo[] = {
    {"Type", xGizmoType},
    {"Name", xGizmoName},
    {"StartInvisible", xStartInvis},
    {"FinishedInvisible", xEndInvis},
    {"FinishedDeactive", xEndDeact},
    {"Reverse", xReverse},
    {"RevInvis", xReverseInvis},
    {"NotStoryMode", xNotStoryMode},
    {"NotFreeplay", xNotFreeplay},
    {"OutputOnly", xOutputOnly},
    {"Timer", xGizTimer},
    {"RandomTime", xGizRandomTime},
    {"NumRandomOutputs", xRand_NumOutputs},
    {"RandomOutputChance", xRand_OutputChance},
    {NULL, NULL},
};

static __used__ void xGizmo(nufpar_s *parser) {
    if (load_flowbox == NULL)
        return;
    FLOWBOXGIZMODATA_s *data = load_flowbox->data;
    load_flowbox->type = 0;
    load_gizmotype = -1;
    load_g_flags = 0;
    load_t_randomTime = 0;
    if (data == NULL) {
        load_flowbox->data = reinterpret_cast<FLOWBOXGIZMODATA_s *>(
            GizmoBufferAlloc(load_buff, load_endbuff, sizeof(FLOWBOXGIZMODATA_s)));
        data = load_flowbox->data;
        data->capacity = load_numgizmos;
        data->gizmos = reinterpret_cast<FLOWBOXGIZMOREF_s **>(
            GizmoBufferAlloc(load_buff, load_endbuff, load_numgizmos * sizeof(FLOWBOXGIZMOREF_s *)));
        data->gizmo_count = 0;
    }
    NuStrCpy(load_gizmoname, "");
    NuFParPushCom(parser, cfgtab_Gizmo);
    while (NuFParGetLine(parser)) {
        NuFParGetWord(parser);
        if (NuStrICmp(parser->word_buf, "}") == 0)
            break;
        NuFParInterpretWord(parser);
    }
    NuFParPopCom(parser);
    if (load_gizmotype < 0 || NuStrLen(load_gizmoname) == 0)
        return;
    i32 name_size = NuStrLen(load_gizmoname) + NuStrLen(gizmotypes->types[load_gizmotype].prefix) + 1;
    data->gizmos[data->gizmo_count] =
        reinterpret_cast<FLOWBOXGIZMOREF_s *>(GizmoBufferAlloc(load_buff, load_endbuff, sizeof(FLOWBOXGIZMOREF_s)));
    data->gizmos[data->gizmo_count]->name =
        reinterpret_cast<char *>(GizmoBufferAlloc(load_buff, load_endbuff, name_size));
    NuStrCpy(data->gizmos[data->gizmo_count]->name, load_gizmoname);
    if (load_gizmotype == giztimer_gizmotype_id) {
        data->gizmos[data->gizmo_count]->gizmo = createGizTimer(NULL, load_t_time, load_t_randomTime, load_gizmoname);
    } else if (load_gizmotype == gizrandom_gizmotype_id) {
        data->gizmos[data->gizmo_count]->gizmo =
            createGizRandom(NULL, load_r_noutputs, load_r_outputChance, load_gizmoname);
    } else if (load_gizmotype == gizspecial_gizmotype_id) {
        data->gizmos[data->gizmo_count]->gizmo = createGizSpecial(NULL, load_gizmoname);
        if (data->gizmos[data->gizmo_count]->gizmo != NULL) {
            NuStrCpy(data->gizmos[data->gizmo_count]->name, gizmotypes->types[load_gizmotype].prefix);
            NuStrNCat(data->gizmos[data->gizmo_count]->name, load_gizmoname,
                      32 - NuStrLen(gizmotypes->types[load_gizmotype].prefix));
        } else
            goto set_flags;
    } else {
        data->gizmos[data->gizmo_count]->gizmo =
            GizmoFindByName(load_gizflow->gizmo_sys, load_gizmotype, load_gizmoname);
    }
    ++data->gizmo_count;
set_flags:
    load_flowbox->state_flags_low = (load_flowbox->state_flags_low & ~0xdc) | ((load_g_flags & 1) << 2) |
                                    ((load_g_flags & 2) << 2) | ((load_g_flags & 8) << 1) | ((load_g_flags & 4) << 4) |
                                    (load_g_flags & 0x80);
    load_flowbox->state_flags_high = (load_flowbox->state_flags_high & ~0x70) | (load_g_flags & 0x10) |
                                     (((load_g_flags >> 5) ^ 1) & 1) << 6 | (((load_g_flags >> 6) ^ 1) & 1) << 5;
}

struct FLOWREMAP {
    i16 parents[16];
    i16 children[32];
    u8 parent_outputs[16];
};
DECOMP_ASSERT(sizeof(FLOWREMAP) == 0x70, "Flow remap ABI");
DECOMP_ASSERT(offsetof(FLOWREMAP, children) == 0x20, "Flow remap children offset");
DECOMP_ASSERT(offsetof(FLOWREMAP, parent_outputs) == 0x60, "Flow remap output offset");
static FLOWREMAP *remap;
static i32 numRemaps;
static i32 load_nparents;
static i32 load_nchildren;
static i32 load_parents[16];
static i32 load_children[32];
static u8 load_parent_output_ix[16];

static __used__ void remapChildren(i32 id) {
    for (i32 i = 0; load_nchildren < 32 && remap[~id].children[i] != id; ++i) {
        i32 child = remap[~id].children[i];
        if (child < 0)
            remapChildren(child);
        else
            load_children[load_nchildren++] = child;
    }
}

static __used__ void remapParent(i32 id) {
    for (i32 i = 0; load_nparents < 16 && remap[~id].parents[i] != id; ++i) {
        i32 parent = remap[~id].parents[i];
        if (parent < 0)
            remapParent(parent);
        else {
            load_parents[load_nparents] = parent;
            load_parent_output_ix[load_nparents] = remap[~id].parent_outputs[i];
            ++load_nparents;
        }
    }
}

static __used__ void xChild(nufpar_s *parser) {
    if (load_nchildren < 32) {
        i32 child = NuFParGetInt(parser);
        if (child < 0)
            remapChildren(child);
        else
            load_children[load_nchildren++] = child;
    }
}

static __used__ void xParent(nufpar_s *parser) {
    if (load_nparents < 16) {
        i32 parent = NuFParGetInt(parser);
        if (parent < 0) {
            NuFParGetInt(parser);
            remapParent(parent);
        } else {
            i32 index = load_nparents;
            load_parents[index] = parent;
            load_parent_output_ix[index] = NuFParGetInt(parser);
            ++load_nparents;
        }
    }
}

static void xChild_Col(nufpar_s *parser) {
    if (load_nchildren <= 31) {
        FLOWREMAP *entry = &remap[numRemaps];
        i32 index = load_nchildren;
        entry->children[index] = NuFParGetInt(parser);
        ++load_nchildren;
    }
}

static void xParent_Col(nufpar_s *parser) {
    if (load_nparents <= 15) {
        FLOWREMAP *entry = &remap[numRemaps];
        i32 index = load_nparents;
        entry->parents[index] = NuFParGetInt(parser);
        entry = &remap[numRemaps];
        index = load_nparents;
        entry->parent_outputs[index] = NuFParGetInt(parser);
        ++load_nparents;
    }
}

static NUFPCOMJMP cfgtab_Collapse[] = {
    {"Parent", xParent_Col},
    {"Child", xChild_Col},
    {NULL, NULL},
};

static __used__ void xCollapse(nufpar_s *parser) {
    load_nparents = 0;
    load_nchildren = 0;
    NuFParPushCom(parser, cfgtab_Collapse);
    while (NuFParGetLine(parser)) {
        NuFParGetWord(parser);
        if (NuStrICmp(parser->word_buf, "}") == 0)
            break;
        NuFParInterpretWord(parser);
    }
    NuFParPopCom(parser);
    FLOWREMAP *entry = &remap[numRemaps];
    i32 sentinel = ~numRemaps;
    ++numRemaps;
    entry->parents[load_nparents] = sentinel;
    entry->children[load_nchildren] = sentinel;
}

static void loadSumBox(nufpar_s *parser) {
    load_conditionParam = NuFParGetInt(parser);
}

struct FLOWCONDITIONTYPE {
    i32 type;
    char *name;
    nufpcomfn *load;
};
static FLOWCONDITIONTYPE ConditionTypes[] = {
    {0, "All", NULL},  {1, "Any", NULL},           {2, "None", NULL}, {3, "Sum", loadSumBox},
    {4, "loop", NULL}, {5, "exactly", loadSumBox}, {-1, NULL, NULL},
};
DECOMP_ASSERT(sizeof(FLOWCONDITIONTYPE) == 12, "Flow condition type ABI");

static void xConditionType(nufpar_s *parser) {
    NuFParGetWord(parser);
    i32 index = 0;
    FLOWCONDITIONTYPE *type = ConditionTypes;
    while (load_conditiontype == -1) {
        if (NuStrICmp(type->name, parser->word_buf) == 0) {
            load_conditiontype = index;
            if (type->load != NULL)
                type->load(parser);
        }
        ++index;
        ++type;
        if (type->type == -1)
            break;
    }
}

static void xMonitorInputs(nufpar_s *) {
    load_flowbox->state_flags_high |= 2;
}

static NUFPCOMJMP cfgtab_Condition[] = {
    {"Type", xConditionType},
    {"MonitorInputs", xMonitorInputs},
    {NULL, NULL},
};

static __used__ void xCondition(nufpar_s *parser) {
    if (load_flowbox == NULL)
        return;
    load_flowbox->type = 1;
    load_conditiontype = -1;
    load_conditionParam = 0xff;
    NuStrCpy(load_gizmoname, "");
    NuFParPushCom(parser, cfgtab_Condition);
    while (NuFParGetLine(parser)) {
        NuFParGetWord(parser);
        if (NuStrICmp(parser->word_buf, "}") == 0)
            break;
        NuFParInterpretWord(parser);
    }
    NuFParPopCom(parser);
    if (load_conditiontype >= 0) {
        FLOWBOX_s *box = load_flowbox;
        u8 *condition = reinterpret_cast<u8 *>(GizmoBufferAlloc(load_buff, load_endbuff, 4));
        if (condition != NULL) {
            condition[0] = load_conditiontype;
            condition[1] = load_conditionParam;
        }
        box->condition_data = condition;
    }
}

static void xAction(nufpar_s *parser) {
    if (load_flowbox == NULL)
        return;
    load_flowbox->type = 2;
    FLOWBOXACTION_s *previous = NULL;
    char parameters[16][64];
    while (NuFParGetLine(parser)) {
        NuFParGetWord(parser);
        if (NuStrICmp(parser->word_buf, "}") == 0)
            break;
        GIZACTIONDEFN_s *definition = gizactiondefs;
        if (definition == NULL)
            continue;
        while (definition->name != NULL && NuStrICmp(parser->word_buf, definition->name) != 0)
            ++definition;
        if (definition->name == NULL)
            continue;
        FLOWBOXACTION_s *action =
            reinterpret_cast<FLOWBOXACTION_s *>(GizmoBufferAlloc(load_buff, load_endbuff, sizeof(FLOWBOXACTION_s)));
        if (action == NULL)
            continue;
        action->next = NULL;
        action->parameters = NULL;
        action->parameter_count = 0;
        action->definition = NULL;
        if (previous != NULL)
            previous->next = action;
        else
            load_flowbox->actions = action;
        action->definition = definition;
        i32 count = 0;
        while (NuFParGetWord(parser)) {
            while (NuStrCmp(parser->word_buf, "\\") == 0) {
                NuFParGetLine(parser);
                if (!NuFParGetWord(parser))
                    goto parameters_done;
            }
            NuStrCpy(parameters[count++], parser->word_buf);
        }
    parameters_done:
        if (count != 0) {
            action->parameters =
                reinterpret_cast<char **>(GizmoBufferAlloc(load_buff, load_endbuff, count * sizeof(char *)));
            if (action->parameters != NULL) {
                action->parameter_count = count;
                for (i32 i = 0; i < count; ++i) {
                    char **destination = &action->parameters[i];
                    VARIPTR *end = load_endbuff;
                    VARIPTR *buffer = load_buff;
                    i32 length = NuStrLen(parameters[i]);
                    char *parameter = NULL;
                    if (length != 0) {
                        parameter = reinterpret_cast<char *>(GizmoBufferAlloc(buffer, end, length + 1));
                        NuStrCpy(parameter, parameters[i]);
                    }
                    *destination = parameter;
                }
            }
        }
        previous = action;
    }
}

static void xName(nufpar_s *parser) {
    NuFParGetWord(parser);
    if (parser->word_buf != NULL)
        NuStrNCpy(load_name, parser->word_buf, 32);
}
static void xNumGizmos(nufpar_s *parser) {
    load_numgizmos = NuFParGetInt(parser);
}
static void xAIAssistID(nufpar_s *parser) {
    FLOWBOX_s *box = load_flowbox;
    reinterpret_cast<u8 *>(&box->runtime_id)[0] = NuFParGetInt(parser);
}
static void xFlowBoxCount(nufpar_s *) {
    ++load_nflowboxes;
}
static NUFPCOMJMP cfgtab_FlowBox[] = {
    {"Parent", xParent},        {"Child", xChild},           {"Gizmo", xGizmo},
    {"Condition", xCondition},  {"Action", xAction},         {"Name", xName},
    {"Num_Gizmos", xNumGizmos}, {"AIAssistId", xAIAssistID}, {NULL, NULL},
};

static void xFlowBox(nufpar_s *parser) {
    load_nparents = 0;
    load_nchildren = 0;
    load_numgizmos = 1;
    NuFParPushCom(parser, cfgtab_FlowBox);
    while (NuFParGetLine(parser)) {
        NuFParGetWord(parser);
        if (NuStrICmp(parser->word_buf, "}") == 0)
            break;
        NuFParInterpretWord(parser);
    }
    NuFParPopCom(parser);
    if (load_gizflow != NULL && load_flowbox != NULL) {
        if (load_nparents != 0) {
            load_flowbox->parent_count = load_nparents;
            load_flowbox->parents = reinterpret_cast<FLOWBOX_s **>(
                GizmoBufferAlloc(load_buff, load_endbuff, load_nparents * sizeof(FLOWBOX_s *)));
            load_flowbox->output_indices =
                reinterpret_cast<u8 *>(GizmoBufferAlloc(load_buff, load_endbuff, load_nparents));
            for (i32 i = 0; i < load_flowbox->parent_count; ++i) {
                load_flowbox->parents[i] = &load_gizflow->flowboxes[load_parents[i]];
                load_flowbox->output_indices[i] = load_parent_output_ix[i];
            }
        }
        if (load_nchildren != 0) {
            load_flowbox->child_count = load_nchildren;
            load_flowbox->children = reinterpret_cast<FLOWBOX_s **>(
                GizmoBufferAlloc(load_buff, load_endbuff, load_nchildren * sizeof(FLOWBOX_s *)));
            for (i32 i = 0; i < load_flowbox->child_count; ++i)
                load_flowbox->children[i] = &load_gizflow->flowboxes[load_children[i]];
        }
        load_flowbox->name = reinterpret_cast<char *>(GizmoBufferAlloc(load_buff, load_endbuff, strlen(load_name) + 1));
        NuStrCpy(load_flowbox->name, load_name);
    }
    ++load_flowbox;
    --load_nflowboxes;
}

static NUFPCOMJMP cfgtab_GitCount[] = {{"FlowBox", xFlowBoxCount}, {NULL, NULL}};
static NUFPCOMJMP cfgtab_Git[] = {{"FlowBox", xFlowBox}, {"Collapse", xCollapse}, {NULL, NULL}};

void *LoadGizFlow(void *, GIZMOSYS_s *system, char *path, VARIPTR *buffer, VARIPTR *end) {
    FLOWREMAP remaps[96];
    numRemaps = 0;
    remap = remaps;
    load_buff = buffer;
    load_endbuff = end;
    load_nflowboxes = 0;
    GIZFLOW_s *flow = NULL;
    NUFILE file = NuFileOpen(path, NUFILE_READ);
    if (file != 0) {
        nufpar_s *parser = NuFParOpen(file);
        if (parser != NULL) {
            NuFParPushCom(parser, cfgtab_GitCount);
            while (NuFParGetLine(parser)) {
                NuFParGetWord(parser);
                NuFParInterpretWord(parser);
            }
            NuFParClose(parser);
        }
        if (load_nflowboxes != 0) {
            parser = NuFParOpen(file);
            if (parser != NULL) {
                flow = reinterpret_cast<GIZFLOW_s *>(GizmoBufferAlloc(load_buff, load_endbuff, sizeof(GIZFLOW_s)));
                if (flow != NULL) {
                    flow->flowbox_count = load_nflowboxes;
                    flow->gizmo_sys = system;
                    flow->flowboxes = reinterpret_cast<FLOWBOX_s *>(
                        GizmoBufferAlloc(load_buff, load_endbuff, load_nflowboxes * sizeof(FLOWBOX_s)));
                    load_flowbox = flow->flowboxes;
                    load_gizflow = flow;
                    NuFParPushCom(parser, cfgtab_Git);
                    while (NuFParGetLine(parser)) {
                        NuFParGetWord(parser);
                        NuFParInterpretWord(parser);
                    }
                }
                NuFParClose(parser);
            }
        }
        NuFileClose(file);
        if (flow != NULL)
            flow->pointers_need_reset = 1;
    }
    load_buff = NULL;
    load_endbuff = NULL;
    load_gizflow = NULL;
    load_flowbox = NULL;
    return flow;
}

FLOWBOX_s *FlowBoxFindByName(GIZFLOW_s *system, char *name) {
    if (name != NULL && system != NULL) {
        for (i32 i = 0; i < system->flowbox_count; ++i) {
            if (NuStrICmp(system->flowboxes[i].name, name) == 0)
                return &system->flowboxes[i];
        }
    }
    return NULL;
}

void SetGizFlowVisible(GIZFLOW_s *system) {
    if (system == NULL)
        return;
    FLOWBOX_s *box = system->flowboxes;
    for (i32 i = 0; i < system->flowbox_count; ++i, ++box) {
        if (box->type == 0 && box->data != NULL) {
            FLOWBOXGIZMODATA_s *data = box->data;
            for (i32 j = 0; j < data->gizmo_count; ++j) {
                FLOWBOXGIZMOREF_s *ref = data->gizmos[j];
                if (ref != NULL && ref->gizmo != NULL)
                    GizmoSetVisibility(system->gizmo_sys, ref->gizmo, 1, 1);
            }
        }
    }
}

void GizFlowStoreProgress(GIZFLOW_s *system, GIZFLOWPROGRESS_s *progress) {
    if (progress == NULL || system == NULL)
        return;
    memset(progress, 0, sizeof(*progress));
    FLOWBOX_s *box = system->flowboxes;
    i32 count = system->flowbox_count;
    progress->valid = 1;
    for (i32 i = 0; i < count; ++i, ++box) {
        i32 word = i >> 5;
        u32 bit = 1u << (i & 31);
        if ((box->state_flags_low & 1) != 0)
            progress->active[word] |= bit;
        if ((box->state_flags_high & 1) != 0)
            progress->triggered[word] |= bit;
        if ((box->state_flags_low & 2) != 0)
            progress->completed[word] |= bit;
        if ((box->state_flags_low & 0x20) != 0)
            progress->latched[word] |= bit;
        if ((box->state_flags_high & 4) != 0)
            progress->output_state[word] |= bit;
    }
}

i32 GizmoTypeGetProgress(GIZMOSYS_s *system, void *, i32 progress_index, i32 type_id, char *name, void **result) {
    if (name != NULL && type_id == -1)
        type_id = GizmoGetTypeIDByName(system, name);
    if (type_id == -1)
        return 0;
    GIZMOTYPE *type = &gizmotypes->types[type_id];
    void *progress = NULL;
    if (type->buffer != NULL && progress_index >= 0 && progress_index < gizmotypes->unknown)
        progress = type->buffer[progress_index].void_ptr;
    *result = progress;
    return type->fns.unknown1 != -1 ? type->fns.unknown1 : 0;
}

void PerformActionFlowBox(GIZFLOW_s *system, FLOWBOX_s *box) {
    if (box != NULL && box->type == 2) {
        for (FLOWBOXACTION_s *action = box->actions; action != NULL; action = action->next) {
            if (action->definition != NULL && action->definition->action_fn != NULL) {
                action->definition->action_fn(system, box, action->parameters, action->parameter_count);
            }
        }
    }
}

void GizmoSysStoreProgress(GIZMOSYS_s *system, void *world, i32 progress_index) {
    GizmoSysClearLevelProgress(world, progress_index);
    if (gizmotypes == NULL || system == NULL)
        return;
    GIZMOTYPE *type = gizmotypes->types;
    GIZMOSET *set = system->sets;
    if (progress_index < 0) {
        for (i32 i = 0; i < gizmotypes->count; ++i, ++type, ++set) {
            if (type->fns.store_progress_fn != NULL)
                type->fns.store_progress_fn(world, set->unknown, NULL);
        }
        return;
    }
    for (i32 i = 0; i < gizmotypes->count; ++i, ++type, ++set) {
        if (type->fns.store_progress_fn != NULL) {
            void *progress = NULL;
            if (type->buffer != NULL)
                progress = type->buffer[progress_index].void_ptr;
            type->fns.store_progress_fn(world, set->unknown, progress);
        }
    }
}

void GizmoTypeStoreProgress(GIZMOSYS_s *system, void *world, i32 progress_index, i32 type_id, char *name) {
    if (name != NULL && type_id == -1)
        type_id = GizmoGetTypeIDByName(system, name);
    if (type_id == -1)
        return;
    GIZMOTYPE *type = &gizmotypes->types[type_id];
    GIZMOSET *set = &system->sets[type_id];
    void *progress = NULL;
    if (type->buffer != NULL && progress_index >= 0 && progress_index < gizmotypes->unknown)
        progress = type->buffer[progress_index].void_ptr;
    if (type->fns.clear_progress_fn != NULL)
        type->fns.clear_progress_fn(world, progress);
    if (type->fns.store_progress_fn != NULL)
        type->fns.store_progress_fn(world, set->unknown, progress);
}

static void CheckIfParentsFinished(GIZFLOW_s *system, FLOWBOX_s *box) {
    for (i32 i = 0; i < box->parent_count; ++i) {
        FLOWBOX_s *parent = box->parents[i];
        if ((parent->state_flags_low & 0x20) == 0)
            continue;

        i32 child_index;
        for (child_index = 0; child_index < parent->child_count; ++child_index) {
            FLOWBOX_s *child = parent->children[child_index];
            if ((child->state_flags_low & 0xc0) != 0) {
                if ((child->state_flags & 0x102) != 2)
                    break;
            } else if ((child->state_flags_low & 1) != 0 && child != box) {
                break;
            }
        }
        if (child_index != parent->child_count)
            continue;

        if ((parent->state_flags_low & 8) != 0) {
            FLOWBOXGIZMODATA_s *data = parent->data;
            for (i32 j = 0; j < data->gizmo_count; ++j)
                GizmoSetVisibility(system->gizmo_sys, data->gizmos[j]->gizmo, 0, 1);
        } else if ((parent->state_flags_low & 0x10) != 0) {
            FLOWBOXGIZMODATA_s *data = parent->data;
            for (i32 j = 0; j < data->gizmo_count; ++j)
                GizmoActivate(system->gizmo_sys, data->gizmos[j]->gizmo, 0, 1);
        }
        if ((parent->state_flags_low & 0x40) != 0) {
            parent->state_flags_high &= ~1;
            CheckIfParentsFinished(system, parent);
        }
        parent->state_flags_low &= ~0x20;
    }
}

static void ProcessFlowBox(GIZFLOW_s *, FLOWBOX_s *, u8);

static void ResetForLoopEx(GIZFLOW_s *flow, FLOWBOX_s *root, FLOWBOX_s *box, i32 checksum) {
    if (box != root && box->loop_checksum != checksum) {
        box->loop_checksum = checksum;
        if (box->type == 0 && (box->state_flags_high & 0x10) == 0 && box->data != NULL) {
            FLOWBOXGIZMODATA_s *data = box->data;
            for (i32 i = 0; i < data->gizmo_count; ++i) {
                GizmoActivate(flow->gizmo_sys, data->gizmos[i]->gizmo, 0, 1);
            }
        }
        for (i32 i = 0; i < box->child_count; ++i) {
            ResetForLoopEx(flow, root, box->children[i], checksum);
        }
    }
}

static void ResetGizmoFlowBox(GIZFLOW_s *giz_flow, FLOWBOX_s *flow_box) {
    FLOWBOXGIZMODATA_s &data = *flow_box->data;
    if ((flow_box->state_flags & 0x1000) != 0 || data.gizmo_count <= 0) {
        return;
    }
    for (i32 i = 0; i < data.gizmo_count; ++i) {
        GizmoActivate(giz_flow->gizmo_sys, data.gizmos[i]->gizmo, 0, 1);
    }
}

static i32 ProcessGizmoFlowBox(GIZFLOW_s *, FLOWBOX_s *, u8);
static i32 ProcessActionFlowBox(GIZFLOW_s *, FLOWBOX_s *, u8);
static i32 ProcessConditionFlowBox(GIZFLOW_s *, FLOWBOX_s *, u8);
static i32 CheckOutputGizmoFlowBox(GIZFLOW_s *, FLOWBOX_s *, u8);
static i32 CheckOutputActionFlowBox(GIZFLOW_s *, FLOWBOX_s *, u8);
static i32 CheckOutputConditionFlowBox(GIZFLOW_s *, FLOWBOX_s *, u8);
static u8 getNextLoopChecksum();
struct FLOWBOXTYPE {
    void (*reset)(GIZFLOW_s *, FLOWBOX_s *);
    i32 (*process)(GIZFLOW_s *, FLOWBOX_s *, u8);
    i32 (*check_output)(GIZFLOW_s *, FLOWBOX_s *, u8);
};
static FLOWBOXTYPE flowboxtypes[] = {
    {ResetGizmoFlowBox, ProcessGizmoFlowBox, CheckOutputGizmoFlowBox},
    {NULL, ProcessConditionFlowBox, CheckOutputConditionFlowBox},
    {NULL, ProcessActionFlowBox, CheckOutputActionFlowBox},
};
DECOMP_ASSERT(sizeof(flowboxtypes) == 0x24, "Flow-box callback table ABI");

static void ProcessFlowBox(GIZFLOW_s *system, FLOWBOX_s *box, u8 frame) {
    if (box->state_flags_high & 1) {
        i32 i;
        for (i = 0; i < box->parent_count; ++i) {
            FLOWBOX_s *parent = box->parents[i];
            if (!flowboxtypes[parent->type].check_output(system, parent, box->output_indices[0]))
                break;
        }
        FLOWBOXGIZMODATA_s *data = box->data;
        if (i == box->parent_count) {
            for (i32 j = 0; j < data->gizmo_count; ++j)
                GizmoActivateReverse(system->gizmo_sys, data->gizmos[j]->gizmo, 0, box->state_flags_low >> 7, 1);
        } else {
            for (i32 j = 0; j < data->gizmo_count; ++j)
                GizmoActivateReverse(system->gizmo_sys, data->gizmos[j]->gizmo, 1, box->state_flags_low >> 7, 1);
        }
    }
    if ((box->state_flags_low & 1) == 0)
        return;
    if (!flowboxtypes[box->type].process(system, box, frame))
        return;

    if ((box->state_flags_high & 0xc) == 0xc)
        box->state_flags_high &= ~4;
    box->state_flags_high |= 8;
    box->state_flags_low = (box->state_flags_low & ~1) | 2;
    if (box->type == 0) {
        if (box->state_flags_low & 0x18) {
            if (box->child_count != 0) {
                box->state_flags_low |= 0x20;
            } else {
                FLOWBOXGIZMODATA_s *data = box->data;
                if (box->state_flags_low & 8) {
                    for (i32 i = 0; i < data->gizmo_count; ++i)
                        GizmoSetVisibility(system->gizmo_sys, data->gizmos[i]->gizmo, 0, 1);
                } else if (box->state_flags_low & 0x10) {
                    for (i32 i = 0; i < data->gizmo_count; ++i)
                        GizmoActivate(system->gizmo_sys, data->gizmos[i]->gizmo, 0, 1);
                }
                box->state_flags_low &= ~0x20;
            }
        }
        if (box->state_flags_low & 0xc0) {
            box->state_flags_low |= 0x20;
            box->state_flags_high |= 1;
            FLOWBOX_s **parents = box->parents;
            i32 count = box->parent_count;
            for (i32 i = 0; i < count; ++i) {
                if (parents[i]->type == 1)
                    parents[i]->state_flags_high |= 2;
                if (parents[i]->type == 0 && (parents[i]->state_flags_low & 0x40)) {
                    parents[i]->state_flags_high |= 1;
                    parents[i]->state_flags_low |= 0x20;
                }
            }
        }
    }
    if (box->type != 0)
        CheckIfParentsFinished(system, box);
    for (i32 i = 0; i < box->child_count; ++i) {
        FLOWBOX_s **child = &box->children[i];
        (*child)->state_flags_low |= 1;
        if ((*child)->last_process_frame != frame || box->type == 1) {
            (*child)->last_process_frame = frame;
            ProcessFlowBox(system, *child, frame);
        }
    }
}

void ProcessGizFlow(GIZFLOW_s *system, float) {
    if (system == NULL)
        return;
    ++system->field_0x0c;
    FLOWBOX_s *box = system->flowboxes;
    for (i32 i = 0; i < system->flowbox_count; ++i, ++box) {
        if (box->last_process_frame != system->field_0x0c && (box->state_flags & 0x101))
            ProcessFlowBox(system, box, system->field_0x0c);
        box->last_process_frame = system->field_0x0c;
    }
}

static i32 ProcessGizmoFlowBox(GIZFLOW_s *system, FLOWBOX_s *box, u8) {
    FLOWBOX_s **parents = box->parents;
    u8 *outputs = box->output_indices;
    for (i32 i = 0; i < box->parent_count; ++i) {
        FLOWBOX_s *parent = parents[i];
        if (!flowboxtypes[parent->type].check_output(system, parent, outputs[i]))
            return 0;
    }
    CheckIfParentsFinished(system, box);
    if ((box->state_flags_high & 0x10) != 0 || ((box->state_flags_high & 0x40) == 0 && FreePlay == 0) ||
        ((box->state_flags_high & 0x20) == 0 && FreePlay != 0))
        return 1;
    FLOWBOXGIZMODATA_s *data = box->data;
    for (i32 i = 0; i < data->gizmo_count; ++i)
        GizmoActivate(system->gizmo_sys, data->gizmos[i]->gizmo, 1, 1);
    return 1;
}

static i32 ProcessActionFlowBox(GIZFLOW_s *system, FLOWBOX_s *box, u8) {
    FLOWBOX_s **parents = box->parents;
    u8 *outputs = box->output_indices;
    for (i32 i = 0; i < box->parent_count; ++i) {
        FLOWBOX_s *parent = parents[i];
        if (!flowboxtypes[parent->type].check_output(system, parent, outputs[i]))
            return 0;
    }
    PerformActionFlowBox(system, box);
    return 1;
}

static i32 ProcessConditionFlowBox(GIZFLOW_s *flow, FLOWBOX_s *box, u8) {
    u8 *condition = reinterpret_cast<u8 *>(box->data);
    if (condition == NULL) {
        FLOWBOX_s **parents = box->parents;
        u8 *outputs = box->output_indices;
        for (i32 i = 0; i < box->parent_count; ++i) {
            FLOWBOX_s *parent = parents[i];
            if (flowboxtypes[parent->type].check_output(flow, parent, outputs[i]) == 0) {
                return 0;
            }
        }
        return 1;
    }
    switch (condition[0]) {
        case 0: {
            FLOWBOX_s **parents = box->parents;
            u8 *outputs = box->output_indices;
            for (i32 i = 0; i < box->parent_count; ++i) {
                FLOWBOX_s *parent = parents[i];
                if (flowboxtypes[parent->type].check_output(flow, parent, outputs[i]) == 0) {
                    return 0;
                }
            }
            break;
        }
        case 2: {
            FLOWBOX_s **parents = box->parents;
            u8 *outputs = box->output_indices;
            for (i32 i = 0; i < box->parent_count; ++i) {
                FLOWBOX_s *parent = parents[i];
                if (flowboxtypes[parent->type].check_output(flow, parent, outputs[i]) != 0) {
                    return 0;
                }
            }
            break;
        }
        case 1:
        case 3:
        case 5: {
            const i32 required = condition[0] == 1 ? 1 : condition[1];
            FLOWBOX_s **parents = box->parents;
            u8 *outputs = box->output_indices;
            i32 count = 0;
            for (i32 i = 0; i < box->parent_count; ++i) {
                FLOWBOX_s *parent = parents[i];
                count += flowboxtypes[parent->type].check_output(flow, parent, outputs[i]) != 0;
            }
            if (condition[0] == 5) {
                return count == required;
            }
            if (count < required) {
                return 0;
            }
            break;
        }
        case 4: {
            i32 required = box->parent_count;
            if ((box->state_flags_high & 4) != 0) {
                required -= box->loop_parent_count;
            }
            if (box->parent_count != 0) {
                FLOWBOX_s **parents = box->parents;
                u8 *outputs = box->output_indices;
                i32 count = 0;
                for (i32 i = 0; i < box->parent_count; ++i) {
                    FLOWBOX_s *parent = parents[i];
                    count += flowboxtypes[parent->type].check_output(flow, parent, outputs[i]) != 0;
                }
                if (count < required) {
                    return 0;
                }
            }
            break;
        }
    }
    if (condition[0] == 4) {
        for (i32 i = 0; i < box->child_count; ++i) {
            const u8 checksum = getNextLoopChecksum();
            ResetForLoopEx(flow, box, box->children[i], checksum);
        }
    }
    return 1;
}

static i32 CheckOutputGizmoFlowBox(GIZFLOW_s *system, FLOWBOX_s *box, u8 output) {
    if (((box->state_flags_high & 0x40) == 0 && FreePlay == 0) ||
        ((box->state_flags_high & 0x20) == 0 && FreePlay != 0))
        return 1;
    FLOWBOXGIZMODATA_s *data = box->data;
    i32 result = 1;
    for (i32 i = 0; i < data->gizmo_count; ++i) {
        if (!GizmoGetOutput(system->gizmo_sys, data->gizmos[i]->gizmo, output, (box->state_flags_high >> 4) & 1))
            result = 0;
    }
    return result;
}

static i32 CheckOutputActionFlowBox(GIZFLOW_s *, FLOWBOX_s *box, u8) {
    return (box->state_flags_low >> 1) & 1;
}

static i32 CheckOutputConditionFlowBox(GIZFLOW_s *system, FLOWBOX_s *box, u8) {
    if ((box->state_flags_high & 2) == 0)
        return (box->state_flags_low >> 1) & 1;
    u8 saved = box->state_flags_high & 4;
    box->state_flags_high |= 4;
    i32 result = flowboxtypes[box->type].process(system, box, 0) != 0;
    box->state_flags_high = (box->state_flags_high & ~4) | saved;
    return result;
}

static i32 Loop_CountLoopingInputsEx(FLOWBOX_s *loop, FLOWBOX_s *box, i32 count, u8 checksum) {
    if (loop == box) {
        --box->loop_checksum;
        return count + 1;
    }
    for (i32 i = 0; i < box->child_count; ++i) {
        if (box->children[i]->loop_checksum != checksum) {
            box->children[i]->loop_checksum = checksum;
            count = Loop_CountLoopingInputsEx(loop, box->children[i], count, checksum);
        }
    }
    return count;
}

static u8 getNextLoopChecksum() {
    static u8 loop_checksum;
    return ++loop_checksum;
}

void DynamicAddGizmoToFlow(GIZFLOW_s *system, GIZMO_s *gizmo) {
    if (gizmo == NULL || system == NULL || gizmotypes == NULL || gizmo->type_id >= gizmotypes->count ||
        gizmotypes->types[gizmo->type_id].fns.get_gizmo_name_fn == NULL)
        return;
    FLOWBOX_s *box = system->flowboxes;
    for (i32 i = 0; i < system->flowbox_count; ++i, ++box) {
        FLOWBOXGIZMODATA_s *data = box->data;
        if (data == NULL)
            continue;
        for (i32 j = 0; j < data->gizmo_count; ++j) {
            if (data->gizmos[j]->gizmo != NULL)
                continue;
            char *name = gizmotypes->types[gizmo->type_id].fns.get_gizmo_name_fn(gizmo);
            if (NuStrICmp(data->gizmos[j]->name, name) != 0)
                continue;
            data->gizmos[j]->gizmo = gizmo;
            if ((box->state_flags_low & 1) == 0) {
                if (flowboxtypes[box->type].reset != NULL)
                    flowboxtypes[box->type].reset(system, box);
                if ((box->state_flags_low & 5) == 4 && box->type == 0)
                    GizmoSetVisibility(system->gizmo_sys, data->gizmos[j]->gizmo, 0, 1);
            }
        }
    }
}

void ResetGizFlowPointers(GIZFLOW_s *system) {
    if (system == NULL || system->flowbox_count == 0)
        return;
    FLOWBOX_s *box = system->flowboxes;
    for (i32 i = 0; i < system->flowbox_count; ++i, ++box) {
        if (box->type != 0)
            continue;
        FLOWBOXGIZMODATA_s *data = box->data;
        for (i32 j = 0; j < data->gizmo_count; ++j) {
            FLOWBOXGIZMOREF_s *ref = data->gizmos[j];
            if (ref->gizmo != NULL) {
                ref->gizmo = GizmoFindByName(system->gizmo_sys, ref->gizmo->type_id, ref->name);
            } else {
                ref->gizmo = GizmoFindByName(system->gizmo_sys, -1, ref->name);
                if (data->gizmos[j]->gizmo != NULL && (box->state_flags_low & 1) == 0) {
                    if (flowboxtypes[box->type].reset != NULL)
                        flowboxtypes[box->type].reset(system, box);
                    if ((box->state_flags_low & 5) == 4 && box->type == 0)
                        GizmoSetVisibility(system->gizmo_sys, data->gizmos[j]->gizmo, 0, 1);
                }
            }
        }
    }
}

void ResetGizFlow(GIZFLOW_s *system, GIZFLOWPROGRESS_s *progress) {
    if (system == NULL)
        return;
    system->field_0x0c = 0;
    FLOWBOX_s *box = system->flowboxes;
    ResetGizFlowPointers(system);
    for (i32 i = 0; i < system->flowbox_count; ++i, ++box) {
        box->last_process_frame = 0;
        box->state_flags_low &= ~0x20;
        if (progress != NULL && progress->valid != 0) {
            i32 word = i >> 5;
            u32 bit = 1u << (i & 31);
            box->state_flags_low = (box->state_flags_low & ~1) | ((progress->active[word] & bit) != 0);
            box->state_flags_low = (box->state_flags_low & ~2) | (((progress->completed[word] & bit) != 0) << 1);
            box->state_flags_low |= ((progress->latched[word] & bit) != 0) << 5;
            box->state_flags_high = (box->state_flags_high & ~4) | (((progress->output_state[word] & bit) != 0) << 2);
            if (box->type == 1 && box->condition_data[0] == 4) {
                u8 checksum = getNextLoopChecksum();
                i32 count = 0;
                for (i32 child = 0; child < box->child_count; ++child) {
                    count = Loop_CountLoopingInputsEx(box, box->children[child], count, checksum);
                }
                box->loop_parent_count = count;
            }
            continue;
        }
        box->state_flags_low &= ~2;
        box->state_flags_high &= ~1;
        if (box->type == 1 && box->condition_data[0] == 4) {
            u8 checksum = getNextLoopChecksum();
            i32 count = 0;
            for (i32 child = 0; child < box->child_count; ++child) {
                count = Loop_CountLoopingInputsEx(box, box->children[child], count, checksum);
            }
            box->loop_parent_count = count;
            box->state_flags_high |= 4;
            if (box->parent_count == box->loop_parent_count) {
                box->state_flags_high |= 8;
                box->state_flags_low |= 1;
            } else
                box->state_flags_low &= ~1;
        } else {
            box->state_flags_low = (box->state_flags_low & ~1) | (box->parent_count == 0);
        }

        if (flowboxtypes[box->type].reset != NULL && (box->state_flags_high & 0x10) == 0)
            flowboxtypes[box->type].reset(system, box);
        if (box->type == 0) {
            if (((box->state_flags_high & 0x40) == 0 && FreePlay == 0) ||
                ((box->state_flags_high & 0x20) == 0 && FreePlay != 0)) {
                FLOWBOXGIZMODATA_s *data = box->data;
                for (i32 gizmo = 0; gizmo < data->gizmo_count; ++gizmo)
                    GizmoSetVisibility(system->gizmo_sys, data->gizmos[gizmo]->gizmo, 0, 1);
            } else if ((box->state_flags_low & 5) == 4) {
                FLOWBOXGIZMODATA_s *data = box->data;
                for (i32 gizmo = 0; gizmo < data->gizmo_count; ++gizmo)
                    GizmoSetVisibility(system->gizmo_sys, data->gizmos[gizmo]->gizmo, 0, 1);
            }
        }
    }
}
