// Original RTL TU basename is rtl.c; the source is compiled as C++.
#include "decomp.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/world/world.h"
#include "legoapi/render/core/render.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/numtl.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nupostparams.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nulst.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"

#include <math.h>
#include <float.h>
#include <stdio.h>
#include <string.h>

struct nuqtdim_s;
struct nuqthdr_s;
struct rtl_s;
struct rtlindex_s {
    i32 enabled;
    u8 reserved_04[8];
    i32 width;
    i32 depth;
    f32 scale;
    f32 offset_x;
    f32 offset_z;
    char **cells;
    u8 reserved_24[4];
};
DECOMP_ASSERT(sizeof(rtlindex_s) == 0x28, "RTL index size");
struct rtlidata_s {
    union {
        u8 data[sizeof(rtldata_s)];
        struct {
            rtl_s *directional_lights[3];
            f32 directional_strengths[3];
            rtl_s *ambient_lights[3];
            f32 ambient_strengths[3];
            rtl_s *anti_lights[3];
            f32 anti_strengths[3];
            i32 anti_light_count;
            rtl_s *cached_light;
            NUVEC shadow_direction;
            f32 cached_value;
            NUVEC previous_shadow_direction;
            f32 previous_shadow_value;
            f32 shadow_blend;
            u16 cached_light_uid;
            u8 reserved_76[2];
            NUVEC intensity[3];
            NUVEC direction[3];
            NUVEC ambient;
            u8 reserved_cc[0x54];
            f32 field_120;
            NUVEC blended_shadow_direction;
            f32 blended_shadow_value;
            NUVEC field_134;
            f32 specular_value;
        };
    };
};
DECOMP_ASSERT(sizeof(rtlidata_s) == 0x144, "rtlidata_s size");
DECOMP_ASSERT(offsetof(rtlidata_s, anti_light_count) == 0x48, "rtlidata_s anti-light count offset");
DECOMP_ASSERT(offsetof(rtlidata_s, cached_light) == 0x4c, "rtlidata_s cached light offset");
DECOMP_ASSERT(offsetof(rtlidata_s, cached_light_uid) == 0x74, "rtlidata_s cached UID offset");
DECOMP_ASSERT(offsetof(rtlidata_s, intensity) == 0x78, "rtlidata_s intensity offset");
DECOMP_ASSERT(offsetof(rtlidata_s, direction) == 0x9c, "rtlidata_s direction offset");
DECOMP_ASSERT(offsetof(rtlidata_s, ambient) == 0xc0, "rtlidata_s ambient offset");
DECOMP_ASSERT(offsetof(rtlidata_s, blended_shadow_direction) == 0x124, "rtlidata_s blended shadow offset");
struct NUFRUSTRUM;

static NULSTHDR *rtl_dynamic_pool;
static i32 rtl_dynamic_lights_enabled = 1;
static f32 rtl_shadow_blend_rate = 2.0f;
static NUVEC rtl_shadow_flicker = {0.1f, 0.1f, 0.1f};
static i32 rtl_dynamic_max;
static i32 rtl_dynamic_cnt;
static f32 rtl_frametime;
static i16 rtl_uid = 1;
i32 rtl_error = 0;
u16 rtltimer1 = 0;
f32 edrtl_text_scale = 1.0f;
static f32 default_modifiers = 1.0f;
f32 *modifiers = &default_modifiers;
i32 modifier_cnt = 1;
extern char **modifier_names;
static i32 curFogLoc = -1;
f32 rtltimer1adv = 2500.0f;
static i32 numsegs = 16;
static i32 hide_types[9];
f32 min_r = 1.0f;
f32 def_fr = 2.0f;
static i32 fogmode;
static NUCAMERA *usr_cam;
static NUMTL *mtls[3];
static i32 edrtl_mode;
static i32 ed_just_entered;
static i32 rtl_near_clip_at_cursor;
static f32 lockflash;
static f32 lockflash_rate = 0.7f;
static i32 maxundo;
static rtl_s *rtl_undo;
static rtl_s **curr_rtl_undo;
static rtl_s **rtl_locked_undo;
static rtl_s **base_rtl_undo;
static NUVEC *curpos_undo;
static i32 rtl_undo_cnt;
static i32 rtl_undo_maxcnt;
static i32 rtl_undo_ix;
static eduiitem_s *undo_item;
static eduiitem_s *redo_item;
static NUVEC pcpos;
static i32 helpmode;
static i32 menu_cancelled;
static i32 peax;
static i32 peay;
static f32 scale_rate = 0.1f;
static f32 camscale_factor = 0.1f;
static u32 ctl[2][8] = {
    {0x80, 0x10, 0x40, 0x1000, 0x100, 0x20, 0x8000, 0x2000},
    {0x40, 0x10, 0x20, 0x1000, 0x80, 0x100, 0x4, 0x8},
};

extern "C" {
    rtlset *curr_set = NULL;
    rtl_s *curr_rtl;
    rtl_s *rtl_locked;
    rtl_s *base_rtl;
    i32 RTL_EditorActive;
    i32 rtled_menu_active;
    eduimenu_s *edrtl_active_menu;
    extern rtlfog_s *curr_fog;
    extern rtl_s clipboard_light;
}

extern "C" {
    static void NuVecClear(NUVEC *v) {
        v->x = v->y = v->z = 0.0f;
    }
}

extern "C" {
    static __used__ void NuRndrSetDirectionalLights(NUVEC *dir0, NUCOLOUR3 *colour0, NUVEC *dir1, NUCOLOUR3 *colour1,
                                                    NUVEC *dir2, NUCOLOUR3 *colour2) {
        NuRndrSetDirectionalLightsPS(dir0, colour0, dir1, colour1, dir2, colour2);
    }

    static __used__ void NuRndrSetAmbientLight(NUCOLOUR3 *colour) {
        NuRndrSetAmbientLightPS(colour);
    }

    static __used__ void NuRndrSetSpecularLight(nuvec_s *direction, nucolour4_s *intensity) {
        if (direction) {
            NuRndrLightingStateCurrent.specular_direction = *direction;
            NuRndrLightingStateCurrent.field_0x60 = 0;
        } else {
            NuRndrLightingStateCurrent.field_0x60 = 1;
        }
        if (intensity) {
            NuRndrLightingStateCurrent.specular_intensity = *intensity;
            NuRndrLightingStateCurrent.field_0x74 = 1;
        } else {
            NuRndrLightingStateCurrent.field_0x74 = 0;
        }
        NuRndrSetSpecularLightPS(direction, intensity);
    }
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

extern "C" {
    void rtlSetAssocName(i32 index, char *name);
    void rtlSetUserIdName(i32 index, char *name);

    i32 rtlFindByUserId(usize rtl_set, i32 user_id) {
        if (rtl_set != 0) {
            rtlset *set = reinterpret_cast<rtlset *>(rtl_set);
            for (i32 i = 0; i < 128; ++i) {
                if (set->lights[i].type != 0 && set->lights[i].field_68 == user_id) {
                    return i;
                }
            }
        }
        return -1;
    }

    i32 rtlGetDirection(usize rtl_set, i32 id, void **out) {
        if (rtl_set != 0 && out != NULL) {
            rtlset *set = reinterpret_cast<rtlset *>(rtl_set);
            if (id < 0 || id > 128)
                return 0;
            *out = &set->lights[id].direction;
            return 1;
        }
        return 0;
    }

    i32 rtlDynamicAlloc(void) {
        if (rtl_dynamic_pool == NULL)
            return -1;
        rtl_s *light = reinterpret_cast<rtl_s *>(NuLstAllocTail(rtl_dynamic_pool));
        if (light != NULL) {
            light->type = 4;
            NuVecClear(&light->position);
            light->inner_radius = 1.0f;
            light->outer_radius = 2.0f;
            light->colour.x = 1.0f;
            light->colour.y = 1.0f;
            light->colour.z = 1.0f;
            light->secondary_colour.x = 0.5f;
            light->secondary_colour.y = 0.5f;
            light->secondary_colour.z = 0.5f;
            light->type = 2;
            light->disabled = 0;
            light->field_5e = 0;
            light->field_60 = 0;
            light->parameters[0] = 0.1f;
            light->parameters[1] = 0.1f;
            light->parameters[2] = 0.1f;
            light->parameters[3] = 0.1f;
            light->parameter_54 = 0.0f;
            light->pitch = 0;
            light->yaw = 0;
            light->direction.x = 0.0f;
            light->direction.y = 0.0f;
            light->direction.z = 1.0f;
            light->field_64 = 0.0f;
            light->intensity = 1.0f;
            light->field_7a = -1;
            light->field_79 = -1;
            light->field_7b = 0;
            light->field_7c = 0;
            NuVecRotateX(&light->direction, &light->direction, light->pitch);
            NuVecRotateY(&light->direction, &light->direction, light->yaw);
            light->uid = rtl_uid++;
            if (rtl_uid == 0)
                ++rtl_uid;
            ++rtl_dynamic_cnt;
            NULNKHDR *header = reinterpret_cast<NULNKHDR *>(light) - 1;
            return header->id;
        }
        return -1;
    }

    void rtlDynamicFree(i32 id) {
        if (rtl_dynamic_pool != NULL && id >= 0 && id < rtl_dynamic_max) {
            NULNKHDR *light = NuLstGetByIdx(rtl_dynamic_pool, id);
            if (light != NULL) {
                NuLstFree(light);
                --rtl_dynamic_cnt;
            }
        }
    }

    i32 rtlDynamicAllocTemplate(rtlset *set, i32 user_id) {
        i32 id = -1;
        i32 template_id = rtlFindByUserId(reinterpret_cast<usize>(set), user_id);
        if (template_id >= 0) {
            id = rtlDynamicAlloc();
            if (id >= 0) {
                rtl_s *light = reinterpret_cast<rtl_s *>(NuLstGetByIdx(rtl_dynamic_pool, id));
                rtlset *source = set;
                *light = source->lights[template_id];
                return id;
            }
        }
        return -1;
    }

    i32 rtlDynamicSetType(i32 id, i32 type) {
        if (rtl_dynamic_pool == NULL || id < 0 || id >= rtl_dynamic_max || type <= 0 || type >= 9)
            return 0;
        rtl_s *light = reinterpret_cast<rtl_s *>(NuLstGetByIdx(rtl_dynamic_pool, id));
        if (light == NULL)
            return 0;
        light->type = type;
        return 1;
    }

    i32 rtlDynamicSetPos(i32 id, NUVEC *position) {
        if (rtl_dynamic_pool == NULL || id < 0 || id >= rtl_dynamic_max)
            return 0;
        rtl_s *light = reinterpret_cast<rtl_s *>(NuLstGetByIdx(rtl_dynamic_pool, id));
        if (light == NULL || position == NULL)
            return 0;
        light->position = *position;
        return 1;
    }

    i32 rtlDynamicSetRadii(i32 id, f32 inner, f32 outer) {
        if (rtl_dynamic_pool == NULL || id < 0 || id >= rtl_dynamic_max)
            return 0;
        rtl_s *light = reinterpret_cast<rtl_s *>(NuLstGetByIdx(rtl_dynamic_pool, id));
        if (light == NULL)
            return 0;
        light->inner_radius = inner;
        if (outer < inner)
            outer = inner;
        light->outer_radius = outer;
        return 1;
    }

    i32 rtlDynamicSetColours(i32 id, NUVEC *colour, NUVEC *secondary) {
        if (rtl_dynamic_pool == NULL || id < 0 || id >= rtl_dynamic_max)
            return 0;
        rtl_s *light = reinterpret_cast<rtl_s *>(NuLstGetByIdx(rtl_dynamic_pool, id));
        if (light == NULL || (colour == NULL && secondary == NULL))
            return 0;
        if (colour != NULL)
            light->colour = *colour;
        if (secondary != NULL)
            light->secondary_colour = *secondary;
        return 1;
    }

    bool rtlDynamicEnable(i32 id, i32 enabled) {
        if (rtl_dynamic_pool == NULL || id < 0 || id >= rtl_dynamic_max)
            return false;
        rtl_s *light = reinterpret_cast<rtl_s *>(NuLstGetByIdx(rtl_dynamic_pool, id));
        if (light == NULL)
            return false;
        bool previous = (light->flags & 1) == 0;
        light->flags = (light->flags & ~1) | (enabled == 0);
        return previous;
    }

    i32 rtlDynamicSetDirection(i32 id, NUVEC *direction) {
        if (rtl_dynamic_pool == NULL || id < 0 || id >= rtl_dynamic_max)
            return 0;
        rtl_s *light = reinterpret_cast<rtl_s *>(NuLstGetByIdx(rtl_dynamic_pool, id));
        if (light != NULL && direction != NULL) {
            light->direction = *direction;
            return 1;
        }
        return 0;
    }

    i32 rtlDynamicMasterEnable(i32 enabled) {
        i32 previous = rtl_dynamic_lights_enabled;
        rtl_dynamic_lights_enabled = enabled;
        return previous;
    }

    i32 rtlResetDynamic(void) {
        if (rtl_dynamic_pool != NULL) {
            NULNKHDR *entry = NuLstGetNext(rtl_dynamic_pool, NULL);
            while (entry != NULL) {
                NULNKHDR *next = NuLstGetNext(rtl_dynamic_pool, entry);
                NuLstFree(entry);
                entry = next;
            }
            rtl_dynamic_cnt = 0;
        }
        return rtl_dynamic_max;
    }

    i32 rtlInitDynamic(VARIPTR *buffer, VARIPTR end, i32 max_lights) {
        rtl_dynamic_pool = NuLstCreateBuff(max_lights, sizeof(rtl_s), buffer, end, 0x10);
        rtl_dynamic_max = max_lights;
        rtl_dynamic_cnt = 0;
        return max_lights;
    }

    void rtlSetShadowFlickerScale(NUVEC *scale) {
        rtl_shadow_flicker = *scale;
    }

    void rtlSetShadowFlickerBlendTime(f32 time) {
        if (time != 0.0f)
            rtl_shadow_blend_rate = 1.0f / time;
        else
            rtl_shadow_blend_rate = 10000.0f;
    }
}

static __used__ void rtlSwapEndianess32(void *) {
}

void rtlSwapSetEndianess(rtlset *) {
}

extern "C" {
    void IndexLights(rtlset *set, VARIPTR *buffer, i32 buffer_end) {
        struct RTLGRID {
            u32 valid;
            u32 reserved_04;
            u32 reserved_08;
            i32 columns;
            i32 rows;
            f32 scale;
            f32 x_offset;
            f32 z_offset;
            u32 cells;
            u32 reserved_24;
        };

        RTLGRID *grid = static_cast<RTLGRID *>(buffer->void_ptr);
        buffer->addr += sizeof(RTLGRID);
        memset(grid, 0, sizeof(*grid));

        f32 minimum_x = FLT_MAX;
        f32 maximum_x = -FLT_MAX;
        f32 minimum_z = FLT_MAX;
        f32 maximum_z = -FLT_MAX;
        for (i32 index = 0; index < 128 && set->lights[index].type != 0; ++index) {
            rtl_s *light = &set->lights[index];
            minimum_x = MIN(minimum_x, light->position.x - light->outer_radius);
            maximum_x = MAX(maximum_x, light->position.x + light->outer_radius);
            minimum_z = MIN(minimum_z, light->position.z - light->outer_radius);
            maximum_z = MAX(maximum_z, light->position.z + light->outer_radius);
        }

        const f32 width = maximum_x - minimum_x;
        const f32 depth = maximum_z - minimum_z;
        if (minimum_x > maximum_x || minimum_z > maximum_z || width <= 0.0f || depth <= 0.0f) {
            return;
        }

        grid->x_offset = -minimum_x;
        grid->z_offset = -minimum_z;
        if (width <= depth) {
            grid->rows = 16;
            grid->columns = MIN(static_cast<i32>(width * 16.0f / depth) + 1, 16);
            grid->scale = 16.0f / depth;
        } else {
            grid->columns = 16;
            grid->rows = MIN(static_cast<i32>(depth * 16.0f / width) + 1, 16);
            grid->scale = 16.0f / width;
        }

        buffer->addr = ALIGN(buffer->addr, 4);
        grid->cells = static_cast<u32>(buffer->addr);
        u32 *cells = buffer->u32_ptr;
        buffer->addr += grid->columns * grid->rows * sizeof(u32);

        for (i32 row = 0; row < grid->rows; ++row) {
            const f32 minimum_cell_z = static_cast<f32>(row) / grid->scale - grid->z_offset;
            const f32 maximum_cell_z = static_cast<f32>(row + 1) / grid->scale - grid->z_offset;
            for (i32 column = 0; column < grid->columns; ++column) {
                const f32 minimum_cell_x = static_cast<f32>(column) / grid->scale - grid->x_offset;
                const f32 maximum_cell_x = static_cast<f32>(column + 1) / grid->scale - grid->x_offset;
                u8 *indices = buffer->u8_ptr;
                cells[row * grid->columns + column] = static_cast<u32>(buffer->addr);
                *indices = 0;
                ++buffer->addr;

                for (i32 index = 0; index < 128 && set->lights[index].type != 0; ++index) {
                    rtl_s *light = &set->lights[index];
                    const bool directional = light->type == 5;
                    const bool overlaps = light->position.x + light->outer_radius >= minimum_cell_x &&
                                          light->position.x - light->outer_radius <= maximum_cell_x &&
                                          light->position.z + light->outer_radius >= minimum_cell_z &&
                                          light->position.z - light->outer_radius <= maximum_cell_z;
                    if (directional || overlaps) {
                        ++indices[0];
                        indices[indices[0]] = static_cast<u8>(index);
                        ++buffer->addr;
                    }
                }
            }
        }
        (void)buffer_end;
        grid->valid = 1;
    }

    rtlset *rtlLoadSet(char *path, VARIPTR *buffer, i32 buffer_end) {
        buffer->addr = ALIGN(buffer->addr, 4);
        rtlset *set = static_cast<rtlset *>(buffer->void_ptr);
        memset(set, 0, 0x4f84);

        if (NuFileLoadBuffer(path, set, buffer_end - buffer->addr) > 0) {
            rtlSwapSetEndianess(set);
        }

        u8 *bytes = reinterpret_cast<u8 *>(set);
        for (i32 i = 0; i < 0x80; ++i) {
            *reinterpret_cast<i16 *>(bytes + i * 0x8c + 0x6e) = static_cast<i16>(i + 1);
            *reinterpret_cast<rtlset **>(bytes + i * 0x8c + 0x80) = set;
            *reinterpret_cast<f32 *>(bytes + i * 0x8c + 0x70) = 1.0f;
        }
        *reinterpret_cast<u32 *>(bytes) = 5;
        buffer->addr += 0x4f84;
        IndexLights(set, buffer, buffer_end);
        return set;
    }

    void rtlSaveSet(char *path, rtlset *set) {
        rtlSwapSetEndianess(set);
        i32 file = NuFileOpen(path, NUFILE_WRITE);
        if (file != 0) {
            NuFileWrite(file, set, sizeof(rtlset));
            NuFileClose(file);
        }
        rtlSwapSetEndianess(set);
    }

    rtlset *rtlGetCurrentSet(void) {
        return curr_set;
    }

    void rtlResetEx(rtldata_s *data, i32 reset_cached) {
        const NUVEC black = {0.0f, 0.0f, 0.0f};
        rtlidata_s *lighting_data = reinterpret_cast<rtlidata_s *>(data);
        for (i32 index = 0; index < 3; ++index) {
            lighting_data->ambient_lights[index] = NULL;
            lighting_data->directional_lights[index] = lighting_data->ambient_lights[index];
            lighting_data->ambient_strengths[index] = 0.0f;
            lighting_data->directional_strengths[index] = lighting_data->ambient_strengths[index];
        }
        lighting_data->anti_light_count = 0;
        lighting_data->field_120 = 1.0f;
        lighting_data->ambient = black;
        lighting_data->intensity[2] = lighting_data->ambient;
        lighting_data->intensity[1] = lighting_data->intensity[2];
        lighting_data->intensity[0] = lighting_data->intensity[1];
        lighting_data->direction[2] = nuvec_x;
        lighting_data->direction[1] = lighting_data->direction[2];
        lighting_data->direction[0] = lighting_data->direction[1];
        lighting_data->field_134.x = 1.0f;
        lighting_data->field_134.y = 0.0f;
        lighting_data->field_134.z = 0.0f;
        if (reset_cached != 0) {
            lighting_data->cached_light = NULL;
            lighting_data->cached_value = 0.0f;
            lighting_data->shadow_blend = 0.0f;
            lighting_data->blended_shadow_value = 0.0f;
            lighting_data->cached_light_uid = 0;
        }
    }

    void rtlReset(rtldata_s *data) {
        rtlResetEx(data, 0);
    }
}

static __used__ void InsertLight(rtl_s *light, rtlidata_s *lighting_data, float strength) {
    rtldata_s *data = reinterpret_cast<rtldata_s *>(lighting_data);
    const i32 pointer_offset = light->type == 1 ? 0x18 : 0x00;
    const i32 strength_offset = light->type == 1 ? 0x24 : 0x0c;
    for (i32 slot = 0; slot < 3; ++slot) {
        if (*reinterpret_cast<f32 *>(data->data + strength_offset + slot * 4) < strength) {
            for (i32 move = 2; move > slot; --move) {
                *reinterpret_cast<rtl_s **>(data->data + pointer_offset + move * 4) =
                    *reinterpret_cast<rtl_s **>(data->data + pointer_offset + (move - 1) * 4);
                *reinterpret_cast<f32 *>(data->data + strength_offset + move * 4) =
                    *reinterpret_cast<f32 *>(data->data + strength_offset + (move - 1) * 4);
            }
            *reinterpret_cast<rtl_s **>(data->data + pointer_offset + slot * 4) = light;
            *reinterpret_cast<f32 *>(data->data + strength_offset + slot * 4) = strength;
            return;
        }
    }
}

static __used__ void InsertAntiLight(rtl_s *light, rtlidata_s *lighting_data, float strength) {
    rtldata_s *data = reinterpret_cast<rtldata_s *>(lighting_data);
    i32 *count = reinterpret_cast<i32 *>(data->data + 0x48);
    if (*count < 3) {
        *reinterpret_cast<rtl_s **>(data->data + 0x30 + *count * 4) = light;
        *reinterpret_cast<f32 *>(data->data + 0x3c + *count * 4) = strength;
        ++*count;
    }
}

static __used__ f32 ApplyAntilights(rtl_s *light, rtlidata_s *lighting_data, f32 strength) {
    f32 anti_strength = 0.0f;
    for (i32 index = 0; index < lighting_data->anti_light_count; ++index) {
        if (lighting_data->anti_lights[index]->field_5e == 0 ||
            (static_cast<u16>(lighting_data->anti_lights[index]->field_5e) & static_cast<u16>(light->field_5e)) != 0) {
            anti_strength = MAX(anti_strength, lighting_data->anti_strengths[index]);
        }
    }
    strength *= 1.0f - anti_strength;
    return strength;
}

extern "C" {
    void rtlSetModifiers(f32 *values, char **names, i32 count) {
        modifiers = values;
        modifier_names = names;
        modifier_cnt = count < 32 ? count : 32;
    }

    void rtlSetLights(rtldata_s *data) {
        const NUVEC *directions = reinterpret_cast<const NUVEC *>(data->data + 0x9c);
        const NUCOLOUR3 *colours = reinterpret_cast<const NUCOLOUR3 *>(data->data + 0x78);
        NuRndrSetDirectionalLightsPS(&directions[0], &colours[0], &directions[1], &colours[1], &directions[2],
                                     &colours[2]);
        NuRndrSetAmbientLightPS(reinterpret_cast<const NUCOLOUR3 *>(data->data + 0xc0));
    }

    void rtlSetSpecularLight(rtldata_s *data) {
        rtlidata_s *record = reinterpret_cast<rtlidata_s *>(data);
        if (record->specular_value > 0.0f)
            NuRndrSetSpecularLight(&record->field_134, NULL);
    }
}

static __used__ bool InsideLineXZ(float x, float z, float x0, float z0, float x1, float z1) {
    return 0.0f <= (x - x0) * (z1 - z0) + (z - z0) * (x0 - x1);
}

static f32 ClampUnit(f32 value) {
    if (value < 0.0f) {
        return 0.0f;
    }
    return value > 1.0f ? 1.0f : value;
}

static __used__ i32 rtlCalcLights(nuvec_s *position, numtx_s *rotation, f32 scale, rtlidata_s *lighting_data) {
    rtldata_s *data = reinterpret_cast<rtldata_s *>(lighting_data);
    for (i32 slot = 0; slot < 3; ++slot) {
        rtl_s *light = *reinterpret_cast<rtl_s **>(data->data + slot * 4);
        NUVEC *colour = reinterpret_cast<NUVEC *>(data->data + 0x78 + slot * sizeof(NUVEC));
        NUVEC *direction = reinterpret_cast<NUVEC *>(data->data + 0x9c + slot * sizeof(NUVEC));
        if (light == NULL) {
            *colour = {0.0f, 0.0f, 0.0f};
            *direction = {0.0f, 1.0f, 0.0f};
        } else {
            bool invalid = false;
            switch (light->type) {
                case 2:
                case 3:
                case 6:
                case 8:
                    if (position == NULL) {
                        invalid = true;
                    } else {
                        NuVecSub(direction, &light->position, position);
                        NuVecNorm(direction, direction);
                    }
                    break;
                case 4:
                    *direction = light->direction;
                    break;
                default:
                    *direction = {0.0f, 0.0f, 1.0f};
                    NuVecRotateX(direction, direction, light->pitch);
                    NuVecRotateY(direction, direction, light->yaw);
                    NuVecMtxRotate(direction, direction, &global_camera.mtx);
                    // Directional lights use 2.0 as their selection priority,
                    // but their shading strength starts at one.
                    *reinterpret_cast<f32 *>(data->data + 0x0c + slot * 4) = 1.0f;
                    break;
            }

            if (invalid) {
                *colour = {0.0f, 0.0f, 0.0f};
            } else {
                const f32 strength = static_cast<f32>(ApplyAntilights(
                    light, lighting_data, *reinterpret_cast<f32 *>(data->data + 0x0c + slot * 4) * light->intensity));
                NuVecScale(colour, &light->ambient, strength);
            }
        }
        if (rotation != NULL) {
            NuVecMtxRotate(direction, direction, rotation);
        }
    }

    if (scale != 1.0f) {
        for (i32 slot = 0; slot < 3; ++slot) {
            NUVEC *colour = reinterpret_cast<NUVEC *>(data->data + 0x78 + slot * sizeof(NUVEC));
            NuVecScale(colour, colour, scale);
        }
    }

    NUVEC *ambient = reinterpret_cast<NUVEC *>(data->data + 0xc0);
    NuVecClear(ambient);
    for (i32 slot = 0; slot < 3; ++slot) {
        rtl_s *light = *reinterpret_cast<rtl_s **>(data->data + 0x18 + slot * 4);
        if (light == NULL) {
            continue;
        }
        const f32 strength = static_cast<f32>(ApplyAntilights(
            light, lighting_data, *reinterpret_cast<f32 *>(data->data + 0x24 + slot * 4) * light->intensity));
        ambient->x = MIN(ambient->x + light->ambient.x * strength, 1.0f);
        ambient->y = MIN(ambient->y + light->ambient.y * strength, 1.0f);
        ambient->z = MIN(ambient->z + light->ambient.z * strength, 1.0f);
    }
    if (scale != 1.0f) {
        NuVecScale(ambient, ambient, scale);
    }
    NuVecNorm(reinterpret_cast<NUVEC *>(data->data + 0x134), reinterpret_cast<NUVEC *>(data->data + 0x134));
    return 0;
}

static __used__ rtl_s *GetNextRTL(void *set, rtl_s *light, char *indices, int *index) {
    if (set != NULL) {
        if (indices != NULL && index != NULL) {
            if (*index < static_cast<i32>(*indices)) {
                ++*index;
                light = &static_cast<rtlset *>(set)->lights[indices[*index]];
            } else {
                light = NULL;
            }
        } else {
            ++light;
        }
    } else {
        light = reinterpret_cast<rtl_s *>(NuLstGetNext(rtl_dynamic_pool, reinterpret_cast<NULNKHDR *>(light)));
    }
    return light;
}

extern "C" {
    f32 rtlSpecularValue(rtldata_s *data) {
        return data != NULL ? reinterpret_cast<rtlidata_s *>(data)->specular_value : 0.0f;
    }

    void rtlSetSpecularValue(rtldata_s *data, f32 value) {
        if (data != NULL)
            reinterpret_cast<rtlidata_s *>(data)->specular_value = value;
    }
}

extern "C" {
    static void rtlInsertLight(u8 *light, rtldata_s *data, f32 strength) {
        const bool ambient = light[0x58] == 1;
        const i32 pointer_offset = ambient ? 0x18 : 0x00;
        const i32 strength_offset = ambient ? 0x24 : 0x0c;
        for (i32 slot = 0; slot < 3; ++slot) {
            if (*reinterpret_cast<f32 *>(data->data + strength_offset + slot * 4) < strength) {
                for (i32 move = 2; move > slot; --move) {
                    *reinterpret_cast<u8 **>(data->data + pointer_offset + move * 4) =
                        *reinterpret_cast<u8 **>(data->data + pointer_offset + (move - 1) * 4);
                    *reinterpret_cast<f32 *>(data->data + strength_offset + move * 4) =
                        *reinterpret_cast<f32 *>(data->data + strength_offset + (move - 1) * 4);
                }
                *reinterpret_cast<u8 **>(data->data + pointer_offset + slot * 4) = light;
                *reinterpret_cast<f32 *>(data->data + strength_offset + slot * 4) = strength;
                return;
            }
        }
    }

    static f32 rtlDistanceStrength(const u8 *light, const NUVEC *position) {
        const NUVEC *light_position = reinterpret_cast<const NUVEC *>(light);
        const f32 dx = position->x - light_position->x;
        const f32 dy = position->y - light_position->y;
        const f32 dz = position->z - light_position->z;
        const f32 inner = *reinterpret_cast<const f32 *>(light + 0x34);
        const f32 outer = *reinterpret_cast<const f32 *>(light + 0x40);
        const f32 distance_sq = dx * dx + dy * dy + dz * dz;
        if (distance_sq >= outer * outer) {
            return 0.0f;
        }
        if (inner >= outer) {
            return 1.0f;
        }
        const f32 distance = sqrtf(distance_sq);
        return ClampUnit(1.0f - (distance - inner) / (outer - inner));
    }

} // extern "C"

static void rtlApplySetScaleLoop(void *set, rtlidata_s *lighting_data, NUVEC *position, NUMTX *rotation, i32 identity,
                                 f32 scale) {
    (void)rotation;
    (void)identity;
    (void)scale;
    rtldata_s *data = reinterpret_cast<rtldata_s *>(lighting_data);
    if (set != NULL) {
        rtl_s *light = static_cast<rtlset *>(set)->lights;
        for (i32 i = 0; i < 0x80 && light[i].type != 0; ++i) {
            if (light[i].disabled != 0) {
                continue;
            }
            const f32 strength =
                light[i].type == 5 ? 2.0f : rtlDistanceStrength(reinterpret_cast<u8 *>(&light[i]), position);
            if (strength == 0.0f) {
                continue;
            }
            if (light[i].type == 7) {
                InsertAntiLight(&light[i], lighting_data, strength);
            } else {
                InsertLight(&light[i], lighting_data, strength);
            }
        }
    } else if (rtl_dynamic_pool != NULL) {
        for (NULNKHDR *entry = NuLstGetNext(rtl_dynamic_pool, NULL); entry != NULL;
             entry = NuLstGetNext(rtl_dynamic_pool, entry)) {
            rtl_s *light = reinterpret_cast<rtl_s *>(entry);
            if (light->disabled != 0) {
                continue;
            }
            const f32 strength = light->type == 5 ? 2.0f : rtlDistanceStrength(reinterpret_cast<u8 *>(light), position);
            if (strength == 0.0f) {
                continue;
            }
            if (light->type == 7) {
                InsertAntiLight(light, lighting_data, strength);
            } else {
                InsertLight(light, lighting_data, strength);
            }
        }
    }
}

static __used__ void rtlCalcShadow(rtlidata_s *data) {
    if (data->cached_light != NULL && rtl_frametime != 0.0f &&
        (data->cached_light->type == 3 || data->cached_light->type == 8)) {
        data->shadow_direction.x += (NuRandFloat() - 0.5f) * rtl_shadow_flicker.x;
        data->shadow_direction.y += (NuRandFloat() - 0.5f) * rtl_shadow_flicker.y;
        data->shadow_direction.z += (NuRandFloat() - 0.5f) * rtl_shadow_flicker.z;
    }

    if (data->cached_value != 0.0f)
        NuVecNorm(&data->shadow_direction, &data->shadow_direction);

    if (data->shadow_blend == 0.0f) {
        data->blended_shadow_direction = data->shadow_direction;
        data->blended_shadow_value = data->cached_value;
        return;
    }

    if (data->shadow_blend == 1.0f) {
        data->previous_shadow_direction = data->blended_shadow_direction;
        data->previous_shadow_value = data->blended_shadow_value;
        data->shadow_blend = 0.999f;
    }

    if (data->previous_shadow_direction.x == 0.0f && data->previous_shadow_direction.y == 0.0f &&
        data->previous_shadow_direction.z == 0.0f) {
        data->blended_shadow_direction = data->shadow_direction;
    } else {
        NuVecLerp(&data->blended_shadow_direction, &data->previous_shadow_direction, &data->shadow_direction,
                  data->shadow_blend);
    }
    NuVecNorm(&data->blended_shadow_direction, &data->blended_shadow_direction);
    data->blended_shadow_value =
        data->previous_shadow_value * data->shadow_blend + (1.0f - data->shadow_blend) * data->cached_value;
    data->shadow_blend = MAX(0.0f, data->shadow_blend - rtl_frametime * rtl_shadow_blend_rate);
}

extern "C" {
    void rtlApplySetScale(void *set, rtldata_s *data, NUVEC *position, NUMTX *rotation, i32 identity, f32 scale) {
        rtlidata_s local_data;
        rtlidata_s *lighting_data;

        _NuTimeBarSlotBegin(0, 6, "RTL srch");
        if (data == NULL) {
            lighting_data = &local_data;
            rtlResetEx(reinterpret_cast<rtldata_s *>(lighting_data), 1);
        } else {
            lighting_data = reinterpret_cast<rtlidata_s *>(data);
            rtlResetEx(reinterpret_cast<rtldata_s *>(lighting_data), 0);
            if (lighting_data->cached_light != NULL &&
                static_cast<u16>(lighting_data->cached_light->uid) != lighting_data->cached_light_uid) {
                lighting_data->cached_light = NULL;
                lighting_data->cached_light_uid = 0;
                lighting_data->cached_value = 0.0f;
            }
        }

        if (rtl_dynamic_pool != NULL && rtl_dynamic_lights_enabled != 0) {
            rtlApplySetScaleLoop(NULL, lighting_data, position, rotation, identity, scale);
        }
        if (set != NULL) {
            rtlApplySetScaleLoop(set, lighting_data, position, rotation, identity, scale);
        }
        rtlCalcLights(position, rotation, scale, lighting_data);
        rtlCalcShadow(lighting_data);
        _NuTimeBarSlotEnd(0, 6);
    }

} // extern "C"

static __used__ void rtlApplyModifiersToChainLight(rtl_s *light) {
    const NUVEC zero = {0.0f, 0.0f, 0.0f};
    if (light->field_7c != NULL && light->field_79 != -1) {
        light->colour = zero;
        light->secondary_colour = zero;
        NuVecClear(&light->direction);
        rtl_s *source = &light->field_7c[light->field_79];
        for (;;) {
            f32 scale = modifiers[source->field_7b];
            light->colour.x += source->colour.x * scale;
            light->colour.y += source->colour.y * scale;
            light->colour.z += source->colour.z * scale;
            light->secondary_colour.x += source->secondary_colour.x * scale;
            light->secondary_colour.y += source->secondary_colour.y * scale;
            light->secondary_colour.z += source->secondary_colour.z * scale;
            light->direction.x += source->direction.x * scale;
            light->direction.y += source->direction.y * scale;
            light->direction.z += source->direction.z * scale;
            if (source->field_79 == -1)
                break;
            source = &light->field_7c[source->field_79];
        }
        NuVecNorm(&light->direction, &light->direction);
    }
}

static __used__ void rtlApplyModifiersToSingleLight(rtl_s *light) {
    if (light->field_79 == -1 && light->field_7a == -1)
        return;
    f32 scale = modifiers[light->field_7b];
    light->ambient.x += light->colour.x * scale;
    light->ambient.y += light->colour.y * scale;
    light->ambient.z += light->colour.z * scale;
}

static __used__ void rtlProcessLight(rtl_s *light, f32 elapsed) {
    if (light->field_79 != -1 && light->field_7a == -1)
        rtlApplyModifiersToChainLight(light);

    switch (light->type) {
        case 0:
            return;
        case 3:
            if (light->parameter_54 >= 0.0f) {
                light->parameter_54 -= elapsed;
                light->ambient = light->colour;
                if (light->parameter_54 <= 0.0f)
                    light->parameter_54 = -light->parameters[2] - NuRandFloat() * light->parameters[3];
            } else {
                light->parameter_54 += elapsed;
                light->ambient = light->secondary_colour;
                if (light->parameter_54 >= 0.0f)
                    light->parameter_54 = light->parameters[0] + NuRandFloat() * light->parameters[1];
            }
            break;
        case 6:
            if (light->field_64 == 0.0f) {
                light->ambient.x = light->colour.x;
                light->ambient.y = light->colour.y;
                light->ambient.z = light->colour.z;
                break;
            }
            if (light->field_64 > 0.0f) {
                light->parameter_54 += elapsed;
                f32 blend = MIN(1.0f, MAX(0.0f, light->parameter_54 / light->field_64));
                light->ambient.x = light->colour.x * blend + light->secondary_colour.x * (1.0f - blend);
                light->ambient.y = light->colour.y * blend + light->secondary_colour.y * (1.0f - blend);
                light->ambient.z = light->colour.z * blend + light->secondary_colour.z * (1.0f - blend);
                if (light->parameter_54 >= light->field_64) {
                    light->field_64 = -light->parameters[2] - NuRandFloat() * light->parameters[3];
                    light->parameter_54 = 0.0f;
                }
            } else {
                light->parameter_54 -= elapsed;
                f32 blend = MIN(1.0f, MAX(0.0f, light->parameter_54 / light->field_64));
                light->ambient.x = light->secondary_colour.x * blend + light->colour.x * (1.0f - blend);
                light->ambient.y = light->secondary_colour.y * blend + light->colour.y * (1.0f - blend);
                light->ambient.z = light->secondary_colour.z * blend + light->colour.z * (1.0f - blend);
                if (light->parameter_54 <= light->field_64) {
                    light->field_64 = light->parameters[0] + NuRandFloat() * light->parameters[1];
                    light->parameter_54 = 0.0f;
                }
            }
            break;
        case 8:
            light->ambient.x = light->secondary_colour.x * light->blend + light->colour.x * (1.0f - light->blend);
            light->ambient.y = light->secondary_colour.y * light->blend + light->colour.y * (1.0f - light->blend);
            light->ambient.z = light->secondary_colour.z * light->blend + light->colour.z * (1.0f - light->blend);
            light->blend += light->blend_rate * elapsed;
            light->blend = MIN(1.0f, MAX(0.0f, light->blend));
            light->parameter_54 -= elapsed;
            if (light->parameter_54 <= 0.0f) {
                light->parameter_54 = light->parameters[2];
                if (light->blend_rate >= 0.0f)
                    light->blend_rate = -(light->parameters[0] + NuRandFloat() * light->parameters[1]);
                else
                    light->blend_rate = light->parameters[0] + NuRandFloat() * light->parameters[1];
            }
            break;
        default:
            light->ambient = light->colour;
            break;
    }

    if (light->field_79 == -1 && light->field_7a == -1)
        rtlApplyModifiersToSingleLight(light);
}

extern "C" {

    char *rtlGetEnvPath(void) {
        return const_cast<char *>("_new");
    }

    char *rtlGetEnvSceneName(void) {
        return WORLD->config_file;
    }

    rtlset *rtlGetEnvSet(void) {
        return WORLD->rtl_set;
    }

    void rtlProcessLights(void *set, f32 frame_time) {
        rtl_s *light;
        i32 i;
        rtl_frametime = frame_time;
        if (rtl_dynamic_pool != NULL) {
            light = reinterpret_cast<rtl_s *>(NuLstGetNext(rtl_dynamic_pool, NULL));
            while (light != NULL) {
                rtlProcessLight(light, frame_time);
                light = reinterpret_cast<rtl_s *>(NuLstGetNext(rtl_dynamic_pool, reinterpret_cast<NULNKHDR *>(light)));
            }
        }
        if (set != NULL) {
            rtlset *light_set = static_cast<rtlset *>(set);
            light = light_set->lights;
            for (i = 0; i < 128; ++i) {
                if (light->type == 0)
                    break;
                if (light->field_7a == -1)
                    rtlProcessLight(light, frame_time);
                ++light;
            }
        }
    }

} // extern "C"

static void RefreshUI();

static void edrtlSaveUndo() {
    if (maxundo == 0 || curr_set == NULL)
        return;

    memmove(rtl_undo + rtl_undo_ix * 128, curr_set->lights, sizeof(curr_set->lights));
    edcamGetPosAng(&curpos_undo[rtl_undo_ix], NULL, NULL);
    curr_rtl_undo[rtl_undo_ix] = curr_rtl;
    rtl_locked_undo[rtl_undo_ix] = rtl_locked;
    base_rtl_undo[rtl_undo_ix] = base_rtl;
    rtl_undo_ix = (rtl_undo_ix + 1) & (maxundo - 1);
    i32 next_count = rtl_undo_cnt + 1;
    if (next_count >= maxundo)
        next_count = maxundo - 1;
    rtl_undo_cnt = next_count;
    rtl_undo_maxcnt = rtl_undo_cnt;
    undo_item->disabled = 0;
    redo_item->disabled = 1;
}

static void edrtlUndo() {
    if (maxundo == 0 || curr_set == NULL || rtl_undo_cnt == 0)
        return;

    memmove(rtl_undo + rtl_undo_ix * 128, curr_set->lights, sizeof(curr_set->lights));
    edcamGetPosAng(&curpos_undo[rtl_undo_ix], NULL, NULL);
    curr_rtl_undo[rtl_undo_ix] = curr_rtl;
    rtl_locked_undo[rtl_undo_ix] = rtl_locked;
    base_rtl_undo[rtl_undo_ix] = base_rtl;

    rtl_undo_ix = (rtl_undo_ix - 1) & (maxundo - 1);
    --rtl_undo_cnt;
    memmove(curr_set->lights, rtl_undo + rtl_undo_ix * 128, sizeof(curr_set->lights));
    edcamSetPos(&curpos_undo[rtl_undo_ix]);
    pcpos = curpos_undo[rtl_undo_ix];
    curr_rtl = curr_rtl_undo[rtl_undo_ix];
    rtl_locked = rtl_locked_undo[rtl_undo_ix];
    base_rtl = base_rtl_undo[rtl_undo_ix];
    redo_item->disabled = 0;
    if (rtl_undo_cnt == 0)
        undo_item->disabled = 1;
    RefreshUI();
}

static void edrtlRedo() {
    if (maxundo == 0 || curr_set == NULL || rtl_undo_cnt >= rtl_undo_maxcnt)
        return;

    rtl_undo_ix = (rtl_undo_ix + 1) & (maxundo - 1);
    ++rtl_undo_cnt;
    memmove(curr_set->lights, rtl_undo + rtl_undo_ix * 128, sizeof(curr_set->lights));
    edcamSetPos(&curpos_undo[rtl_undo_ix]);
    pcpos = curpos_undo[rtl_undo_ix];
    curr_rtl = curr_rtl_undo[rtl_undo_ix];
    rtl_locked = rtl_locked_undo[rtl_undo_ix];
    base_rtl = base_rtl_undo[rtl_undo_ix];
    undo_item->disabled = 0;
    if (rtl_undo_cnt == rtl_undo_maxcnt)
        redo_item->disabled = 1;
    RefreshUI();
}

static void edrtlInvalidateUndo() {
    rtl_undo_cnt = 0;
    rtl_undo_maxcnt = 0;
    rtl_undo_ix = 0;
    undo_item->disabled = 1;
    redo_item->disabled = 1;
}

static __used__ i32 rtlCmp(rtl_s *first, rtl_s *second) {
    i32 different = 0;
    different |= memcmp(&first->position, &second->position, sizeof(NUVEC));
    different |= memcmp(&first->direction, &second->direction, sizeof(NUVEC));
    different |= memcmp(&first->colour, &second->colour, sizeof(NUVEC));
    different |= memcmp(&first->secondary_colour, &second->secondary_colour, sizeof(NUVEC));
    different |= first->inner_radius != second->inner_radius;
    different |= first->outer_radius != second->outer_radius;
    different |= first->parameters[0] != second->parameters[0];
    different |= first->parameters[1] != second->parameters[1];
    different |= first->parameters[2] != second->parameters[2];
    different |= first->parameters[3] != second->parameters[3];
    different |= first->type != second->type;
    different |= first->disabled != second->disabled;
    different |= first->pitch != second->pitch;
    different |= first->yaw != second->yaw;
    different |= first->field_5e != second->field_5e;
    different |= first->field_60 != second->field_60;
    different |= first->field_68 != second->field_68;
    different |= first->intensity != second->intensity;
    return different;
}

extern "C" {
    void rtlSetMinR(f32 radius) {
        min_r = radius;
        def_fr = min_r * 2.0f;
    }

    void rtlSetUndoBuffer(VARIPTR *buffer, VARIPTR, i32 count) {
        maxundo = NuMiscNextPow2(count);
        if (maxundo <= 1)
            maxundo = 2;

        buffer->addr = ALIGN(buffer->addr, 4);
        rtl_undo = reinterpret_cast<rtl_s *>(buffer->addr);
        buffer->addr += maxundo * sizeof(rtl_s) * 128;

        buffer->addr = ALIGN(buffer->addr, 4);
        curr_rtl_undo = reinterpret_cast<rtl_s **>(buffer->addr);
        buffer->addr += maxundo * sizeof(rtl_s *);

        buffer->addr = ALIGN(buffer->addr, 4);
        rtl_locked_undo = reinterpret_cast<rtl_s **>(buffer->addr);
        buffer->addr += maxundo * sizeof(rtl_s *);

        buffer->addr = ALIGN(buffer->addr, 4);
        base_rtl_undo = reinterpret_cast<rtl_s **>(buffer->addr);
        buffer->addr += maxundo * sizeof(rtl_s *);

        buffer->addr = ALIGN(buffer->addr, 4);
        curpos_undo = reinterpret_cast<NUVEC *>(buffer->addr);
        buffer->addr += maxundo * sizeof(NUVEC);
    }

    rtl_s *rtlAlloc(void) {
        if (curr_set != NULL) {
            for (i32 i = 0; i < 128; ++i) {
                if (curr_set->lights[i].type == 0)
                    return &curr_set->lights[i];
            }
        }
        return NULL;
    }

    void rtlFree(rtl_s *light) {
        if (light->field_79 != -1 && light->field_7a == -1) {
            while (light->field_79 != -1)
                rtlFree(&curr_set->lights[light->field_79]);
        }
        if (light->field_7a != -1) {
            curr_set->lights[light->field_7a].field_79 = light->field_79;
            if (light->field_79 != -1)
                curr_set->lights[light->field_79].field_7a = light->field_7a;
        }
        i32 index = light - curr_set->lights;
        for (rtl_s *entry = curr_set->lights; entry < &curr_set->lights[128]; ++entry) {
            if (entry->field_79 >= index)
                --entry->field_79;
            if (entry->field_7a >= index)
                --entry->field_7a;
        }
        rtl_s *next = light + 1;
        while (light < &curr_set->lights[128] && light->type != 0) {
            *light = *next;
            ++light;
            ++next;
        }
        --light;
        light->type = 0;
    }

    rtlfog_s *fogAlloc(void) {
        if (curr_set != NULL) {
            for (i32 i = 0; i < 32; ++i) {
                if (curr_set->fog[i].type == 0)
                    return &curr_set->fog[i];
            }
        }
        return NULL;
    }

    void fogFree(rtlfog_s *fog) {
        rtlfog_s *next = fog + 1;
        while (fog < &curr_set->fog[32] && fog->type != 0) {
            *fog = *next;
            ++fog;
            ++next;
        }
        --fog;
        fog->type = 0;
    }

    rtlfog_s *rtlGetFogSet(rtlset *set, NUVEC *position) {
        i32 i;
        rtlset *fog_set = set;
        i32 selected = -1;
        if (fog_set != NULL) {
            for (i = 0; i < 32; ++i) {
                if (fog_set->fog[i].type == 0) {
                    continue;
                }
                f32 distance_squared =
                    (position->x - fog_set->fog[i].position.x) * (position->x - fog_set->fog[i].position.x) +
                    (position->y - fog_set->fog[i].position.y) * (position->y - fog_set->fog[i].position.y) +
                    (position->z - fog_set->fog[i].position.z) * (position->z - fog_set->fog[i].position.z);
                if (distance_squared < fog_set->fog[i].radius * fog_set->fog[i].radius) {
                    if (selected != -1) {
                        if (fog_set->fog[i].radius < fog_set->fog[selected].radius) {
                            selected = i;
                        }
                    } else {
                        selected = i;
                    }
                }
            }
            if (selected != -1) {
                return &fog_set->fog[selected];
            }
        }
        return NULL;
    }

} // extern "C"

// RTL editor state and callbacks from the same original rtl.c text/data run.

struct nuvtx_tc1_s;
struct numtl_s;
struct numtx_s;

typedef rtlfog_s EDRTLFOG_s;

rtlfog_s *curr_fog;
rtl_s clipboard_light;
rtlfog_s clipboard_fog;
extern f32 *modifiers;
static f32 game_nearclip;
static f32 game_farclip;
static i32 rtl_zoff;
static i32 ctl_ix = 1;
static rtl_s menu_undo;
i32 delete_menu_active;

extern NUQFNT *system_qfont;
extern "C" {
    void AddColourPick(eduimenu_s *, eduiitem_s *, f32 *, f32 *, f32 *, u32 *);
    void CreateColourPicker();
    void cbCancelSubMenu(eduimenu_s *, eduimenu_s *);
    void cbCancelSubMenuFromItem(eduimenu_s *, eduiitem_s *, u32);
    void cbTriggerSubMenu(eduimenu_s *, eduiitem_s *, u32);
    void cbModifierAdjust(eduimenu_s *, eduiitem_s *, u32);
    void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *, u32);
}

static char *default_modifier_names[] = {"default"};
char **modifier_names = default_modifier_names;
static eduimenu_s *associd_menu;
static char *camdir_id_names[16] = {"ID 0", "ID 1", "ID 2",  "ID 3",  "ID 4",  "ID 5",  "ID 6",  "ID 7",
                                    "ID 8", "ID 9", "ID 10", "ID 11", "ID 12", "ID 13", "ID 14", "ID 15"};
static eduiitem_s *associd_items[16];
static eduimenu_s *excludeid_menu;
static eduiitem_s *excludeid_items[16];
static eduimenu_s *userid_menu;
static char *userid_names[16] = {"NO ID", "ID 1", "ID 2",  "ID 3",  "ID 4",  "ID 5",  "ID 6",  "ID 7",
                                 "ID 8",  "ID 9", "ID 10", "ID 11", "ID 12", "ID 13", "ID 14", "ID 15"};
extern "C" void rtlSetAssocName(i32 index, char *name) {
    if (index < 16)
        camdir_id_names[index] = name;
}

extern "C" void rtlSetUserIdName(i32 index, char *name) {
    if (index < 16 && index > 0)
        userid_names[index] = name;
}
static eduiitem_s *userid_items[16];
static eduimenu_s *type_menu;
static eduiitem_s *point_item;
static eduiitem_s *jonflicker_item;
static eduiitem_s *pointflicker_item;
static eduiitem_s *pointblend_item;
static eduiitem_s *ambient_item;
static eduiitem_s *directional_item;
static eduiitem_s *antilight_item;
static eduiitem_s *camdir_item;
static eduimenu_s *modifier_menu;
static eduiitem_s *modifier_items[32];
eduimenu_s *modifier_adj_menu;
static eduiitem_s *modifier_adj_items[32];
static eduimenu_s *flicker_menu;
static eduiitem_s *t_hi_item;
static eduiitem_s *rt_hi_item;
static eduiitem_s *t_low_item;
static eduiitem_s *rt_low_item;
static eduimenu_s *jonflicker_menu;
static eduiitem_s *jon_t_hi_item;
static eduiitem_s *jon_rt_hi_item;
static eduiitem_s *jon_t_low_item;
static eduimenu_s *prop_menu;
static eduiitem_s *colour_item;
static eduiitem_s *flicker_item;
static eduiitem_s *associd_item;
static eduiitem_s *multiplier_item;
static eduiitem_s *groupid_item;
static eduiitem_s *modifierid_item;
static eduiitem_s *castshadow_item;
static eduiitem_s *hasspecular_item;
static eduimenu_s *hide_menu;
static eduiitem_s *hide_point_item;
static eduiitem_s *hide_pointflicker_item;
static eduiitem_s *hide_pointblend_item;
static eduiitem_s *hide_ambient_item;
static eduiitem_s *hide_directional_item;
static eduiitem_s *hide_antilight_item;
static eduiitem_s *hide_camdir_item;
static eduimenu_s *delete_menu;
static eduimenu_s *global_confirm_menu;
static eduimenu_s *global_menu;
static eduiitem_s *global_scale_item;
static eduimenu_s *load_confirm_menu;
static eduimenu_s *main_menu;
static eduiitem_s *prop_item;
static eduiitem_s *copy_item;
static eduiitem_s *paste_item;
static eduiitem_s *pasteinto_item;
static eduiitem_s *copytogroup_item;
static eduimenu_s *fog_menu;
static eduiitem_s *fogcol_item;
static eduiitem_s *fogalpha_item;
static eduiitem_s *fogdensity_item;
static eduiitem_s *fogdensitywii_item;
static eduiitem_s *fogstart_item;
static eduiitem_s *fogend_item;
static eduiitem_s *fogstartpsp_item;
static eduiitem_s *fogendpsp_item;
static eduiitem_s *hazecol_item;
static eduiitem_s *hazedensity_item;
static eduiitem_s *blurdensity_item;
static eduiitem_s *fogadjrng_item;
static eduiitem_s *fogadjnear_item;
static eduiitem_s *fogadjfar_item;
static eduiitem_s *dof_fstop_item;
static eduimenu_s *fog_main_menu;
static eduiitem_s *fog_item;
static eduiitem_s *fog_copy_item;
static eduiitem_s *fog_paste_item;
static eduiitem_s *fog_pasteinto_item;
static void cbCancelMenu(eduimenu_s *, eduimenu_s *) {
    rtled_menu_active = 0;
}
static void cbCancelDeleteMenu(eduimenu_s *, eduimenu_s *) {
    delete_menu_active = 0;
}
static void cbDeleteYes(eduimenu_s *, eduiitem_s *item, u32) {
    if (curr_rtl) {
        edrtlSaveUndo();
        rtlFree(curr_rtl);
        rtl_locked = NULL;
    }
    delete_menu_active = 0;
    item->highlighted = 0;
}
static void cbDeleteNo(eduimenu_s *, eduiitem_s *item, u32) {
    delete_menu_active = 0;
    item->highlighted = 0;
}
static __used__ void RefreshUI() {
    NuQFntPushCoordinateSystem(static_cast<NUQFNT_CSMODE>(1));
    NuQFntSetScale(system_qfont, edrtl_text_scale, edrtl_text_scale);

    prop_item->disabled = 1;
    copy_item->disabled = 1;
    flicker_item->disabled = 1;
    paste_item->disabled = clipboard_light.type == 0;
    pasteinto_item->disabled = clipboard_light.type == 0 || curr_rtl == NULL;
    copytogroup_item->disabled = curr_rtl == NULL || curr_rtl->group_id == 0;

    if (curr_rtl != NULL) {
        prop_item->disabled = 0;
        copy_item->disabled = 0;
        colour_item->disabled = curr_rtl->field_79 != -1 && curr_rtl->field_7a == -1;
        switch (curr_rtl->type) {
            case 1:
                eduiMenuHighlight(type_menu, ambient_item);
                break;
            case 2:
                eduiMenuHighlight(type_menu, point_item);
                break;
            case 3:
                eduiMenuHighlight(type_menu, pointflicker_item);
                flicker_item->data_ptr = flicker_menu;
                flicker_item->disabled = 0;
                break;
            case 4:
                eduiMenuHighlight(type_menu, directional_item);
                break;
            case 5:
                eduiMenuHighlight(type_menu, camdir_item);
                break;
            case 6:
                eduiMenuHighlight(type_menu, pointblend_item);
                flicker_item->data_ptr = flicker_menu;
                flicker_item->disabled = 0;
                break;
            case 7:
                eduiMenuHighlight(type_menu, antilight_item);
                break;
            case 8:
                eduiMenuHighlight(type_menu, jonflicker_item);
                flicker_item->data_ptr = jonflicker_menu;
                flicker_item->disabled = 0;
                break;
        }
        castshadow_item->highlighted = curr_rtl->cast_shadow;
        hasspecular_item->highlighted = curr_rtl->has_specular;
        i32 modifier_index = curr_rtl->field_7b;
        if (modifier_index > modifier_cnt - 1)
            modifier_index = modifier_cnt - 1;
        eduiMenuHighlight(modifier_menu, modifier_items[modifier_index]);
        for (i32 i = 0; i < 16; ++i) {
            if (((curr_rtl->field_5e >> i) & 1) != associd_items[i]->highlighted)
                eduiMenuHighlight(associd_menu, associd_items[i]);
        }
        for (i32 i = 0; i < 16; ++i) {
            if (((curr_rtl->field_60 >> i) & 1) != excludeid_items[i]->highlighted)
                eduiMenuHighlight(excludeid_menu, excludeid_items[i]);
        }
        for (i32 i = 0; i < 16; ++i) {
            if (curr_rtl->field_68 == i)
                eduiMenuHighlight(userid_menu, userid_items[i]);
        }
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(multiplier_item), curr_rtl->intensity, 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(groupid_item), curr_rtl->group_id, 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(t_hi_item), curr_rtl->parameters[0], 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(rt_hi_item), curr_rtl->parameters[1], 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(t_low_item), curr_rtl->parameters[2], 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(rt_low_item), curr_rtl->parameters[3], 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(jon_t_hi_item), curr_rtl->parameters[0], 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(jon_rt_hi_item), curr_rtl->parameters[1], 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(jon_t_low_item), curr_rtl->parameters[2], 0, 0);
    }
    if (clipboard_light.type != 0) {
        u32 colour = 0x80000000 | static_cast<u32>(clipboard_light.colour.x * 255.0f) & 0xff |
                     (static_cast<u32>(clipboard_light.colour.y * 255.0f) & 0xff) << 8 |
                     (static_cast<u32>(clipboard_light.colour.z * 255.0f) & 0xff) << 16;
        paste_item->colours[2] = colour;
        pasteinto_item->colours[2] = colour;
    }

    fog_copy_item->disabled = 1;
    fog_paste_item->disabled = clipboard_fog.type == 0;
    fog_pasteinto_item->disabled = clipboard_fog.type == 0 || curr_fog == NULL;
    fog_item->disabled = 1;
    if (curr_fog != NULL) {
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(fogalpha_item), curr_fog->colour >> 24, 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(fogdensity_item), curr_fog->density, 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(fogdensitywii_item), curr_fog->density_wii, 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(fogstartpsp_item), curr_fog->start_psp, 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(fogendpsp_item), curr_fog->end_psp, 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(fogstart_item), curr_fog->start, 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(fogend_item), curr_fog->end, 0, 0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(dof_fstop_item), curr_fog->depth_of_field_fstop / 10.0f, 0,
                               0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(hazedensity_item), curr_fog->low_quality_colour >> 24, 0,
                               0);
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(blurdensity_item), curr_fog->low_quality_density / 128.0f,
                               0, 0);
        fog_item->disabled = 0;
        fog_copy_item->disabled = 0;
    }
    for (i32 i = 0; i < modifier_cnt; ++i)
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(modifier_adj_items[i]), modifiers[i], 0, 0);
    NuQFntPopCoordinateSystem();
}
static char *rtl_ext = const_cast<char *>(".rtl");
extern "C" void rtlSetExt(char *extension) {
    rtl_ext = extension;
}
static void cbLoad(eduimenu_s *, eduiitem_s *, u32) {
    char path[256];
    i32 buffer_end = 0x7fffffff;
    VARIPTR buffer;
    buffer.void_ptr = curr_set;
    sprintf(path, "%s%s%s", rtlGetEnvPath(), rtlGetEnvSceneName(), rtl_ext);
    curr_set = rtlLoadSet(path, &buffer, buffer_end);
    curr_rtl = NULL;
    base_rtl = NULL;
    edrtlInvalidateUndo();
    RefreshUI();
}
static void cbSave(eduimenu_s *, eduiitem_s *, u32) {
    char path[256];
    sprintf(path, "%s%s%s", rtlGetEnvPath(), rtlGetEnvSceneName(), rtl_ext);
    rtlSaveSet(path, curr_set);
}
static void cbMultiplier(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_rtl)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_rtl->intensity = slider->value;
}
static void cbGroupID(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_rtl)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_rtl->group_id = static_cast<i32>(slider->value);
}
static void cbModifierType(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_rtl)
        return;
    curr_rtl->field_7b = item->data;
    RefreshUI();
}
extern "C" void cbModifierAdjust(eduimenu_s *, eduiitem_s *item, u32) {
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    modifiers[item->data] = slider->value;
}
static void cbToggleCastShadow(eduimenu_s *, eduiitem_s *item, u32) {
    curr_rtl->cast_shadow = item->highlighted;
}
static void cbToggleHasSpecular(eduimenu_s *, eduiitem_s *item, u32) {
    curr_rtl->has_specular = item->highlighted;
}
static void cbHighColour(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (!curr_rtl)
        return;
    AddColourPick(menu, item, &curr_rtl->colour.x, &curr_rtl->colour.y, &curr_rtl->colour.z, NULL);
}
static void cbLowColour(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (!curr_rtl)
        return;
    AddColourPick(menu, item, &curr_rtl->secondary_colour.x, &curr_rtl->secondary_colour.y,
                  &curr_rtl->secondary_colour.z, NULL);
}
static void cbHighTime(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_rtl)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_rtl->parameters[0] = slider->value;
}
static void cbRHighTime(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_rtl)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_rtl->parameters[1] = slider->value;
}
static void cbLowTime(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_rtl)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_rtl->parameters[2] = slider->value;
}
static void cbRLowTime(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_rtl)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_rtl->parameters[3] = slider->value;
}
static void cbLightType(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_rtl)
        return;
    curr_rtl->type = item->data;
    RefreshUI();
}
static void cbAssocID(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_rtl)
        return;
    curr_rtl->field_5e &= ~(1 << item->data);
    if (item->highlighted)
        curr_rtl->field_5e |= 1 << item->data;
    curr_rtl->field_60 &= ~curr_rtl->field_5e;
    RefreshUI();
}
static void cbExcludeID(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_rtl)
        return;
    curr_rtl->field_60 &= ~(1 << item->data);
    if (item->highlighted)
        curr_rtl->field_60 |= 1 << item->data;
    curr_rtl->field_5e &= ~curr_rtl->field_60;
    RefreshUI();
}
static void cbUserID(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_rtl)
        return;
    curr_rtl->field_68 = item->data;
    RefreshUI();
}
static void cbFogColour(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (!curr_fog)
        return;
    AddColourPick(menu, item, NULL, NULL, NULL, &curr_fog->colour);
}
static void cbHazeColour(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (!curr_fog)
        return;
    AddColourPick(menu, item, NULL, NULL, NULL, &curr_fog->low_quality_colour);
}
static void cbFogAlpha(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_fog)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_fog->colour &= 0xffffff;
    curr_fog->colour |= static_cast<i32>(slider->value) << 24;
}
static void cbFogDensity(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_fog)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_fog->density = slider->value;
}
static void cbFogDensityWii(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_fog)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_fog->density_wii = slider->value;
}
static void cbHazeDensity(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_fog)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_fog->low_quality_colour &= 0xffffff;
    curr_fog->low_quality_colour |= static_cast<i32>(slider->value) << 24;
}
static void cbBlurDensity(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_fog)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_fog->low_quality_density = static_cast<i32>(slider->value * 128.0f);
}
static void cbFogStartPSP(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_fog)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_fog->start_psp = slider->value;
}
static void cbFogEndPSP(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_fog)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_fog->end_psp = slider->value;
}
static void cbFogStart(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_fog)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_fog->start = slider->value;
}
static void cbFogEnd(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_fog)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_fog->end = slider->value;
}
static void cbFogAdjRng(eduimenu_s *, eduiitem_s *item, u32) {
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    f32 range = slider->value;
    if (fogstart_item) {
        static_cast<edui_slider_s *>(fogstart_item)->range = range;
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(fogstart_item),
                               static_cast<edui_slider_s *>(fogstart_item)->value, 0, 0);
    }
    if (fogend_item) {
        static_cast<edui_slider_s *>(fogend_item)->range = range;
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(fogend_item),
                               static_cast<edui_slider_s *>(fogend_item)->value, 0, 0);
    }
    if (fogstartpsp_item) {
        static_cast<edui_slider_s *>(fogstartpsp_item)->range = range;
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(fogstartpsp_item),
                               static_cast<edui_slider_s *>(fogstartpsp_item)->value, 0, 0);
    }
    if (fogendpsp_item) {
        static_cast<edui_slider_s *>(fogendpsp_item)->range = range;
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(fogendpsp_item),
                               static_cast<edui_slider_s *>(fogendpsp_item)->value, 0, 0);
    }
}
static void cbFogAdjNear(eduimenu_s *, eduiitem_s *item, u32) {
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    game_nearclip = slider->value;
    if (fogadjfar_item) {
        static_cast<edui_slider_s *>(fogadjfar_item)->minimum = game_nearclip * 2.0f;
        game_farclip = game_farclip > game_nearclip + 1.0f ? game_farclip : game_nearclip + 1.0f;
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(fogadjfar_item), game_farclip, 0, 0);
    }
}
static void cbFogAdjFar(eduimenu_s *, eduiitem_s *item, u32) {
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    game_farclip = slider->value;
    if (fogadjnear_item) {
        static_cast<edui_slider_s *>(fogadjnear_item)->range =
            game_farclip - static_cast<edui_slider_s *>(fogadjnear_item)->minimum;
        game_nearclip = game_nearclip > game_farclip ? game_farclip : game_nearclip;
        eduiItemSliderSetValEx(static_cast<edui_slider_s *>(fogadjnear_item), game_nearclip, 0, 0);
    }
}
static void cbDOFFStop(eduimenu_s *, eduiitem_s *item, u32) {
    if (!curr_fog)
        return;
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    curr_fog->depth_of_field_fstop = static_cast<i32>(slider->value * 10.0f);
}
static void cbCopyLight(eduimenu_s *, eduiitem_s *, u32) {
    if (curr_rtl)
        clipboard_light = *curr_rtl;
}
static void cbPasteLight(eduimenu_s *, eduiitem_s *, u32) {
    if (clipboard_light.type == 0)
        return;
    edrtlSaveUndo();
    curr_rtl = rtlAlloc();
    if (curr_rtl == NULL)
        return;
    *curr_rtl = clipboard_light;
    if (base_rtl == NULL) {
        curr_rtl->field_79 = -1;
        curr_rtl->field_7a = -1;
    } else {
        curr_rtl->field_79 = base_rtl->field_79;
        i8 current_index = static_cast<i8>(curr_rtl - curr_set->lights);
        if (curr_rtl->field_79 != -1)
            curr_set->lights[curr_rtl->field_79].field_7a = current_index;
        curr_rtl->field_7a = static_cast<i8>(base_rtl - curr_set->lights);
        base_rtl->field_79 = current_index;
    }
    curr_rtl->position = pcpos;
}
static void cbPasteIntoLight(eduimenu_s *menu, eduiitem_s *item, u32 flags) {
    if (clipboard_light.type == 0)
        return;
    edrtlSaveUndo();
    if (curr_rtl == NULL) {
        cbPasteLight(menu, item, flags);
        return;
    }
    NUVEC position = curr_rtl->position;
    *curr_rtl = clipboard_light;
    if (base_rtl == NULL) {
        curr_rtl->field_79 = -1;
        curr_rtl->field_7a = -1;
    } else {
        curr_rtl->field_79 = base_rtl->field_79;
        i8 current_index = static_cast<i8>(curr_rtl - curr_set->lights);
        if (curr_rtl->field_79 != -1)
            curr_set->lights[curr_rtl->field_79].field_7a = current_index;
        curr_rtl->field_7a = static_cast<i8>(base_rtl - curr_set->lights);
        base_rtl->field_79 = current_index;
    }
    curr_rtl->position = position;
}
static void cbCopyToGroup(eduimenu_s *, eduiitem_s *, u32) {
    i32 save_undo = 1;
    if (curr_rtl && curr_rtl->group_id) {
        for (i32 i = 0; i < 128; ++i) {
            if (&curr_set->lights[i] == curr_rtl)
                continue;
            if (curr_set->lights[i].type && curr_set->lights[i].group_id == curr_rtl->group_id) {
                if (save_undo) {
                    edrtlSaveUndo();
                    save_undo = 0;
                }
                NUVEC position = curr_set->lights[i].position;
                i32 uid = static_cast<u16>(curr_set->lights[i].uid);
                i32 user_id = curr_set->lights[i].field_68;
                curr_set->lights[i] = *curr_rtl;
                curr_set->lights[i].position = position;
                curr_set->lights[i].uid = uid;
                curr_set->lights[i].field_68 = user_id;
            }
        }
    }
}
static void cbUndoLight(eduimenu_s *, eduiitem_s *, u32) {
    edrtlUndo();
}
static void cbRedoLight(eduimenu_s *, eduiitem_s *, u32) {
    edrtlRedo();
}
static void cbCopyFog(eduimenu_s *, eduiitem_s *, u32) {
    if (curr_fog)
        clipboard_fog = *curr_fog;
}
static void cbPasteFog(eduimenu_s *, eduiitem_s *, u32) {
    if (clipboard_fog.type) {
        curr_fog = fogAlloc();
        if (curr_fog) {
            *curr_fog = clipboard_fog;
            curr_fog->position = pcpos;
        }
    }
}
static void cbPasteIntoFog(eduimenu_s *menu, eduiitem_s *item, u32 flags) {
    if (clipboard_fog.type) {
        if (curr_fog) {
            NUVEC position = curr_fog->position;
            *curr_fog = clipboard_fog;
            curr_fog->position = position;
        } else {
            cbPasteFog(menu, item, flags);
        }
    }
}
static void cbHideType(eduimenu_s *, eduiitem_s *item, u32) {
    hide_types[item->data] = item->highlighted;
}
static void cbNoZBuffer(eduimenu_s *, eduiitem_s *item, u32) {
    rtl_zoff = item->highlighted;
}
static void cbLightProperties(eduimenu_s *menu, eduiitem_s *item, u32 flags) {
    menu_undo = *curr_rtl;
    cbTriggerSubMenu(menu, item, flags);
}
static void cbCancelLightProperties(eduimenu_s *menu, eduimenu_s *) {
    if (rtlCmp(curr_rtl, &menu_undo)) {
        rtl_s light = *curr_rtl;
        *curr_rtl = menu_undo;
        edrtlSaveUndo();
        *curr_rtl = light;
    }
    eduiMenuDetach(menu);
}
static void cbSetControls(eduimenu_s *, eduiitem_s *item, u32) {
    ctl_ix = item->data;
}
static void cbScaleAllMultipliersUp(eduimenu_s *menu, eduiitem_s *, u32) {
    edrtlSaveUndo();
    for (i32 i = 0; i < 128; ++i)
        curr_set->lights[i].intensity =
            curr_set->lights[i].intensity * static_cast<edui_slider_s *>(global_scale_item)->value;
    eduiMenuAttach(menu, global_confirm_menu);
    global_confirm_menu->x = menu->x + 10;
    global_confirm_menu->y = menu->y + 10;
    RefreshUI();
}
static void cbScaleAllMultipliersDown(eduimenu_s *menu, eduiitem_s *, u32) {
    edrtlSaveUndo();
    for (i32 i = 0; i < 128; ++i)
        curr_set->lights[i].intensity =
            curr_set->lights[i].intensity / static_cast<edui_slider_s *>(global_scale_item)->value;
    eduiMenuAttach(menu, global_confirm_menu);
    global_confirm_menu->x = menu->x + 10;
    global_confirm_menu->y = menu->y + 10;
    RefreshUI();
}
extern "C" void rtlScaleSetMultipliers(f32 scale, rtlset *set_ptr) {
    rtlset *set = set_ptr;
    for (i32 i = 0; i < 128; ++i)
        set->lights[i].intensity = set->lights[i].intensity * scale;
}
static __used__ void InitUI() {
    static i32 initialised;
    i32 i;
    u32 colours[4] = {0x80000000, 0x800000ff, 0x80808080, 0x80f0f0f0};
    if (initialised)
        return;
    NuQFntPushCoordinateSystem(static_cast<NUQFNT_CSMODE>(1));
    NuQFntSetScale(system_qfont, edrtl_text_scale, edrtl_text_scale);
    initialised = 1;
    CreateColourPicker();
    associd_menu = eduiMenuCreate(100, 70, 180, 300, 0, &cbCancelSubMenu, "Assoc ID");
    for (i = 0; i <= 15; ++i) {
        associd_items[i] =
            eduiMenuAddItem(associd_menu, eduiItemToggleCreate(i, colours, 0, (i + 1), cbAssocID, camdir_id_names[i]));
    }
    eduiMenuFitWidth(associd_menu, 5);
    excludeid_menu = eduiMenuCreate(100, 70, 180, 300, 0, &cbCancelSubMenu, "Exclude ID");
    for (i = 0; i <= 15; ++i) {
        excludeid_items[i] = eduiMenuAddItem(
            excludeid_menu, eduiItemToggleCreate(i, colours, 0, (i + 1), cbExcludeID, camdir_id_names[i]));
    }
    eduiMenuFitWidth(excludeid_menu, 5);
    userid_menu = eduiMenuCreate(100, 70, 180, 300, 0, &cbCancelSubMenu, "User ID");
    for (i = 0; i <= 15; ++i) {
        userid_items[i] =
            eduiMenuAddItem(userid_menu, eduiItemCheckCreate(i, colours, 0, 1, cbUserID, userid_names[i]));
    }
    type_menu = eduiMenuCreate(100, 70, 180, 300, 0, &cbCancelSubMenu, "Light Type");
    point_item = eduiMenuAddItem(type_menu, eduiItemCheckCreate(2, colours, 0, 2, cbLightType, "Point"));
    jonflicker_item = eduiMenuAddItem(type_menu, eduiItemCheckCreate(8, colours, 0, 2, cbLightType, "Jon Flicker"));
    pointflicker_item = eduiMenuAddItem(type_menu, eduiItemCheckCreate(3, colours, 0, 2, cbLightType, "Point Flicker"));
    pointblend_item = eduiMenuAddItem(type_menu, eduiItemCheckCreate(6, colours, 0, 2, cbLightType, "Point Blend"));
    ambient_item = eduiMenuAddItem(type_menu, eduiItemCheckCreate(1, colours, 0, 2, cbLightType, "Ambient"));
    directional_item = eduiMenuAddItem(type_menu, eduiItemCheckCreate(4, colours, 0, 2, cbLightType, "Directional"));
    antilight_item = eduiMenuAddItem(type_menu, eduiItemCheckCreate(7, colours, 0, 2, cbLightType, "Antilight"));
    camdir_item = eduiMenuAddItem(type_menu, eduiItemCheckCreate(5, colours, 0, 2, cbLightType, "Camdir"));
    eduiMenuFitWidth(type_menu, 5);
    modifier_menu = eduiMenuCreate(100, 70, 180, 300, 0, &cbCancelSubMenu, "Modifiers");
    for (i = 0; i < modifier_cnt; ++i) {
        modifier_items[i] =
            eduiMenuAddItem(modifier_menu, eduiItemCheckCreate(i, colours, 0, 2, cbModifierType, modifier_names[i]));
    }
    eduiMenuFitWidth(modifier_menu, 5);
    modifier_adj_menu = eduiMenuCreate(100, 70, 180, 300, 0, &cbCancelSubMenu, "Adjust Modifiers");
    for (i = 0; i < modifier_cnt; ++i) {
        modifier_adj_items[i] =
            eduiMenuAddItem(modifier_adj_menu, eduiItemSliderCreate(i, colours, 0, &cbModifierAdjust, 0.0f, 1.0f, 1.0f,
                                                                    modifier_names[i]));
        eduiItemSliderSetGranularity(static_cast<edui_slider_s *>(modifier_adj_items[i]), 0.10000000149011612f);
    }
    eduiMenuFitWidth(modifier_adj_menu, 5);
    flicker_menu = eduiMenuCreate(100, 70, 180, 300, 0, &cbCancelSubMenu, "Flicker Light Options");
    eduiMenuAddItem(flicker_menu, eduiItemSelCreate(0, colours, 0, 1, cbLowColour, "Low Colour..."));
    t_hi_item =
        eduiMenuAddItem(flicker_menu, eduiItemSliderCreate(0, colours, 0, cbHighTime, 0.0f, 4.0f, 4.0f, "High Time"));
    rt_hi_item = eduiMenuAddItem(
        flicker_menu, eduiItemSliderCreate(0, colours, 0, cbRHighTime, 0.0f, 4.0f, 4.0f, "Random High Time"));
    t_low_item =
        eduiMenuAddItem(flicker_menu, eduiItemSliderCreate(0, colours, 0, cbLowTime, 0.0f, 4.0f, 4.0f, "Low Time"));
    rt_low_item = eduiMenuAddItem(flicker_menu,
                                  eduiItemSliderCreate(0, colours, 0, cbRLowTime, 0.0f, 4.0f, 4.0f, "Random Low Time"));
    eduiMenuFitWidth(flicker_menu, 5);
    jonflicker_menu = eduiMenuCreate(100, 70, 180, 300, 0, &cbCancelSubMenu, "Flicker Light Options");
    eduiMenuAddItem(jonflicker_menu, eduiItemSelCreate(0, colours, 0, 1, cbLowColour, "Low Colour..."));
    jon_t_hi_item =
        eduiMenuAddItem(jonflicker_menu, eduiItemSliderCreate(0, colours, 0, cbHighTime, 0.0f, 10.0f, 4.0f, "Step"));
    jon_rt_hi_item = eduiMenuAddItem(
        jonflicker_menu, eduiItemSliderCreate(0, colours, 0, cbRHighTime, 0.0f, 10.0f, 4.0f, "Random Step"));
    jon_t_low_item =
        eduiMenuAddItem(jonflicker_menu, eduiItemSliderCreate(0, colours, 0, cbLowTime, 0.0f, 4.0f, 4.0f, "Duration"));
    eduiMenuFitWidth(jonflicker_menu, 5);
    prop_menu = eduiMenuCreate(100, 70, 180, 300, 0, cbCancelLightProperties, "Light Properties");
    eduiMenuAddItem(prop_menu,
                    eduiItemSelCreate(reinterpret_cast<usize>(type_menu), colours, 0, 1, &cbTriggerSubMenu, "Type..."));
    colour_item = eduiMenuAddItem(prop_menu, eduiItemSelCreate(0, colours, 0, 1, cbHighColour, "Colour..."));
    flicker_item = eduiMenuAddItem(prop_menu, eduiItemSelCreate(reinterpret_cast<usize>(flicker_menu), colours, 0, 1,
                                                                &cbTriggerSubMenu, "Flicker Light Options..."));
    associd_item = eduiMenuAddItem(prop_menu, eduiItemSelCreate(reinterpret_cast<usize>(associd_menu), colours, 0, 1,
                                                                &cbTriggerSubMenu, "Associate ID(s)..."));
    eduiMenuAddItem(prop_menu, eduiItemSelCreate(reinterpret_cast<usize>(excludeid_menu), colours, 0, 1,
                                                 &cbTriggerSubMenu, "Exclude ID(s)..."));
    eduiMenuAddItem(prop_menu, eduiItemSelCreate(reinterpret_cast<usize>(userid_menu), colours, 0, 1, &cbTriggerSubMenu,
                                                 "User ID..."));
    multiplier_item =
        eduiMenuAddItem(prop_menu, eduiItemSliderCreate(0, colours, 0, cbMultiplier, 0.5f, 127.0f, 1.0f, "Multiplier"));
    eduiItemSliderSetGranularity(static_cast<edui_slider_s *>(multiplier_item), 0.10000000149011612f);
    eduiItemSliderSetFmt(static_cast<edui_slider_s *>(multiplier_item), "%.1f");
    groupid_item = eduiMenuAddItem(prop_menu, eduiItemSliderCreateInt(0, colours, 0, cbGroupID, 0, 255, 0, "Group ID"));
    modifierid_item = eduiMenuAddItem(prop_menu, eduiItemSelCreate(reinterpret_cast<usize>(modifier_menu), colours, 0,
                                                                   1, &cbTriggerSubMenu, "Modifier..."));
    castshadow_item =
        eduiMenuAddItem(prop_menu, eduiItemToggleCreate(0, colours, 0, 2, cbToggleCastShadow, "Cast Shadow"));
    hasspecular_item =
        eduiMenuAddItem(prop_menu, eduiItemToggleCreate(0, colours, 0, 3, cbToggleHasSpecular, "Has Specular"));
    eduiMenuFitWidth(prop_menu, 5);
    hide_menu = eduiMenuCreate(100, 70, 180, 300, 0, &cbCancelSubMenu, "Hide Lights");
    hide_point_item = eduiMenuAddItem(hide_menu, eduiItemToggleCreate(2, colours, 0, 1, cbHideType, "Point"));
    hide_pointflicker_item =
        eduiMenuAddItem(hide_menu, eduiItemToggleCreate(3, colours, 0, 2, cbHideType, "Point Flicker"));
    hide_pointblend_item =
        eduiMenuAddItem(hide_menu, eduiItemToggleCreate(6, colours, 0, 2, cbHideType, "Point Blend"));
    hide_ambient_item = eduiMenuAddItem(hide_menu, eduiItemToggleCreate(1, colours, 0, 3, cbHideType, "Ambient"));
    hide_directional_item =
        eduiMenuAddItem(hide_menu, eduiItemToggleCreate(4, colours, 0, 4, cbHideType, "Directional"));
    hide_antilight_item = eduiMenuAddItem(hide_menu, eduiItemToggleCreate(7, colours, 0, 4, cbHideType, "Antilight"));
    hide_camdir_item = eduiMenuAddItem(hide_menu, eduiItemToggleCreate(5, colours, 0, 4, cbHideType, "Camdir"));
    eduiMenuFitWidth(hide_menu, 5);
    delete_menu = eduiMenuCreate(100, 70, 180, 300, 0, cbCancelDeleteMenu, "Delete Light?");
    eduiMenuAddItem(delete_menu, eduiItemSelCreate(0, colours, 0, 1, cbDeleteYes, "Yes"));
    eduiMenuAddItem(delete_menu, eduiItemSelCreate(0, colours, 0, 1, cbDeleteNo, "No"));
    eduiMenuFitWidth(delete_menu, 5);
    global_confirm_menu = eduiMenuCreate(100, 70, 180, 300, 0, cbCancelDeleteMenu, "Global Change Applied");
    eduiMenuAddItem(global_confirm_menu, eduiItemSelCreate(0, colours, 0, 1, &cbCancelSubMenuFromItem, "Okay"));
    eduiMenuFitWidth(global_confirm_menu, 5);
    global_menu = eduiMenuCreate(100, 70, 180, 300, 0, &cbCancelSubMenu, "Global Adjustments");
    global_scale_item =
        eduiMenuAddItem(global_menu, eduiItemSliderCreate(0, colours, 0, 0, 2.0f, 8.0f, 2.0f, "Scale Amount"));
    eduiItemSliderSetGranularity(static_cast<edui_slider_s *>(global_scale_item), 1.0f);
    eduiItemSliderSetFmt(static_cast<edui_slider_s *>(global_scale_item), "%.0f");
    eduiMenuAddItem(global_menu,
                    eduiItemSelCreate(0, colours, 0, 1, cbScaleAllMultipliersUp, "Scale All Multipliers Up"));
    eduiMenuAddItem(global_menu,
                    eduiItemSelCreate(0, colours, 0, 1, cbScaleAllMultipliersDown, "Scale All Multipliers Down"));
    eduiMenuFitWidth(global_menu, 5);
    load_confirm_menu = eduiMenuCreate(100, 70, 180, 300, 0, &cbCancelSubMenu, "Load lights?");
    eduiMenuAddItem(load_confirm_menu, eduiItemSelCreate(0, colours, 0, 1, cbLoad, "Yes"));
    eduiMenuAddItem(load_confirm_menu, eduiItemSelCreate(0, colours, 0, 1, &cbCancelSubMenuFromItem, "No"));
    eduiMenuFitWidth(delete_menu, 5);
    main_menu = eduiMenuCreate(100, 70, 180, 300, 0, cbCancelMenu, "Light Editor Options");
    prop_item = eduiMenuAddItem(main_menu, eduiItemSelCreate(reinterpret_cast<usize>(prop_menu), colours, 0, 1,
                                                             cbLightProperties, "Light Properties..."));
    copy_item = eduiMenuAddItem(main_menu, eduiItemSelCreate(0, colours, 0, 1, cbCopyLight, "Copy"));
    paste_item = eduiMenuAddItem(main_menu, eduiItemSelCreate(0, colours, 0, 1, cbPasteLight, "Paste New"));
    pasteinto_item = eduiMenuAddItem(main_menu, eduiItemSelCreate(0, colours, 0, 1, cbPasteIntoLight, "Paste Into"));
    copytogroup_item = eduiMenuAddItem(main_menu, eduiItemSelCreate(0, colours, 0, 1, cbCopyToGroup, "Copy To Group"));
    undo_item = eduiMenuAddItem(main_menu, eduiItemSelCreate(0, colours, 0, 1, cbUndoLight, "Undo"));
    redo_item = eduiMenuAddItem(main_menu, eduiItemSelCreate(0, colours, 0, 1, cbRedoLight, "Redo"));
    undo_item->disabled = 1;
    redo_item->disabled = 1;
    eduiMenuAddItem(main_menu, eduiItemToggleCreate(0, colours, 0, 2, cbNoZBuffer, "No Z Buffer"));
    eduiMenuAddItem(main_menu, eduiItemToggleCreate(0, colours, 0, 3, &cbNearClipAtCursor, "Near Clip At Cursor"));
    eduiMenuAddItem(main_menu, eduiItemSelCreate(reinterpret_cast<usize>(modifier_adj_menu), colours, 0, 1,
                                                 &cbTriggerSubMenu, "Adjust Modifiers..."));
    eduiMenuAddItem(main_menu, eduiItemSelCreate(reinterpret_cast<usize>(hide_menu), colours, 0, 1, &cbTriggerSubMenu,
                                                 "Hide Lights..."));
    eduiMenuAddItem(main_menu, eduiItemSelCreate(reinterpret_cast<usize>(global_menu), colours, 0, 1, &cbTriggerSubMenu,
                                                 "Global Adjustments..."));
    eduiMenuAddItem(main_menu, eduiItemSelCreate(0, colours, 0, 1, cbSave, "Save Lights/Fog"));
    eduiMenuAddItem(main_menu, eduiItemSelCreate(reinterpret_cast<usize>(load_confirm_menu), colours, 0, 1,
                                                 &cbTriggerSubMenu, "Load Lights/Fog"));
    eduiMenuAddItem(main_menu, eduiItemCheckCreate(1, colours, 1, 4, cbSetControls, "Ralph Controls"));
    eduiMenuAddItem(main_menu, eduiItemCheckCreate(0, colours, 0, 4, cbSetControls, "Steve Controls"));
    eduiMenuFitWidth(main_menu, 5);
    fog_menu = eduiMenuCreate(100, 70, 180, 300, 0, &cbCancelSubMenu, "Fog Settings");
    fogcol_item = eduiMenuAddItem(fog_menu, eduiItemSelCreate(0, colours, 0, 1, cbFogColour, "Fog Colour..."));
    fogalpha_item = eduiMenuAddItem(
        fog_menu, eduiItemSliderCreate(0, colours, 0, cbFogAlpha, 0.0f, 128.0f, 1.0f, "Fog Density (alpha)"));
    fogdensity_item = eduiMenuAddItem(
        fog_menu, eduiItemSliderCreate(0, colours, 0, cbFogDensity, 0.0f, 4.0f, 1.0f, "Fog Density (new)"));
    fogdensitywii_item = eduiMenuAddItem(
        fog_menu, eduiItemSliderCreate(0, colours, 0, cbFogDensityWii, 0.0f, 4.0f, 1.0f, "Fog Density (Wii)"));
    static_cast<edui_slider_s *>(fogdensity_item)->granularity = 1.9999999494757503e-05f;
    fogstart_item = eduiMenuAddItem(
        fog_menu, eduiItemSliderCreate(0, colours, 0, cbFogStart, 0.0f, 2000.0f, 4.0f, "Fog Start (PS2)"));
    fogend_item =
        eduiMenuAddItem(fog_menu, eduiItemSliderCreate(0, colours, 0, cbFogEnd, 0.0f, 2000.0f, 4.0f, "Fog End (PS2)"));
    fogstartpsp_item = eduiMenuAddItem(
        fog_menu, eduiItemSliderCreate(0, colours, 0, cbFogStartPSP, 0.0f, 2000.0f, 4.0f, "Fog Start (PSP)"));
    fogendpsp_item = eduiMenuAddItem(
        fog_menu, eduiItemSliderCreate(0, colours, 0, cbFogEndPSP, 0.0f, 2000.0f, 4.0f, "Fog End (PSP)"));
    static_cast<edui_slider_s *>(fogstart_item)->granularity = 0.125f;
    static_cast<edui_slider_s *>(fogend_item)->granularity = 10.0f;
    static_cast<edui_slider_s *>(fogstartpsp_item)->granularity = 0.125f;
    static_cast<edui_slider_s *>(fogendpsp_item)->granularity = 10.0f;
    hazecol_item = eduiMenuAddItem(fog_menu, eduiItemSelCreate(0, colours, 0, 1, cbHazeColour, "Haze Colour..."));
    hazedensity_item = eduiMenuAddItem(
        fog_menu, eduiItemSliderCreate(0, colours, 0, cbHazeDensity, 0.0f, 128.0f, 1.0f, "Haze Density"));
    blurdensity_item =
        eduiMenuAddItem(fog_menu, eduiItemSliderCreate(0, colours, 0, cbBlurDensity, 0.0f, 1.0f, 1.0f, "Blur Density"));
    fogadjrng_item = eduiMenuAddItem(
        fog_menu, eduiItemSliderCreate(0, colours, 0, cbFogAdjRng, 0.0f, 2000.0f, 1000.0f, "Fog Distance Range"));
    fogadjnear_item =
        eduiMenuAddItem(fog_menu, eduiItemSliderCreate(0, colours, 0, cbFogAdjNear, 0.0010000000474974513f, 2000.0f,
                                                       0.10000000149011612f, "Near Clip (editor only)"));
    fogadjfar_item = eduiMenuAddItem(fog_menu, eduiItemSliderCreate(0, colours, 0, cbFogAdjFar, 0.0010000000474974513f,
                                                                    5000.0f, 1000.0f, "Far Clip (editor only)"));
    dof_fstop_item =
        eduiMenuAddItem(fog_menu, eduiItemSliderCreate(0, colours, 0, cbDOFFStop, 1.0f, 21.0f, 1.0f, "DOF - F-Stop"));
    static_cast<edui_slider_s *>(dof_fstop_item)->granularity = 0.10000000149011612f;
    eduiMenuFitWidth(fog_menu, 5);
    fog_main_menu = eduiMenuCreate(100, 70, 180, 300, 0, cbCancelMenu, "Fog Editor Options");
    fog_item = eduiMenuAddItem(fog_main_menu, eduiItemSelCreate(reinterpret_cast<usize>(fog_menu), colours, 0, 1,
                                                                &cbTriggerSubMenu, "Fog Settings..."));
    fog_copy_item = eduiMenuAddItem(fog_main_menu, eduiItemSelCreate(0, colours, 0, 1, cbCopyFog, "Copy"));
    fog_paste_item = eduiMenuAddItem(fog_main_menu, eduiItemSelCreate(0, colours, 0, 1, cbPasteFog, "Paste New"));
    fog_pasteinto_item =
        eduiMenuAddItem(fog_main_menu, eduiItemSelCreate(0, colours, 0, 1, cbPasteIntoFog, "Paste Into"));
    eduiMenuAddItem(fog_main_menu, eduiItemSelCreate(0, colours, 0, 1, cbSave, "Save Lights/Fog"));
    eduiMenuAddItem(fog_main_menu, eduiItemSelCreate(0, colours, 0, 1, cbLoad, "Load Lights/Fog"));
    eduiMenuAddItem(fog_main_menu, eduiItemCheckCreate(1, colours, 1, 4, cbSetControls, "Ralph Controls"));
    eduiMenuAddItem(fog_main_menu, eduiItemCheckCreate(0, colours, 0, 4, cbSetControls, "Steve Controls"));
    eduiMenuFitWidth(fog_main_menu, 5);
    NuQFntPopCoordinateSystem();
}

// RTL editor callbacks from the same original text run.

// Burnset persistence and editing state from the same original RTL editor run.

burnset_s *edrtl_edit_burnset;

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

extern "C" burnset_s *edrtlBurnoutLoad(char *filename, VARIPTR *buffer, i32) {
    buffer->addr = (buffer->addr + 3) & ~static_cast<usize>(3);
    burnset_s *set = static_cast<burnset_s *>(buffer->void_ptr);
    edrtlInitBurnset(set);
    set->field_560 = edrtlBurnoutLoadSet(filename, set);
    edrtl_edit_burnset = set;
    buffer->char_ptr += sizeof(burnset_s);
    return set;
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

void edrtlRemoveBurnout(i32 index) {
    if (!edrtl_edit_burnset || !edrtl_edit_burnset->burnouts[index].active)
        return;
    edrtl_edit_burnset->burnouts[index].active = 0;
    --edrtl_edit_burnset->active_count;
    edrtl_edit_burnset->selected_index = -1;
}

void edrtlPlaceBurnout(i32 index, nuvec_s *position) {
    if (!edrtl_edit_burnset || !edrtl_edit_burnset->burnouts[index].active)
        return;
    edrtl_edit_burnset->burnouts[index].position = *position;
}

static void edrtlSetBurnoutStartAngle(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_04 = slider->value;
    }
}

static void edrtlSetBurnoutMinIntensity(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_08 = slider->value;
    }
}

static void edrtlSetBurnoutEndAngle(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_0c = slider->value;
    }
}

static void edrtlSetBurnoutMaxIntensity(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_10 = slider->value;
    }
}

static void edrtlSetBurnoutGlobalScale(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_14 = slider->value;
    }
}

static void edrtlSetBurnoutDispersion(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_1c = slider->value;
    }
}

static void edrtlSetBurnoutFragmentGlowFactor(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_20 = slider->value;
    }
}

static void edrtlSetBurnoutThreshold(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_24 = slider->value;
    }
}

static void edrtlSetBurnoutSourceAvailable(eduimenu_s *, eduiitem_s *, u32) {
    if (edrtl_edit_burnset)
        edrtl_edit_burnset->parameters.field_28 = !edrtl_edit_burnset->parameters.field_28;
}

static void edrtlSetBurnoutSourceDirectionX(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_2c = slider->value;
    }
}

static void edrtlSetBurnoutSourceDirectionY(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_30 = slider->value;
    }
}

static void edrtlSetBurnoutSourceDirectionZ(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_34 = slider->value;
    }
}

static void edrtlSetBurnoutSourceInnerRadius(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_38 = slider->value;
    }
}

static void edrtlSetBurnoutSourceInnerIntensity(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_3c = slider->value;
    }
}

static void edrtlSetBurnoutSourceOuterRadius(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_40 = slider->value;
    }
}

static void edrtlSetBurnoutSourceOuterIntensity(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_44 = slider->value;
    }
}

static void edrtlSetBurnoutSourceFallOffPower(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->parameters.field_48 = slider->value;
    }
}

extern "C" {
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void eduiCreateMessageMenu(eduimenu_s *, char *, i32);
    extern eduimenu_s *edrtl_burn_main_menu;
    eduimenu_s *edrtl_burn_defaults_menu;
    eduimenu_s *edrtl_burn_radius_menu;
    eduimenu_s *edrtl_burn_set_menu;
    eduimenu_s *edrtl_burn_transitions_menu;
}

static void edrtlCancelBurnDefaultsMenu(eduimenu_s *, eduimenu_s *) {
    if (edrtl_burn_defaults_menu != NULL) {
        eduiMenuDestroy(edrtl_burn_defaults_menu);
        edrtl_burn_defaults_menu = NULL;
    }
}

static void edrtlBurnDefaultsMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edrtl_burn_defaults_menu = eduiMenuCreate(70, 70, 220, 300, ed_fnt, edrtlCancelBurnDefaultsMenu, "Level Defaults");
    if (edrtl_edit_burnset == NULL || edrtl_burn_defaults_menu == NULL)
        return;
    burn_parameters_s &p = edrtl_edit_burnset->parameters;
#define BURN_DEFAULT_SLIDER(callback, minimum, maximum, value, label)                                                  \
    eduiMenuAddItem(edrtl_burn_defaults_menu,                                                                          \
                    eduiItemSliderCreate(0, edblack, 0, callback, minimum, maximum, value, label))
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutGlobalScale, 0.0f, 4.0f, p.field_14, "Global Scale/Intensity");
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutStartAngle, 0.0f, 180.0f, p.field_04, "Start Angle");
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutMinIntensity, 0.0f, 1.0f, p.field_08, "Min Intensity");
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutEndAngle, 0.0f, 180.0f, p.field_0c, "End Angle");
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutMaxIntensity, 0.0f, 1.0f, p.field_10, "Max Intensity");
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutDispersion, 0.0f, 4.0f, p.field_1c, "Flare/Dispersion");
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutFragmentGlowFactor, 0.0f, 10.0f, p.field_20, "Fragment Glow Factor");
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutThreshold, 0.0f, 2.0f, p.field_24, "Threshold");
    eduiMenuAddItem(edrtl_burn_defaults_menu, eduiItemToggleCreate(0, edblack, p.field_28 != 0, 1,
                                                                   edrtlSetBurnoutSourceAvailable, "Source Available"));
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutSourceDirectionX, -1.0f, 2.0f, p.field_2c, "Source Direction X");
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutSourceDirectionY, -1.0f, 2.0f, p.field_30, "Source Direction Y");
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutSourceDirectionZ, -1.0f, 2.0f, p.field_34, "Source Direction Z");
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutSourceInnerRadius, 0.0f, 2.0f, p.field_38, "Source Inner Radius");
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutSourceInnerIntensity, 0.0f, 1.0f, p.field_3c, "Source Inner Intensity");
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutSourceOuterRadius, 0.0f, 180.0f, p.field_40, "Source Outer Radius");
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutSourceOuterIntensity, 0.0f, 1.0f, p.field_44, "Source Outer Intensity");
    BURN_DEFAULT_SLIDER(edrtlSetBurnoutSourceFallOffPower, 0.0f, 10.0f, p.field_48, "Source FallOff Power");
#undef BURN_DEFAULT_SLIDER
    eduiMenuAttach(parent, edrtl_burn_defaults_menu);
    edrtl_burn_defaults_menu->x = parent->x + 10;
    edrtl_burn_defaults_menu->y = parent->y + 40;
}

static void edrtlSetBurnoutNormalRate(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->field_b8 = slider->value;
    }
}

static void edrtlSetBurnoutOvershootRate(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->field_bc = slider->value;
    }
}

static void edrtlSetBurnoutOvershootCutin(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->field_c0 = slider->value;
    }
}

static void edrtlSetBurnoutOvershootAmount(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->field_c4 = slider->value;
    }
}

static void edrtlSetBurnoutOverbrightCap(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->field_c8 = slider->value;
    }
}

static void edrtlSetBurnoutOverdarkCap(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->field_cc = slider->value;
    }
}

static void edrtlCancelBurnTransitionsMenu(eduimenu_s *, eduimenu_s *) {
    if (edrtl_burn_transitions_menu != NULL) {
        eduiMenuDestroy(edrtl_burn_transitions_menu);
        edrtl_burn_transitions_menu = NULL;
    }
}

static void edrtlBurnTransitionsMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edrtl_burn_transitions_menu =
        eduiMenuCreate(70, 70, 220, 250, ed_fnt, edrtlCancelBurnTransitionsMenu, "Transitions");
    if (edrtl_edit_burnset == NULL || edrtl_burn_transitions_menu == NULL)
        return;
    burnset_s *set = edrtl_edit_burnset;
#define BURN_TRANSITION_SLIDER(callback, minimum, maximum, value, label)                                               \
    eduiMenuAddItem(edrtl_burn_transitions_menu,                                                                       \
                    eduiItemSliderCreate(0, edblack, 0, callback, minimum, maximum, value, label))
    BURN_TRANSITION_SLIDER(edrtlSetBurnoutNormalRate, 0.01f, 4.99f, set->field_b8, "Normal Rate");
    BURN_TRANSITION_SLIDER(edrtlSetBurnoutOvershootRate, 0.01f, 4.99f, set->field_bc, "Overshoot Rate");
    BURN_TRANSITION_SLIDER(edrtlSetBurnoutOvershootCutin, 0.0f, 2.0f, set->field_c0, "Overshoot Cutin");
    BURN_TRANSITION_SLIDER(edrtlSetBurnoutOvershootAmount, 0.0f, 2.0f, set->field_c4, "Overshoot Amount");
    BURN_TRANSITION_SLIDER(edrtlSetBurnoutOverbrightCap, 0.0f, 2.0f, set->field_c8,
                           "edrtl_edit_burnset->Overbright Cap");
    BURN_TRANSITION_SLIDER(edrtlSetBurnoutOverdarkCap, 0.0f, 2.0f, set->field_cc, "edrtl_edit_burnset->Overdark Cap");
#undef BURN_TRANSITION_SLIDER
    eduiMenuAttach(parent, edrtl_burn_transitions_menu);
    edrtl_burn_transitions_menu->x = parent->x + 10;
    edrtl_burn_transitions_menu->y = parent->y + 40;
}

static void edrtlSetBurnRadius(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->field_558 = slider->value;
    }
}
static void edrtlSetBurnFalloff(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->field_55c = slider->value;
    }
}
static void edrtlCancelBurnRadiusMenu(eduimenu_s *, eduimenu_s *) {
    if (edrtl_burn_radius_menu != NULL) {
        eduiMenuDestroy(edrtl_burn_radius_menu);
        edrtl_burn_radius_menu = NULL;
    }
}
static void edrtlBurnRadiusMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edrtl_burn_radius_menu = eduiMenuCreate(70, 70, 220, 300, ed_fnt, edrtlCancelBurnRadiusMenu, "Radius Defaults");
    if (edrtl_edit_burnset == NULL || edrtl_burn_radius_menu == NULL)
        return;
    eduiMenuAddItem(edrtl_burn_radius_menu, eduiItemSliderCreate(0, edblack, 0, edrtlSetBurnRadius, 0.2f, 19.8f,
                                                                 edrtl_edit_burnset->field_558, "Radius"));
    eduiMenuAddItem(edrtl_burn_radius_menu, eduiItemSliderCreate(0, edblack, 0, edrtlSetBurnFalloff, 0.0f, 10.0f,
                                                                 edrtl_edit_burnset->field_55c, "Falloff"));
    eduiMenuAttach(parent, edrtl_burn_radius_menu);
    edrtl_burn_radius_menu->x = parent->x + 10;
    edrtl_burn_radius_menu->y = parent->y + 40;
}
static void edrtlSetBurnsetThreshold(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->burnouts[edrtl_edit_burnset->selected_index].field_10 = slider->value;
    }
}
static void edrtlSetBurnsetIntensity(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->burnouts[edrtl_edit_burnset->selected_index].field_14 = slider->value;
    }
}
static void edrtlSetBurnsetFlare(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->burnouts[edrtl_edit_burnset->selected_index].field_18 = slider->value;
    }
}
static void edrtlSetBurnsetRadius(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->burnouts[edrtl_edit_burnset->selected_index].field_1c = slider->value;
    }
}
static void edrtlSetBurnsetFalloff(eduimenu_s *, eduiitem_s *item, u32) {
    if (edrtl_edit_burnset) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        edrtl_edit_burnset->burnouts[edrtl_edit_burnset->selected_index].field_20 = slider->value;
    }
}
static void edrtlCancelBurnSetMenu(eduimenu_s *, eduimenu_s *) {
    if (edrtl_burn_set_menu != NULL) {
        eduiMenuDestroy(edrtl_burn_set_menu);
        edrtl_burn_set_menu = NULL;
    }
}
static void edrtlBurnSetMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edrtl_burn_set_menu = eduiMenuCreate(70, 70, 220, 300, ed_fnt, edrtlCancelBurnSetMenu, "Burnset Properties");
    if (edrtl_edit_burnset == NULL || edrtl_burn_set_menu == NULL)
        return;
    burnout_s &burnout = edrtl_edit_burnset->burnouts[edrtl_edit_burnset->selected_index];
#define BURN_SET_SLIDER(callback, minimum, maximum, value, label)                                                      \
    eduiMenuAddItem(edrtl_burn_set_menu, eduiItemSliderCreate(0, edblack, 0, callback, minimum, maximum, value, label))
    BURN_SET_SLIDER(edrtlSetBurnsetThreshold, 0.0f, 2.0f, burnout.field_10, "Threshold");
    BURN_SET_SLIDER(edrtlSetBurnsetIntensity, 0.0f, 2.0f, burnout.field_14, "Intensity");
    BURN_SET_SLIDER(edrtlSetBurnsetFlare, 0.0f, 2.0f, burnout.field_18, "Flare");
    BURN_SET_SLIDER(edrtlSetBurnsetRadius, 0.2f, 19.8f, burnout.field_1c, "Radius");
    BURN_SET_SLIDER(edrtlSetBurnsetFalloff, 0.0f, 10.0f, burnout.field_20, "Falloff");
#undef BURN_SET_SLIDER
    eduiMenuAttach(parent, edrtl_burn_set_menu);
    edrtl_burn_set_menu->x = parent->x + 10;
    edrtl_burn_set_menu->y = parent->y + 40;
}
static void edrtlBurnoutFileSave(eduimenu_s *menu, eduiitem_s *, u32) {
    char filepath[256];
    char directory[256];
    char name[256];
    char extension[256];
    strcpy(directory, edbits_level_save_directory[0] ? edbits_level_save_directory : ".");
    strcpy(name, edbits_level_save_name[0] ? edbits_level_save_name : "burnout");
    strcpy(extension, edbits_level_save_extension[0] ? edbits_level_save_extension : "bur");
    sprintf(filepath, "%s\\%s.%s", directory, name, extension);
    if (edrtlBurnoutSave(filepath, edrtl_edit_burnset))
        eduiCreateMessageMenu(menu, const_cast<char *>("Saved OK"), 1);
    else
        eduiCreateMessageMenu(menu, const_cast<char *>("File Save Error"), 0);
}
static void edrtlBurnoutFileLoad(eduimenu_s *menu, eduiitem_s *, u32) {
    char filepath[256];
    char directory[256];
    char name[256];
    char extension[256];
    strcpy(directory, edbits_level_save_directory[0] ? edbits_level_save_directory : ".");
    strcpy(name, edbits_level_save_name[0] ? edbits_level_save_name : "burnout");
    strcpy(extension, edbits_level_save_extension[0] ? edbits_level_save_extension : "bur");
    sprintf(filepath, "%s\\%s.%s", directory, name, extension);
    edrtlResetBurnset(edrtl_edit_burnset);
    if (NuFileExists(filepath))
        edrtlBurnoutLoadSet(filepath, edrtl_edit_burnset);
    eduiCreateMessageMenu(menu, const_cast<char *>("Loaded OK"), 1);
}
static void edrtlCancelBurnMainMenu(eduimenu_s *, eduimenu_s *) {
    edrtl_active_menu = NULL;
    if (edrtl_burn_main_menu != NULL) {
        eduiMenuDestroy(edrtl_burn_main_menu);
        edrtl_burn_main_menu = NULL;
    }
}
static void edrtlBurnMainMenu() {
    edrtl_burn_main_menu = eduiMenuCreate(70, 70, 220, 300, ed_fnt, edrtlCancelBurnMainMenu, "Burnout Menu");
    if (edrtl_burn_main_menu == NULL)
        return;
    eduiMenuAddItem(edrtl_burn_main_menu,
                    eduiItemSelCreate(1, edblack, 0, 0, edrtlBurnDefaultsMenu, "Level Defaults..."));
    eduiMenuAddItem(edrtl_burn_main_menu,
                    eduiItemSelCreate(1, edblack, 0, 0, edrtlBurnRadiusMenu, "Radius Defaults..."));
    if (edrtl_edit_burnset != NULL && edrtl_edit_burnset->selected_index != -1)
        eduiMenuAddItem(edrtl_burn_main_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edrtlBurnSetMenu, "Burnset Properties..."));
    else
        eduiMenuAddItem(edrtl_burn_main_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Burnset Properties..."));
    eduiMenuAddItem(edrtl_burn_main_menu,
                    eduiItemSelCreate(1, edblack, 0, 0, edrtlBurnTransitionsMenu, "Transitions..."));
    eduiMenuAddItem(edrtl_burn_main_menu, eduiItemSelCreate(1, edblack, 0, 0, edrtlBurnoutFileSave, "Save Burnouts"));
    eduiMenuAddItem(edrtl_burn_main_menu, eduiItemSelCreate(1, edblack, 0, 0, edrtlBurnoutFileLoad, "Load Burnouts"));
}
static void edrtlInit() {
    usr_cam = NuCameraCreate();
    clipboard_light.type = 0;
    mtls[0] = NuMtlCreate3D(1);
    if (mtls[0] != NULL) {
        mtls[0]->attribs.z_mode = 0;
        mtls[0]->attribs.alpha_mode = 0;
        NuMtlUpdate(mtls[0]);
    }
    mtls[1] = NuMtlCreate3D(1);
    if (mtls[1] != NULL) {
        mtls[1]->attribs.z_mode = 3;
        mtls[1]->attribs.alpha_mode = 0;
        NuMtlUpdate(mtls[1]);
    }
    mtls[2] = NuMtlCreate3D(1);
    if (mtls[2] != NULL) {
        mtls[2]->attribs.z_mode = 3;
        mtls[2]->attribs.alpha_mode = 1;
        NuMtlUpdate(mtls[2]);
    }
    InitUI();
}

// RTL editor subsystem stubs (static, internal linkage).

static void edrtlClose() {
}

static void edrtlEnter() {
    RTL_EditorActive = 1;
    ed_just_entered = 1;
    edmainExtCamera(usr_cam);
    curr_set = rtlGetEnvSet();
    *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(curr_set) + 0x4f84) = 0;
    rtl_locked = NULL;
    rtled_menu_active = 0;
    delete_menu_active = 0;
    curr_fog = NULL;
    curr_rtl = NULL;
    base_rtl = NULL;
    game_nearclip = global_camera.near_clip;
    game_farclip = global_camera.far_clip;
    if (fogadjfar_item) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(fogadjfar_item);
        slider->range = game_farclip * 2.0f;
        slider->minimum = game_nearclip;
        eduiItemSliderSetValEx(slider, game_farclip, 0, 0);
    }
    if (fogadjnear_item) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(fogadjnear_item);
        slider->range = game_farclip - slider->minimum;
        eduiItemSliderSetValEx(slider, game_nearclip, 0, 0);
    }
    edrtlInvalidateUndo();
}

static void edrtlLeave() {
    edmainExtCamera(NULL);
    RTL_EditorActive = 0;
}

static __used__ rtl_s *FindNearestRTL(nuvec_s *position, int ignore_radius) {
    i32 nearest = -1;
    f32 nearest_distance = FLT_MAX;
    i32 i;
    f32 distance;
    if (base_rtl != NULL) {
        i = base_rtl->field_79;
        while (i != -1) {
            distance = (curr_set->lights[i].position.x - position->x) * (curr_set->lights[i].position.x - position->x) +
                       (curr_set->lights[i].position.y - position->y) * (curr_set->lights[i].position.y - position->y) +
                       (curr_set->lights[i].position.z - position->z) * (curr_set->lights[i].position.z - position->z);
            if ((ignore_radius || distance < curr_set->lights[i].outer_radius * curr_set->lights[i].outer_radius) &&
                distance < nearest_distance) {
                nearest_distance = distance;
                nearest = i;
            }
            i = curr_set->lights[i].field_79;
        }
    } else {
        for (i = 0; i < 128; ++i) {
            if (hide_types[curr_set->lights[i].type])
                continue;
            if (curr_set->lights[i].field_7a != -1)
                continue;
            distance = (curr_set->lights[i].position.x - position->x) * (curr_set->lights[i].position.x - position->x) +
                       (curr_set->lights[i].position.y - position->y) * (curr_set->lights[i].position.y - position->y) +
                       (curr_set->lights[i].position.z - position->z) * (curr_set->lights[i].position.z - position->z);
            if ((ignore_radius || distance < curr_set->lights[i].outer_radius * curr_set->lights[i].outer_radius) &&
                distance < nearest_distance) {
                nearest_distance = distance;
                nearest = i;
            }
        }
    }
    return nearest != -1 ? &curr_set->lights[nearest] : NULL;
}

void SelectNextRTL() {
    rtl_s *light;
    if (base_rtl != NULL) {
        if (curr_rtl->field_79 != -1) {
            curr_rtl = &curr_set->lights[curr_rtl->field_79];
        } else {
            light = curr_rtl;
            while (light->field_7a != -1) {
                curr_rtl = light;
                light = &curr_set->lights[light->field_7a];
            }
        }
    } else {
        i32 count = 0;
        light = curr_rtl;
        while (count < 128) {
            ++light;
            if (light >= &curr_set->lights[128])
                light = curr_set->lights;
            if (light->type != 0 && light->field_7a == -1)
                break;
            ++count;
        }
        curr_rtl = light;
    }
}

void SelectPrevRTL() {
    rtl_s *light;
    if (base_rtl != NULL) {
        light = &curr_set->lights[curr_rtl->field_7a];
        if (light->field_7a != -1) {
            curr_rtl = light;
        } else {
            while (curr_rtl->field_79 != -1)
                curr_rtl = &curr_set->lights[curr_rtl->field_79];
        }
    } else {
        i32 count = 0;
        light = curr_rtl;
        while (count < 128) {
            if (light == curr_set->lights)
                light = &curr_set->lights[127];
            else
                --light;
            if (light->type != 0 && light->field_7a == -1)
                break;
            ++count;
        }
        curr_rtl = light;
    }
}

static i32 edrtlProcRTL(float delta_time, nupad_s *pad) {
    static i32 dragging;
    static rtl_s *drag_rtl;
    static i32 dragundo = 1;
    static NUVEC dragstart;
    static NUVEC dragoff;

    helpmode = 0;
    if (delete_menu_active) {
        eduiMenuProcess(delete_menu, delta_time, pad);
        return 0;
    }
    if (rtled_menu_active) {
        eduiMenuProcess(main_menu, delta_time, pad);
        if (!rtled_menu_active) {
            menu_cancelled = 1;
            dragging = 0;
        }
        return 0;
    }
    if (ctl[ctl_ix][4] & pad->digital_buttons_pressed) {
        rtled_menu_active = 1;
        RefreshUI();
        return 0;
    }
    if (ctl[ctl_ix][5] & pad->digital_buttons) {
        helpmode = 5;
        edcamGetPosAng(&pcpos, &peax, &peay);
        if (!curr_rtl) {
            curr_rtl = FindNearestRTL(&pcpos, 1);
        } else {
            edcamSetPos(&curr_rtl->position);
            if (ctl[ctl_ix][7] & pad->digital_buttons_pressed)
                SelectNextRTL();
            else if (ctl[ctl_ix][6] & pad->digital_buttons_pressed)
                SelectPrevRTL();
        }
        if (rtl_locked)
            rtl_locked = curr_rtl;
        return 0;
    }

    NUVEC previous_position;
    edcamGetPosAng(&previous_position, NULL, NULL);
    edcamGetDist();
    edcamMoveEx(pad, delta_time);
    const i16 previous_pitch = static_cast<i16>(peax);
    const i16 previous_yaw = static_cast<i16>(peay);
    edcamGetPosAng(&pcpos, &peax, &peay);
    const f32 distance = NuFabs(edcamGetDist());
    const f32 move_scale = camscale_factor * distance < 1.0f ? 1.0f : camscale_factor * distance;

    if (menu_cancelled && pad->digital_buttons)
        return 0;
    menu_cancelled = 0;
    if (ed_just_entered && pad->digital_buttons)
        return 0;
    ed_just_entered = 0;
    if (pad->digital_buttons_pressed & 0x800)
        return 1;

    helpmode = 1;
    if (dragging) {
        helpmode = 2;
        if (!(ctl[ctl_ix][2] & pad->digital_buttons)) {
            dragging = 0;
            return 0;
        }
        rtl_s updated = *drag_rtl;
        NuVecSub(&dragoff, &pcpos, &dragstart);
        NuVecAdd(&updated.position, &updated.position, &dragoff);
        dragstart = pcpos;
        updated.pitch += static_cast<i16>(peax) - previous_pitch;
        updated.yaw += static_cast<i16>(peay) - previous_yaw;
        edcamSetAng(previous_pitch, previous_yaw);
        peax = previous_pitch;
        peay = previous_yaw;
        updated.direction.x = 0.0f;
        updated.direction.y = 0.0f;
        updated.direction.z = 1.0f;
        NuVecRotateX(&updated.direction, &updated.direction, updated.pitch);
        NuVecRotateY(&updated.direction, &updated.direction, updated.yaw);
        updated.inner_radius +=
            scale_rate * (static_cast<i32>(pad->analog_left_pad_right) - static_cast<i32>(pad->analog_left_pad_left)) *
            move_scale * delta_time;
        updated.outer_radius +=
            scale_rate * (static_cast<i32>(pad->analog_left_pad_up) - static_cast<i32>(pad->analog_left_pad_down)) *
            move_scale * delta_time;
        if (updated.inner_radius < min_r)
            updated.inner_radius = min_r;
        if (updated.outer_radius < updated.inner_radius)
            updated.outer_radius = updated.inner_radius;
        if (dragundo && rtlCmp(&updated, drag_rtl)) {
            edrtlSaveUndo();
            dragundo = 0;
        }
        *drag_rtl = updated;
        return 0;
    }

    curr_rtl = rtl_locked ? rtl_locked : FindNearestRTL(&pcpos, 0);
    if (base_rtl && (pad->digital_buttons_pressed & 0x8000)) {
        edcamSetPos(&base_rtl->position);
        base_rtl = NULL;
        rtl_locked = NULL;
    } else if (ctl[ctl_ix][0] & pad->digital_buttons_pressed) {
        edrtlSaveUndo();
        curr_rtl = rtlAlloc();
        if (curr_rtl) {
            rtl_locked = NULL;
            curr_rtl->position = pcpos;
            curr_rtl->inner_radius = min_r;
            curr_rtl->outer_radius = def_fr;
            curr_rtl->colour.x = curr_rtl->colour.y = curr_rtl->colour.z = 1.0f;
            curr_rtl->secondary_colour.x = curr_rtl->secondary_colour.y = curr_rtl->secondary_colour.z = 0.5f;
            curr_rtl->type = 2;
            curr_rtl->disabled = 0;
            curr_rtl->field_5e = 0;
            curr_rtl->field_60 = 0;
            curr_rtl->parameters[0] = curr_rtl->parameters[1] = curr_rtl->parameters[2] = curr_rtl->parameters[3] =
                0.1f;
            curr_rtl->parameter_54 = 0.0f;
            curr_rtl->pitch = 0;
            curr_rtl->yaw = 0;
            curr_rtl->direction.x = 0.0f;
            curr_rtl->direction.y = 0.0f;
            curr_rtl->direction.z = 1.0f;
            curr_rtl->field_64 = 0.0f;
            NuVecRotateX(&curr_rtl->direction, &curr_rtl->direction, curr_rtl->pitch);
            NuVecRotateY(&curr_rtl->direction, &curr_rtl->direction, curr_rtl->yaw);
            curr_rtl->field_79 = -1;
            curr_rtl->field_7a = -1;
            curr_rtl->field_7b = 0;
            curr_rtl->field_7c = curr_set->lights;
            if (base_rtl) {
                *curr_rtl = *base_rtl;
                curr_rtl->field_79 = base_rtl->field_79;
                const i8 current_index = static_cast<i8>(curr_rtl - curr_set->lights);
                if (curr_rtl->field_79 != -1)
                    curr_set->lights[curr_rtl->field_79].field_7a = current_index;
                curr_rtl->field_7a = static_cast<i8>(base_rtl - curr_set->lights);
                base_rtl->field_79 = current_index;
            }
        }
    } else if (curr_rtl) {
        helpmode = curr_rtl->field_7a == -1 ? 3 : 4;
        if (ctl[ctl_ix][2] & pad->digital_buttons_pressed) {
            dragging = 1;
            dragundo = 1;
            dragstart = pcpos;
            drag_rtl = curr_rtl;
        } else if (ctl[ctl_ix][1] & pad->digital_buttons_pressed) {
            delete_menu_active = 1;
            delete_menu->selected = delete_menu->first;
        } else if (ctl[ctl_ix][3] & pad->digital_buttons_pressed) {
            rtl_locked = rtl_locked ? NULL : curr_rtl;
        } else if (pad->digital_buttons_pressed & 0x8000) {
            if (!base_rtl)
                base_rtl = curr_rtl;
            rtl_locked = NULL;
        }
    }
    if (pad->digital_buttons_pressed & 0x4000) {
        curr_fog = NULL;
        edrtl_mode = 2;
    }
    return 0;
}

static __used__ EDRTLFOG_s *FindNearestFog(nuvec_s *position) {
    i32 nearest = -1;
    f32 nearest_distance = FLT_MAX;
    i32 i;
    f32 distance;
    for (i = 0; i < 32; ++i) {
        if (hide_types[curr_set->lights[i].type])
            continue;
        distance = (curr_set->fog[i].position.x - position->x) * (curr_set->fog[i].position.x - position->x) +
                   (curr_set->fog[i].position.y - position->y) * (curr_set->fog[i].position.y - position->y) +
                   (curr_set->fog[i].position.z - position->z) * (curr_set->fog[i].position.z - position->z);
        if (distance < curr_set->fog[i].radius * curr_set->fog[i].radius && distance < nearest_distance) {
            nearest_distance = distance;
            nearest = i;
        }
    }
    if (nearest != -1) {
        curFogLoc = nearest;
        return &curr_set->fog[nearest];
    }
    return NULL;
}

extern "C" rtlfog_s *edrtlGetFogSet(void) {
    if (fogmode == 0)
        return curr_fog;
    return FindNearestFog(NUMTX_GET_ROW_VEC(&global_camera.mtx, 3));
}

static EDRTLFOG_s *SelectPrevFog() {
    i32 index;
    i32 count = 0;
    index = curFogLoc - 1;
    if (index < 0) {
        index = 32;
    }
    if (curr_set != NULL) {
        while (index != curFogLoc) {
            if (curr_set->fog[index].type != 0) {
                curFogLoc = index;
                return &curr_set->fog[index];
            }
            if (index == 0) {
                index = 32;
            }
            ++count;
            if (count > 31) {
                break;
            }
            --index;
        }
    }
    return NULL;
}

static EDRTLFOG_s *SelectNextFog() {
    i32 index;
    i32 count = 0;
    if (curFogLoc == -1 || curFogLoc == 31) {
        index = 0;
    } else {
        index = curFogLoc + 1;
    }
    if (curr_set != NULL) {
        while (index != curFogLoc) {
            if (curr_set->fog[index].type != 0) {
                curFogLoc = index;
                return &curr_set->fog[index];
            }
            if (index > 31) {
                index = -1;
            }
            ++count;
            if (count > 31) {
                break;
            }
            ++index;
        }
    }
    return NULL;
}

extern "C" {
    i32 fog_editor_active;
}

static i32 edrtlProcFog(float delta_time, nupad_s *pad) {
    static i32 dragging;
    static NUVEC dragoff;
    static NUVEC dragstart;
    static EDRTLFOG_s *drag_fog;

    fog_editor_active = 1;
    if (pad->digital_buttons_pressed & ctl[ctl_ix][4]) {
        rtled_menu_active = 1;
        RefreshUI();
    }
    if (rtled_menu_active != 0) {
        eduiMenuProcess(fog_main_menu, delta_time, pad);
        if (rtled_menu_active == 0) {
            menu_cancelled = 1;
            dragging = 0;
        }
        return 0;
    }

    if (pad->digital_buttons & ctl[ctl_ix][5]) {
        if (curr_fog != NULL)
            edcamSetPos(&curr_fog->position);
        if (pad->digital_buttons_pressed & ctl[ctl_ix][7]) {
            curr_fog = SelectNextFog();
            if (curr_fog != NULL)
                edcamSetPos(&curr_fog->position);
        } else if (pad->digital_buttons_pressed & ctl[ctl_ix][6]) {
            curr_fog = SelectPrevFog();
            if (curr_fog != NULL)
                edcamSetPos(&curr_fog->position);
        }
        return 0;
    }

    edcamMoveEx(pad, delta_time);
    edcamGetPosAng(&pcpos, &peax, &peay);
    f32 camera_step = camscale_factor * NuFabs(edcamGetDist());
    if (camera_step < min_r)
        camera_step = min_r;

    if (menu_cancelled != 0 && pad->digital_buttons != 0)
        return 0;
    menu_cancelled = 0;
    if (ed_just_entered != 0 && pad->digital_buttons != 0)
        return 0;
    ed_just_entered = 0;
    if (pad->digital_buttons_pressed & 0x800)
        return 1;

    if (dragging != 0) {
        if ((pad->digital_buttons & ctl[ctl_ix][2]) == 0) {
            dragging = 0;
        } else {
            NuVecSub(&dragoff, &pcpos, &dragstart);
            NuVecAdd(&drag_fog->position, &drag_fog->position, &dragoff);
            dragstart = pcpos;
            curr_fog->radius +=
                scale_rate *
                (static_cast<i32>(pad->analog_left_pad_right) - static_cast<i32>(pad->analog_left_pad_left)) *
                camera_step * delta_time;
            if (curr_fog->radius < min_r)
                curr_fog->radius = min_r;
        }
        return 0;
    }

    curr_fog = FindNearestFog(&pcpos);
    if (pad->digital_buttons_pressed & ctl[ctl_ix][0]) {
        curr_fog = fogAlloc();
        if (curr_fog != NULL) {
            curr_fog->type = 1;
            curr_fog->position = pcpos;
            curr_fog->radius = min_r;
            curr_fog->start = 10.0f;
            curr_fog->end = 100.0f;
            curr_fog->start_psp = 10.0f;
            curr_fog->end_psp = 100.0f;
            curr_fog->colour = 0x80808080;
            curr_fog->low_quality_density = 0;
            curr_fog->low_quality_colour = 0;
            curr_fog->density = 0.0f;
            curr_fog->density_wii = 0.0f;
            curr_fog->depth_of_field_fstop = 1;
        }
    } else if (curr_fog != NULL) {
        if (pad->digital_buttons_pressed & ctl[ctl_ix][2]) {
            dragging = 1;
            dragstart = pcpos;
            drag_fog = curr_fog;
        } else if (pad->digital_buttons_pressed & ctl[ctl_ix][1]) {
            fogFree(curr_fog);
        }
    }
    if (pad->digital_buttons_pressed & 0x4000)
        edrtl_mode = 1;
    if (pad->digital_buttons_pressed & 0x1000)
        fogmode = 1 - fogmode;
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

eduimenu_s *edrtl_burn_main_menu;

static i32 edrtlProcBurn(float delta_time, nupad_s *pad) {
    if (edrtl_edit_burnset == NULL) {
        edrtl_edit_burnset =
            static_cast<burnset_s *>(NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(burnset_s), 4, 1, "", 0));
        edrtlInitBurnset(edrtl_edit_burnset);
    }

    if (edrtl_active_menu != NULL) {
        eduiMenuProcess(edrtl_active_menu, delta_time, pad);
    } else {
        if (!(pad->digital_buttons & 0x100))
            edcamMoveEx(pad, delta_time);
        edcamGetPosAng(&pcpos, &peax, &peay);

        if (pad->digital_buttons_pressed & 0x80) {
            edrtlBurnMainMenu();
            edrtl_active_menu = edrtl_burn_main_menu;
        }

        if (pad->digital_buttons & 0x100) {
            burnset_s *set = edrtl_edit_burnset;
            if (set != NULL && set->selected_index != -1) {
                if (pad->digital_buttons_pressed & 8) {
                    do {
                        ++set->selected_index;
                        if (set->selected_index == 32)
                            set->selected_index = 0;
                    } while (!set->burnouts[set->selected_index].active);
                }
                if (pad->digital_buttons_pressed & 2) {
                    do {
                        --set->selected_index;
                        if (set->selected_index == -1)
                            set->selected_index = 31;
                    } while (!set->burnouts[set->selected_index].active);
                }
            } else {
                edrtlDetermineNearestBurn(-1.0f, set);
            }
            if (set != NULL && set->selected_index != -1) {
                burnout_s *burnout = &set->burnouts[set->selected_index];
                edcamSetPos(&burnout->position);
                set->field_558 = burnout->field_1c;
                set->field_55c = burnout->field_20;
            }
        }

        edcamGetPosAng(&pcpos, &peax, &peay);
        if (pad->digital_buttons_pressed & 0x4000)
            edrtl_mode = 0;
        if (pad->digital_buttons_pressed & 0x40)
            edrtlAddBurnout(&pcpos);
        if ((pad->digital_buttons & 0x20) && edrtl_edit_burnset != NULL && edrtl_edit_burnset->selected_index != -1)
            edrtlPlaceBurnout(edrtl_edit_burnset->selected_index, &pcpos);
        if ((pad->digital_buttons_pressed & 0x10) && edrtl_edit_burnset != NULL &&
            edrtl_edit_burnset->selected_index != -1)
            edrtlRemoveBurnout(edrtl_edit_burnset->selected_index);
    }
    edrtlDetermineNearestBurn(-1.0f, edrtl_edit_burnset);
    return (pad->digital_buttons_pressed & 0x800) != 0;
}

extern "C" void edrtlCalculateBurnout(burnset_s *set, f32 *threshold, f32 *intensity, f32 *dispersion,
                                      NUVEC *camera_position, f32 frame_time) {
    if (set == NULL)
        set = edrtl_edit_burnset;
    if (set == NULL) {
        *threshold = 0.0f;
        *intensity = 0.0f;
        *dispersion = 0.0f;
        return;
    }

    f32 nearest_distance = -1.0f;
    i32 nearest_index = -1;
    for (i32 i = 0; i < 32; ++i) {
        burnout_s &burnout = set->burnouts[i];
        if (!burnout.active)
            continue;
        f32 distance = NuVecDist(camera_position, &burnout.position, NULL);
        if (distance < burnout.field_1c + burnout.field_20 &&
            (nearest_distance < 0.0f || distance < nearest_distance)) {
            nearest_index = i;
            nearest_distance = distance;
        }
    }

    f32 desired_threshold;
    f32 desired_intensity;
    f32 desired_dispersion;
    if (nearest_index == -1) {
        desired_threshold = set->parameters.field_24;
        desired_intensity = set->parameters.field_14;
        desired_dispersion = set->parameters.field_1c;
    } else {
        burnout_s &burnout = set->burnouts[nearest_index];
        if (nearest_distance <= burnout.field_1c) {
            desired_threshold = burnout.field_10;
            desired_intensity = burnout.field_14;
            desired_dispersion = burnout.field_18;
        } else {
            f32 fraction = (nearest_distance - burnout.field_1c) / burnout.field_20;
            desired_threshold = burnout.field_10 * (1.0f - fraction) + set->parameters.field_24 * fraction;
            desired_intensity = burnout.field_14 * (1.0f - fraction) + set->parameters.field_14 * fraction;
            desired_dispersion = burnout.field_18 * (1.0f - fraction) + set->parameters.field_1c * fraction;
        }
    }

    if (set->field_b4) {
        set->parameters_copy.field_24 = desired_threshold;
        set->parameters_copy.field_14 = desired_intensity;
        set->parameters_copy.field_1c = desired_dispersion;
        set->field_b4 = 0;
    } else {
        f32 step = set->field_b8 * frame_time;
        if (desired_threshold > set->parameters_copy.field_24) {
            f32 moved = set->parameters_copy.field_24 + step;
            set->parameters_copy.field_24 = moved > desired_threshold ? desired_threshold : moved;
        } else if (desired_threshold < set->parameters_copy.field_24) {
            f32 moved = set->parameters_copy.field_24 - step;
            set->parameters_copy.field_24 = desired_threshold > moved ? desired_threshold : moved;
        }
        if (desired_dispersion > set->parameters_copy.field_1c) {
            f32 moved = set->parameters_copy.field_1c + step;
            set->parameters_copy.field_1c = desired_dispersion > moved ? moved : desired_dispersion;
        } else if (desired_dispersion < set->parameters_copy.field_1c) {
            f32 moved = set->parameters_copy.field_1c - step;
            set->parameters_copy.field_1c = moved > desired_dispersion ? desired_dispersion : moved;
        }

        if (!set->field_b0 && !set->field_a8 && !set->field_ac) {
            if (desired_intensity >= set->parameters_copy.field_14 + set->field_c0)
                set->field_a8 = 1;
            else if (set->parameters_copy.field_14 - set->field_c0 >= desired_intensity)
                set->field_ac = 1;
        }

        f32 intensity_target = desired_intensity;
        if (set->field_a8 || set->field_ac) {
            step = set->field_bc * frame_time;
            if (set->field_a8) {
                intensity_target = desired_intensity + set->field_c4;
                if (intensity_target > set->field_c8)
                    intensity_target = desired_intensity > set->field_c8 ? desired_intensity : set->field_c8;
            } else {
                intensity_target = desired_intensity - set->field_c4;
                if (intensity_target < set->field_cc)
                    intensity_target = set->field_cc > desired_intensity ? desired_intensity : set->field_cc;
            }
        }
        if (intensity_target > set->parameters_copy.field_14) {
            f32 moved = set->parameters_copy.field_14 + step;
            set->parameters_copy.field_14 = moved > intensity_target ? intensity_target : moved;
        } else if (intensity_target < set->parameters_copy.field_14) {
            f32 moved = set->parameters_copy.field_14 - step;
            set->parameters_copy.field_14 = intensity_target > moved ? intensity_target : moved;
        }
        if (set->field_b0 && set->parameters_copy.field_14 == intensity_target)
            set->field_b0 = 0;
        if ((set->field_a8 || set->field_ac) && set->parameters_copy.field_14 == intensity_target) {
            set->field_a8 = 0;
            set->field_ac = 0;
            set->field_b0 = 1;
        }
    }
    *threshold = set->parameters_copy.field_24;
    *intensity = set->parameters_copy.field_14;
    *dispersion = set->parameters_copy.field_1c;
}

extern "C" void edrtlCalculateBurnoutEx(burnset_s *set, NuBloomParameters *parameters, NUVEC *camera_position,
                                        f32 frame_time) {
    memcpy(parameters, &set->parameters, sizeof(*parameters));
    edrtlCalculateBurnout(set, &parameters->threshold, &parameters->intensity, &parameters->blur_iterations,
                          camera_position, frame_time);
}

struct edrtl_line_vertex_s {
    NUVEC position;
    u8 unused_0c[0x0c];
    i32 colour;
    u8 unused_1c[0x08];
};

extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, numtl_s *);

static void edrtlRndrLine3d(nuvtx_tc1_s *vertices, numtl_s *, numtx_s *matrix) {
    const edrtl_line_vertex_s *line = reinterpret_cast<const edrtl_line_vertex_s *>(vertices);
    NUVEC start = line[0].position;
    NUVEC end = line[1].position;
    if (matrix != NULL) {
        NUMTX_ALIGNED16 aligned;
        NUMTX *transform = reinterpret_cast<NUMTX *>(matrix);
        if ((reinterpret_cast<usize>(matrix) & 0xf) != 0) {
            memcpy(&aligned, matrix, sizeof(aligned));
            transform = &aligned;
        }
        NuVecMtxTransform(&start, &start, transform);
        NuVecMtxTransform(&end, &end, transform);
    }
    NuRndrLine3dDbg(start.x, start.y, start.z, end.x, end.y, end.z, line[0].colour);
}

static i32 edrtlProc(float delta_time, nupad_s *pad) {
    lockflash -= delta_time;
    if (lockflash < 0.0f)
        lockflash += lockflash_rate;
    if (edrtl_mode == 0)
        return edrtlProcRTL(delta_time, pad);
    if (edrtl_mode == 2)
        return edrtlProcFog(delta_time, pad);
    if (edrtl_mode == 1)
        return edrtlProcBurn(delta_time, pad);
    return 1;
}

static f32 cursor_size = 1.0f;

static void edrtlDrawCursor() {
    edrtl_line_vertex_s line[2] = {};
    line[0].colour = -1;
    line[1].colour = -1;
    line[0].position = pcpos;
    line[1].position = pcpos;
    line[0].position.x -= cursor_size;
    line[1].position.x += cursor_size;
    edrtlRndrLine3d(reinterpret_cast<nuvtx_tc1_s *>(line), mtls[0], NULL);
    line[0].position = pcpos;
    line[1].position = pcpos;
    line[0].position.y -= cursor_size;
    line[1].position.y += cursor_size;
    edrtlRndrLine3d(reinterpret_cast<nuvtx_tc1_s *>(line), mtls[0], NULL);
    line[0].position = pcpos;
    line[1].position = pcpos;
    line[0].position.z -= cursor_size;
    line[1].position.z += cursor_size;
    edrtlRndrLine3d(reinterpret_cast<nuvtx_tc1_s *>(line), mtls[0], NULL);
}

extern "C" void edrtlDrawLightEx(i32, i32);

extern "C" void edrtlDrawLight(i32 index) {
    edrtlDrawLightEx(index, 0);
}

extern "C" void edrtlDrawLightEx(i32 index, i32 style) {
    rtl_s *light = &curr_set->lights[index];
    i32 colour = 0x80000000 | ((static_cast<i32>(light->colour.z * 255.0f) & 0xff) << 16) |
                 ((static_cast<i32>(light->colour.y * 255.0f) << 8) & 0xffff) |
                 (static_cast<i32>(light->colour.x * 255.0f) & 0xff);
    i32 inner_colour = style == 1 ? ~colour : colour;
    i32 outer_colour = style == 2 ? ~colour : colour;
    if (light->type == 1 || light->type == 2 || light->type == 4) {
        if (light->type == 4) {
            edrtl_line_vertex_s line[2] = {};
            line[0].position = light->position;
            line[0].colour = colour;
            line[1].position = light->position;
            line[1].colour = colour;
            NUVEC scaled;
            NuVecScale(&scaled, &light->direction, light->outer_radius);
            NuVecSub(&line[1].position, &line[0].position, &scaled);
            edrtlRndrLine3d(reinterpret_cast<nuvtx_tc1_s *>(line), NULL, NULL);
        }
        RndrOSphere(&light->position, light->inner_radius, inner_colour, numsegs,
                    reinterpret_cast<usize>(mtls[rtl_zoff != 0]));
        if (light->outer_radius > light->inner_radius) {
            RndrOSphere(&light->position, light->outer_radius, outer_colour, numsegs,
                        reinterpret_cast<usize>(mtls[rtl_zoff != 0]));
        }
        if (light == curr_rtl) {
            RndrOSquare(&light->position, light->outer_radius, -1);
        }
    } else if (light->type == 5) {
        edrtl_line_vertex_s line[2] = {};
        line[0].position = light->position;
        line[0].colour = colour;
        line[1].colour = colour;
        NUVEC scaled;
        NuVecScale(&scaled, &light->direction, 2.0f);
        NuVecMtxRotate(&scaled, &scaled, &global_camera.mtx);
        NuVecSub(&line[1].position, &line[0].position, &scaled);
        edrtlRndrLine3d(reinterpret_cast<nuvtx_tc1_s *>(line), NULL, NULL);
        RndrOSphere(&light->position, 2.0f, colour, numsegs, reinterpret_cast<usize>(mtls[rtl_zoff != 0]));
        if (light == curr_rtl) {
            RndrOSquare(&light->position, 2.0f, -1);
        }
    }
}

static void edrtlDrawLights() {
    if (base_rtl != NULL) {
        for (i32 index = base_rtl->field_79; index != -1; index = curr_set->lights[index].field_79) {
            edrtlDrawLight(index);
        }
    } else {
        for (i32 index = 0; index < 128; ++index) {
            rtl_s *light = &curr_set->lights[index];
            if (!hide_types[light->type] && light->field_7a == -1) {
                edrtlDrawLight(index);
            }
        }
    }
}

extern "C" void edrtlDrawFog(EDRTLFOG_s *fog) {
    if (fog != NULL) {
        i32 colour = (fog->colour & 0xffffff) | 0x80000000;
        switch (fog->type) {
            default:
                break;
            case 1:
                RndrOSphere(&fog->position, fog->radius, colour, numsegs, 0);
                break;
        }
    }
}

static void edrtlDrawFogs() {
    for (i32 index = 0; index < 32; ++index) {
        edrtlDrawFog(&curr_set->fog[index]);
    }
}

static void edrtlDrawBurnouts() {
    if (edrtl_edit_burnset == NULL) {
        return;
    }
    for (i32 index = 0; index < 32; ++index) {
        burnout_s *burnout = &edrtl_edit_burnset->burnouts[index];
        const bool selected = index == edrtl_edit_burnset->selected_index;
        const i32 colour = selected ? 0xf0f0f0f0 : 0x60606060;
        const i32 segments = selected ? 48 : 16;
        if (burnout->active) {
            RndrOSphere(&burnout->position, burnout->field_1c, colour, segments, 0);
            if (burnout->field_20 > 0.0f) {
                RndrOSphere(&burnout->position, burnout->field_1c + burnout->field_20, colour, segments, 0);
            }
        }
    }
}

static void edrtlDrawHelp() {
    static const char *help_text[2][6] = {
        {NULL, "SQR=ADD, O=SELECT", "L-UP/DOWN=ADJ FALLOFF, L-LEFT/RIGHT=ADJ INNER RADIUS",
         "SQR=ADD, TRI=DEL, L-UP=LOCK, X=ADJUST, O=SELECT\n", "X=ADD, TRI=DEL,L-UP=LOCK,O=ADJUST, SELECT=SELECT",
         "L-LEFT=PREV, L-RIGHT=NEXT"},
        {NULL, "X=ADD, SELECT=SELECT", "L-UP/DOWN=ADJ FALLOFF, L-LEFT/RIGHT=ADJ INNER RADIUS",
         "X=ADD, TRI=DEL,L-UP=LOCK,O=ADJUST, SELECT=SELECT", "L-DOWN=FOG EDITOR L-LEFT=EDIT / ADD ALTERNATE SETTINGS",
         "L1-PREV, R1-NEXT"},
    };
    NuQFntSet(system_qfont);
    NuQFntSetScale(system_qfont, edrtl_text_scale, edrtl_text_scale);
    NuQFntSetColour(system_qfont, 0x80ffffff);
    i32 y = numsegs * 16;
    const char *source = help_text[ctl_ix][helpmode];
    while (source != NULL && *source != 0) {
        char line[512];
        char *out = line;
        while (*source != 0 && *source != '\n') {
            *out++ = *source++;
        }
        if (*source != 0) {
            ++source;
        }
        *out = 0;
        NuQFntPrintEx(system_qfont, 640, y, 16, line);
        y += static_cast<i32>(NuQFntHeight(system_qfont));
    }
}

static i32 rtl_debug;

static void edrtlDrawRTLInfo() {
    static const char *light_types[10] = {
        "INVALID", "AMBIENT",     "POINT",     "POINT FLICKER", "DIRECTIONAL",
        "CAMDIR",  "POINT BLEND", "ANTILIGHT", "JON FLICKER",   "ALSO INVALID",
    };
    NuQFntSet(system_qfont);
    NuQFntSetScale(system_qfont, edrtl_text_scale, edrtl_text_scale);
    NuQFntSetColour(system_qfont, 0x80ffffff);
    i32 height = static_cast<i32>(NuQFntHeight(system_qfont));
    if (rtl_debug) {
        i32 y = 512;
        NuQFntPrintEx(system_qfont, 5440, y, 16, "ix - ty - nx : pr");
        y += height;
        for (i32 i = 0; i < 10; ++i) {
            NuQFntSet(system_qfont);
            const rtl_s *light = &curr_set->lights[i];
            NuQFntPrintEx(system_qfont, 5440, y, 16, "%02d - %02d - %02d : %02d", i, light->type, light->field_79,
                          light->field_7a);
            y += height;
        }
    }
    if (base_rtl) {
        i32 y = 512;
        NuQFntSet(system_qfont);
        for (i32 i = 0; i < modifier_cnt; ++i) {
            NuQFntPrintEx(system_qfont, 9600, y, 32, "%s = %.2f", modifier_names[i], static_cast<f64>(modifiers[i]));
            y += height;
        }
    }

    const i32 x = 640;
    i32 y = 512;
    if (curr_rtl) {
        i32 lines = 8;
        if (curr_rtl->field_68 != 0)
            ++lines;
        if (curr_rtl->type == 5 && curr_rtl->field_5e != 0)
            ++lines;
        if (curr_rtl->field_60 != 0)
            ++lines;
        if (curr_rtl->group_id != 0)
            ++lines;
        if (rtl_locked != NULL)
            ++lines;
        NuRndrRect2di(128, y - height, 4160, (lines + 1) * height, 0x20404040, mtls[2]);
        NuQFntSet(system_qfont);
        if (base_rtl)
            NuQFntPrintEx(system_qfont, x, y, 16, "RTL Editor %d/%d (Parent %d)",
                          static_cast<i32>(curr_rtl - curr_set->lights), 128,
                          static_cast<i32>(base_rtl - curr_set->lights));
        else
            NuQFntPrintEx(system_qfont, x, y, 16, "RTL Editor %d/%d", static_cast<i32>(curr_rtl - curr_set->lights),
                          128);
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, base_rtl ? "MODE: Alternate Set (DPAD Left to exit)" : "MODE: Normal");
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, "POS: %.03f %.03f %.03f", static_cast<f64>(curr_rtl->position.x),
                      static_cast<f64>(curr_rtl->position.y), static_cast<f64>(curr_rtl->position.z));
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, "TYPE: %s", light_types[curr_rtl->type <= 9 ? curr_rtl->type : 9]);
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, "RADIUS: %.03f", static_cast<f64>(curr_rtl->inner_radius));
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, "FALL OFF: %.03f", static_cast<f64>(curr_rtl->outer_radius));
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, "MULTIPLIER: %.0f", static_cast<f64>(curr_rtl->intensity));
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, "CAST SHADOW: %s", curr_rtl->cast_shadow ? "YES" : "NO");
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, "SPECULAR: %s", curr_rtl->has_specular ? "YES" : "NO");
        y += height;
        if (curr_rtl->field_68 != 0) {
            NuQFntPrintEx(system_qfont, x, y, 16, "USER ID: (%d)%s", curr_rtl->field_68,
                          userid_names[curr_rtl->field_68]);
            y += height;
        }
        if (curr_rtl->field_7a == -1 && curr_rtl->field_79 == -1) {
            i32 modifier_index = curr_rtl->field_7b;
            if (modifier_index >= modifier_cnt)
                modifier_index = modifier_cnt - 1;
            if (modifier_index < 0)
                modifier_index = 0;
            NuQFntPrintEx(system_qfont, x, y, 16, "MODIFIER: %s", modifier_names[modifier_index]);
            y += height;
        }
        if (curr_rtl->type == 5 && curr_rtl->field_5e != 0) {
            NuQFntPrintEx(system_qfont, x, y, 16, "[HAS ASSOCS]");
            y += height;
        }
        if (curr_rtl->field_60 != 0) {
            NuQFntPrintEx(system_qfont, x, y, 16, "[HAS EXCLUSIONS]");
            y += height;
        }
        if (curr_rtl->group_id != 0) {
            NuQFntPrintEx(system_qfont, x, y, 16, "GROUP ID: (%d)", curr_rtl->group_id);
            y += height;
        }
        if (curr_rtl->field_79 != -1 && curr_rtl->field_7a == -1) {
            i32 count = 0;
            rtl_s *light = curr_rtl;
            while (light->field_79 != -1) {
                ++count;
                light = &light->field_7c[light->field_79];
            }
            NuQFntPrintEx(system_qfont, x, y, 16, "[HAS %d ALTERNATE SETTINGS]", count);
            y += height;
        }
        if (curr_rtl->field_7a != -1) {
            i32 forward = 0;
            rtl_s *light = curr_rtl;
            while (light->field_7a != -1) {
                ++forward;
                light = &curr_set->lights[light->field_7a];
            }
            i32 backward = 0;
            while (light->field_79 != -1) {
                ++backward;
                light = &curr_set->lights[light->field_79];
            }
            NuQFntPrintEx(system_qfont, x, y, 16, "[ALTERNATE SETTING %d/%d]", forward, backward);
            y += height;
        }
        if (rtl_locked) {
            if (lockflash < lockflash_rate * 0.5f)
                NuQFntPrintEx(system_qfont, x, y, 16, "**LOCKED**");
            y += height;
        }
    } else {
        NuRndrRect2di(128, y - height, 4160, height * 2, 0x20404040, mtls[2]);
        NuQFntSet(system_qfont);
        NuQFntPrintEx(system_qfont, x, y, 16, "RTL Editor");
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, base_rtl ? "MODE: Alternate Set (DPAD Left to exit)" : "MODE: Normal");
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, "POS: %.03f %.03f %.03f", static_cast<f64>(pcpos.x),
                      static_cast<f64>(pcpos.y), static_cast<f64>(pcpos.z));
        y += height;
    }
    if (maxundo)
        NuQFntPrintEx(system_qfont, PS2_REZ_W * 16, 512, 16, "UNDO %d/%d/%d", rtl_undo_cnt, rtl_undo_maxcnt, maxundo);
    edrtlDrawHelp();
}

static void edrtlDrawFogInfo() {
    const char *fog_types[3] = {"INVALID", "SPHERICAL", "ALSO INVALID"};
    NuQFntSet(system_qfont);
    NuQFntSetScale(system_qfont, edrtl_text_scale, edrtl_text_scale);
    NuQFntSetColour(system_qfont, 0x80ffffff);
    i32 x = 640;
    i32 y = 512;
    i32 height = static_cast<i32>(NuQFntHeight(system_qfont));
    if (curr_fog != NULL) {
        NuRndrRect2di(128, y - height, 4800, 1440, 0x20000080, mtls[2]);
        NuQFntSet(system_qfont);
        NuQFntPrintEx(system_qfont, x, y, 16, "Fog Editor");
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, fogmode == 0 ? "FOGMODE: SELECTION" : "FOGMODE: CAMERA POS");
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, "POS: %.03f %.03f %.03f", static_cast<f64>(curr_fog->position.x),
                      static_cast<f64>(curr_fog->position.y), static_cast<f64>(curr_fog->position.z));
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, "TYPE: %s", fog_types[curr_fog->type]);
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, "RADIUS: %.03f", static_cast<f64>(curr_fog->radius));
    } else {
        NuRndrRect2di(128, y - height, 4800, height * 5 / 2, 0x20000080, mtls[2]);
        NuQFntSet(system_qfont);
        NuQFntPrintEx(system_qfont, x, y, 16, "Fog Editor");
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, fogmode == 0 ? "FOGMODE: SELECTION" : "FOGMODE: CAMERA POS");
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, "POS: %.03f %.03f %.03f", static_cast<f64>(pcpos.x),
                      static_cast<f64>(pcpos.y), static_cast<f64>(pcpos.z));
    }
}

static void edrtlDrawBurnInfo() {
    NuQFntSet(system_qfont);
    NuQFntSetScale(system_qfont, 1.2f, 1.2f);
    NuQFntSetColour(system_qfont, 0x80ffffff);
    i32 x = 640;
    i32 y = 512;
    i32 height = static_cast<i32>(NuQFntHeight(system_qfont));
    NuRndrRect2di(128, y - height, 4800, 1440, 0x20000080, mtls[2]);
    NuQFntSet(system_qfont);
    NuQFntSetScale(system_qfont, 1.2f, 1.2f);
    NuQFntPrintEx(system_qfont, x, y, 16, "Burnout Editor");
    y += height;
    if (edrtl_edit_burnset != NULL) {
        NuQFntPrintEx(system_qfont, x, y, 16, "Used: %d / %d", edrtl_edit_burnset->active_count, 32);
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, "Default Radius: %0.2f", static_cast<f64>(edrtl_edit_burnset->field_558));
        y += height;
        NuQFntPrintEx(system_qfont, x, y, 16, "Default Falloff: %0.2f",
                      static_cast<f64>(edrtl_edit_burnset->field_55c));
        y += 2 * height;
        NuQFntPrintEx(system_qfont, x, y, 16, "Current Global Scale/Intensity: %0.2f",
                      static_cast<f64>(edrtl_edit_burnset->parameters_copy.field_14));
    } else {
        NuQFntPrintEx(system_qfont, x, y, 16, "NO BURNSET LOADED");
    }
}

static void edrtlRender() {
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntPushPrintMode(2);
    usr_cam->near_clip =
        rtl_near_clip_at_cursor ? NuVecDist(&pcpos, NUMTX_GET_ROW_VEC(&usr_cam->mtx, 3), NULL) - 1.0f : game_nearclip;
    usr_cam->far_clip = game_farclip;
    edcamSet();
    if (edrtl_mode == 0) {
        edrtlDrawLights();
        edrtlDrawCursor();
        edrtlDrawRTLInfo();
        if (rtled_menu_active) {
            NuQFntSetScale(system_qfont, edrtl_text_scale, edrtl_text_scale);
            eduiMenuRender(main_menu);
        }
        if (delete_menu_active)
            eduiMenuRender(delete_menu);
    } else if (edrtl_mode == 2) {
        edrtlDrawFogs();
        edrtlDrawCursor();
        edrtlDrawFogInfo();
        if (rtled_menu_active)
            eduiMenuRender(fog_main_menu);
    } else if (edrtl_mode == 1) {
        edrtlDrawCursor();
        edrtlDrawBurnInfo();
        edrtlDrawBurnouts();
        if (edrtl_active_menu)
            eduiMenuRender(edrtl_active_menu);
    }
    NuQFntPopCoordinateSystem();
    NuQFntPopPrintMode();
}

extern "C" {
    ed_module_s edrtldesc = {NULL,        NULL,       "Realtime Light Editor",
                             edrtlInit,   edrtlClose, edrtlEnter,
                             edrtlLeave,  NULL,       NULL,
                             NULL,        0x2e6c7472, edrtlProc,
                             edrtlRender, NULL};
}

extern "C" void rtlFrameUpdate(f32 frame_time) {
    rtltimer1 = static_cast<u16>(static_cast<i32>(rtltimer1adv * frame_time) + rtltimer1);
    NuTimeBarSlotReset(0, 6);
}
