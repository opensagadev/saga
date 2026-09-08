#include "decomp.h"
#include <string.h>
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/gizmo/base/gizmo.h"
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

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" {
    extern i16 id_BATMAN;
    extern i16 id_ROBIN;
    extern i16 id_BODYGUARD, id_GEONOSIAN, id_CHEWBACCA;
}

struct SPECIAL_LAYER_s {
    i16 *character_id;
    char *name;
    u32 mask;
};

DECOMP_ASSERT(sizeof(SPECIAL_LAYER_s) == 0xc, "SPECIAL_LAYER_s size");

static SPECIAL_LAYER_s SpecialLayer[] = {
    {&id_BATMAN, "bombbackpack", 0},       {&id_BATMAN, "sonargun", 0},          {&id_BATMAN, "infrared_goggles", 0},
    {&id_BATMAN, "mask_black", 0},         {&id_BATMAN, "mask_blue", 0},         {&id_BATMAN, "mask_red", 0},
    {&id_BATMAN, "cape_black", 0},         {&id_BATMAN, "cape_blue", 0},         {&id_BATMAN, "body_grey_nextgen", 0},
    {&id_BATMAN, "body_grey_high", 0},     {&id_BATMAN, "body_grey_low", 0},     {&id_BATMAN, "body_black_nextgen", 0},
    {&id_BATMAN, "body_black_high", 0},    {&id_BATMAN, "body_black_low", 0},    {&id_BATMAN, "body_blue_nextgen", 0},
    {&id_BATMAN, "body_blue_high", 0},     {&id_BATMAN, "body_blue_low", 0},     {&id_BATMAN, "face_hands_black", 0},
    {&id_BATMAN, "face_hands_blue", 0},    {&id_BATMAN, "face_hands_red", 0},    {&id_BATMAN, "hips_black_nextgen", 0},
    {&id_BATMAN, "hips_black_high", 0},    {&id_BATMAN, "hips_blue_nextgen", 0}, {&id_BATMAN, "hips_blue_high", 0},
    {&id_BATMAN, "hips_red_nextgen", 0},   {&id_BATMAN, "hips_red_high", 0},     {&id_ROBIN, "magnetic_boots", 0},
    {&id_ROBIN, "scuba_gear", 0},          {&id_ROBIN, "hack_pack", 0},          {&id_ROBIN, "vacuum_gun", 0},
    {&id_ROBIN, "nextgen_limbs_green", 0}, {&id_ROBIN, "hires_limbs_green", 0},  {&id_ROBIN, "lowres_limbs_green", 0},
    {&id_ROBIN, "nextgen_limbs_white", 0}, {&id_ROBIN, "hires_limbs_white", 0},  {&id_ROBIN, "lowres_limbs_white", 0},
    {&id_ROBIN, "nextgen_limbs_grey", 0},  {&id_ROBIN, "hires_limbs_grey", 0},   {&id_ROBIN, "lowres_limbs_grey", 0},
    {&id_ROBIN, "nextgen_limbs_blue", 0},  {&id_ROBIN, "hires_limbs_blue", 0},   {&id_ROBIN, "lowres_limbs_blue", 0},
};

// Shares the original translation-unit-local layer table with FixUpLayers.
u32 AdjustLayerBits(u32 mask, GameObject_s *object) {
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    u32 cape = data->cape_layer == -1 ? 0 : 1u << (static_cast<u32>(data->cape_layer) & 31);
    u32 hair = data->hair_layer == -1 ? 0 : 1u << (static_cast<u32>(data->hair_layer) & 31);
    if (object->field_0x108e != 0)
        mask &= ~hair;
    SUIT_s *suit = static_cast<SUIT_s *>(object->suit);
    if (suit != NULL && object->id == id_BATMAN) {
        if ((suit->flags & 4) != 0) {
            mask = (mask & ~cape) | SpecialLayer[0].mask;
            mask = (mask & ~SpecialLayer[3].mask) | SpecialLayer[5].mask;
            if ((mask & SpecialLayer[20].mask) != 0)
                mask = (mask & ~SpecialLayer[20].mask) | SpecialLayer[24].mask;
            else if ((mask & SpecialLayer[21].mask) != 0)
                mask = (mask & ~SpecialLayer[21].mask) | SpecialLayer[25].mask;
            mask = (mask & ~SpecialLayer[17].mask) | SpecialLayer[19].mask;
        } else if ((suit->flags & 2) != 0) {
            mask &= ~cape;
        } else if ((suit->flags & 8) != 0) {
            mask = (mask & ~cape) | SpecialLayer[1].mask | SpecialLayer[7].mask;
            mask = (mask & ~SpecialLayer[3].mask) | SpecialLayer[4].mask;
            if ((mask & SpecialLayer[20].mask) != 0)
                mask = (mask & ~SpecialLayer[20].mask) | SpecialLayer[22].mask;
            else if ((mask & SpecialLayer[21].mask) != 0)
                mask = (mask & ~SpecialLayer[21].mask) | SpecialLayer[23].mask;
            mask = (mask & ~SpecialLayer[17].mask) | SpecialLayer[18].mask;
            if ((mask & SpecialLayer[8].mask) != 0)
                mask = (mask & ~SpecialLayer[8].mask) | SpecialLayer[14].mask;
            else if ((mask & SpecialLayer[9].mask) != 0)
                mask = (mask & ~SpecialLayer[9].mask) | SpecialLayer[15].mask;
            else if ((mask & SpecialLayer[10].mask) != 0)
                mask = (mask & ~SpecialLayer[10].mask) | SpecialLayer[16].mask;
        } else if ((suit->flags & 1) != 0) {
            mask |= SpecialLayer[2].mask;
            if ((mask & SpecialLayer[8].mask) != 0)
                mask = (mask & ~SpecialLayer[8].mask) | SpecialLayer[11].mask;
            else if ((mask & SpecialLayer[9].mask) != 0)
                mask = (mask & ~SpecialLayer[9].mask) | SpecialLayer[12].mask;
            else if ((mask & SpecialLayer[10].mask) != 0)
                mask = (mask & ~SpecialLayer[10].mask) | SpecialLayer[13].mask;
        }
    } else if (suit != NULL && object->id == id_ROBIN) {
        if ((suit->flags & 0x10) != 0) {
            mask = (mask & ~(hair | cape)) | SpecialLayer[27].mask;
            if ((mask & SpecialLayer[30].mask) != 0)
                mask = (mask & ~SpecialLayer[30].mask) | SpecialLayer[39].mask;
            else if ((mask & SpecialLayer[31].mask) != 0)
                mask = (mask & ~SpecialLayer[31].mask) | SpecialLayer[40].mask;
            else if ((mask & SpecialLayer[32].mask) != 0)
                mask = (mask & ~SpecialLayer[32].mask) | SpecialLayer[41].mask;
        } else if ((suit->flags & 0x40) != 0) {
            mask = (mask & ~cape) | SpecialLayer[26].mask;
            if ((mask & SpecialLayer[30].mask) != 0)
                mask = (mask & ~SpecialLayer[30].mask) | SpecialLayer[36].mask;
            else if ((mask & SpecialLayer[31].mask) != 0)
                mask = (mask & ~SpecialLayer[31].mask) | SpecialLayer[37].mask;
            else if ((mask & SpecialLayer[32].mask) != 0)
                mask = (mask & ~SpecialLayer[32].mask) | SpecialLayer[38].mask;
        } else if ((suit->flags & 0x20) != 0) {
            mask = (mask & ~cape) | SpecialLayer[28].mask;
            if ((mask & SpecialLayer[30].mask) != 0)
                mask = (mask & ~SpecialLayer[30].mask) | SpecialLayer[33].mask;
            else if ((mask & SpecialLayer[31].mask) != 0)
                mask = (mask & ~SpecialLayer[31].mask) | SpecialLayer[34].mask;
            else if ((mask & SpecialLayer[32].mask) != 0)
                mask = (mask & ~SpecialLayer[32].mask) | SpecialLayer[35].mask;
        } else if ((suit->flags & 0x80) != 0) {
            mask = (mask & ~cape) | SpecialLayer[29].mask;
        }
    }
    if (object->id == id_BODYGUARD) {
        if (object->current_hp <= 1)
            mask &= ~0x10u;
    } else if (object->id == id_GEONOSIAN) {
        mask |= (object->field_0xefd & 2) != 0 ? 0x20 : 0x40;
    } else if (CharacterCustomiser != NULL && object->id == CharacterCustomiser->character_ids[0]) {
        if ((CharacterCustomiser->pieces[static_cast<u16>(Game.customizer.pieces[5])].layer_flags & 0x40) == 0)
            mask |= cape;
    } else if (CharacterCustomiser != NULL && object->id == CharacterCustomiser->character_ids[1]) {
        if ((CharacterCustomiser->pieces[static_cast<u16>(Game.customizer.secondary_pieces[5])].layer_flags & 0x40) ==
            0)
            mask |= cape;
    } else if (object->id == id_CHEWBACCA) {
        if (Cheat_IsOn(4) != 0)
            mask |= 0xc0;
        data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    }
    if (data->ride_layers_off != 0 && object->field_0xcc0 != NULL && object->field_0x7a5 == 0x3b)
        mask &= ~data->ride_layers_off;
    return mask;
}

i32 LayerFromName(GAMECHARACTERDATA_s *character, char *name);

void FixUpLayers() {
    for (i32 model_index = 0; model_index < apicharsys->loaded_model_count; ++model_index) {
        CHARACTERMODEL_s *model = &apicharsys->models[model_index];
        GAMECHARACTERDATA_s *character = &GCDataList[model->model_id];

        for (i32 layer_index = 0; layer_index < character->layer_count; ++layer_index) {
            GAMECHARACTERLAYER_s *layer = &character->layers[layer_index];
            layer->hierarchy_layer_index = NuHGobjGetLayerIndex(layer->name, model->hierarchy);
        }
    }

    SPECIAL_LAYER_s *layer = SpecialLayer;
    SPECIAL_LAYER_s *layer_end = SpecialLayer + sizeof(SpecialLayer) / sizeof(SpecialLayer[0]);
    for (; layer != layer_end; ++layer) {
        layer->mask = 0;
        if (layer->character_id != NULL && *layer->character_id != -1) {
            layer->mask = 1 << LayerFromName(&GCDataList[*layer->character_id], layer->name);
        }
    }
}

i32 LayerFromName(GAMECHARACTERDATA_s *character, char *name) {
    for (i32 i = 0; i < character->layer_count; ++i) {
        if (NuStrICmp(name, character->layers[i].name) == 0) {
            return character->layers[i].mask_bit;
        }
    }
    return -1;
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
