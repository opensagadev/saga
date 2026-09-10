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

void SetLights(NUCOLOUR3 *colour0, NUVEC *direction0, NUCOLOUR3 *colour1, NUVEC *direction1, NUCOLOUR3 *colour2,
               NUVEC *direction2, NUVEC *ambient);

extern "C" {
    void rtlResetEx(rtldata_s *data, i32 reset_cached);
    void rtlApplySetScale(void *, rtldata_s *, NUVEC *, NUMTX *, i32, f32);
    void rtlDynamicMasterEnable(i32 enabled);
}

void SetFlicker(GameObject_s *object, float duration) {
    object->field_0x1024 = duration;
    object->flicker_flags &= ~7;
}

void ResetLights(nuvec_s *position, rtldata_s *data, void *set) {
    rtlResetEx(data, 1);
    if (position != NULL) {
        rtlApplySetScale(set, data, position, NULL, -1, 1.0f);
    }
}

extern "C" {
    NULIGHTINGSTATE NuRndrLightingStateCurrent = {};
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

rtldata_s lev_rtldata;

extern "C" {
    void rtlSetLights(rtldata_s *);
    void NuLightSpotFadeSet(u32);
}

static constexpr u32 kNeutralSpotLightFade = 0x80808080u;

i32 Lighting_HighlightFlash;
i32 (*Lighting_BlueFlickerFn)(GameObject_s *);
i32 Lighting_FlashRedOnLastHeart;
extern f32 GhostLightMul;

void SetLevelLights(void *set, float) {
    rtlApplySetScale(set, &lev_rtldata, reinterpret_cast<NUVEC *>(&global_camera.mtx.m30), NULL, 0x10, 1.0f);
    rtlSetLights(&lev_rtldata);
}

void LightGameObject(GameObject_s *object, void *set) {
    rtlApplySetScale(set, &object->light_data, &object->apiobj.position, NULL, -1, 1.0f);

    const rtldata_s &target = object->light_data;
    OBJECTLIGHTINGSTATE_s &current = object->lighting_state;
    const bool reset = (object->field_0xefc & 0x80) != 0;

    auto update_colour = [reset](NUCOLOUR3 &value, const NUCOLOUR3 &next) {
        if (reset) {
            value = next;
        } else {
            value.r = SeekValF(value.r, next.r, 5.0f);
            value.g = SeekValF(value.g, next.g, 5.0f);
            value.b = SeekValF(value.b, next.b, 5.0f);
        }
    };
    auto update_direction = [reset](NUVEC &value, const NUVEC &next) {
        if (reset) {
            value = next;
        } else {
            value.x = SeekValF(value.x, next.x, 5.0f);
            value.y = SeekValF(value.y, next.y, 5.0f);
            value.z = SeekValF(value.z, next.z, 5.0f);
        }
        if (value.x != 0.0f || value.y != 0.0f || value.z != 0.0f) {
            NuVecNorm(&value, &value);
        }
    };

    if (reset) {
        current.ambient = target.ambient;
    } else {
        current.ambient.x = SeekValF(current.ambient.x, target.ambient.x, 5.0f);
        current.ambient.y = SeekValF(current.ambient.y, target.ambient.y, 5.0f);
        current.ambient.z = SeekValF(current.ambient.z, target.ambient.z, 5.0f);
    }
    for (i32 light = 0; light < 3; ++light) {
        update_colour(current.intensity[light], target.intensity[light]);
        update_direction(current.direction[light], target.direction[light]);
    }
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

void LightSabreDebris(GameObject_s *) {
}

void SetSpotLightMode() {
    NuLightSpotFadeSet(kNeutralSpotLightFade);
}

void SetCreatureLights(APIOBJECT_s *object) {
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

    OBJECTLIGHTINGSTATE_s lights = owner->lighting_state;
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

void SetLights_RTLDATA(rtldata_s *data, float scale) {
    if (scale == 1.0f) {
        rtlSetLights(data);
        return;
    }
    rtldata_s scaled = *data;
    for (i32 i = 0; i < 3; ++i) {
        scaled.intensity[i].r *= scale;
        scaled.intensity[i].g *= scale;
        scaled.intensity[i].b *= scale;
    }
    scaled.ambient.x *= scale;
    scaled.ambient.y *= scale;
    scaled.ambient.z *= scale;
    rtlSetLights(&scaled);
}

void FreeGameObjectLights() {
}

void TurnEpisodeDoorLightsOn(i32) {
}

void LightSabre_ColourFromObj(i32, i32 *) {
}

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

extern "C" rtlset *rtlLoadSet(char *, VARIPTR *, i32);

void LoadLights(WORLDINFO_s *world, char *path) {
    char filename[268];
    sprintf(filename, "%s.rtl", path);
    world->rtl_set = rtlLoadSet(filename, &world->giz_buffer, world->unknown_0108.addr);
}

extern "C" {

    void IndexLights(rtlset *, VARIPTR *, i32) {
    }

} // extern "C"
