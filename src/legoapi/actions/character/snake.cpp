#include "decomp.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/characters/motion/gameanim.h"
#include "globals.h"
#include <string.h>
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/characters/motion.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern i16 id_GAMORREANGUARD;

void GrabVictim(GameObject_s *object, GameObject_s *victim) {
    if (object->character_context != -1) {
        return;
    }

    object->character_context = 0x38;
    i16 animation = 0x64;
    i32 animation_offset = 0x190;
    if (victim->id == id_GAMORREANGUARD) {
        animation_offset = victim->apiobj.field_0x27c == -1 ? 0x7c : 0x190;
        animation = victim->apiobj.field_0x27c == -1 ? 0x1f : 0x64;
    }
    object->context_animation = animation;
    if (*(void **)((char *)object->apiobj.character_model->model_data_b + animation_offset) != NULL) {
        object->airborne_action_duration = AnimDuration(object->id, animation, 0.0f, 0.0f, 1);
    } else {
        object->airborne_action_duration = 2.0f;
    }
    object->blowup_target = NULL;
    object->context_animation_timer = 0.0f;
    object->field_0x780 = victim;
    object->force_throw_target = NULL;
    object->field_0xe24 &= ~1;
    object->context_flags &= ~0x40;
}

extern "C" {
    SNAKEBODY_s snakebodies[4];
}
static nuhspecial_s snake_hspecials[3];

void InitSnakes(WORLDINFO_s *world) {
    memset(snakebodies, 0, sizeof(snakebodies));
    NuSpecialFind(world->current_gscn, &snake_hspecials[0], (char *)"Snake_bit_1", 1);
    NuSpecialFind(world->current_gscn, &snake_hspecials[1], (char *)"Snake_bit_2", 1);
    NuSpecialFind(world->current_gscn, &snake_hspecials[2], (char *)"Snake_bit_3", 1);
}

SNAKEBODY_s *CreateSnakeBody(GameObject_s *object, i32 segment_count) {
    for (i32 index = 0; index < 4; ++index) {
        SNAKEBODY_s *body = &snakebodies[index];
        if ((body->flags & 1) != 0)
            continue;
        object->snake_body = body;
        body->flags |= 1;
        body->segment_count = static_cast<u16>(segment_count);
        body->scale = 1.0f;
        for (i32 segment = 0; segment < body->segment_count; ++segment) {
            body->segments[segment].yaw = object->apiobj.field_0x276;
            body->segments[segment].pitch = 0;
            body->segments[segment].ground_height = 1000000000.0f;
        }
        return body;
    }
    return NULL;
}

void DestroySnakeBody(GameObject_s *object) {
    if (object != NULL && object->snake_body != NULL) {
        memset(object->snake_body, 0, sizeof(*object->snake_body));
        object->snake_body = NULL;
    }
}

f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);

void UpdateSnakeBody(GameObject_s *object) {
    if (object == NULL || object->snake_body == NULL)
        return;

    object->field_0x1004 = object->snake_body->scale;
    NUVEC position = object->apiobj.lower_position;
    position.y += object->snake_body->scale * 0.15f;
    if (object->snake_body->segment_count == 0)
        return;

    i32 segment_index = 0;
    do {
        object->snake_body->segments[segment_index].position = position;
        u16 target_yaw = segment_index == 0 ? object->apiobj.field_0x276
                                            : static_cast<u16>(object->snake_body->segments[segment_index - 1].yaw);
        object->snake_body->segments[segment_index].yaw =
            SeekRot(static_cast<u16>(object->snake_body->segments[segment_index].yaw), target_yaw, 5.0f);
        object->snake_body->segments[segment_index].pitch = 0;
        NUVEC offset = {0.0f, 0.0f, -(object->snake_body->scale * 0.15f)};
        NuVecRotateY(&offset, &offset, static_cast<NUANG>(object->snake_body->segments[segment_index].yaw));
        NuVecAdd(&position, &object->snake_body->segments[segment_index].position, &offset);

        f32 ground = GameShadow(NULL, &position, 5.0f, 0x1f);
        if (ground == 2000000.0f)
            ground = object->snake_body->segments[segment_index].position.y;
        if (object->snake_body->segments[segment_index].ground_height == 1000000000.0f)
            object->snake_body->segments[segment_index].ground_height = ground;
        else
            object->snake_body->segments[segment_index].ground_height =
                SeekValF(object->snake_body->segments[segment_index].ground_height, ground, 10.0f);
        if (ground == 2000000.0f)
            continue;

        position.y = ground;
        f32 length = NuVecDist(&position, &object->snake_body->segments[segment_index].position, &offset);
        NuVecScale(&offset, &offset, object->snake_body->scale * 0.15f / length);
        NuVecAdd(&position, &object->snake_body->segments[segment_index].position, &offset);

        f32 vertical = -offset.y / (object->snake_body->scale * 0.15f);
        f32 absolute = __builtin_fabsf(vertical);
        f32 curved = __builtin_fminf(NuFsqrt(1.0f - vertical * vertical), absolute);
        f32 side = (absolute - 0.7071067690849304f) * 3.402820018375656e+38f;
        side = __builtin_fminf(__builtin_fmaxf(side, 0.0f), 1.0f);
        f32 sign = vertical * 3.402820018375656e+38f;
        sign = __builtin_fminf(__builtin_fmaxf(sign, 0.0f), 1.0f);
        f32 t = curved * side;
        f32 t2 = t * t;
        f32 t3 = t * t2;
        f32 t4 = t2 * t2;
        f32 t5 = t2 * t3;
        f32 angle = (side + sign) * 0.785398006439209f - t;
        angle += (-0.16666699945926666f * t) * t2;
        angle += (-0.07500000298023224f * t2) * t3;
        angle += (-0.04464289918541908f * t3) * t4;
        angle += (-0.03038189932703972f * t4) * t5;
        object->snake_body->segments[segment_index].pitch = static_cast<i16>(static_cast<i32>(angle * 10430.400390625f));
    } while (object->snake_body->segment_count > ++segment_index);
}

void DrawSnakeBody(GameObject_s *object) {
    if (object == NULL || object->snake_body == NULL || object->snake_body->segment_count == 0)
        return;
    i32 segment_index = 0;
    do {
        object->field_0x1004 = object->snake_body->scale;
        NUVEC scale;
        NUMTX matrix;
        NUANGVEC angles;
        angles.y = NuAngAdd(object->snake_body->segments[segment_index].yaw, 0x8000);
        angles.x = object->snake_body->segments[segment_index].pitch;
        NuMtxSetRotationXYVU0(&matrix, &angles);
        if (object->snake_body->scale != 1.0f) {
            scale.z = object->snake_body->scale;
            scale.y = object->snake_body->scale;
            scale.x = object->snake_body->scale;
            NuMtxPreScale(&matrix, &scale);
        }
        matrix.m30 += object->snake_body->segments[segment_index].position.x;
        matrix.m31 += object->snake_body->segments[segment_index].position.y + object->snake_body->scale * 0.02f;
        matrix.m32 += object->snake_body->segments[segment_index].position.z;
        i32 special_index = segment_index == object->snake_body->segment_count - 1 ? 2 : segment_index % 2;
        nuhspecial_s *special = &snake_hspecials[special_index];
        if (NuSpecialExistsFn(special))
            NuSpecialDrawAt(special, &matrix);
    } while (object->snake_body->segment_count > ++segment_index);
}

static void AddSnakeSegmentDebris(GameObject_s *object, i32 segment_index) {
    NUMTX matrix;
    NUANGVEC angles;
    angles.y = NuAngAdd(object->snake_body->segments[segment_index].yaw, 0x8000);
    angles.x = object->snake_body->segments[segment_index].pitch;
    NuMtxSetRotationXYVU0(&matrix, &angles);
    if (object->snake_body->scale != 1.0f) {
        NUVEC scale = {object->snake_body->scale, object->snake_body->scale, object->snake_body->scale};
        NuMtxPreScale(&matrix, &scale);
    }
    matrix.m30 += object->snake_body->segments[segment_index].position.x;
    matrix.m31 += object->snake_body->segments[segment_index].position.y + object->snake_body->scale * 0.02f;
    matrix.m32 += object->snake_body->segments[segment_index].position.z;
    i32 special_index = segment_index == object->snake_body->segment_count - 1 ? 2 : segment_index % 2;
    nuhspecial_s *special = &snake_hspecials[special_index];
    if (NuSpecialExistsFn(special)) {
        NUVEC momentum;
        SetKillPartMom(&momentum);
        momentum.y += 1.0f;
        ADDPART_s part = Default_ADDPART;
        part.matrix = &matrix;
        part.velocity = &momentum;
        part.field_14 = 0.1f;
        part.field_18 = 0.1f;
        part.gravity = -5.0f;
        part.special = special;
        part.flags = 0x90;
        part.stop_fn = PartStop_Flickerer;
        part.draw_fn = PartDraw_Flickerer;
        part.field_3c = PartImpact_Brick;
        part.time_step = FRAMETIME;
        part.lighting = reinterpret_cast<PARTLIGHTSOURCE_s *>(&object->light_data);
        AddPart(&part);
    }
}

void BlowUpSnakeBody(GameObject_s *object) {
    if (object != NULL && object->snake_body != NULL) {
        for (i32 index = 0; index < object->snake_body->segment_count; ++index)
            AddSnakeSegmentDebris(object, index);
        DestroySnakeBody(object);
    }
}

void SnakeBeenHit(GameObject_s *object) {
    if (object != NULL && object->snake_body != NULL && object->snake_body->segment_count > 2) {
        AddSnakeSegmentDebris(object, object->snake_body->segment_count - 1);
        AddSnakeSegmentDebris(object, object->snake_body->segment_count - 2);
        object->snake_body->segment_count -= 2;
    }
}

void EatVictim(GameObject_s *object) {
    object->character_context = -1;
    if (object->field_0x780 == NULL || (object->field_0xe24 & 1) == 0) {
        return;
    }
    object->context_animation = 0x4e;
    if (object->apiobj.character_model->model_data_b[0x4e] == NULL) {
        return;
    }
    object->character_context = 0x3f;
    object->context_animation_timer = AnimDuration(object->id, 0x4e, 0.0f, 0.0f, 1);
    object->context_flags &= ~0x40;
}
