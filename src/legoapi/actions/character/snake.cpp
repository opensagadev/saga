#include "decomp.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/render/fx/parts.h"
#include "globals.h"
#include <string.h>
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void GrabVictim(GameObject_s *, GameObject_s *) {
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

extern "C" void NuMtxSetRotationXYVU0(NUMTX *, NUANGVEC *);

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

void SnakeBeenHit(GameObject_s *object) {
    if (object != NULL && object->snake_body != NULL && object->snake_body->segment_count > 2) {
        AddSnakeSegmentDebris(object, object->snake_body->segment_count - 1);
        AddSnakeSegmentDebris(object, object->snake_body->segment_count - 2);
        object->snake_body->segment_count -= 2;
    }
}

void BlowUpSnakeBody(GameObject_s *object) {
    if (object != NULL && object->snake_body != NULL) {
        for (i32 index = 0; index < object->snake_body->segment_count; ++index)
            AddSnakeSegmentDebris(object, index);
        DestroySnakeBody(object);
    }
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

void UpdateSnakeBody(GameObject_s *) {
}

void DestroySnakeBody(GameObject_s *object) {
    if (object != NULL && object->snake_body != NULL) {
        memset(object->snake_body, 0, sizeof(*object->snake_body));
        object->snake_body = NULL;
    }
}

void EatVictim(GameObject_s *) {
}
