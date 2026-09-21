// Original RTL TU basename is rtl.c; the source is compiled as C++.
#include "decomp.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/core/render.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/numtl.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nulst.h"
#include "nu2api/numath/nurand.h"

#include <math.h>
#include <float.h>
#include <string.h>

struct nuqtdim_s;
struct nuqthdr_s;
struct rtl_s;
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
            u8 reserved_140[4];
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

extern "C" {
    rtlset *curr_set = NULL;
    rtl_s *curr_rtl;
    rtl_s *rtl_locked;
    rtl_s *base_rtl;
    i32 RTL_EditorActive;
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
    void rtlSetAssocName(void) {
        STUBBED();
    }

    void rtlSetUserIdName(void) {
        STUBBED();
    }

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

    void rtlResetDynamic(void) {
        if (rtl_dynamic_pool != NULL) {
            NULNKHDR *entry = NuLstGetNext(rtl_dynamic_pool, NULL);
            while (entry != NULL) {
                NULNKHDR *next = NuLstGetNext(rtl_dynamic_pool, entry);
                NuLstFree(entry);
                entry = next;
            }
            rtl_dynamic_cnt = 0;
        }
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
    STUBBED();
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

    void rtlSetSpecularLight(void) {
        STUBBED();
    }
}

static __used__ bool InsideLineXZ(float, float, float, float, float, float) {
    STUBBED();
    return false;
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

static __used__ rtl_s *GetNextRTL(void *, rtl_s *, char *, int *) {
    STUBBED();
    return nullptr;
}

extern "C" {
    void rtlSpecularValue(void) {
        STUBBED();
    }

    void rtlSetSpecularValue(void) {
        STUBBED();
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

static __used__ void rtlProcessLight(rtl_s *light, f32 frame_time) {
    f32 scale;
    if (light->field_79 != -1 && light->field_7a == -1)
        rtlApplyModifiersToChainLight(light);
    switch (light->type) {
        case 0:
            return;
        case 3:
            if (light->parameter_54 >= 0.0f) {
                light->parameter_54 -= frame_time;
                light->ambient = light->colour;
                if (light->parameter_54 <= 0.0f)
                    light->parameter_54 = -light->parameters[2] - NuRandFloat() * light->parameters[3];
            } else {
                light->parameter_54 += frame_time;
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
            } else if (light->field_64 > 0.0f) {
                light->parameter_54 += frame_time;
                scale = MIN(1.0f, (MAX(0.0f, light->parameter_54 / light->field_64)));
                light->ambient.x = light->colour.x * scale + (1.0f - scale) * light->secondary_colour.x;
                light->ambient.y = light->colour.y * scale + (1.0f - scale) * light->secondary_colour.y;
                light->ambient.z = light->colour.z * scale + (1.0f - scale) * light->secondary_colour.z;
                if (light->parameter_54 >= light->field_64) {
                    light->field_64 = -light->parameters[2] - NuRandFloat() * light->parameters[3];
                    light->parameter_54 = 0.0f;
                }
            } else {
                light->parameter_54 -= frame_time;
                scale = MIN(1.0f, (MAX(0.0f, light->parameter_54 / light->field_64)));
                light->ambient.x = light->secondary_colour.x * scale + (1.0f - scale) * light->colour.x;
                light->ambient.y = light->secondary_colour.y * scale + (1.0f - scale) * light->colour.y;
                light->ambient.z = light->secondary_colour.z * scale + (1.0f - scale) * light->colour.z;
                if (light->parameter_54 <= light->field_64) {
                    light->field_64 = light->parameters[0] + NuRandFloat() * light->parameters[1];
                    light->parameter_54 = 0.0f;
                }
            }
            break;
        case 8:
            light->ambient.x = light->secondary_colour.x * light->blend + (1.0f - light->blend) * light->colour.x;
            light->ambient.y = light->secondary_colour.y * light->blend + (1.0f - light->blend) * light->colour.y;
            light->ambient.z = light->secondary_colour.z * light->blend + (1.0f - light->blend) * light->colour.z;
            light->blend += light->blend_rate * frame_time;
            light->blend = MAX(0.0f, (MIN(1.0f, light->blend)));
            light->parameter_54 -= frame_time;
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

    void rtlGetEnvPath(void) {
        STUBBED();
    }

    void rtlGetEnvSceneName(void) {
        STUBBED();
    }

    void rtlGetEnvSet(void) {
        STUBBED();
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

static void edrtlSaveUndo() {
    STUBBED();
}

static void edrtlUndo() {
    STUBBED();
}

static void edrtlRedo() {
    STUBBED();
}

static void edrtlInvalidateUndo() {
    STUBBED();
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

    void rtlSetUndoBuffer(void) {
        STUBBED();
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
static NUVEC pcpos;
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
static eduiitem_s *undo_item;
static eduiitem_s *redo_item;
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
    STUBBED();
}
static void cbCancelDeleteMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
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
static void cbDeleteNo(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static __used__ void RefreshUI() {
    STUBBED();
}
extern "C" void rtlSetExt(void) {
    STUBBED();
}
static void cbLoad(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void cbSave(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
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
    STUBBED();
}
static void cbPasteIntoLight(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
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

extern "C" burnset_s *edrtlBurnoutLoad(char *filename, VARIPTR *buffer) {
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

static void edrtlCancelBurnDefaultsMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edrtlBurnDefaultsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
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
    STUBBED();
}

static void edrtlBurnTransitionsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
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
    STUBBED();
}
static void edrtlBurnRadiusMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
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
    STUBBED();
}
static void edrtlBurnSetMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edrtlBurnoutFileSave(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edrtlBurnoutFileLoad(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edrtlCancelBurnMainMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
static void edrtlBurnMainMenu() {
    STUBBED();
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
    STUBBED();
}

static void edrtlEnter() {
    STUBBED();
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

static void edrtlProcRTL(float, nupad_s *) {
    STUBBED();
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

static void edrtlProcFog(float, nupad_s *) {
    STUBBED();
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

static void edrtlProcBurn(float, nupad_s *) {
    STUBBED();
}

extern "C" void edrtlCalculateBurnout(void) {
    STUBBED();
}

extern "C" void edrtlCalculateBurnoutEx(burnset_s *, NuBloomParameters *, NUVEC *, f32) {
    STUBBED();
}

static void edrtlRndrLine3d(nuvtx_tc1_s *, numtl_s *, numtx_s *) {
    STUBBED();
}

static i32 edrtlProc(float, nupad_s *) {
    STUBBED();
    return 0;
}

static void edrtlDrawCursor() {
    STUBBED();
}

extern "C" void edrtlDrawLight(void) {
    STUBBED();
}

extern "C" void edrtlDrawLightEx(void) {
    STUBBED();
}

static void edrtlDrawLights() {
    STUBBED();
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
    STUBBED();
}

static void edrtlDrawBurnouts() {
    STUBBED();
}

static void edrtlDrawHelp() {
    STUBBED();
}

static void edrtlDrawRTLInfo() {
    STUBBED();
}

static void edrtlDrawFogInfo() {
    STUBBED();
}

static void edrtlDrawBurnInfo() {
    STUBBED();
}

static void edrtlRender() {
    STUBBED();
}

extern "C" void rtlFrameUpdate(f32 frame_time) {
    rtltimer1 = static_cast<u16>(static_cast<i32>(rtltimer1adv * frame_time) + rtltimer1);
    NuTimeBarSlotReset(0, 6);
}
