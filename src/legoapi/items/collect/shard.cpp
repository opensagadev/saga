#include "decomp.h"
#include "globals.h"
#include "legoapi/gizmos/traps/shards.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void NewBuzzFrames(nupad_s *, i32, i32);
i32 qrand();
void Shard_Collect(SHARD_s *shard, GameObject_s *object) {
    shard->collection_time = 0.0f;
    shard->collecting = 1;
    shard->collector = object;
    NuVecRotateZ(&shard->collection_velocity, &v010, shard->angle_z);
    NuVecRotateZ(&shard->collection_velocity, &shard->collection_velocity, shard->angle_x);
    NuVecScale(&shard->collection_velocity, &shard->collection_velocity, 3.0f);
    NewBuzzFrames(shard->collector->pad_gamepad->pad, 1, 0);
    shard->tumble_x = qrand() <= 0x7fff;
}

SHARD *Shard_FindNearest(WORLDINFO_s *world, nuvec_s *position, GameObject_s *object, float *distance_squared) {
    SHARD *nearest = NULL;
    f32 best_distance = 1000000000.0f;
    SHARD *shard = static_cast<SHARD *>(world->shards);
    for (i32 i = 0; i < world->shard_count; ++i, ++shard) {
        if (object != NULL && (shard->state_flags & 0x0f) != 3)
            continue;
        f32 distance = NuVecDistSqr(position, &shard->current_position, NULL);
        if (distance < best_distance) {
            best_distance = distance;
            nearest = shard;
        }
    }
    if (distance_squared != NULL)
        *distance_squared = best_distance;
    return nearest;
}

void Shards_HandleLostObj(WORLDINFO_s *world, GameObject_s *object) {
    SHARD *shard = static_cast<SHARD *>(world->shards);
    for (i32 i = 0; i < world->shard_count; ++i, ++shard) {
        if ((shard->state_flags & 0x0c) == 4 && shard->collector == object) {
            shard->collector = NULL;
            shard->collection_time = 0.0f;
            shard->state_flags &= ~4;
        }
    }
}
