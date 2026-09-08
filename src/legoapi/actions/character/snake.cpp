#include "decomp.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
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

void SnakeBeenHit(GameObject_s *) {
}

void BlowUpSnakeBody(GameObject_s *) {
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
            body->segments[segment].rotation = object->apiobj.field_0x276;
            body->segments[segment].state = 0;
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
