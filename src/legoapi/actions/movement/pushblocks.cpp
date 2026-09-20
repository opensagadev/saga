#include <math.h>
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/numath.h"
#include "decomp.h"
#include "globals.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

pushblock_s *BlockInBlock(WORLDINFO_s *, pushblock_s *, i32, pushblock_s **);

enum PushBlockCompletionFlags {
    PUSH_BLOCK_FIRST_OUTPUT_FLAG = 1 << 3,
    PUSH_BLOCK_ANY_OUTPUT_MASK = 0x7f8,
};

u32 (*CanPushBlocksFn)(GameObject_s *) = NULL;
i32 pushposincrease;
i32 runoutofpostabspace;

void SetAnimFrame(nuhspecial_s *, f32);
void ResetPushProgress(WORLDINFO_s *, void *);
void ResetSinglePushBlockHeight(WORLDINFO_s *, pushblock_s *, i32);
i32 TerrainBlockOnBlock(WORLDINFO_s *, pushblock_s *, NUVEC *, f32 *);
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);

void KnockPushBlock(pushblock_s *, nuvec_s *) {
    STUBBED();
}

i32 NewBlockAction(GameObject_s *object) {
    i32 actions[3];
    i32 count = 0;
    for (i32 action = 0; action < apicharsys->model_id_capacity && count < 3; ++action) {
        if ((ActionInfo[action].flags & 8) != 0 && object->apiobj.character_model->model_data_b[action] != NULL) {
            actions[count++] = action;
        }
    }
    if (count == 0)
        return 0;
    i32 action = actions[0];
    if (count != 1) {
        do {
            action = actions[qrand() / (0xffff / count + 1)];
        } while (action == object->previous_block_animation);
    }
    object->previous_block_animation = action;
    object->context_animation = action;
    return 1;
}

pushblock_s *NearestPushBlock(WORLDINFO_s *world, nuvec_s *position, float range) {
    if (position == NULL || world == NULL)
        return NULL;
    const NUVEC minimum = {position->x - range, position->y - range, position->z - range};
    const NUVEC maximum = {position->x + range, position->y + range, position->z + range};
    pushblock_s *nearest = NULL;
    f32 nearest_distance = 1000000000.0f;
    for (i32 i = 0; i < world->push_block_count; ++i) {
        pushblock_s *block = &world->push_blocks[i];
        if ((block->packed_state_flags & 0x1040100) != 0x1040000)
            continue;
        NUVEC *candidate = block->position;
        if (candidate->x < minimum.x || candidate->x > maximum.x || candidate->z < minimum.z ||
            candidate->z > maximum.z || candidate->y < minimum.y || candidate->y > maximum.y)
            continue;
        const f32 distance = NuVecDistSqr(position, candidate, NULL);
        if (distance < nearest_distance) {
            nearest = block;
            nearest_distance = distance;
        }
    }
    return nearest;
}

extern "C" f32 NuFmax(f32, f32);
void NewBuzz(nupad_s *, f32, i32);
void PushSeekComplete(pushblock_s *block, i32 index) {
    block->completion_flags =
        (block->completion_flags & 0xf807) | ((((block->completion_flags >> 3) | (1u << (index & 31))) & 0xff) << 3);
    block->runtime_flags_0c9 &= ~2;
    if (!(block->flags_0cb & 0x20)) {
        block->flags_0cb |= 2;
        block->runtime_flags_0c9 |= 1;
    }
    if (block->pushing_object)
        NewBuzz(block->pushing_object->pad_gamepad->pad, 0.1f, 0);
}

i32 OtherBlockInRange(WORLDINFO_s *world, pushblock_s *block, nuvec_s *position, i32 excluded) {
    NUVEC centre;
    f32 radius;
    NuSpecialGetRadius(&block->special, &centre, &radius);
    radius *= radius;
    for (i32 i = 0; i < world->push_block_count; ++i) {
        if (i == excluded)
            continue;
        pushblock_s *other = &world->push_blocks[i];
        if ((other->packed_state_flags & 0x01040000) != 0x01040000 || (other->flags_0cb & 4))
            continue;
        if (!NuSpecialGetVisibilityFn(&other->special))
            continue;
        NUVEC minimum, maximum;
        NuSpecialGetBounds(&other->special, &minimum, &maximum);
        NuFmax(fabsf(minimum.x), maximum.x);
        NuFmax(fabsf(minimum.z), maximum.z);
        f32 other_radius;
        NuSpecialGetRadius(&other->special, &centre, &other_radius);
        centre = *other->position;
        f32 x = centre.x - position->x, z = centre.z - position->z;
        f32 distance = (x * x + z * z) - other_radius * other_radius;
        if (radius >= distance || 0.0f >= distance)
            return 1;
    }
    return 0;
}

void ResetSinglePushBlock(WORLDINFO_s *, pushblock_s *block, i32) {
    block->platform_id = FindPlatInst(NuSpecialGetInstanceix(&block->special));
    SetAnimFrame(&block->special, 1.0f);
    block->runtime_flags_0c8 &= 0xf2;
    block->flags_0cb &= ~2u;
    block->runtime_flags_0c9 &= 0xf0;
    block->completion_flags &= 0xf807;
    block->packed_state_flags &= 0xfffc7fff;
    block->pushing_object = NULL;

    if (NuSpecialExistsFn(&block->special) == 0) {
        block->bounds_min = v000;
        block->bounds_max = v000;
        block->position = &v000;
        return;
    }

    NuSpecialGetBounds(&block->special, &block->bounds_min, &block->bounds_max);
    NUMTX *matrix = NuSpecialGetInstanceMtx(&block->special);
    block->position = reinterpret_cast<NUVEC *>(&matrix->m30);
    if ((block->runtime_flags_0c9 & 0x70) != 0 && runoutofpostabspace == 0) {
        *block->position = block->snap_positions[0];
    }
}

void NearestFacingPushBlock(WORLDINFO_s *, GameObject_s *, float) {
    STUBBED();
}

void GizmoPushBlockInitAndReset(WORLDINFO_s *world, void *progress) {
    world->push_block_position_count = 0;
    runoutofpostabspace = 0;
    pushposincrease = 0;
    world->push_block_positions = reinterpret_cast<NUVEC *>(world->giz_buffer.void_ptr);
    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr + world->current_level->max_push_block_end_pos * sizeof(NUVEC), 4);
    ResetPushProgress(world, progress);

    for (i32 index = 0; index < world->push_block_count; ++index) {
        pushblock_s *block = &world->push_blocks[index];
        block->runtime_flags_0c9 &= 0x8f;
        if (NuSpecialGetInstAnim(&block->special) != NULL) {
            i32 positions = static_cast<i32>(NuSpecialGetAnimEndFrame(&block->special)) & 7;
            block->output_count = positions;
            block->runtime_flags_0c9 = (block->runtime_flags_0c9 & 0x8f) | ((positions & 7) << 4);
            const i32 required = world->push_block_position_count + positions;
            if (required > world->current_level->max_push_block_end_pos) {
                pushposincrease += runoutofpostabspace == 0
                                       ? required - world->current_level->max_push_block_end_pos
                                       : positions;
                runoutofpostabspace = 1;
            } else if (positions != 0 && runoutofpostabspace == 0) {
                block->snap_positions = &world->push_block_positions[world->push_block_position_count];
                for (i32 position = 0; position < positions; ++position) {
                    NUMTX evaluated;
                    EvalAnim(&block->special, static_cast<f32>(position + 1), &evaluated, 0);
                    block->snap_positions[position] = *reinterpret_cast<NUVEC *>(&evaluated.m30);
                    block->snap_positions[position].x += NuSpecialGetMtx(&block->special)->m30;
                    block->snap_positions[position].y += NuSpecialGetMtx(&block->special)->m31;
                    block->snap_positions[position].z += NuSpecialGetMtx(&block->special)->m32;
                    ++world->push_block_position_count;
                }
            }
        }
        ResetSinglePushBlock(world, block, index);
    }

    for (i32 index = 0; index < world->push_block_count; ++index) {
        pushblock_s *block = &world->push_blocks[index];
        block->velocity = v000;
        block->target_velocity = v000;
        block->snap_origin = *block->position;

        const f32 left = fabsf(block->bounds_min.x) - 0.006f;
        const f32 right = fabsf(block->bounds_max.x) - 0.006f;
        const f32 back = fabsf(block->bounds_min.z) - 0.006f;
        const f32 front = fabsf(block->bounds_max.z) - 0.006f;
        const f32 y = block->position->y - fabsf(block->bounds_min.y) + 0.026f;
        NUVEC corners[4] = {
            {block->position->x - left, y, block->position->z - back},
            {block->position->x + right, y, block->position->z - back},
            {block->position->x + right, y, block->position->z + front},
            {block->position->x - left, y, block->position->z + front},
        };
        f32 heights[4];
        PlatOnOff(block->platform_id, 0);
        for (i32 corner = 0; corner < 4; ++corner) {
            NewTerrPlatformsOff();
            heights[corner] = GameShadow(NULL, &corners[corner], 0.1f, -1);
            block->terrain_info[corner] = static_cast<i8>(ShadowInfo());
            block->extra_terrain_info[corner] = static_cast<i8>(EShadowInfo());
        }
        PlatOnOff(block->platform_id, 1);

        if (heights[0] == heights[1] && heights[1] == heights[2] && heights[2] == heights[3]) {
            NewTerrPlatformsOff();
            block->ground_height = GameShadow(NULL, block->position, 5.0f, -1);
            if (block->ground_height == 2000000.0f) {
                block->ground_height = 0.0f;
            }
        } else {
            block->ground_height = (heights[0] + heights[1] + heights[2] + heights[3]) * 0.25f;
        }
        TerrainBlockOnBlock(world, block, corners, heights);
        block->previous_extra_terrain_info = *reinterpret_cast<i32 *>(block->extra_terrain_info);
    }

    for (i32 index = 0; index < world->push_block_count; ++index) {
        ResetSinglePushBlockHeight(world, &world->push_blocks[index], index);
    }
}

void ResetSinglePushBlockHeight(WORLDINFO_s *world, pushblock_s *block, i32 index) {
    BlockInBlock(world, block, index, &block->block_below);

    const f32 epsilon = 0.01f;
    f32 support_height;
    if (block->block_below != NULL) {
        pushblock_s *support = block->block_below;
        support_height = support->bounds_max.y + support->position->y + epsilon;
        block->support_height = support_height;
    } else {
        support_height = block->support_height;
    }

    NUVEC *position = block->position;
    f32 position_y = position->y;
    f32 penetration = block->bounds_min.y + position_y - support_height;
    block->vertical_penetration = penetration;
    if (penetration > epsilon) {
        position->y = position_y - penetration;
        block->vertical_penetration = 0.0f;
    }
}

i32 GizPushBlock_EndFrameCompleted(pushblock_s *push_block, i32 output_index) {
    if (push_block == NULL) {
        return -1;
    }

    if (output_index == 0) {
        return (push_block->completion_flags & PUSH_BLOCK_ANY_OUTPUT_MASK) != 0;
    }
    const u8 completed_outputs = push_block->completion_flags / PUSH_BLOCK_FIRST_OUTPUT_FLAG;
    return (completed_outputs >> output_index) & 1;
}

void PushBlock(GameObject_s *) {
    STUBBED();
}
