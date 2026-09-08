#include "legoapi/characters/core/charconfig.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/world/level.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
    extern void *perm_debrissys;
    extern u16 SURFACEBITS_DUST;
}
i32 LayerFromName(GAMECHARACTERDATA_s *, char *);
i32 MakeLayerList_Name(CHARACTERMODEL_s *, i16 *, u32);

static void CC_CharClipToBlobShadows(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_098[0] = (charconfig.runtime->flags_098[0] & 0x7f) | (enabled << 7);
}

static void CC_already_got_hat(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x10;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x10u;
    }
}

static void CC_always_stop_to_shoot(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_098[0] = (charconfig.runtime->flags_098[0] & 0xfb) | (enabled << 2);
}

static void CC_atatheadmovement(NUFPAR *parser) {
    charconfig.runtime->flags_094[3] |= 0x40;
}

static void CC_backwards_factor(NUFPAR *parser) {
    charconfig.runtime->field_0x20 = NuFParGetFloat(parser);
}

static void CC_baddie(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x4;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x4u;
    }
}

static void CC_beast(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x40000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x40000000u;
    }
}

static void CC_blobshadow_alpha(NUFPAR *parser) {
    charconfig.runtime->field_0xf6 = static_cast<u8>(NuFParGetInt(parser));
}

static void CC_blobshadow_size(NUFPAR *parser) {
    charconfig.runtime->field_0x8c = fabsf(NuFParGetFloat(parser));
}

static void CC_bypass_security(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x2000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x2000000u;
    }
}

static void CC_can_communicate(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[0] = (charconfig.runtime->flags_094[0] & 0x7f) | (enabled << 7);
}

static void CC_can_drag_bombs(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x400;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x400u;
    }
}

static void CC_can_open_doors(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x400;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x400u;
    }
}

static void CC_can_shoot_offscreen(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x1000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x1000u;
    }
}

static void CC_cannon(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x800;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x800u;
    }
}

static void CC_cape_layer_index(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        charconfig.runtime->cape_layer = static_cast<i8>(locator);
    }
}

static void CC_choke(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x2;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x2u;
    }
}

static void CC_clear_sfx_misc(NUFPAR *parser) {
    charconfig.runtime->sfx_misc[0] = -1;
}

static void CC_cloak_down_angle(NUFPAR *parser) {
    charconfig.runtime->field_0x60 = (NuFParGetFloat(parser) * 3.1415927f) / 180.0f;
}

static void CC_cloak_up_angle(NUFPAR *parser) {
    charconfig.runtime->field_0x5c = (NuFParGetFloat(parser) * 3.1415927f) / 180.0f;
}

static void CC_collision_locator(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->collision_locator = static_cast<i8>(locator);
        }
    }
}

static void CC_complex_shadow(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x10000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x10000u;
    }
}

static void CC_deflect_bolts_in_minikit_bonus(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[3] = (charconfig.runtime->flags_094[3] & 0xfb) | (enabled << 2);
}

static void CC_die_usecurrentlayers(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[2] = (charconfig.runtime->flags_094[2] & 0xfb) | (enabled << 2);
}

static void CC_dont_draw_rider(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_098[0] = (charconfig.runtime->flags_098[0] & 0xbf) | (enabled << 6);
}

static void CC_dont_move_out_of_way(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x2000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x2000u;
    }
}

static void CC_double_jump_hover(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[1] = (charconfig.runtime->flags_094[1] & 0xdf) | (enabled << 5);
}

static void CC_droid(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x10;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x10u;
    }
}

static void CC_extra_character_locator(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->extra_character_locator = static_cast<i8>(locator);
        }
    }
}

static void CC_ghost(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x8000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x8000u;
    }
}

static void CC_grapple_locator(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->grapple_locators[0] = static_cast<i8>(locator);
        }
    }
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->grapple_locators[1] = static_cast<i8>(locator);
        }
    }
}

static void CC_hair_layer_index(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        charconfig.runtime->hair_layer = static_cast<i8>(locator);
    }
}

static void CC_has_scene_element(NUFPAR *parser) {
    charconfig.character->flags |= 1;
}

static void CC_hazard_protection(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x4000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x4000000u;
    }
}

static void CC_helmet_locator(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->helmet_locator = static_cast<i8>(locator);
        }
    }
}

static void CC_hover_over_mud(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[2] = (charconfig.runtime->flags_094[2] & 0x7f) | (enabled << 7);
}

static void CC_icon(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        charconfig.character->field20_0x42 = static_cast<i16>(LevelObject_FindIndexFromName(parser->word_buf));
        if (charconfig.character->field20_0x42 == -1 && (charconfig.flags & 0x40) != 0) {
            if (LevelObject_AddExtra(parser->word_buf, 3) != 0) {
                charconfig.character->field20_0x42 = static_cast<i16>(LevelObject_FindIndexFromName(parser->word_buf));
                NuStrCat(parser->word_buf, "1");
                LevelObject_AddExtra(parser->word_buf, 3);
            }
        }
    }
}

static void CC_immune_to_forcepush(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[1] = (charconfig.runtime->flags_094[1] & 0xfd) | (enabled << 1);
}

static void CC_immune_to_magnets(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[1] = (charconfig.runtime->flags_094[1] & 0xfe) | (enabled << 0);
}

static void CC_jedi(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x8;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x8u;
    }
}

static void CC_jump_move_speed_scale(NUFPAR *parser) {
    charconfig.runtime->field_0x84 = NuFParGetFloat(parser);
}

static void CC_layer(NUFPAR *parser) {
    GAMECHARACTERDATA_s *data = charconfig.runtime;
    if ((charconfig.flags & 2) == 0 || data->layer_count >= 32)
        return;
    if (NuFParGetWord(parser) == 0 || NuStrLen(parser->word_buf) >= 24)
        return;
    GAMECHARACTERLAYER_s *layer = &data->layers[data->layer_count];
    NuStrCpy(layer->name, parser->word_buf);
    const u32 bit = NuFParGetInt(parser);
    if (bit >= 32)
        return;
    for (i32 i = 0; i < data->layer_count; ++i) {
        if (data->layers[i].mask_bit == static_cast<i32>(bit))
            return;
    }
    charconfig.named_layers = 1;
    layer->mask_bit = static_cast<i16>(bit);
    layer->hierarchy_layer_index = -1;
    ++data->layer_count;
    data->make_layer_list = MakeLayerList_Name;
}

static void CC_layers_special(NUFPAR *parser) {
    u32 *mask = &charconfig.runtime->layer_mask_special;
    *mask = 0;
    i32 count = 0;
    if (charconfig.named_layers == 0) {
        while (NuFParGetWord(parser) != 0) {
            const u32 layer = NuAToI(parser->word_buf);
            if (layer < 32) {
                ++count;
                *mask |= 1u << layer;
            }
        }
    } else {
        while (NuFParGetWord(parser) != 0) {
            const i32 layer = LayerFromName(charconfig.runtime, parser->word_buf);
            if (layer != -1) {
                ++count;
                *mask |= 1u << layer;
            }
        }
    }
    if (count != 0) {
        charconfig.runtime->layer_mask_dead = *mask;
        charconfig.runtime->layer_mask_low = *mask;
        charconfig.runtime->layer_mask_medium = *mask;
        charconfig.runtime->layer_mask = *mask;
    }
}

static void CC_lowres_swapdist(NUFPAR *parser) {
    charconfig.runtime->field_0xb8 = NuFParGetFloat(parser);
}

static void CC_mass(NUFPAR *parser) {
    charconfig.character->field13_0x2c = NuFParGetFloat(parser);
}

static void CC_max_viewheight(NUFPAR *parser) {
    charconfig.runtime->maxviewheight = NuFParGetFloat(parser);
}

static void CC_maxy(NUFPAR *parser) {
    charconfig.character->bounds_max_y = NuFParGetFloat(parser);
}

static void CC_mediumres_swapdist(NUFPAR *parser) {
    charconfig.runtime->field_0xb4 = NuFParGetFloat(parser);
}

static void CC_min_viewheight(NUFPAR *parser) {
    charconfig.runtime->minviewheight = NuFParGetFloat(parser);
}

static void CC_minikit_noscenewhenplayable(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[1] = (charconfig.runtime->flags_094[1] & 0xfb) | (enabled << 2);
}

static void CC_minikit_with_scene(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x4000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x4000000u;
    }
    charconfig.runtime->flags_094[0] =
        (charconfig.runtime->flags_094[0] & 0xfe) | ((charconfig.character->model_flags >> 26) & 1);
}

static void CC_miny(NUFPAR *parser) {
    charconfig.character->bounds_min_y = NuFParGetFloat(parser);
}

static void CC_no_offpath_teleport(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x400000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x400000u;
    }
}

static void CC_no_start_punch_sfx(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_098[0] = (charconfig.runtime->flags_098[0] & 0xef) | (enabled << 4);
}

static void CC_no_weapon_draw(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[2] = (charconfig.runtime->flags_094[2] & 0xdf) | (enabled << 5);
}

static void CC_oldheadmovement(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x20000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x20000u;
    }
}

static void CC_only_active_when_taken_over(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x20000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x20000000u;
    }
}

static void CC_prefers_brawling(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x80000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x80000000u;
    }
}

static void CC_punch_weapon_out(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x20;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x20u;
    }
}

static void CC_put_weapon_away_on_shoot(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_098[0] = (charconfig.runtime->flags_098[0] & 0xfd) | (enabled << 1);
}

static void CC_radius(NUFPAR *parser) {
    charconfig.character->collision_radius = NuFParGetFloat(parser);
}

static void CC_ride_layersoff(NUFPAR *parser) {
    u32 *mask = &charconfig.runtime->ride_layers_off;
    *mask = 0;
    i32 count = 0;
    if (charconfig.named_layers == 0) {
        while (NuFParGetWord(parser) != 0) {
            const u32 layer = NuAToI(parser->word_buf);
            if (layer < 32) {
                ++count;
                *mask |= 1u << layer;
            }
        }
    } else {
        while (NuFParGetWord(parser) != 0) {
            const i32 layer = LayerFromName(charconfig.runtime, parser->word_buf);
            if (layer != -1) {
                ++count;
                *mask |= 1u << layer;
            }
        }
    }
}

static void CC_rocket_locator(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->rocket_locator = static_cast<i8>(locator);
        }
    }
}

static void CC_scale(NUFPAR *parser) {
    charconfig.character->model_scale = NuFParGetFloat(parser);
}

static void CC_second_shot_only(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_098[0] = (charconfig.runtime->flags_098[0] & 0xf7) | (enabled << 3);
}

static void CC_set_timebaseupdate1(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const f32 distance = NuAToF(parser->word_buf);
        if (NuFParGetWord(parser) != 0) {
            const i32 frames = NuAToI(parser->word_buf);
            charconfig.runtime->ai_update_distance_0 = distance;
            charconfig.runtime->ai_update_interval_0 = static_cast<u8>(frames);
            charconfig.flags |= 0x10;
        }
    }
}

static void CC_set_timebaseupdate2(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const f32 distance = NuAToF(parser->word_buf);
        if (NuFParGetWord(parser) != 0) {
            const i32 frames = NuAToI(parser->word_buf);
            charconfig.runtime->ai_update_distance_0 = distance;
            charconfig.runtime->ai_update_interval_0 = static_cast<u8>(frames);
            charconfig.flags |= 0x10;
        }
    }
}

static void CC_set_timebaseupdate3(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const f32 distance = NuAToF(parser->word_buf);
        if (NuFParGetWord(parser) != 0) {
            const i32 frames = NuAToI(parser->word_buf);
            charconfig.runtime->ai_update_distance_0 = distance;
            charconfig.runtime->ai_update_interval_0 = static_cast<u8>(frames);
            charconfig.flags |= 0x10;
        }
    }
}

static void CC_set_timebaseupdate4(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const f32 distance = NuAToF(parser->word_buf);
        if (NuFParGetWord(parser) != 0) {
            const i32 frames = NuAToI(parser->word_buf);
            charconfig.runtime->ai_update_distance_0 = distance;
            charconfig.runtime->ai_update_interval_0 = static_cast<u8>(frames);
            charconfig.flags |= 0x10;
        }
    }
}

static void CC_shadow_locators(NUFPAR *parser) {
    charconfig.runtime->shadow_locators = 0;
    while (NuFParGetWord(parser) != 0) {
        const u32 locator = NuAToI(parser->word_buf);
        if (locator < 16) {
            charconfig.runtime->shadow_locators |= 1u << locator;
        }
    }
}

static void CC_shadow_orientation(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x200000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x200000u;
    }
}

static void CC_shield_hit_points(NUFPAR *parser) {
    charconfig.runtime->field_0xf5 = static_cast<u8>(NuFParGetInt(parser));
}

static void CC_shield_locator(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->shield_locator = static_cast<i8>(locator);
        }
    }
}

static void CC_single_jump_slam(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_098[0] = (charconfig.runtime->flags_098[0] & 0xdf) | (enabled << 5);
}

static void CC_slide_orientation(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x200;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x200u;
    }
}

static void CC_slow_down_time(NUFPAR *parser) {
    charconfig.runtime->field_0x80 = NuFParGetFloat(parser);
}

static void CC_streak(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        if (NuStrICmp(parser->word_buf, "red") == 0) {
            charconfig.runtime->field_0x117 = 0;
        } else if (NuStrICmp(parser->word_buf, "green") == 0) {
            charconfig.runtime->field_0x117 = 1;
        } else if (NuStrICmp(parser->word_buf, "blue") == 0) {
            charconfig.runtime->field_0x117 = 2;
        } else if (NuStrICmp(parser->word_buf, "purple") == 0) {
            charconfig.runtime->field_0x117 = 3;
        }
    }
}

static void CC_streak_1_locators(NUFPAR *parser) {
    charconfig.runtime->streak_joints[0][0] = -1;
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator) < 16) {
            charconfig.runtime->streak_joints[0][0] = static_cast<i8>(locator);
        }
    }
    charconfig.runtime->streak_joints[0][1] = -1;
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator) < 16) {
            charconfig.runtime->streak_joints[0][1] = static_cast<i8>(locator);
        }
    }
}

static void CC_streak_2_locators(NUFPAR *parser) {
    charconfig.runtime->streak_joints[1][0] = -1;
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator) < 16) {
            charconfig.runtime->streak_joints[1][0] = static_cast<i8>(locator);
        }
    }
    charconfig.runtime->streak_joints[1][1] = -1;
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator) < 16) {
            charconfig.runtime->streak_joints[1][1] = static_cast<i8>(locator);
        }
    }
}

static void CC_streak_3_locators(NUFPAR *parser) {
    charconfig.runtime->streak_joints[2][0] = -1;
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator) < 16) {
            charconfig.runtime->streak_joints[2][0] = static_cast<i8>(locator);
        }
    }
    charconfig.runtime->streak_joints[2][1] = -1;
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator) < 16) {
            charconfig.runtime->streak_joints[2][1] = static_cast<i8>(locator);
        }
    }
}

static void CC_streak_4_locators(NUFPAR *parser) {
    charconfig.runtime->streak_joints[3][0] = -1;
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator) < 16) {
            charconfig.runtime->streak_joints[3][0] = static_cast<i8>(locator);
        }
    }
    charconfig.runtime->streak_joints[3][1] = -1;
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator) < 16) {
            charconfig.runtime->streak_joints[3][1] = static_cast<i8>(locator);
        }
    }
}

static void CC_super_strength(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x800000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x800000u;
    }
}

static void CC_thingy_locator(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->thingy_locator = static_cast<i8>(locator);
        }
    }
}

static void CC_thrust_locators(NUFPAR *parser) {
    charconfig.runtime->thrust_locators = 0;
    while (NuFParGetWord(parser) != 0) {
        const u32 locator = NuAToI(parser->word_buf);
        if (locator < 16) {
            charconfig.runtime->thrust_locators |= 1u << locator;
        }
    }
}

static void CC_tightrope_tilt(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x10000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x10000000u;
    }
}

static void CC_tightrope_walk(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x80000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x80000u;
    }
}

static void CC_toggle_if_in_minikit_bonus(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[3] = (charconfig.runtime->flags_094[3] & 0xfd) | (enabled << 1);
}

static void CC_toggle_if_not_in_collection(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[3] = (charconfig.runtime->flags_094[3] & 0xfe) | (enabled << 0);
}

static void CC_transformatron(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x1000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x1000000u;
    }
}

static void CC_turning_circle(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x100;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x100u;
    }
}

static void CC_wait_for_weapon_in_out(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[1] = (charconfig.runtime->flags_094[1] & 0xef) | (enabled << 4);
}

static void CC_weapon(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        charconfig.runtime->weapon_model = static_cast<i16>(LevelObject_FindIndexFromName(parser->word_buf));
    }
}

static void CC_weapon_locator_1(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->weapon_joints[0] = static_cast<i8>(locator);
        }
    }
}

static void CC_weapon_locator_2(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->weapon_joints[1] = static_cast<i8>(locator);
        }
    }
}

static void CC_weapon_locator_3(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->weapon_joints[2] = static_cast<i8>(locator);
        }
    }
}

static void CC_weapon_locator_4(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->weapon_joints[3] = static_cast<i8>(locator);
        }
    }
}

static void CC_weapon_shoot_locator_1(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->weapon_shoot_joints[0] = static_cast<i8>(locator);
        }
    }
}

static void CC_weapon_shoot_locator_2(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->weapon_shoot_joints[1] = static_cast<i8>(locator);
        }
    }
}

static void CC_weapon_shoot_locator_3(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->weapon_shoot_joints[2] = static_cast<i8>(locator);
        }
    }
}

static void CC_weapon_shoot_locator_4(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->weapon_shoot_joints[3] = static_cast<i8>(locator);
        }
    }
}

static void CC_white_lightning(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[2] = (charconfig.runtime->flags_094[2] & 0xfe) | (enabled << 0);
}

static void CC_zipup(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x100000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x100000u;
    }
}

static void CC_acceleration(NUFPAR *parser) {
    charconfig.runtime->field_0x44 = NuFParGetFloat(parser);
}

static void CC_air_gravity(NUFPAR *parser) {
    charconfig.runtime->field_0x24 = NuFParGetFloat(parser);
}

static void CC_SetAnimFlags(NUFPAR *parser, CHARACTERANIM_s *animation, u32 flags) {
    animation->flags |= flags;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        animation->flags &= ~flags;
    }
}

static void CC_SetAnimMiscFlags(NUFPAR *parser, CHARACTERANIM_s *animation, u16 flags) {
    animation->misc_flags |= flags;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        animation->misc_flags &= ~flags;
    }
}

static void CC_anim_start(NUFPAR *parser) {
    if (charconfig.animation_count >= 100) {
        while (NuFParGetLine(parser) != 0) {
            if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "anim_end") == 0)
                break;
        }
        return;
    }
    if (NuFParGetWord(parser) == 0 || NuStrLen(parser->word_buf) >= 40)
        return;
    char name[40];
    NuStrCpy(name, parser->word_buf);
    CHARACTERANIM_s animation;
    memset(&animation, 0, sizeof(animation));
    animation.action_id = -1;
    animation.flags = (charconfig.flags & 1) != 0 ? 14 : 6;
    animation.blend_in_time = 0.2f;
    animation.blend_out_time = 0.2f;
    animation.playback_rate = 30.0f;
    animation.locator = 0xff;
    while (NuFParGetLine(parser) != 0) {
        if (NuFParGetWord(parser) == 0)
            continue;
        if (NuStrICmp(parser->word_buf, "action") == 0) {
            if (NuFParGetWord(parser) != 0)
                animation.action_id = static_cast<i16>(ActionFromName(parser->word_buf));
            if (animation.action_id == -1)
                return;
        } else if (NuStrICmp(parser->word_buf, "bsa") == 0) {
            if (NuFParGetWord(parser) != 0) {
                if (NuStrICmp(parser->word_buf, "on") == 0)
                    animation.flags |= 8;
                else if (NuStrICmp(parser->word_buf, "off") == 0)
                    animation.flags &= ~8u;
            }
        } else if (NuStrICmp(parser->word_buf, "cycle") == 0) {
            if (NuFParGetWord(parser) != 0) {
                if (NuStrICmp(parser->word_buf, "on") == 0)
                    animation.flags |= 2;
                else if (NuStrICmp(parser->word_buf, "off") == 0)
                    animation.flags &= ~2u;
            }
        } else if (NuStrICmp(parser->word_buf, "type") == 0) {
            if (NuFParGetWord(parser) != 0) {
                if (NuStrICmp(parser->word_buf, "idle") == 0)
                    animation.flags |= 0x10;
                else
                    animation.flags &= ~0x10u;
            }
        } else if (NuStrICmp(parser->word_buf, "fpsec") == 0) {
            animation.playback_rate = NuFParGetFloat(parser);
        } else if (NuStrICmp(parser->word_buf, "blend_in") == 0) {
            animation.blend_in_time = NuFParGetFloat(parser);
        } else if (NuStrICmp(parser->word_buf, "blend_out") == 0) {
            animation.blend_out_time = NuFParGetFloat(parser);
        } else if (NuStrICmp(parser->word_buf, "speedmul_speed") == 0) {
            animation.movement_speed = NuFParGetFloat(parser);
        } else if (NuStrICmp(parser->word_buf, "speedmul_maxfps") == 0) {
            animation.movement_rate_cap = NuFParGetFloat(parser);
        } else if (NuStrICmp(parser->word_buf, "translate") == 0) {
            CC_SetAnimFlags(parser, &animation, 0x20);
        } else if (NuStrICmp(parser->word_buf, "can_play_backwards") == 0) {
            CC_SetAnimFlags(parser, &animation, 0x80);
        } else if (NuStrICmp(parser->word_buf, "footsteps") == 0) {
            animation.flags |= 0x100;
            while (NuFParGetWord(parser) != 0) {
                if (NuStrICmp(parser->word_buf, "off") == 0) {
                    animation.flags &= ~0x70100u;
                    break;
                } else if (NuStrICmp(parser->word_buf, "4frames") == 0)
                    animation.flags |= 0x10000;
                else if (NuStrICmp(parser->word_buf, "judder") == 0)
                    animation.flags |= 0x20000;
                else if (NuStrICmp(parser->word_buf, "shake") == 0)
                    animation.flags |= 0x40000;
            }
        } else if (NuStrICmp(parser->word_buf, "no_headturn") == 0 ||
                   NuStrICmp(parser->word_buf, "headturn_off") == 0) {
            animation.flags |= 0x40;
        } else if (NuStrICmp(parser->word_buf, "headturn") == 0) {
            if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
                animation.flags |= 0x40;
            else
                animation.flags &= ~0x40u;
        } else if (NuStrICmp(parser->word_buf, "minreps") == 0) {
            animation.minimum_repetitions = static_cast<i8>(abs(NuFParGetInt(parser)));
        } else if (NuStrICmp(parser->word_buf, "maxreps") == 0) {
            animation.maximum_repetitions = static_cast<i8>(abs(NuFParGetInt(parser)));
        } else if (NuStrICmp(parser->word_buf, "speed") == 0) {
            animation.action_speed = NuFParGetFloat(parser);
        } else if (NuStrICmp(parser->word_buf, "frame") == 0 || NuStrICmp(parser->word_buf, "frame1") == 0 ||
                   NuStrICmp(parser->word_buf, "frame_hit") == 0) {
            animation.event_frame_1 = NuFParGetFloat(parser);
        } else if (NuStrICmp(parser->word_buf, "frame2") == 0 || NuStrICmp(parser->word_buf, "frame_end") == 0) {
            animation.event_frame_2 = NuFParGetFloat(parser);
        } else if (NuStrICmp(parser->word_buf, "frame3") == 0) {
            animation.event_frame_3 = NuFParGetFloat(parser);
        } else if (NuStrICmp(parser->word_buf, "frame4") == 0) {
            animation.event_frame_4 = NuFParGetFloat(parser);
        } else if (NuStrICmp(parser->word_buf, "frame_stop") == 0) {
            animation.stop_frame = NuFParGetFloat(parser);
        } else if (NuStrICmp(parser->word_buf, "priority") == 0) {
            animation.priority = static_cast<u8>(NuFParGetInt(parser));
        } else if (NuStrICmp(parser->word_buf, "gun_off") == 0) {
            animation.flags |= 0x400;
        } else if (NuStrICmp(parser->word_buf, "gun_on") == 0) {
            animation.flags |= 0x800;
        } else if (NuStrICmp(parser->word_buf, "gun") == 0) {
            if (NuFParGetWord(parser) != 0) {
                if (NuStrICmp(parser->word_buf, "on") == 0)
                    animation.flags |= 0x800;
                else if (NuStrICmp(parser->word_buf, "off") == 0)
                    animation.flags |= 0x400;
            }
        } else if (NuStrICmp(parser->word_buf, "locator") == 0) {
            if (NuFParGetWord(parser) != 0) {
                const u32 locator = NuAToI(parser->word_buf);
                if (locator < 16)
                    animation.locator = static_cast<u8>(locator);
            }
        } else if (NuStrICmp(parser->word_buf, "is_punch") == 0) {
            CC_SetAnimMiscFlags(parser, &animation, 1);
        } else if (NuStrICmp(parser->word_buf, "is_kick") == 0) {
            CC_SetAnimMiscFlags(parser, &animation, 2);
        } else if (NuStrICmp(parser->word_buf, "is_whip") == 0) {
            CC_SetAnimMiscFlags(parser, &animation, 4);
        } else if (NuStrICmp(parser->word_buf, "extra_character_off") == 0) {
            CC_SetAnimMiscFlags(parser, &animation, 8);
        } else if (NuStrICmp(parser->word_buf, "effect_start") == 0) {
            if ((charconfig.flags & 0x60) != 0x60) {
                while (NuFParGetLine(parser) != 0) {
                    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "effect_end") == 0)
                        break;
                }
            } else if (charconfig.effect_count < 32 && animation.action_id != -1) {
                CHARACTER_EFFECT_s *effect = &charconfig.effect_scratch[charconfig.effect_count];
                effect->frame_1 = 1.0f;
                effect->frame_2 = 1.0f;
                effect->rumble = 0.0f;
                effect->shake = 0.0f;
                effect->judder = 0.0f;
                effect->minimum = 0.0f;
                effect->maximum = 0.0f;
                effect->character_id = charconfig.character_id;
                effect->debris_id = -1;
                effect->action_id = animation.action_id;
                effect->flags = 0x8000;
                effect->locators = 0;
                effect->random = 0;
                effect->particle_count = 0;
                effect->sound_id = -1;
                effect->surfaces = 0xffff;
                effect->bits_on = 0;
                effect->bits_off = 0;
                while (NuFParGetLine(parser) != 0) {
                    if (NuFParGetWord(parser) == 0)
                        continue;
                    if (NuStrICmp(parser->word_buf, "deb_name") == 0) {
                        if (NuFParGetWord(parser) != 0) {
                            effect->debris_id =
                                perm_debrissys == NULL
                                    ? -1
                                    : static_cast<i16>(FindGameDebris(static_cast<APIDEBRISSYS_s *>(perm_debrissys),
                                                                      parser->word_buf));
                        }
                    } else if (NuStrICmp(parser->word_buf, "condition") == 0) {
                        if (NuFParGetWord(parser) != 0) {
                            if (NuStrICmp(parser->word_buf, "on_ground") == 0)
                                effect->flags |= 0x10;
                            else if (NuStrICmp(parser->word_buf, "under_water") == 0)
                                effect->flags |= 0x20;
                            else if (NuStrICmp(parser->word_buf, "above_water") == 0)
                                effect->flags |= 0x40;
                            else if (NuStrICmp(parser->word_buf, "intersect_water") == 0)
                                effect->flags |= 0x80;
                        }
                    } else if (NuStrICmp(parser->word_buf, "in_only") == 0)
                        effect->flags |= 0x100;
                    else if (NuStrICmp(parser->word_buf, "not_in") == 0)
                        effect->flags |= 0x200;
                    else if (NuStrICmp(parser->word_buf, "player_only") == 0)
                        effect->flags |= 0x400;
                    else if (NuStrICmp(parser->word_buf, "num_particles") == 0) {
                        const i32 count = NuFParGetInt(parser);
                        if (count > 0) {
                            effect->particle_count = static_cast<u8>(count);
                            effect->flags |= 1;
                        }
                    } else if (NuStrICmp(parser->word_buf, "frame") == 0 ||
                               NuStrICmp(parser->word_buf, "frame1") == 0) {
                        effect->frame_1 = NuFParGetFloat(parser);
                    } else if (NuStrICmp(parser->word_buf, "frame2") == 0)
                        effect->frame_2 = NuFParGetFloat(parser);
                    else if (NuStrICmp(parser->word_buf, "random") == 0)
                        effect->random = static_cast<u8>(static_cast<i32>(NuFParGetFloat(parser) * 255.0f));
                    else if (NuStrICmp(parser->word_buf, "rumble") == 0)
                        effect->rumble = fabsf(NuFParGetFloat(parser));
                    else if (NuStrICmp(parser->word_buf, "buzz") == 0)
                        effect->shake = fabsf(NuFParGetFloat(parser));
                    else if (NuStrICmp(parser->word_buf, "judder") == 0)
                        effect->judder = NuFParGetFloat(parser);
                    else if (NuStrICmp(parser->word_buf, "timing") == 0) {
                        if (NuFParGetWord(parser) != 0) {
                            if (NuStrICmp(parser->word_buf, "constant") == 0)
                                effect->flags |= 4;
                            else if (NuStrICmp(parser->word_buf, "between") == 0)
                                effect->flags |= 8;
                        }
                    } else if (NuStrICmp(parser->word_buf, "particles_per_frame") == 0) {
                        effect->particle_rate = fabsf(NuFParGetFloat(parser));
                        effect->flags |= 1;
                    } else if (NuStrICmp(parser->word_buf, "particles_per_second") == 0) {
                        effect->particle_rate = fabsf(NuFParGetFloat(parser));
                        effect->flags |= 0x800001;
                    } else if (NuStrICmp(parser->word_buf, "locators") == 0) {
                        while (NuFParGetWord(parser) != 0) {
                            const u32 locator = NuAToI(parser->word_buf);
                            if (locator < 16)
                                effect->locators |= 1u << locator;
                        }
                    } else if (NuStrICmp(parser->word_buf, "sfx") == 0) {
                        if (NuFParGetWord(parser) != 0)
                            effect->sound_id = static_cast<i16>(GetSfxId(parser->word_buf));
                    } else if (NuStrICmp(parser->word_buf, "sfx_2d") == 0)
                        effect->flags &= ~0x8000u;
                    else if (NuStrICmp(parser->word_buf, "volume") == 0) {
                        effect->minimum = fabsf(NuFParGetFloat(parser));
                        effect->flags |= 0x10000;
                    } else if (NuStrICmp(parser->word_buf, "surfaces") == 0) {
                        if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "non_solid") == 0)
                            effect->surfaces = ~SURFACEBITS_DUST;
                    } else if (NuStrICmp(parser->word_buf, "at_ter_surface") == 0)
                        effect->flags |= 0x2000;
                    else if (NuStrICmp(parser->word_buf, "at_obj_bottom") == 0)
                        effect->flags |= 0x1000;
                    else if (NuStrICmp(parser->word_buf, "at_locator_average") == 0)
                        effect->flags |= 0x800;
                    else if (NuStrICmp(parser->word_buf, "at_ter_layer") == 0)
                        effect->flags |= 0x4000;
                    else if (NuStrICmp(parser->word_buf, "min_mode") == 0) {
                        if (NuFParGetWord(parser) != 0) {
                            if (NuStrICmp(parser->word_buf, "xz_speed") == 0)
                                effect->flags |= 0x40000;
                            else if (NuStrICmp(parser->word_buf, "xyz_speed") == 0)
                                effect->flags |= 0x100000;
                        }
                    } else if (NuStrICmp(parser->word_buf, "max_mode") == 0) {
                        if (NuFParGetWord(parser) != 0) {
                            if (NuStrICmp(parser->word_buf, "xz_speed") == 0)
                                effect->flags |= 0x80000;
                            else if (NuStrICmp(parser->word_buf, "xyz_speed") == 0)
                                effect->flags |= 0x200000;
                        }
                    } else if (NuStrICmp(parser->word_buf, "min_val") == 0)
                        effect->minimum = NuFParGetFloat(parser);
                    else if (NuStrICmp(parser->word_buf, "max_val") == 0)
                        effect->maximum = NuFParGetFloat(parser);
                    else if (NuStrICmp(parser->word_buf, "at_obj_speed") == 0)
                        effect->flags |= 0x400000;
                    else if (NuStrICmp(parser->word_buf, "left_footprint") == 0)
                        effect->flags |= 0x1000000;
                    else if (NuStrICmp(parser->word_buf, "right_footprint") == 0)
                        effect->flags |= 0x2000000;
                    else if (NuStrICmp(parser->word_buf, "use_locator_matrix") == 0)
                        effect->flags |= 0x4000000;
                    else if (NuStrICmp(parser->word_buf, "bits_on") == 0) {
                        while (NuFParGetWord(parser) != 0) {
                            const u32 bit = NuAToI(parser->word_buf);
                            if (bit < 8)
                                effect->bits_on |= 1u << bit;
                        }
                    } else if (NuStrICmp(parser->word_buf, "bits_off") == 0) {
                        while (NuFParGetWord(parser) != 0) {
                            const u32 bit = NuAToI(parser->word_buf);
                            if (bit < 8)
                                effect->bits_off |= 1u << bit;
                        }
                    } else if (NuStrICmp(parser->word_buf, "effect_end") == 0) {
                        ++charconfig.effect_count;
                        break;
                    }
                }
            }
        } else if (NuStrICmp(parser->word_buf, "anim_end") == 0) {
            if (animation.action_id == -1)
                return;
            if ((charconfig.flags & 0x20) != 0) {
                NuStrCpy(charconfig.animation_names[charconfig.animation_count], name);
                charconfig.character->animations[charconfig.animation_count++] = animation;
                charconfig.arena->addr += sizeof(animation);
                return;
            }
            if (charconfig.character->animations == NULL)
                return;
            i32 index = 0;
            while (charconfig.character->animations[index].name != NULL &&
                   NuStrICmp(charconfig.character->animations[index].name, name) != 0)
                ++index;
            if (index < charconfig.character->field5_0x14 && charconfig.character->animations[index].name != NULL) {
                CHARACTERANIM_s *existing = &charconfig.character->animations[index];
                existing->blend_in_time = animation.blend_in_time;
                existing->blend_out_time = animation.blend_out_time;
                existing->playback_rate = animation.playback_rate;
                existing->movement_speed = animation.movement_speed;
                existing->movement_rate_cap = animation.movement_rate_cap;
                existing->action_speed = animation.action_speed;
                existing->event_frame_1 = animation.event_frame_1;
                existing->event_frame_2 = animation.event_frame_2;
                existing->priority = animation.priority;
                existing->event_frame_3 = animation.event_frame_3;
                existing->flags = animation.flags;
                existing->minimum_repetitions = animation.minimum_repetitions;
                existing->event_frame_4 = animation.event_frame_4;
                existing->maximum_repetitions = animation.maximum_repetitions;
                existing->stop_frame = animation.stop_frame;
                existing->misc_flags = animation.misc_flags;
                existing->locator = animation.locator;
            }
            return;
        }
    }
}

static void CC_astromech(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x50;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x50u;
    }
}

static void CC_banking(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x1u;
    }
}

static void CC_blaster(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x10000080;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x10000080u;
    }
}

static void CC_bounty_hunter(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x1000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x1000000u;
    }
}

static void CC_bsa_default(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        if (NuStrICmp(parser->word_buf, "on") == 0) {
            charconfig.flags |= 1;
        } else if (NuStrICmp(parser->word_buf, "off") == 0) {
            charconfig.flags &= ~1;
        }
    }
}

static void CC_buck_rider(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[2] = (charconfig.runtime->flags_094[2] & 0xfd) | (enabled << 1);
}

static void CC_can_fire(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x10000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x10000000u;
    }
}

static void CC_can_flatten(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x1000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x1000u;
    }
}

static void CC_cannotbigjump(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x200000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x200000u;
    }
}

static void CC_cannot_kill(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[1] = (charconfig.runtime->flags_094[1] & 0xf7) | (enabled << 3);
}

static void CC_can_poo(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[3] = (charconfig.runtime->flags_094[3] & 0x7f) | (enabled << 7);
}

static void CC_can_take_over(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x40;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x40u;
    }
}

static void CC_can_zap(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_098[0] = (charconfig.runtime->flags_098[0] & 0xfe) | (enabled << 0);
}

static void CC_cape_layer(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        charconfig.runtime->cape_layer = static_cast<i8>(LayerFromName(charconfig.runtime, parser->word_buf));
    }
}

static void CC_car_wheels(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x100000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x100000u;
    }
}

static void CC_chatter_delay(NUFPAR *parser) {
    charconfig.runtime->field_0x11e = static_cast<u8>(NuFParGetInt(parser));
}

static void CC_cloak_joint(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (locator >= 0) {
            charconfig.runtime->cloak_joint = static_cast<i8>(locator);
        }
    }
}

static void CC_cloak_joint2(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (locator >= 0) {
            charconfig.runtime->cloak_joint_2 = static_cast<i8>(locator);
        }
    }
}

static void CC_coin_value(NUFPAR *parser) {
    i32 value = NuFParGetInt(parser);
    if (value < 0)
        value = 0;
    if (value > 10000)
        value = 10000;
    charconfig.runtime->field_0xee = static_cast<i16>(value / 10) * 10;
}

static void CC_combat_roll(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x40000000;
    charconfig.runtime->flags_094[2] &= 0xe7;
    while (NuFParGetWord(parser) != 0) {
        if (NuStrICmp(parser->word_buf, "off") == 0) {
            charconfig.runtime->flags_090 &= ~0x40000000u;
        } else if (NuStrICmp(parser->word_buf, "can_get_up") == 0) {
            charconfig.runtime->flags_094[2] |= 8;
        } else if (NuStrICmp(parser->word_buf, "can_direct_land") == 0) {
            charconfig.runtime->flags_094[2] |= 0x10;
        }
    }
}

static void CC_dontmove(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x80;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x80u;
    }
}

static void CC_dontpush(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x4000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x4000u;
    }
}

static void CC_dont_turn(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[0] = (charconfig.runtime->flags_094[0] & 0xfd) | (enabled << 1);
}

static void CC_extra_toggle(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x80000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x80000000u;
    }
}

static void CC_fixed_layers(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[0] = (charconfig.runtime->flags_094[0] & 0xef) | (enabled << 4);
}

static void CC_glide_anytime(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[0] = (charconfig.runtime->flags_094[0] & 0xbf) | (enabled << 6);
}

static void CC_got_batarang(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x20000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x20000000u;
    }
}

static void CC_hair_layer(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        charconfig.runtime->hair_layer = static_cast<i8>(LayerFromName(charconfig.runtime, parser->word_buf));
    }
}

static void CC_hand_locators(NUFPAR *parser) {
    charconfig.runtime->hand_locators[0] = -1;
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator) < 16) {
            charconfig.runtime->hand_locators[0] = static_cast<i8>(locator);
        }
    }
    charconfig.runtime->hand_locators[1] = -1;
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator) < 16) {
            charconfig.runtime->hand_locators[1] = static_cast<i8>(locator);
        }
    }
}

static void CC_has_grapple(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[3] = (charconfig.runtime->flags_094[3] & 0xf7) | (enabled << 3);
}

static void CC_has_no_turn(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x10000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x10000u;
    }
}

static void CC_has_whip(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[3] = (charconfig.runtime->flags_094[3] & 0xdf) | (enabled << 5);
}

static void CC_head_joint(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (locator >= 0) {
            charconfig.runtime->head_joint = static_cast<i8>(locator);
        }
    }
}

static void CC_head_locator(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->head_locator = static_cast<i8>(locator);
        }
    }
}

static void CC_headrotrate(NUFPAR *parser) {
    charconfig.runtime->field_0x58 = (NuFParGetFloat(parser) * 3.1415927f) / 180.0f;
}

static void CC_heardistance(NUFPAR *parser) {
    charconfig.runtime->heardistance = NuFParGetFloat(parser);
}

static void CC_high_jump(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x400000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x400000u;
    }
}

static void CC_hit_points(NUFPAR *parser) {
    charconfig.runtime->hitpoints = static_cast<u8>(NuFParGetInt(parser));
}

static void CC_hover_height(NUFPAR *parser) {
    charconfig.runtime->field_0x28 = NuFParGetFloat(parser);
}

static void CC_hover_time(NUFPAR *parser) {
    charconfig.runtime->field_0x48 = fabsf(NuFParGetFloat(parser));
}

static void CC_hover_wings(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[3] = (charconfig.runtime->flags_094[3] & 0xef) | (enabled << 4);
}

static void CC_idle_speed(NUFPAR *parser) {
    charconfig.runtime->field_0x10 = NuFParGetFloat(parser);
}

static void CC_is_a_boy(NUFPAR *parser) {
    charconfig.runtime->flags_090 &= ~0x20000u;
}

static void CC_is_a_girl(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x20000;
}

static void CC_jetpack(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x8000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x8000u;
    }
}

static void CC_jump_2_speed(NUFPAR *parser) {
    charconfig.runtime->field_0x38 = NuFParGetFloat(parser);
}

static void CC_jump_speed(NUFPAR *parser) {
    charconfig.runtime->field_0x2c = NuFParGetFloat(parser);
}

static void CC_layers_dead(NUFPAR *parser) {
    u32 *mask = &charconfig.runtime->layer_mask_dead;
    *mask = 0;
    i32 count = 0;
    if (charconfig.named_layers == 0) {
        while (NuFParGetWord(parser) != 0) {
            const u32 layer = NuAToI(parser->word_buf);
            if (layer < 32) {
                ++count;
                *mask |= 1u << layer;
            }
        }
    } else {
        while (NuFParGetWord(parser) != 0) {
            const i32 layer = LayerFromName(charconfig.runtime, parser->word_buf);
            if (layer != -1) {
                ++count;
                *mask |= 1u << layer;
            }
        }
    }
}

static void CC_layers_high(NUFPAR *parser) {
    u32 *mask = &charconfig.runtime->layer_mask;
    *mask = 0;
    i32 count = 0;
    if (charconfig.named_layers == 0) {
        while (NuFParGetWord(parser) != 0) {
            const u32 layer = NuAToI(parser->word_buf);
            if (layer < 32) {
                ++count;
                *mask |= 1u << layer;
            }
        }
    } else {
        while (NuFParGetWord(parser) != 0) {
            const i32 layer = LayerFromName(charconfig.runtime, parser->word_buf);
            if (layer != -1) {
                ++count;
                *mask |= 1u << layer;
            }
        }
    }
    if (count != 0) {
        charconfig.runtime->layer_mask_low = *mask;
        charconfig.runtime->layer_mask_medium = *mask;
    }
}

static void CC_layers_low(NUFPAR *parser) {
    u32 *mask = &charconfig.runtime->layer_mask_low;
    *mask = 0;
    i32 count = 0;
    if (charconfig.named_layers == 0) {
        while (NuFParGetWord(parser) != 0) {
            const u32 layer = NuAToI(parser->word_buf);
            if (layer < 32) {
                ++count;
                *mask |= 1u << layer;
            }
        }
    } else {
        while (NuFParGetWord(parser) != 0) {
            const i32 layer = LayerFromName(charconfig.runtime, parser->word_buf);
            if (layer != -1) {
                ++count;
                *mask |= 1u << layer;
            }
        }
    }
}

static void CC_layers_medium(NUFPAR *parser) {
    u32 *mask = &charconfig.runtime->layer_mask_medium;
    *mask = 0;
    i32 count = 0;
    if (charconfig.named_layers == 0) {
        while (NuFParGetWord(parser) != 0) {
            const u32 layer = NuAToI(parser->word_buf);
            if (layer < 32) {
                ++count;
                *mask |= 1u << layer;
            }
        }
    } else {
        while (NuFParGetWord(parser) != 0) {
            const i32 layer = LayerFromName(charconfig.runtime, parser->word_buf);
            if (layer != -1) {
                ++count;
                *mask |= 1u << layer;
            }
        }
    }
    if (count != 0) {
        charconfig.runtime->layer_mask_low = *mask;
    }
}

static void CC_lift_hover(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[0] = (charconfig.runtime->flags_094[0] & 0xdf) | (enabled << 5);
}

static void CC_lightning(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x4;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x4u;
    }
}

static void CC_loop_height(NUFPAR *parser) {
    charconfig.runtime->field_0x88 = NuFParGetFloat(parser);
}

static void CC_maxheadtilt(NUFPAR *parser) {
    charconfig.runtime->field_0x54 = (NuFParGetFloat(parser) * 3.1415927f) / 180.0f;
}

static void CC_maxheadturn(NUFPAR *parser) {
    charconfig.runtime->field_0x50 = (NuFParGetFloat(parser) * 3.1415927f) / 180.0f;
}

static void CC_minikit(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x4000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x4000000u;
    }
}

static void CC_name_id(NUFPAR *parser) {
    charconfig.character->name_id = NuFParGetInt(parser);
}

static void CC_neutral(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x200;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x200u;
    }
}

static void CC_no_category(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[1] = (charconfig.runtime->flags_094[1] & 0xbf) | (enabled << 6);
}

static void CC_no_force(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[1] = (charconfig.runtime->flags_094[1] & 0x7f) | (enabled << 7);
}

static void CC_no_jump(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[0] = (charconfig.runtime->flags_094[0] & 0xfb) | (enabled << 2);
}

static void CC_no_jump_fire(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[2] = (charconfig.runtime->flags_094[2] & 0xbf) | (enabled << 6);
}

static void CC_no_kill_parts(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x2000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x2000000u;
    }
}

static void CC_non_stick(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x800;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x800u;
    }
}

static void CC_no_shoot(NUFPAR *parser) {
    u8 enabled = 1;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        enabled = 0;
    }
    charconfig.runtime->flags_094[0] = (charconfig.runtime->flags_094[0] & 0xf7) | (enabled << 3);
}

static void CC_not_got_hat(NUFPAR *parser) {
    charconfig.runtime->flags_090 &= ~0x10u;
}

static void CC_no_tiptoe(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x8;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x8u;
    }
}

static void CC_orientate(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x100;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x100u;
    }
}

static void CC_place_locator(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->place_locator = static_cast<i8>(locator);
        }
    }
}

static void CC_poo_locator(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->poo_locator = static_cast<i8>(locator);
        }
    }
}

static void CC_protocol(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x30;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x30u;
    }
}

static void CC_resist_zap(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x4000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x4000u;
    }
}

static void CC_respawn(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x8000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x8000000u;
    }
}

static void CC_ride_locator(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->ride_locator = static_cast<i8>(locator);
        }
    }
}

static void CC_run_speed(NUFPAR *parser) {
    charconfig.runtime->run_speed = NuFParGetFloat(parser);
}

static void CC_sfx_chatter(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        charconfig.runtime->sfx_chatter = static_cast<i16>(GetSfxId(parser->word_buf));
    }
}

static void CC_sfx_die(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        charconfig.runtime->sfx_die = static_cast<i16>(GetSfxId(parser->word_buf));
    }
}

static void CC_sfx_engine(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        charconfig.runtime->sfx_engine = static_cast<i16>(GetSfxId(parser->word_buf));
    }
}

static void CC_sfx_footstep(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        charconfig.runtime->sfx_footstep = static_cast<i16>(GetSfxId(parser->word_buf));
    }
}

static void CC_sfx_grunt(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        charconfig.runtime->sfx_grunt = static_cast<i16>(GetSfxId(parser->word_buf));
    }
}

static void CC_sfx_hurt(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        charconfig.runtime->sfx_hurt = static_cast<i16>(GetSfxId(parser->word_buf));
    }
}

static void CC_sfx_misc(NUFPAR *parser) {
    i32 index = 0;
    while (index < 6 && charconfig.runtime->sfx_misc[index] != -1)
        ++index;
    if (index < 6 && NuFParGetWord(parser) != 0) {
        charconfig.runtime->sfx_misc[index] = static_cast<i16>(GetSfxId(parser->word_buf));
    }
    if (index + 1 < 6)
        charconfig.runtime->sfx_misc[index + 1] = -1;
}

static void CC_sfx_sabre(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        charconfig.runtime->sfx_sabre = static_cast<i16>(GetSfxId(parser->word_buf));
    }
}

static void CC_sfx_shoot(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        charconfig.runtime->sfx_shoot = static_cast<i16>(GetSfxId(parser->word_buf));
    }
}

static void CC_speed_up_time(NUFPAR *parser) {
    charconfig.runtime->field_0x7c = NuFParGetFloat(parser);
}

static void CC_stop_speed(NUFPAR *parser) {
    charconfig.runtime->field_0x0c = NuFParGetFloat(parser);
}

static void CC_teleport(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x40000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x40000u;
    }
}

static void CC_throw_locator(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        const i32 locator = NuAToI(parser->word_buf);
        if (static_cast<u32>(locator + 1) < 17) {
            charconfig.runtime->throw_locator = static_cast<i8>(locator);
        }
    }
}

static void CC_tiptoe_speed(NUFPAR *parser) {
    charconfig.runtime->field_0x14 = NuFParGetFloat(parser);
}

static void CC_turn_rate(NUFPAR *parser) {
    const f32 rate = NuFParGetFloat(parser);
    if (rate > 0.0f) {
        charconfig.runtime->turn_rate = rate;
        charconfig.flags |= 4;
    }
}

static void CC_turn_rate_2(NUFPAR *parser) {
    const f32 rate = NuFParGetFloat(parser);
    if (rate > 0.0f) {
        charconfig.runtime->field_0x78 = rate;
        charconfig.flags |= 8;
    }
}

static void CC_untargetable(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x80000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x80000u;
    }
}

static void CC_variant(NUFPAR *parser) {
    if (NuFParGetWord(parser) == 0)
        return;
    if (NuStrICmp(parser->word_buf, "stormtrooper") == 0)
        charconfig.runtime->field275_0x116 = 1;
    else if (NuStrICmp(parser->word_buf, "fett") == 0)
        charconfig.runtime->field275_0x116 = 2;
    else if (NuStrICmp(parser->word_buf, "macewindu") == 0)
        charconfig.runtime->field275_0x116 = 3;
    else if (NuStrICmp(parser->word_buf, "battledroid") == 0)
        charconfig.runtime->field275_0x116 = 4;
    else if (NuStrICmp(parser->word_buf, "hoverdroid") == 0)
        charconfig.runtime->field275_0x116 = 17;
    else if (NuStrICmp(parser->word_buf, "walker2legs") == 0)
        charconfig.runtime->field275_0x116 = 15;
    else if (NuStrICmp(parser->word_buf, "walker4legs") == 0)
        charconfig.runtime->field275_0x116 = 16;
    else if (NuStrICmp(parser->word_buf, "wookiee") == 0)
        charconfig.runtime->field275_0x116 = 5;
    else if (NuStrICmp(parser->word_buf, "obiwankenobi") == 0)
        charconfig.runtime->field275_0x116 = 6;
    else if (NuStrICmp(parser->word_buf, "leia") == 0)
        charconfig.runtime->field275_0x116 = 7;
    else if (NuStrICmp(parser->word_buf, "clone") == 0)
        charconfig.runtime->field275_0x116 = 8;
    else if (NuStrICmp(parser->word_buf, "rebel") == 0)
        charconfig.runtime->field275_0x116 = 9;
    else if (NuStrICmp(parser->word_buf, "tie") == 0)
        charconfig.runtime->field275_0x116 = 10;
    else if (NuStrICmp(parser->word_buf, "lando") == 0)
        charconfig.runtime->field275_0x116 = 11;
    else if (NuStrICmp(parser->word_buf, "weirdo") == 0)
        charconfig.runtime->field275_0x116 = 0;
    else if (NuStrICmp(parser->word_buf, "luke") == 0)
        charconfig.runtime->field275_0x116 = 12;
    else if (NuStrICmp(parser->word_buf, "hansolo") == 0)
        charconfig.runtime->field275_0x116 = 13;
    else if (NuStrICmp(parser->word_buf, "padme") == 0)
        charconfig.runtime->field275_0x116 = 14;
    else if (NuStrICmp(parser->word_buf, "critter") == 0)
        charconfig.runtime->field275_0x116 = 18;
    else if (NuStrICmp(parser->word_buf, "naboostarfighter") == 0)
        charconfig.runtime->field275_0x116 = 19;
    else if (NuStrICmp(parser->word_buf, "pod") == 0)
        charconfig.runtime->field275_0x116 = 20;
    else if (NuStrICmp(parser->word_buf, "republicgunship") == 0)
        charconfig.runtime->field275_0x116 = 21;
    else if (NuStrICmp(parser->word_buf, "henchman") == 0)
        charconfig.runtime->field275_0x116 = 22;
}

static void CC_vehicle(NUFPAR *parser) {
    charconfig.character->model_flags |= 0x2000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.character->model_flags &= ~0x2000u;
    }
}

static void CC_viewrange(NUFPAR *parser) {
    charconfig.runtime->viewdistance = NuFParGetFloat(parser);
}

static void CC_walk_speed(NUFPAR *parser) {
    charconfig.runtime->field_0x18 = NuFParGetFloat(parser);
}

static void CC_wall_jump(NUFPAR *parser) {
    charconfig.runtime->flags_090 |= 0x8000000;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0) {
        charconfig.runtime->flags_090 &= ~0x8000000u;
    }
}

NUFPCOMJMP ConfigChar_Keywords[] = {
    {"name_id", CC_name_id},
    {"name", CC_name_id},
    {"mass", CC_mass},
    {"radius", CC_radius},
    {"miny", CC_miny},
    {"maxy", CC_maxy},
    {"scale", CC_scale},
    {"layer", CC_layer},
    {"fixed_layers", CC_fixed_layers},
    {"hair_layer_index", CC_hair_layer_index},
    {"cape_layer_index", CC_cape_layer_index},
    {"hair_layer", CC_hair_layer},
    {"cape_layer", CC_cape_layer},
    {"layers_special", CC_layers_special},
    {"layers_high", CC_layers_high},
    {"layers_medium", CC_layers_medium},
    {"layers_low", CC_layers_low},
    {"layers_dead", CC_layers_dead},
    {"ride_layersoff", CC_ride_layersoff},
    {"bsa_default", CC_bsa_default},
    {"anim_start", CC_anim_start},
    {"collision_locator", CC_collision_locator},
    {"weapon_locator", CC_weapon_locator_1},
    {"weapon_locator_1", CC_weapon_locator_1},
    {"weapon_locator_2", CC_weapon_locator_2},
    {"weapon_locator_3", CC_weapon_locator_3},
    {"weapon_locator_4", CC_weapon_locator_4},
    {"weapon_shoot_locator", CC_weapon_shoot_locator_1},
    {"weapon_shoot_locator_1", CC_weapon_shoot_locator_1},
    {"weapon_shoot_locator_2", CC_weapon_shoot_locator_2},
    {"weapon_shoot_locator_3", CC_weapon_shoot_locator_3},
    {"weapon_shoot_locator_4", CC_weapon_shoot_locator_4},
    {"shield_locator", CC_shield_locator},
    {"throw_locator", CC_throw_locator},
    {"place_locator", CC_place_locator},
    {"grapple_locator", CC_grapple_locator},
    {"grapple_locators", CC_grapple_locator},
    {"extra_character_locator", CC_extra_character_locator},
    {"ride_locator", CC_ride_locator},
    {"poo_locator", CC_poo_locator},
    {"head_locator", CC_head_locator},
    {"rocket_locator", CC_rocket_locator},
    {"thingy_locator", CC_thingy_locator},
    {"helmet_locator", CC_helmet_locator},
    {"shadow_locators", CC_shadow_locators},
    {"thrust_locators", CC_thrust_locators},
    {"hand_locators", CC_hand_locators},
    {"streak_locators", CC_streak_1_locators},
    {"streak_1_locators", CC_streak_1_locators},
    {"streak_2_locators", CC_streak_2_locators},
    {"streak_3_locators", CC_streak_3_locators},
    {"streak_4_locators", CC_streak_4_locators},
    {"head_joint", CC_head_joint},
    {"cloak_joint", CC_cloak_joint},
    {"cloak_joint2", CC_cloak_joint2},
    {"sfx_die", CC_sfx_die},
    {"sfx_hurt", CC_sfx_hurt},
    {"sfx_grunt", CC_sfx_grunt},
    {"sfx_engine", CC_sfx_engine},
    {"sfx_shoot", CC_sfx_shoot},
    {"sfx_footstep", CC_sfx_footstep},
    {"sfx_chatter", CC_sfx_chatter},
    {"chatter_delay", CC_chatter_delay},
    {"sfx_sabre", CC_sfx_sabre},
    {"sfx_misc", CC_sfx_misc},
    {"clear_sfx_misc", CC_clear_sfx_misc},
    {"cloak_up_angle", CC_cloak_up_angle},
    {"cloak_down_angle", CC_cloak_down_angle},
    {"viewrange", CC_viewrange},
    {"heardistance", CC_heardistance},
    {"max_viewheight", CC_max_viewheight},
    {"min_viewheight", CC_min_viewheight},
    {"mediumres_swapdist", CC_mediumres_swapdist},
    {"lowres_swapdist", CC_lowres_swapdist},
    {"turn_rate", CC_turn_rate},
    {"turn_rate_2", CC_turn_rate_2},
    {"banking", CC_banking},
    {"choke", CC_choke},
    {"lightning", CC_lightning},
    {"resist_zap", CC_resist_zap},
    {"ghost", CC_ghost},
    {"has_no_turn", CC_has_no_turn},
    {"speed_up_time", CC_speed_up_time},
    {"slow_down_time", CC_slow_down_time},
    {"jump_move_speed_scale", CC_jump_move_speed_scale},
    {"loop_height", CC_loop_height},
    {"no_tiptoe", CC_no_tiptoe},
    {"already_got_hat", CC_already_got_hat},
    {"not_got_hat", CC_not_got_hat},
    {"punch_weapon_out", CC_punch_weapon_out},
    {"can_take_over", CC_can_take_over},
    {"turning_circle", CC_turning_circle},
    {"slide_orientation", CC_slide_orientation},
    {"can_drag_bombs", CC_can_drag_bombs},
    {"extra_toggle", CC_extra_toggle},
    {"can_flatten", CC_can_flatten},
    {"tightrope_walk", CC_tightrope_walk},
    {"car_wheels", CC_car_wheels},
    {"is_a_girl", CC_is_a_girl},
    {"is_a_boy", CC_is_a_boy},
    {"icon", CC_icon},
    {"weapon", CC_weapon},
    {"shield_hit_points", CC_shield_hit_points},
    {"stop_speed", CC_stop_speed},
    {"idle_speed", CC_idle_speed},
    {"tiptoe_speed", CC_tiptoe_speed},
    {"walk_speed", CC_walk_speed},
    {"run_speed", CC_run_speed},
    {"backwards_factor", CC_backwards_factor},
    {"air_gravity", CC_air_gravity},
    {"jump_speed", CC_jump_speed},
    {"jump_2_speed", CC_jump_2_speed},
    {"acceleration", CC_acceleration},
    {"streak", CC_streak},
    {"hit_points", CC_hit_points},
    {"baddie", CC_baddie},
    {"jedi", CC_jedi},
    {"droid", CC_droid},
    {"protocol", CC_protocol},
    {"astromech", CC_astromech},
    {"cannon", CC_cannon},
    {"bounty_hunter", CC_bounty_hunter},
    {"blaster", CC_blaster},
    {"teleport", CC_teleport},
    {"zipup", CC_zipup},
    {"untargetable", CC_untargetable},
    {"orientate", CC_orientate},
    {"neutral", CC_neutral},
    {"complex_shadow", CC_complex_shadow},
    {"shadow_orientation", CC_shadow_orientation},
    {"can_open_doors", CC_can_open_doors},
    {"non_stick", CC_non_stick},
    {"can_shoot_offscreen", CC_can_shoot_offscreen},
    {"can_fire", CC_can_fire},
    {"can_poo", CC_can_poo},
    {"can_zap", CC_can_zap},
    {"put_weapon_away_on_shoot", CC_put_weapon_away_on_shoot},
    {"always_stop_to_shoot", CC_always_stop_to_shoot},
    {"second_shot_only", CC_second_shot_only},
    {"no_start_punch_sfx", CC_no_start_punch_sfx},
    {"single_jump_slam", CC_single_jump_slam},
    {"dont_draw_rider", CC_dont_draw_rider},
    {"charClipToBlobShadows", CC_CharClipToBlobShadows},
    {"minikit", CC_minikit},
    {"minikit_with_scene", CC_minikit_with_scene},
    {"minikit_noscenewhenplayable", CC_minikit_noscenewhenplayable},
    {"lift_hover", CC_lift_hover},
    {"glide_anytime", CC_glide_anytime},
    {"can_communicate", CC_can_communicate},
    {"immune_to_magnets", CC_immune_to_magnets},
    {"immune_to_forcepush", CC_immune_to_forcepush},
    {"cannot_kill", CC_cannot_kill},
    {"wait_for_weapon_in_out", CC_wait_for_weapon_in_out},
    {"double_jump_hover", CC_double_jump_hover},
    {"no_category", CC_no_category},
    {"no_force", CC_no_force},
    {"white_lightning", CC_white_lightning},
    {"buck_rider", CC_buck_rider},
    {"die_usecurrentlayers", CC_die_usecurrentlayers},
    {"no_weapon_draw", CC_no_weapon_draw},
    {"no_jump_fire", CC_no_jump_fire},
    {"hover_over_mud", CC_hover_over_mud},
    {"toggle_if_not_in_collection", CC_toggle_if_not_in_collection},
    {"toggle_if_in_minikit_bonus", CC_toggle_if_in_minikit_bonus},
    {"deflect_bolts_in_minikit_bonus", CC_deflect_bolts_in_minikit_bonus},
    {"has_grapple", CC_has_grapple},
    {"hover_wings", CC_hover_wings},
    {"has_whip", CC_has_whip},
    {"respawn", CC_respawn},
    {"vehicle", CC_vehicle},
    {"only_active_when_taken_over", CC_only_active_when_taken_over},
    {"beast", CC_beast},
    {"prefers_brawling", CC_prefers_brawling},
    {"dontpush", CC_dontpush},
    {"dont_move_out_of_way", CC_dont_move_out_of_way},
    {"dontmove", CC_dontmove},
    {"jetpack", CC_jetpack},
    {"oldheadmovement", CC_oldheadmovement},
    {"atatheadmovement", CC_atatheadmovement},
    {"cannotbigjump", CC_cannotbigjump},
    {"no_offpath_teleport", CC_no_offpath_teleport},
    {"no_kill_parts", CC_no_kill_parts},
    {"high_jump", CC_high_jump},
    {"super_strength", CC_super_strength},
    {"transformatron", CC_transformatron},
    {"bypass_security", CC_bypass_security},
    {"hazard_protection", CC_hazard_protection},
    {"wall_jump", CC_wall_jump},
    {"got_batarang", CC_got_batarang},
    {"tightrope_tilt", CC_tightrope_tilt},
    {"combat_roll", CC_combat_roll},
    {"has_scene_element", CC_has_scene_element},
    {"dont_turn", CC_dont_turn},
    {"no_jump", CC_no_jump},
    {"no_shoot", CC_no_shoot},
    {"blobshadow_size", CC_blobshadow_size},
    {"blobshadow_alpha", CC_blobshadow_alpha},
    {"hover_height", CC_hover_height},
    {"hover_time", CC_hover_time},
    {"maxheadturn", CC_maxheadturn},
    {"maxheadtilt", CC_maxheadtilt},
    {"headrotrate", CC_headrotrate},
    {"coinvalue", CC_coin_value},
    {"coin_value", CC_coin_value},
    {"timebaseupdate1", CC_set_timebaseupdate1},
    {"timebaseupdate2", CC_set_timebaseupdate2},
    {"timebaseupdate3", CC_set_timebaseupdate3},
    {"timebaseupdate4", CC_set_timebaseupdate4},
    {NULL, NULL},
};

NUFPCOMJMP ConfigChar_GameKeywords[] = {
    {"variant", CC_variant},
    {NULL, NULL},
};
