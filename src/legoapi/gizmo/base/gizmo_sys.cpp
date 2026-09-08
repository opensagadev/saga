#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/trigger/giztimer.h"
#include "legoapi/gizmos/trigger/gizrandom.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "nu2api/nufile/nufpar.h"

#include <stdio.h>
#include <string.h>
struct FLOWBOX_s;
void ResetGizFlowPointers(GIZFLOW_s *giz_flow);

i32 gizmoerrorlogsize = 0x800;

GIZMOSYS *CreateGizmoSys(void *world, VARIPTR *buf, VARIPTR *buf_end) {
    GIZMOSYS *gizmo_sys = NULL;

    if (gizmotypes != NULL) {
        gizmo_sys = reinterpret_cast<GIZMOSYS *>(GizmoBufferAlloc(buf, buf_end, sizeof(GIZMOSYS)));
        if (gizmo_sys != NULL) {
            gizmo_sys->sets = reinterpret_cast<GIZMOSET *>(
                GizmoBufferAlloc(buf, buf_end, gizmotypes->count * static_cast<i32>(sizeof(GIZMOSET))));

            if (gizmo_sys->sets != NULL) {
                GIZMOSET *set = gizmo_sys->sets;
                GIZMOTYPE *type = gizmotypes->types;
                for (i32 type_index = 0; type_index < gizmotypes->count; ++type_index, ++type, ++set) {
                    set->type = type;

                    if (type->fns.get_max_gizmos_fn != NULL) {
                        set->max_count = type->fns.get_max_gizmos_fn(world);
                    }
                    if (set->max_count != 0) {
                        set->gizmos = reinterpret_cast<GIZMO *>(
                            GizmoBufferAlloc(buf, buf_end, set->max_count * static_cast<i32>(sizeof(GIZMO))));
                    }
                    if (type->fns.reserve_buffer_space_fn != NULL) {
                        set->unknown = type->fns.reserve_buffer_space_fn(world);
                    }
                }
            }

            if (gizmoerrorlogsize != 0) {
                gizmo_sys->error_log = reinterpret_cast<char *>(GizmoBufferAlloc(buf, buf_end, gizmoerrorlogsize));
            }
        }
    }

    return gizmo_sys;
}
void LoadGizmoSys(GIZMOSYS_s *gizmo_sys, void *world, char *config_file) {
    if (gizmo_sys != NULL) {
        gizmo_sys->flags |= GIZMOSYS_FLAG_LOADING;
        if (gizmo_sys->error_log != NULL) {
            memset(gizmo_sys->error_log, 0, gizmoerrorlogsize);
        }
        gizmo_sys->flags &= ~3;

        if (gizmotypes->count != 0) {
            char gizmo_name[32];
            char path[256];
            sprintf(path, "%s.giz", config_file);

            EdFileSetMedia(1);
            if (EdFileOpen(path, NUFILE_READ) != 0) {
                EdFileReadInt();
                i32 name_length = EdFileReadInt();
                while (name_length != 0) {
                    memset(gizmo_name, 0, sizeof(gizmo_name));
                    EdFileRead(gizmo_name, name_length);

                    i32 data_length = EdFileReadInt();
                    i32 type_id = GizmoGetTypeIDByName(gizmo_sys, gizmo_name);
                    if (data_length > 0 && type_id >= 0 && type_id < gizmotypes->count &&
                        gizmotypes->types[type_id].fns.load_fn != NULL &&
                        gizmotypes->types[type_id].fns.load_fn(world, gizmo_sys->sets[type_id].unknown) != 0) {
                        name_length = EdFileReadInt();
                        continue;
                    }

                    while (data_length != 0) {
                        EdFileReadChar();
                        --data_length;
                    }
                    name_length = EdFileReadInt();
                }
                EdFileClose();
            }

            GIZMOTYPE *type = gizmotypes->types;
            GIZMOSET *set = gizmo_sys->sets;
            for (i32 type_id = 0; type_id < gizmotypes->count; ++type_id, ++type, ++set) {
                if (type->fns.post_load_fn != NULL) {
                    type->fns.post_load_fn(world, set->unknown);
                }
            }
        }

        gizmo_sys->flags &= ~GIZMOSYS_FLAG_LOADING;
    }
}
void LoadEditorSplines(char *path, VARIPTR *buf, VARIPTR *buf_end) {
    (void)path;
    (void)buf;
    (void)buf_end;
}
static u32 gizmoblowupnametable[256];
static i32 gizmoblowupnametable_numids;

void GizmoBlowupResetNameTable(void) {
    gizmoblowupnametable_numids = 0;
    memset(gizmoblowupnametable, 0, sizeof(gizmoblowupnametable));
}
void Hub_LoadAndFixUpMiniKits(WORLDINFO *world, VARIPTR *buf, VARIPTR *buf_end) {
    (void)world;
    (void)buf;
    (void)buf_end;
}
void MiniKit_Load(MINIKIT *minikit, i32 id, VARIPTR *buf, VARIPTR *buf_end, void *param) {
    (void)minikit;
    (void)id;
    (void)buf;
    (void)buf_end;
    (void)param;
}
void MiniKit_InitPieces(MINIKIT *minikit, i32 count, VARIPTR *buf, VARIPTR *buf_end) {
    (void)minikit;
    (void)count;
    (void)buf;
    (void)buf_end;
}
void CharacterMiniKits_Load(COLLECTION_s *collection, WORLDINFO *world, VARIPTR *buf, VARIPTR *buf_end) {
    (void)collection;
    (void)world;
    (void)buf;
    (void)buf_end;
}
void GizmoSysAddGizmos(GIZMOSYS_s *gizmo_sys, GIZFLOW_s *giz_flow, void *world) {
    if (gizmotypes != NULL && gizmo_sys != NULL) {
        GIZMOTYPE *type = gizmotypes->types;
        GIZMOSET *set = gizmo_sys->sets;
        for (i32 type_id = 0; type_id < gizmotypes->count; ++type_id, ++type, ++set) {
            ResetGizmoType(gizmo_sys, type_id, NULL);
            if (type->fns.add_gizmos_fn != NULL) {
                type->fns.add_gizmos_fn(gizmo_sys, type_id, world, set->unknown);
            }
        }
    }

    if (giz_flow != NULL && giz_flow->pointers_need_reset != 0) {
        ResetGizFlowPointers(giz_flow);
    }
}
// The .git loader uses a count pass followed by a command-table parse.
// Collapsed editor groups are expanded through signed, sentinel-terminated links.
struct FLOWREMAP_s {
    i16 parents[16];
    i16 children[32];
    u8 outputs[16];
};
DECOMP_ASSERT(sizeof(FLOWREMAP_s) == 0x70, "flow remap ABI");
struct FLOWGIZREF_s {
    GIZMO_s *gizmo;
    char *name;
};
struct FLOWACTION_s {
    FLOWACTION_s *next;
    char **arguments;
    i32 argument_count;
    GIZACTIONDEFN_s *definition;
};
DECOMP_ASSERT(sizeof(FLOWACTION_s) == 0x10, "flow action ABI");

extern GIZACTIONDEFN_s *gizactiondefs;
GIZMO_s *createGizTimer(void *, f32, i32, char *);
GIZMO_s *createGizRandom(void *, i32, i32 *, char *);
GIZMO_s *createGizSpecial(void *, char *);

static i32 numRemaps;
static FLOWREMAP_s *remap;
static VARIPTR *load_buff, *load_endbuff;
static i32 load_nflowboxes;
static GIZFLOW_s *load_gizflow;
static FLOWBOX_s *load_flowbox;
static i32 load_nparents, load_nchildren, load_numgizmos;
static i32 load_parents[16];
static u8 load_parent_output_ix[16];
static i32 load_children[32];
static char load_name[32];
static i32 load_conditiontype;
static u8 load_conditionParam;
static char load_gizmoname[32];
static i32 load_gizmotype;
static u16 load_g_flags;
static u8 load_t_randomTime;
static f32 load_t_time;
static i32 load_r_noutputs;
static i32 load_r_outputChance[8];

static void xFlowBoxCount(NUFPAR *) { ++load_nflowboxes; }
static void xStartInvis(NUFPAR *) { load_g_flags |= 1; }
static void xEndDeact(NUFPAR *) { load_g_flags |= 8; }
static void xEndInvis(NUFPAR *) { load_g_flags |= 2; }
static void xReverse(NUFPAR *) { load_g_flags |= 4; }
static void xReverseInvis(NUFPAR *) { load_g_flags |= 0x80; }
static void xNotFreeplay(NUFPAR *) { load_g_flags |= 0x40; }
static void xNotStoryMode(NUFPAR *) { load_g_flags |= 0x20; }
static void xOutputOnly(NUFPAR *) { load_g_flags = 0x10; }
static void xGizRandomTime(NUFPAR *) { load_t_randomTime = 1; }
static void xMonitorInputs(NUFPAR *) { load_flowbox->state_flags |= 0x200; }
static void xNumGizmos(NUFPAR *parser) { load_numgizmos = NuFParGetInt(parser); }
static void loadSumBox(NUFPAR *parser) { load_conditionParam = NuFParGetInt(parser); }
static void xGizTimer(NUFPAR *parser) { load_t_time = NuFParGetFloat(parser); }
static void xRand_NumOutputs(NUFPAR *parser) { load_r_noutputs = NuFParGetInt(parser); }
static void xRand_OutputChance(NUFPAR *parser) {
    const i32 index = NuFParGetInt(parser);
    load_r_outputChance[index] = NuFParGetInt(parser);
}
static void xAIAssistID(NUFPAR *parser) {
    FLOWBOX_s *box = load_flowbox;
    reinterpret_cast<u8 *>(&box->runtime_id)[0] = NuFParGetInt(parser);
}
static void xName(NUFPAR *parser) {
    NuFParGetWord(parser);
    if (parser->word_buf != NULL) NuStrNCpy(load_name, parser->word_buf, 32);
}
static void xGizmoName(NUFPAR *parser) {
    NuFParGetWord(parser);
    if (parser->word_buf != NULL) NuStrNCpy(load_gizmoname, parser->word_buf, 32);
}
static void xGizmoType(NUFPAR *parser) {
    NuFParGetWord(parser);
    if (parser->word_buf != NULL)
        load_gizmotype = GizmoGetTypeIDByName(load_gizflow->gizmo_sys, parser->word_buf);
}

struct FLOWCONDITIONTYPE_s { i32 id; char *name; nufpcomfn *parse; };
static FLOWCONDITIONTYPE_s ConditionTypes[] = {
    {0, "All", NULL}, {1, "Any", NULL}, {2, "None", NULL}, {3, "Sum", loadSumBox},
    {4, "loop", NULL}, {5, "exactly", loadSumBox}, {-1, NULL, NULL},
};
static void xConditionType(NUFPAR *parser) {
    NuFParGetWord(parser);
    for (i32 index = 0; ConditionTypes[index].id != -1 && load_conditiontype == -1; ++index) {
        if (NuStrICmp(ConditionTypes[index].name, parser->word_buf) == 0) {
            load_conditiontype = index;
            if (ConditionTypes[index].parse != NULL) ConditionTypes[index].parse(parser);
        }
    }
}

static void remapParent(i32 id) {
    if (load_nparents >= 16) return;
    for (i32 index = 0; remap[~id].parents[index] != id; ++index) {
        const i32 parent = remap[~id].parents[index];
        if (parent < 0) remapParent(parent);
        else {
            load_parents[load_nparents] = parent;
            load_parent_output_ix[load_nparents++] = remap[~id].outputs[index];
        }
        if (load_nparents >= 16) return;
    }
}
static void remapChildren(i32 id) {
    if (load_nchildren >= 32) return;
    for (i32 index = 0; remap[~id].children[index] != id; ++index) {
        const i32 child = remap[~id].children[index];
        if (child < 0) remapChildren(child);
        else load_children[load_nchildren++] = child;
        if (load_nchildren >= 32) return;
    }
}
static void xParent(NUFPAR *parser) {
    if (load_nparents < 16) {
        const i32 parent = NuFParGetInt(parser);
        if (parent < 0) {
            NuFParGetInt(parser);
            remapParent(parent);
        } else {
            const i32 index = load_nparents;
            load_parents[index] = parent;
            load_parent_output_ix[index] = NuFParGetInt(parser);
            ++load_nparents;
        }
    }
}
static void xChild(NUFPAR *parser) {
    if (load_nchildren < 32) {
        const i32 child = NuFParGetInt(parser);
        if (child < 0) remapChildren(child);
        else load_children[load_nchildren++] = child;
    }
}
static void xParent_Col(NUFPAR *parser) {
    if (load_nparents < 16) {
        remap[numRemaps].parents[load_nparents] = NuFParGetInt(parser);
        remap[numRemaps].outputs[load_nparents] = NuFParGetInt(parser);
        ++load_nparents;
    }
}
static void xChild_Col(NUFPAR *parser) {
    if (load_nchildren < 32) {
        remap[numRemaps].children[load_nchildren] = NuFParGetInt(parser);
        ++load_nchildren;
    }
}

static void xFlowBox(NUFPAR *);
static void xCollapse(NUFPAR *);
static void xCondition(NUFPAR *);
static void xAction(NUFPAR *);
static void xGizmo(NUFPAR *);
static NUFPCOMJMP cfgtab_GitCount[] = {{"FlowBox", xFlowBoxCount}, {NULL, NULL}};
static NUFPCOMJMP cfgtab_Git[] = {{"FlowBox", xFlowBox}, {"Collapse", xCollapse}, {NULL, NULL}};
static NUFPCOMJMP cfgtab_Collapse[] = {{"Parent", xParent_Col}, {"Child", xChild_Col}, {NULL, NULL}};
static NUFPCOMJMP cfgtab_FlowBox[] = {
    {"Parent", xParent}, {"Child", xChild}, {"Gizmo", xGizmo}, {"Condition", xCondition},
    {"Action", xAction}, {"Name", xName}, {"Num_Gizmos", xNumGizmos}, {"AIAssistId", xAIAssistID}, {NULL, NULL},
};
static NUFPCOMJMP cfgtab_Condition[] = {{"Type", xConditionType}, {"MonitorInputs", xMonitorInputs}, {NULL, NULL}};
static NUFPCOMJMP cfgtab_Gizmo[] = {
    {"Type", xGizmoType}, {"Name", xGizmoName}, {"StartInvisible", xStartInvis},
    {"FinishedInvisible", xEndInvis}, {"FinishedDeactive", xEndDeact}, {"Reverse", xReverse},
    {"RevInvis", xReverseInvis}, {"NotStoryMode", xNotStoryMode}, {"NotFreeplay", xNotFreeplay},
    {"OutputOnly", xOutputOnly}, {"Timer", xGizTimer}, {"RandomTime", xGizRandomTime},
    {"NumRandomOutputs", xRand_NumOutputs}, {"RandomOutputChance", xRand_OutputChance}, {NULL, NULL},
};

static void xCollapse(NUFPAR *parser) {
    load_nparents = load_nchildren = 0;
    NuFParPushCom(parser, cfgtab_Collapse);
    while (NuFParGetLine(parser)) {
        NuFParGetWord(parser);
        if (NuStrICmp(parser->word_buf, "}") == 0) break;
        NuFParInterpretWord(parser);
    }
    NuFParPopCom(parser);
    remap[numRemaps].parents[load_nparents] = ~numRemaps;
    remap[numRemaps].children[load_nchildren] = ~numRemaps;
    ++numRemaps;
}
static void xFlowBox(NUFPAR *parser) {
    load_nparents = load_nchildren = 0;
    load_numgizmos = 1;
    NuFParPushCom(parser, cfgtab_FlowBox);
    while (NuFParGetLine(parser)) {
        NuFParGetWord(parser);
        if (NuStrICmp(parser->word_buf, "}") == 0) break;
        NuFParInterpretWord(parser);
    }
    NuFParPopCom(parser);
    if (load_gizflow != NULL && load_flowbox != NULL) {
        if (load_nparents != 0) {
            load_flowbox->parent_count = load_nparents;
            load_flowbox->parents = reinterpret_cast<FLOWBOX_s **>(GizmoBufferAlloc(load_buff, load_endbuff, load_nparents * sizeof(FLOWBOX_s *)));
            load_flowbox->output_indices = reinterpret_cast<u8 *>(GizmoBufferAlloc(load_buff, load_endbuff, load_nparents));
            for (i32 i = 0; i < load_flowbox->parent_count; ++i) {
                load_flowbox->parents[i] = &load_gizflow->flowboxes[load_parents[i]];
                load_flowbox->output_indices[i] = load_parent_output_ix[i];
            }
        }
        if (load_nchildren != 0) {
            load_flowbox->child_count = load_nchildren;
            load_flowbox->children = reinterpret_cast<FLOWBOX_s **>(GizmoBufferAlloc(load_buff, load_endbuff, load_nchildren * sizeof(FLOWBOX_s *)));
            for (i32 i = 0; i < load_flowbox->child_count; ++i)
                load_flowbox->children[i] = &load_gizflow->flowboxes[load_children[i]];
        }
        load_flowbox->name = reinterpret_cast<char *>(GizmoBufferAlloc(load_buff, load_endbuff, strlen(load_name) + 1));
        NuStrCpy(load_flowbox->name, load_name);
    }
    --load_nflowboxes;
    ++load_flowbox;
}
static void xCondition(NUFPAR *parser) {
    if (load_flowbox == NULL) return;
    load_flowbox->type = 1;
    load_conditiontype = -1;
    load_conditionParam = 0xff;
    NuStrCpy(load_gizmoname, "");
    NuFParPushCom(parser, cfgtab_Condition);
    while (NuFParGetLine(parser)) {
        NuFParGetWord(parser);
        if (NuStrICmp(parser->word_buf, "}") == 0) break;
        NuFParInterpretWord(parser);
    }
    NuFParPopCom(parser);
    if (load_conditiontype >= 0) {
        u8 *data = reinterpret_cast<u8 *>(GizmoBufferAlloc(load_buff, load_endbuff, 4));
        if (data != NULL) { data[0] = load_conditiontype; data[1] = load_conditionParam; }
        load_flowbox->data = reinterpret_cast<FLOWBOXGIZMODATA_s *>(data);
    }
}
static void xAction(NUFPAR *parser) {
    if (load_flowbox == NULL) return;
    load_flowbox->type = 2;
    FLOWACTION_s *previous = NULL;
    char arguments[16][64];
    while (NuFParGetLine(parser)) {
        NuFParGetWord(parser);
        if (NuStrICmp(parser->word_buf, "}") == 0) break;
        if (gizactiondefs == NULL) continue;
        GIZACTIONDEFN_s *definition = gizactiondefs;
        while (definition->name != NULL && NuStrICmp(parser->word_buf, definition->name) != 0) ++definition;
        if (definition->name == NULL) continue;
        FLOWACTION_s *action = reinterpret_cast<FLOWACTION_s *>(GizmoBufferAlloc(load_buff, load_endbuff, sizeof(FLOWACTION_s)));
        if (action == NULL) continue;
        memset(action, 0, sizeof(*action));
        if (previous == NULL) load_flowbox->data = reinterpret_cast<FLOWBOXGIZMODATA_s *>(action);
        else previous->next = action;
        action->definition = definition;
        i32 count = 0;
        while (NuFParGetWord(parser)) {
            if (NuStrCmp(parser->word_buf, "\\") == 0) NuFParGetLine(parser);
            else NuStrCpy(arguments[count++], parser->word_buf);
        }
        if (count != 0) {
            action->arguments = reinterpret_cast<char **>(GizmoBufferAlloc(load_buff, load_endbuff, count * sizeof(char *)));
            if (action->arguments != NULL) {
                action->argument_count = count;
                for (i32 i = 0; i < count; ++i) {
                    const i32 length = NuStrLen(arguments[i]);
                    char *value = NULL;
                    if (length != 0) {
                        value = reinterpret_cast<char *>(GizmoBufferAlloc(load_buff, load_endbuff, length + 1));
                        NuStrCpy(value, arguments[i]);
                    }
                    action->arguments[i] = value;
                }
            }
        }
        previous = action;
    }
}
static void xGizmo(NUFPAR *parser) {
    if (load_flowbox == NULL) return;
    FLOWBOXGIZMODATA_s *data = load_flowbox->data;
    load_flowbox->type = 0;
    load_gizmotype = -1;
    load_g_flags = 0;
    load_t_randomTime = 0;
    if (data == NULL) {
        load_flowbox->data = reinterpret_cast<FLOWBOXGIZMODATA_s *>(GizmoBufferAlloc(load_buff, load_endbuff, sizeof(FLOWBOXGIZMODATA_s)));
        data = load_flowbox->data;
        *reinterpret_cast<i32 *>(data->pad_0x04) = load_numgizmos;
        data->gizmos = reinterpret_cast<GIZMO_s ***>(GizmoBufferAlloc(load_buff, load_endbuff, load_numgizmos * sizeof(GIZMO_s **)));
        data->gizmo_count = 0;
    }
    NuStrCpy(load_gizmoname, "");
    NuFParPushCom(parser, cfgtab_Gizmo);
    while (NuFParGetLine(parser)) {
        NuFParGetWord(parser);
        if (NuStrICmp(parser->word_buf, "}") == 0) break;
        NuFParInterpretWord(parser);
    }
    NuFParPopCom(parser);
    if (load_gizmotype < 0 || NuStrLen(load_gizmoname) == 0) return;
    const i32 name_length = NuStrLen(load_gizmoname);
    const i32 prefix_length = NuStrLen(gizmotypes->types[load_gizmotype].prefix);
    data->gizmos[data->gizmo_count] = reinterpret_cast<GIZMO_s **>(GizmoBufferAlloc(load_buff, load_endbuff, sizeof(FLOWGIZREF_s)));
    FLOWGIZREF_s *ref = reinterpret_cast<FLOWGIZREF_s *>(data->gizmos[data->gizmo_count]);
    ref->name = reinterpret_cast<char *>(GizmoBufferAlloc(load_buff, load_endbuff, name_length + prefix_length + 1));
    NuStrCpy(ref->name, load_gizmoname);
    if (load_gizmotype == giztimer_gizmotype_id) ref->gizmo = createGizTimer(NULL, load_t_time, load_t_randomTime, load_gizmoname);
    else if (load_gizmotype == gizrandom_gizmotype_id) ref->gizmo = createGizRandom(NULL, load_r_noutputs, load_r_outputChance, load_gizmoname);
    else if (load_gizmotype == gizspecial_gizmotype_id) {
        ref->gizmo = createGizSpecial(NULL, load_gizmoname);
        if (ref->gizmo != NULL) {
            NuStrCpy(ref->name, gizmotypes->types[load_gizmotype].prefix);
            NuStrNCat(ref->name, load_gizmoname, 32 - NuStrLen(gizmotypes->types[load_gizmotype].prefix));
        }
    } else ref->gizmo = GizmoFindByName(load_gizflow->gizmo_sys, load_gizmotype, load_gizmoname);
    ++data->gizmo_count;
    load_flowbox->state_flags = (load_flowbox->state_flags & 0x8f23) |
        ((load_g_flags & 1) << 2) | ((load_g_flags & 2) << 2) | ((load_g_flags & 8) << 1) |
        ((load_g_flags & 4) << 4) | (load_g_flags & 0x80) | ((load_g_flags & 0x10) << 8) |
        (((load_g_flags ^ 0x20) & 0x20) << 9) | (((load_g_flags ^ 0x40) & 0x40) << 7);
}

void *LoadGizFlow(void *, GIZMOSYS_s *gizmo_sys, char *path, VARIPTR *buf, VARIPTR *buf_end) {
    FLOWREMAP_s remaps[96];
    numRemaps = 0;
    remap = remaps;
    load_buff = buf;
    load_endbuff = buf_end;
    load_nflowboxes = 0;
    GIZFLOW_s *flow = NULL;
    const NUFILE file = NuFileOpen(path, NUFILE_READ);
    if (file != 0) {
        NUFPAR *parser = NuFParOpen(file);
        if (parser != NULL) {
            NuFParPushCom(parser, cfgtab_GitCount);
            while (NuFParGetLine(parser)) { NuFParGetWord(parser); NuFParInterpretWord(parser); }
            NuFParClose(parser);
        }
        if (load_nflowboxes != 0 && (parser = NuFParOpen(file)) != NULL) {
            flow = reinterpret_cast<GIZFLOW_s *>(GizmoBufferAlloc(load_buff, load_endbuff, sizeof(GIZFLOW_s)));
            if (flow != NULL) {
                flow->flowbox_count = load_nflowboxes;
                flow->gizmo_sys = gizmo_sys;
                load_flowbox = reinterpret_cast<FLOWBOX_s *>(GizmoBufferAlloc(load_buff, load_endbuff, load_nflowboxes * sizeof(FLOWBOX_s)));
                flow->flowboxes = load_flowbox;
                load_gizflow = flow;
                NuFParPushCom(parser, cfgtab_Git);
                while (NuFParGetLine(parser)) { NuFParGetWord(parser); NuFParInterpretWord(parser); }
                flow->pointers_need_reset = 1;
            }
            NuFParClose(parser);
        }
        NuFileClose(file);
    }
    load_buff = load_endbuff = NULL;
    load_gizflow = NULL;
    load_flowbox = NULL;
    return flow;
}

static __used__ i32 Loop_CountLoopingInputsEx(FLOWBOX_s *, FLOWBOX_s *, i32, u8) {
    return 0;
}

static __used__ void CheckIfParentsFinished(GIZFLOW_s *, FLOWBOX_s *) {
}

static __used__ void CheckOutputGizmoFlowBox(GIZFLOW_s *, FLOWBOX_s *, unsigned char) {
}

static __used__ void CheckOutputActionFlowBox(GIZFLOW_s *, FLOWBOX_s *, unsigned char) {
}

static __used__ void CheckOutputConditionFlowBox(GIZFLOW_s *, FLOWBOX_s *, unsigned char) {
}
