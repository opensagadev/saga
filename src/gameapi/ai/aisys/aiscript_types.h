#pragma once

#include "gameapi/ai/aisys/aipath.h"
#include "gameapi/ai/aisys/aimessage_types.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuvec.h"

struct AIAREA_s;
struct APIOBJECT_s;
struct AILOCATOR_s;
struct AILOCATORSET_s;

struct AISCRIPTACTIONDEF_s;
struct AISCRIPTCONDITIONDEF_s;

typedef struct AIREFSCRIPT_s {
    NULISTLNK list_node;
    char *name;
    struct AISCRIPT_s *script;
    char *return_state_name;
    struct AISTATE_s *return_state;
    u32 check_global_scripts : 1;
    u32 check_level_scripts : 1;
    NULISTHDR conditions;
} AIREFSCRIPT;

typedef struct AISTATE_s {
    NULISTLNK list_node;
    NULISTHDR conditions;
    NULISTHDR actions;
    char *name;
    NULISTHDR ref_scripts;
} AISTATE;

typedef struct AIACTION_s {
    NULISTLNK list_node;
    char **params;
    i32 param_count;
    struct AISCRIPTACTIONDEF_s *def;
} AIACTION;

typedef struct AICONDITION_s {
    NULISTLNK list_node;
    f32 param_val;
    char type;
    i8 param_idx;
    u16 bool_and : 1;
    u16 keep_blocked : 1;
    u16 is_param_idx_valid : 1;
    u16 is_complex : 1;
    char *complex_arg;
    char *arg;
    void *void_arg;
    struct AISCRIPTCONDITIONDEF_s *def;
    char *next_state_name;
    AISTATE *next_state;
    struct AICONDITION_s *param_cond;
} AICONDITION;

enum AICONDITION_COMPARISON {
    AICONDITION_EQUAL = 0,
    AICONDITION_LESS_THAN = 1,
    AICONDITION_GREATER_THAN = 2,
    AICONDITION_LESS_THAN_OR_EQUAL = 3,
    AICONDITION_GREATER_THAN_OR_EQUAL = 4,
    AICONDITION_NOT_EQUAL = 5,
};

typedef struct AIACTIONMACRO_s {
    NULISTLNK list_node;
    char *name;
    NULISTHDR actions;
} AIACTIONMACRO;

typedef struct AICONDITIONMACRO_s {
    NULISTLNK list_node;
    char *name;
    NULISTHDR conditions;
} AICONDITIONMACRO;

typedef struct AISCRIPTPARAMS_s {
    char *name;
    f32 default_val;
} AISCRIPTPARAMS;

typedef struct AICONSTPARAMS_s {
    char name[32];
    f32 default_val;
} AICONSTPARAMS;

typedef struct AISCRIPT_s {
    NULISTLNK list_node;
    char *name;
    char *derived_from;
    NULISTHDR states;
    AISCRIPTPARAMS params[4];
    AISTATE *base_state;
    u32 is_level_script : 1;
    u32 is_derived : 1;
    u32 is_derived_from_level_script : 1;
    NULISTHDR ref_scripts;
    NULISTHDR condition_macros;
    NULISTHDR action_macros;
} AISCRIPT;

typedef struct AISCRIPTPROCESSSTACK_s {
    f32 complex_params[4];
    u8 is_first_time_state;
    u8 force_complex_eval;
} AISCRIPTPROCESSSTACK;

typedef struct AISCRIPTPROCESS_s {
    AISCRIPT *base_script;
    AISCRIPT *script;

    AISTATE *state;
    NULISTLNK *action_node;
    AISTATE *next_state;
    f32 params[4];
    f32 script_timer;

    AISCRIPTPROCESSSTACK param_stack[2];

    u32 is_first_time_action : 1;
    u32 is_disabled : 1;
    u32 unknown_flag_4 : 1;

    AIREFSCRIPT *active_refs[4];
    i32 active_ref_count;

    union {
        u8 action_data_1;
        u8 hold_special_button;
    };
    u8 action_data_2;
    u16 action_data_6;
    union {
        void *action_data_3;
        APIOBJECT_s *override_control_object;
    };
    union {
        f32 action_data_4;
        f32 follow_direction_fire_range;
    };
    union {
        f32 action_data_5;
        f32 follow_direction_fire_interval;
    };

    NUVEC action_pos;

    AIPATHINFO path_info;

    f32 action_timer;

    AIAREA_s *unknown_a0;
    union {
        AILOCATOR_s *unknown_a4;
        AILOCATOR_s *locator;
    };
    union {
        AILOCATORSET_s *unknown_a8;
        AILOCATORSET_s *locator_set;
    };
    NUGSPLINE *unknown_ac;

    // Types uncertain.
    union {
        u8 unknown_b0;
        u8 creature_set;
    };
    u16 unknown_b2;

    u8 interrupt_priority;
    u8 interrupt_id;

    u16 action_data_7;

    f32 interrupt_timer;
    AISTATE *interrupt_state;
    AISTATE *return_to_state;

    AILOCALMESSAGE_s *local_messages;
} AISCRIPTPROCESS;
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, unknown_a0) == 0xa0, "Script processor area offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, locator) == 0xa4, "Script processor locator offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, locator_set) == 0xa8, "Script processor locator set offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, unknown_ac) == 0xac, "Script processor spline offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, unknown_b0) == 0xb0, "Script processor creature set offset");

// Android x86 script records: allocation sizes and member accesses in
// AIScriptOpenPakFileParse, AIScriptProcessorInit and AIScriptProcess.
DECOMP_ASSERT(sizeof(AIREFSCRIPT) == 0x24, "AIREFSCRIPT size");
DECOMP_ASSERT(offsetof(AIREFSCRIPT, script) == 0x0c, "AIREFSCRIPT script offset");
DECOMP_ASSERT(offsetof(AIREFSCRIPT, return_state) == 0x14, "AIREFSCRIPT return state offset");
DECOMP_ASSERT(offsetof(AIREFSCRIPT, conditions) == 0x1c, "AIREFSCRIPT conditions offset");
DECOMP_ASSERT(sizeof(AISTATE) == 0x24, "AISTATE size");
DECOMP_ASSERT(offsetof(AISTATE, conditions) == 0x08, "AISTATE conditions offset");
DECOMP_ASSERT(offsetof(AISTATE, actions) == 0x10, "AISTATE actions offset");
DECOMP_ASSERT(offsetof(AISTATE, name) == 0x18, "AISTATE name offset");
DECOMP_ASSERT(offsetof(AISTATE, ref_scripts) == 0x1c, "AISTATE references offset");
DECOMP_ASSERT(sizeof(AIACTION) == 0x14, "AIACTION size");
DECOMP_ASSERT(offsetof(AIACTION, params) == 0x08, "AIACTION parameters offset");
DECOMP_ASSERT(offsetof(AIACTION, def) == 0x10, "AIACTION definition offset");
DECOMP_ASSERT(sizeof(AICONDITION) == 0x2c, "AICONDITION size");
DECOMP_ASSERT(offsetof(AICONDITION, param_val) == 0x08, "AICONDITION value offset");
DECOMP_ASSERT(offsetof(AICONDITION, type) == 0x0c, "AICONDITION comparison offset");
DECOMP_ASSERT(offsetof(AICONDITION, param_idx) == 0x0d, "AICONDITION parameter index offset");
DECOMP_ASSERT(offsetof(AICONDITION, complex_arg) == 0x10, "AICONDITION expression offset");
DECOMP_ASSERT(offsetof(AICONDITION, arg) == 0x14, "AICONDITION argument offset");
DECOMP_ASSERT(offsetof(AICONDITION, void_arg) == 0x18, "AICONDITION initialized argument offset");
DECOMP_ASSERT(offsetof(AICONDITION, def) == 0x1c, "AICONDITION definition offset");
DECOMP_ASSERT(offsetof(AICONDITION, next_state) == 0x24, "AICONDITION next state offset");
DECOMP_ASSERT(offsetof(AICONDITION, param_cond) == 0x28, "AICONDITION operand condition offset");
DECOMP_ASSERT(sizeof(AIACTIONMACRO) == 0x14, "AIACTIONMACRO size");
DECOMP_ASSERT(sizeof(AICONDITIONMACRO) == 0x14, "AICONDITIONMACRO size");
DECOMP_ASSERT(sizeof(AISCRIPTPARAMS) == 0x08, "AISCRIPTPARAMS size");
DECOMP_ASSERT(sizeof(AICONSTPARAMS) == 0x24, "AICONSTPARAMS size");
DECOMP_ASSERT(sizeof(AISCRIPT) == 0x58, "AISCRIPT size");
DECOMP_ASSERT(offsetof(AISCRIPT, states) == 0x10, "AISCRIPT states offset");
DECOMP_ASSERT(offsetof(AISCRIPT, params) == 0x18, "AISCRIPT parameters offset");
DECOMP_ASSERT(offsetof(AISCRIPT, base_state) == 0x38, "AISCRIPT base state offset");
DECOMP_ASSERT(offsetof(AISCRIPT, ref_scripts) == 0x40, "AISCRIPT references offset");
DECOMP_ASSERT(offsetof(AISCRIPT, condition_macros) == 0x48, "AISCRIPT condition macros offset");
DECOMP_ASSERT(offsetof(AISCRIPT, action_macros) == 0x50, "AISCRIPT action macros offset");
DECOMP_ASSERT(sizeof(AISCRIPTPROCESSSTACK) == 0x14, "AISCRIPTPROCESSSTACK stride");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESSSTACK, is_first_time_state) == 0x10, "Script operand cache validity offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESSSTACK, force_complex_eval) == 0x11, "Script operand cache fill flag offset");
DECOMP_ASSERT(sizeof(AISCRIPTPROCESS) == 0xc8, "AISCRIPTPROCESS size");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, hold_special_button) == 0x68, "Special button hold state offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, override_control_object) == 0x6c, "AI override saved object offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, params) == 0x14, "Script processor parameters offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, param_stack) == 0x28, "Script processor operand caches offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, active_refs) == 0x54, "Script processor reference stack offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, active_ref_count) == 0x64, "Script processor reference depth offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, action_pos) == 0x78, "Script processor action position offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, follow_direction_fire_range) == 0x70, "Follow-direction fire range offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, follow_direction_fire_interval) == 0x74,
              "Follow-direction fire interval offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, path_info) == 0x84, "Script processor action path offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, action_timer) == 0x9c, "Script processor action timer offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, interrupt_timer) == 0xb8, "Script processor interrupt timer offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, interrupt_state) == 0xbc, "Script processor interrupt state offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, return_to_state) == 0xc0, "Script processor return state offset");
DECOMP_ASSERT(offsetof(AISCRIPTPROCESS, local_messages) == 0xc4, "Script processor local-message head offset");
