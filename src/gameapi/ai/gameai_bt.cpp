#include "decomp.h"
#include "globals.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numusic/sfx.h"

static BOLTTYPE_s *BT_bolttype;
static WORLDINFO_s *BT_worldinfo;
static NUGSCN *BT_scene;
static i32 BT_gdeb_moving_count;

static __used__ void BT_canonlyhitplayers(nufpar_s *) {
    BT_bolttype->field_60 |= 0x80;
}
static __used__ void BT_converge(nufpar_s *) {
    BT_bolttype->field_60 |= 0x400;
}
static __used__ void BT_damage(nufpar_s *parser) {
    BOLTTYPE_s *bolt_type = BT_bolttype;
    bolt_type->field_3c = static_cast<u8>(NuFParGetInt(parser));
}
static __used__ void BT_debris(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0) {
        BOLTTYPE_s *bolt_type = BT_bolttype;
        bolt_type->debris_id = static_cast<i16>(FindGameDebris(BT_worldinfo->debris_sys, parser->word_buf));
    }
}
static __used__ void BT_debris_moving(nufpar_s *parser) {
    if (BT_gdeb_moving_count > 1 || NuFParGetWord(parser) == 0) {
        return;
    }

    BT_bolttype->moving_debris[BT_gdeb_moving_count] =
        static_cast<i16>(FindGameDebris(BT_worldinfo->debris_sys, parser->word_buf));
    i32 count = static_cast<i32>(NuFParGetFloat(parser));
    i32 index = BT_gdeb_moving_count++;
    if (count < 0) {
        count = -count;
    }
    BT_bolttype->moving_debris_counts[index] = static_cast<i16>(count);
}
static __used__ void BT_duration(nufpar_s *parser) {
    BOLTTYPE_s *bolt_type = BT_bolttype;
    bolt_type->field_14 = NuFParGetFloat(parser);
}
static __used__ void BT_glow_obj(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0 && BT_scene != NULL) {
        if (NuSpecialFind(BT_scene, &BT_bolttype->specials.glow_special, parser->word_buf, 1) != 0) {
            BT_bolttype->specials.reference_glow_special = BT_bolttype->specials.glow_special;
        }
    }
}
static __used__ void BT_name(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0 && NuStrLen(parser->word_buf) < 15) {
        NuStrCpy(BT_bolttype->name, parser->word_buf);
    }
}
static __used__ void BT_no_collide(nufpar_s *) {
    BT_bolttype->field_60 |= 0x10000;
}
static __used__ void BT_nodeflect(nufpar_s *) {
    BT_bolttype->field_60 |= 0x100;
}
static __used__ void BT_no_terrain(nufpar_s *) {
    BT_bolttype->field_60 |= 4;
}
static __used__ void BT_obj(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0 && BT_scene != NULL) {
        if (NuSpecialFind(BT_scene, &BT_bolttype->specials.object_special, parser->word_buf, 1) != 0) {
            BT_bolttype->specials.reference_object_special = BT_bolttype->specials.object_special;
        }
    }
}
static __used__ void BT_part_hit(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0) {
        BOLTTYPE_s *bolt_type = BT_bolttype;
        bolt_type->field_38 = static_cast<i16>(FindPartDebris(BT_worldinfo->part_debris_sys, parser->word_buf));
    }
}
static __used__ void BT_radius(nufpar_s *parser) {
    BOLTTYPE_s *bolt_type = BT_bolttype;
    bolt_type->field_1c = NuFParGetFloat(parser);
}
static __used__ void BT_rand_angle(nufpar_s *parser) {
    BOLTTYPE_s *bolt_type = BT_bolttype;
    bolt_type->field_40 = static_cast<i32>(NuFParGetFloat(parser) * (65536.0f / 360.0f));
}
static __used__ void BT_ref_glow_obj(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0 && BT_scene != NULL) {
        NuSpecialFind(BT_scene, &BT_bolttype->specials.reference_glow_special, parser->word_buf, 1);
    }
}
static __used__ void BT_ref_obj(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0 && BT_scene != NULL) {
        NuSpecialFind(BT_scene, &BT_bolttype->specials.reference_object_special, parser->word_buf, 1);
    }
}
static __used__ void BT_scale(nufpar_s *parser) {
    BOLTTYPE_s *bolt_type = BT_bolttype;
    bolt_type->field_20 = NuFParGetFloat(parser);
}
static __used__ void BT_sceneconfig(nufpar_s *parser) {
    if (NuFParGetWord(parser) == 0) {
        return;
    }
    if (NuStrICmp(parser->word_buf, "level") == 0) {
        BT_scene = BT_worldinfo->current_gscn;
    } else if (NuStrICmp(parser->word_buf, "area") == 0) {
        BT_scene = area_scene;
    } else if (NuStrICmp(parser->word_buf, "vehicle") == 0) {
        BT_scene = vehicle_scene;
    } else {
        BT_scene = things_scene;
    }
}
static __used__ void BT_sfx_hit(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0) {
        BOLTTYPE_s *bolt_type = BT_bolttype;
        bolt_type->hit_sfx_id = static_cast<i16>(GetSfxId(parser->word_buf));
    }
}
static __used__ void BT_sfx_shoot(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0) {
        BOLTTYPE_s *bolt_type = BT_bolttype;
        bolt_type->shoot_sfx_id = static_cast<i16>(GetSfxId(parser->word_buf));
    }
}
static __used__ void BT_shadow_obj(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0 && BT_scene != NULL) {
        NuSpecialFind(BT_scene, &BT_bolttype->specials.shadow_special, parser->word_buf, 1);
    }
}
static __used__ void BT_single_debris(nufpar_s *) {
    BT_bolttype->field_60 |= 0x40000000;
}
static __used__ void BT_speed(nufpar_s *parser) {
    BOLTTYPE_s *bolt_type = BT_bolttype;
    bolt_type->field_10 = NuFParGetFloat(parser);
}
static __used__ void BT_trooper_bolt(nufpar_s *) {
    BT_bolttype->field_60 |= 0x80000;
}
