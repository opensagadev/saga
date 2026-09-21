#include "decomp.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/motion.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nutex.h"

#include <stdio.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" {
    NULIGHTINGSTATE NuRndrLightingStateCurrent = {};
}

rtldata_s lev_rtldata;
NUCOLOUR3 panelamb = {1.0f, 1.0f, 1.0f};
NUVEC paneldir[3] = {{-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}};
NUCOLOUR3 panelcol[3] = {{1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f}, {0.5f, 0.5f, 1.0f}};

extern "C" {
    void NuLightSpotFadeSet(u32);
}

static constexpr u32 kNeutralSpotLightFade = 0x80808080u;

i32 Lighting_HighlightFlash;
i32 (*Lighting_BlueFlickerFn)(GameObject_s *);
i32 Lighting_FlashRedOnLastHeart;
extern f32 GhostLightMul;

void SetLights(NUCOLOUR3 *colour0, NUVEC *direction0, NUCOLOUR3 *colour1, NUVEC *direction1, NUCOLOUR3 *colour2,
               NUVEC *direction2, NUVEC *ambient) {
    NuRndrLightingStateCurrent.direction[0] = *direction0;
    NuRndrLightingStateCurrent.direction[1] = *direction1;
    NuRndrLightingStateCurrent.direction[2] = *direction2;
    NuRndrLightingStateCurrent.intensity[0] = *colour0;
    NuRndrLightingStateCurrent.intensity[1] = *colour1;
    NuRndrLightingStateCurrent.intensity[2] = *colour2;
    NuRndrSetDirectionalLightsPS(direction0, colour0, direction1, colour1, direction2, colour2);

    NUCOLOUR3 *ambient_colour = reinterpret_cast<NUCOLOUR3 *>(ambient);
    NuRndrLightingStateCurrent.ambient = *ambient_colour;
    NuRndrSetAmbientLightPS(ambient_colour);
}

static inline __attribute__((always_inline)) void SetPanelLightsInline(
    NUCOLOUR3 *colour0, NUVEC *direction0, NUCOLOUR3 *colour1, NUVEC *direction1, NUCOLOUR3 *colour2,
    NUVEC *direction2, NUVEC *ambient) {
    NuRndrLightingStateCurrent.direction[0] = *direction0;
    NuRndrLightingStateCurrent.direction[1] = *direction1;
    NuRndrLightingStateCurrent.direction[2] = *direction2;
    NuRndrLightingStateCurrent.intensity[0] = *colour0;
    NuRndrLightingStateCurrent.intensity[1] = *colour1;
    NuRndrLightingStateCurrent.intensity[2] = *colour2;
    NuRndrSetDirectionalLightsPS(direction0, colour0, direction1, colour1, direction2, colour2);
    NUCOLOUR3 *ambient_colour = reinterpret_cast<NUCOLOUR3 *>(ambient);
    NuRndrLightingStateCurrent.ambient = *ambient_colour;
    NuRndrSetAmbientLightPS(ambient_colour);
}

__attribute__((force_align_arg_pointer)) void SetLights_RTLDATA(rtldata_s *data, float scale) {
    if (scale == 1.0f) {
        rtlSetLights(data);
        return;
    }
    rtldata_s scaled __attribute__((aligned(16))) = *data;
    scaled.intensity[0].r *= scale;
    scaled.intensity[0].g *= scale;
    scaled.intensity[0].b *= scale;
    scaled.intensity[1].r *= scale;
    scaled.intensity[1].g *= scale;
    scaled.intensity[1].b *= scale;
    scaled.intensity[2].r *= scale;
    scaled.intensity[2].g *= scale;
    scaled.intensity[2].b *= scale;
    scaled.ambient.x *= scale;
    scaled.ambient.y *= scale;
    scaled.ambient.z *= scale;
    rtlSetLights(&scaled);
}

void SetLevelLights(void *set, float scale) {
    f32 specular_value = rtlSpecularValue(&lev_rtldata);
    rtlResetEx(&lev_rtldata, 1);
    rtlApplySetScale(set, &lev_rtldata, reinterpret_cast<NUVEC *>(&global_camera.mtx.m30), NULL, 0x10, 1.0f);
    if (rtlSpecularValue(&lev_rtldata) == 0.0f) {
        rtlSetSpecularValue(&lev_rtldata, specular_value);
    }
    SetLights_RTLDATA(&lev_rtldata, scale);
    rtlSetSpecularLight(&lev_rtldata);

    NUCOLOUR4 ambient = {lev_rtldata.ambient.x, lev_rtldata.ambient.y, lev_rtldata.ambient.z, 128.0f};
    NuRndrSetAmbientLightSpecular(&ambient);
}

// Retail realigns this stack frame for the temporary colour arrays.
void __attribute__((force_align_arg_pointer)) SetPanelLights(float intensity) {
    const f32 scale = 0.5f * intensity;
    NUCOLOUR3 scaled_colours[3];
    NUCOLOUR3 scaled_ambient;
    NUCOLOUR3 *colour0;
    NUCOLOUR3 *colour1;
    NUCOLOUR3 *colour2;
    NUCOLOUR3 *ambient;

    if (scale == 1.0f) {
        colour0 = &panelcol[0];
        colour1 = &panelcol[1];
        colour2 = &panelcol[2];
        ambient = &panelamb;
    } else {
        scaled_colours[0].r = panelcol[0].r * scale;
        scaled_colours[0].g = panelcol[0].g * scale;
        scaled_colours[0].b = panelcol[0].b * scale;
        scaled_colours[1].r = panelcol[1].r * scale;
        scaled_colours[1].g = panelcol[1].g * scale;
        scaled_colours[1].b = panelcol[1].b * scale;
        scaled_colours[2].r = panelcol[2].r * scale;
        scaled_colours[2].g = panelcol[2].g * scale;
        scaled_colours[2].b = panelcol[2].b * scale;
        scaled_ambient.r = panelamb.r * scale;
        scaled_ambient.g = panelamb.g * scale;
        scaled_ambient.b = panelamb.b * scale;
        colour0 = &scaled_colours[0];
        colour1 = &scaled_colours[1];
        colour2 = &scaled_colours[2];
        ambient = &scaled_ambient;
    }

    NuRndrLightingStateCurrent.direction[0] = paneldir[0];
    NuRndrLightingStateCurrent.direction[1] = paneldir[1];
    NuRndrLightingStateCurrent.direction[2] = paneldir[2];
    NuRndrLightingStateCurrent.intensity[0] = colour0[0];
    NuRndrLightingStateCurrent.intensity[1] = colour0[1];
    NuRndrLightingStateCurrent.intensity[2] = colour0[2];
    NuRndrSetDirectionalLightsPS(&paneldir[0], colour0, &paneldir[1], colour1, &paneldir[2], colour2);
    NuRndrLightingStateCurrent.ambient = *ambient;
    NuRndrSetAmbientLightPS(ambient);
}

void ResetLights(nuvec_s *position, rtldata_s *data, void *set) {
    rtlResetEx(data, 1);
    if (position != NULL) {
        rtlApplySetScale(set, data, position, NULL, -1, 1.0f);
    }
}

void SetZeroLights() {
    const NUCOLOUR3 black = {0.0f, 0.0f, 0.0f};

    NuRndrLightingStateCurrent.direction[0] = nuvec_x;
    NuRndrLightingStateCurrent.direction[1] = nuvec_x;
    NuRndrLightingStateCurrent.direction[2] = nuvec_x;
    NuRndrLightingStateCurrent.intensity[0] = black;
    NuRndrLightingStateCurrent.intensity[1] = black;
    NuRndrLightingStateCurrent.intensity[2] = black;
    NuRndrSetDirectionalLightsPS(&nuvec_x, &black, &nuvec_x, &black, &nuvec_x, &black);

    NUCOLOUR3 *ambient = reinterpret_cast<NUCOLOUR3 *>(&nuvec_zero);
    NuRndrLightingStateCurrent.ambient = *ambient;
    NuRndrSetAmbientLightPS(ambient);
}

void LightGameObject(GameObject_s *object, void *set) {
    rtlResetEx(&object->light_data, 1);
    rtlApplySetScale(set, &object->light_data, &object->apiobj.collision_position, NULL, -1, 1.0f);

    const rtldata_s &target = object->light_data;
    OBJECTLIGHTINGSTATE_s &current = object->lighting_state;
    const bool reset = (object->field_0xefc & 0x80) != 0;

    f32 surface_fade = 0.0f;
    if (object->apiobj.field_0x218 != 2000000.0f &&
        (TerSurface[static_cast<i8>(object->apiobj.field_0x281)].flags & 8) != 0) {
        surface_fade = 1.0f;
    }
    if (reset) {
        object->field_0xd6c = surface_fade;
    } else {
        object->field_0xd6c = SeekLinearF(object->field_0xd6c, surface_fade, 3.0f * FRAMETIME);
    }

    if (reset) {
        current.ambient = target.ambient;
    } else {
        current.ambient.x = SeekValF(current.ambient.x, target.ambient.x, 5.0f);
        current.ambient.y = SeekValF(current.ambient.y, target.ambient.y, 5.0f);
        current.ambient.z = SeekValF(current.ambient.z, target.ambient.z, 5.0f);
    }

    if (reset) {
        current.intensity[0] = target.intensity[0];
        current.direction[0] = target.direction[0];
    } else {
        current.intensity[0].r = SeekValF(current.intensity[0].r, target.intensity[0].r, 5.0f);
        current.intensity[0].g = SeekValF(current.intensity[0].g, target.intensity[0].g, 5.0f);
        current.intensity[0].b = SeekValF(current.intensity[0].b, target.intensity[0].b, 5.0f);
        current.direction[0].x = SeekValF(current.direction[0].x, target.direction[0].x, 5.0f);
        current.direction[0].y = SeekValF(current.direction[0].y, target.direction[0].y, 5.0f);
        current.direction[0].z = SeekValF(current.direction[0].z, target.direction[0].z, 5.0f);
    }
    if (current.direction[0].x != 0.0f || current.direction[0].y != 0.0f || current.direction[0].z != 0.0f)
        NuVecNorm(&current.direction[0], &current.direction[0]);

    if (reset) {
        current.intensity[1] = target.intensity[1];
        current.direction[1] = target.direction[1];
    } else {
        current.intensity[1].r = SeekValF(current.intensity[1].r, target.intensity[1].r, 5.0f);
        current.intensity[1].g = SeekValF(current.intensity[1].g, target.intensity[1].g, 5.0f);
        current.intensity[1].b = SeekValF(current.intensity[1].b, target.intensity[1].b, 5.0f);
        current.direction[1].x = SeekValF(current.direction[1].x, target.direction[1].x, 5.0f);
        current.direction[1].y = SeekValF(current.direction[1].y, target.direction[1].y, 5.0f);
        current.direction[1].z = SeekValF(current.direction[1].z, target.direction[1].z, 5.0f);
    }
    if (current.direction[1].x != 0.0f || current.direction[1].y != 0.0f || current.direction[1].z != 0.0f)
        NuVecNorm(&current.direction[1], &current.direction[1]);

    if (reset) {
        current.intensity[2] = target.intensity[2];
        current.direction[2] = target.direction[2];
    } else {
        current.intensity[2].r = SeekValF(current.intensity[2].r, target.intensity[2].r, 5.0f);
        current.intensity[2].g = SeekValF(current.intensity[2].g, target.intensity[2].g, 5.0f);
        current.intensity[2].b = SeekValF(current.intensity[2].b, target.intensity[2].b, 5.0f);
        current.direction[2].x = SeekValF(current.direction[2].x, target.direction[2].x, 5.0f);
        current.direction[2].y = SeekValF(current.direction[2].y, target.direction[2].y, 5.0f);
        current.direction[2].z = SeekValF(current.direction[2].z, target.direction[2].z, 5.0f);
    }
    if (current.direction[2].x != 0.0f || current.direction[2].y != 0.0f || current.direction[2].z != 0.0f)
        NuVecNorm(&current.direction[2], &current.direction[2]);

    object->field_0xefc &= 0x7f;
}

void FindAndSetLights(nuvec_s *position, float scale, void *set) {
    rtldata_s lights;
    rtlResetEx(&lights, 1);
    rtlDynamicMasterEnable(0);
    rtlApplySetScale(set, &lights, position, NULL, -1, scale);
    rtlDynamicMasterEnable(1);
    rtlSetLights(&lights);
}

void SetSpotLightMode() {
    NuLightSpotFadeSet(kNeutralSpotLightFade);
}

__attribute__((force_align_arg_pointer)) void SetCreatureLights(APIOBJECT_s *object) {
    GameObject_s *owner = object->objptr;

    if (Cheats_CheckFlags(1) != 0) {
        SetZeroLights();
        return;
    }

    if (owner->apiobj.field_0x287 != 0) {
        rtldata_s *lights = &owner->light_data;
        SetLights(&lights->intensity[0], &lights->direction[0], &lights->intensity[1], &lights->direction[1],
                  &lights->intensity[2], &lights->direction[2], &lights->ambient);
        return;
    }

    OBJECTLIGHTINGSTATE_s lights;
    lights.ambient = owner->lighting_state.ambient;
    lights.intensity[0] = owner->lighting_state.intensity[0];
    lights.direction[0] = owner->lighting_state.direction[0];
    lights.intensity[1] = owner->lighting_state.intensity[1];
    lights.direction[1] = owner->lighting_state.direction[1];
    lights.intensity[2] = owner->lighting_state.intensity[2];
    lights.direction[2] = owner->lighting_state.direction[2];
    f32 red = 1.0f, green = 1.0f, blue = 1.0f;
    GAMECHARACTERDATA *character = static_cast<GAMECHARACTERDATA *>(owner->apiobj.character_data->field11_0x24);
    if (owner->field_0x1024 > 0.0f) {
        const f32 flash = owner->field_0x1024 / 0.4f;
        red = 1.0f + flash;
        green = blue = 1.0f - flash;
    } else if (Lighting_HighlightFlash != 0 && static_cast<i8>(owner->apiobj.object_flags) < 0 &&
               owner->timer_d5c > 0.0f && (owner->timer_d5c >= 2.0f || NuFmod(owner->timer_d5c, 0.4f) >= 0.2f)) {
        if (owner->apiobj.field_0x27c == 1) {
            red = 1.7f;
            green = 2.0f;
            blue = 1.4f;
        } else {
            red = 1.4f;
            green = 1.85f;
            blue = 2.0f;
        }
    } else if ((character->flags_090 & 0x8000) != 0 && static_cast<i8>(owner->apiobj.object_flags) >= 0) {
        red = 1.4f;
        green = 1.85f;
        blue = 2.0f;
    } else if (Lighting_BlueFlickerFn != NULL && Lighting_BlueFlickerFn(owner) != 0) {
        if (qrand() > 0x7fff) {
            red = 0.25f;
            blue = 0.5f;
        } else {
            red = 1.0f;
            blue = 2.0f;
        }
        character = static_cast<GAMECHARACTERDATA *>(owner->apiobj.character_data->field11_0x24);
        green = blue;
        if ((character->flags_094[2] & 1) != 0)
            red = blue;
        else
            red *= 0.0f;
    } else if (owner->interaction_arrow_blend > 0.0f) {
        const f32 phase = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f) * 2.0f;
        const f32 scale = owner->interaction_arrow_blend * 0.5f * NU_SIN_LUT(static_cast<i32>(phase * 65536.0f)) + 1.0f;
        red = green = blue = scale;
    } else if (Lighting_FlashRedOnLastHeart != 0 && static_cast<i8>(owner->apiobj.object_flags) < 0 &&
               owner->current_hp == 1 && owner->hitpoints > 1 && owner->field_0x1024 < -0.5f) {
        const f32 phase = ((owner->field_0x1024 + 1.0f) * 2.0f) * 65536.0f + 16384.0f;
        const f32 flash = (1.0f - NU_SIN_LUT(static_cast<i32>(phase))) * 0.5f * 0.333f;
        red = 1.0f + flash;
        green = blue = 1.0f - flash;
    } else if (owner->spawn_protection_timer > 0.0f && owner->apiobj.field_0x27c != -1) {
        const f32 phase = NuFmod(2.5f - owner->spawn_protection_timer, 0.5f) * 2.0f;
        const f32 scale = NuTrigTable[static_cast<u16>(static_cast<i32>(phase * 65536.0f)) >> 1] + 1.0f;
        red = green = blue = scale;
    }
    if (owner->field_0xd6c > 0.0f) {
        const f32 scale = owner->field_0xd6c * -0.334f + 1.0f;
        red *= scale;
        green *= scale;
        blue *= scale;
    }
    character = static_cast<GAMECHARACTERDATA *>(owner->apiobj.character_data->field11_0x24);
    if ((character->flags_090 & 0x8000) != 0) {
        red *= GhostLightMul;
        green *= GhostLightMul;
        blue *= GhostLightMul;
        if (red > 2.0f)
            red = 2.0f;
        if (green > 2.0f)
            green = 2.0f;
        if (blue > 2.0f)
            blue = 2.0f;
    }
    if (red != 1.0f) {
        lights.ambient.x *= red;
        for (i32 i = 0; i < 3; ++i)
            lights.intensity[i].r *= red;
    }
    if (green != 1.0f) {
        lights.ambient.y *= green;
        for (i32 i = 0; i < 3; ++i)
            lights.intensity[i].g *= green;
    }
    if (blue != 1.0f) {
        lights.ambient.z *= blue;
        for (i32 i = 0; i < 3; ++i)
            lights.intensity[i].b *= blue;
    }
    SetLights(&lights.intensity[0], &lights.direction[0], &lights.intensity[1], &lights.direction[1],
              &lights.intensity[2], &lights.direction[2], &lights.ambient);
    owner->targeted_flash -= FRAMETIME;
    if (TouchHacks::ShouldFlash(owner->targeted_flash)) {
        NUCOLOUR3 *colour = TouchHacks::GetFlashColour();
        NuRndrLightingStateCurrent.ambient = *colour;
        NuRndrSetAmbientLightPS(colour);
    }
}

void FreeGameObjectLights() {
    i32 i = 0;
    GameObject_s *object = Obj;
    for (; i < HIGHGAMEOBJECT; ++i, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001)
            continue;
        if (object->dynamic_light_id == -1)
            continue;
        rtlDynamicFree(object->dynamic_light_id);
        object->dynamic_light_id = -1;
    }
}

void LoadLights(WORLDINFO_s *world, char *path) {
    char filename[268];
    sprintf(filename, "%s.rtl", path);
    world->rtl_set = rtlLoadSet(filename, &world->giz_buffer, world->unknown_0108.addr);
    sprintf(filename, "%s.bur", path);
    world->burnset = edrtlBurnoutLoad(filename, &world->giz_buffer, world->unknown_0108.addr);
}

void InitGameObjectLights(void) {
    GameObject_s *object = Obj;
    for (i32 i = 0; i < 64; ++i)
        object[i].dynamic_light_id = -1;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001)
            continue;
        object->dynamic_light_id = rtlDynamicAlloc();
        if (object->dynamic_light_id == -1)
            continue;
        rtlDynamicSetType(object->dynamic_light_id, 2);
        rtlDynamicEnable(object->dynamic_light_id, 0);
    }
}
