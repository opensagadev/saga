#include "legoapi/actions/movement/pushblocks.h"

#include <math.h>
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/numath.h"
#include "decomp.h"
#include "nu2api/nucore/nuanim3.h"
#include "globals.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/world/level.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

extern i32 LEGOCONTEXT_PUSH;
void AlertSurroundingCreatures(GameObject_s *, NUVEC *);

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

void KnockPushBlock(pushblock_s *block, nuvec_s *direction) {
    if (block != NULL) {
        block->velocity.x = -direction->x;
        block->velocity.y = -direction->y;
        block->velocity.z = -direction->z;
        NuVecScale(&block->velocity, &block->velocity, 0.005f);
        block->runtime_flags_0c8 |= 0x80;
    }
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

__attribute__((optimize("O2"))) pushblock_s *NearestFacingPushBlock(WORLDINFO_s *world, GameObject_s *object,
                                                                    float range) {
    pushblock_s *nearest = NULL;
    if (world == NULL)
        return nearest;
    i32 count = world->push_block_count;
    pushblock_s *blocks = world->push_blocks;
    if (blocks == NULL)
        return nearest;
    if (count == 0)
        return nearest;
    if (LEGOCONTEXT_PUSH == -1)
        return nearest;
    if (object->character_context != LEGOCONTEXT_PUSH)
        return nearest;
    f32 upper = object->apiobj.upper_position.y;
    u16 facing = object->apiobj.movement_facing_angle;
    f32 lower = object->apiobj.lower_position.y;
    f32 adjust = (upper - lower) * 0.25f;
    f32 max_y = upper - adjust;
    f32 min_y = lower + adjust;
    i32 mode;
    if (facing == 0) {
        mode = 2;
    } else {
        u16 angle16 = facing;
        i32 angle = facing;
        if (angle > 0xfc72) {
            mode = 2;
        } else if (angle16 <= 0x38d) {
            mode = 2;
        } else if (angle16 == 0x4000) {
            mode = 0;
            goto have_mode;
        } else if (angle <= 0x3c72) {
            goto check_8000;
        } else if (angle > 0x438d) {
            goto check_8000;
        } else {
            mode = 0;
            goto have_mode;
        }
    check_8000:
        if (angle16 == 0x8000) {
            mode = 3;
            goto have_mode;
        } else if (angle <= 0x7c72) {
            goto check_c000;
        } else if (angle > 0x838d) {
            goto check_c000;
        } else {
            mode = 3;
            goto have_mode;
        }
    check_c000:
        if (angle16 == 0xc000) {
            mode = 1;
            goto have_mode;
        } else if (angle <= 0xbc72) {
            return nearest;
        } else if (angle > 0xc38d) {
            return nearest;
        } else {
            mode = 1;
        }
    have_mode:;
    }
    nearest = NULL;
    if (world->push_block_count <= 0)
        return nearest;
    f32 nearest_dist = 1000000000.0f;
    for (i32 i = 0; i < world->push_block_count; ++i) {
        pushblock_s *block = &blocks[i];
        if (NuSpecialExistsFn(&block->special) != 0) {
            if (NuSpecialGetOnScreenFn(&block->special) == 0)
                continue;
            if (NuSpecialGetVisibilityFn(&block->special) == 0)
                continue;
        }
        if ((block->packed_state_flags & 0x4000100) != 0)
            continue;
        if ((block->flags_0cb & 2) != 0)
            continue;
        f32 max_x = block->bounds_max.x;
        NUVEC *pos = block->position;
        f32 top = pos->y + fabsf(block->bounds_max.y);
        f32 bottom = pos->y - fabsf(block->bounds_min.y);
        f32 min_x = block->bounds_min.x;
        f32 min_z = block->bounds_min.z;
        f32 max_z = block->bounds_max.z;
        if (top > max_y && bottom > max_y)
            continue;
        if (min_y > top) {
            if (min_y > bottom)
                continue;
        }
        f32 dx = pos->x - object->apiobj.collision_position.x;
        f32 dy = pos->y - object->apiobj.collision_position.y;
        f32 dz = pos->z - object->apiobj.collision_position.z;
        NUVEC delta;
        delta.x = dx;
        delta.y = dy;
        delta.z = dz;
        f32 gap;
        if (mode == 2) {
            goto mode2_geometry;
        }
        if (mode == 3) {
            goto mode3_geometry;
        }
        if (mode == 1) {
            goto mode1_geometry;
        }
    mode0_geometry: {
        f32 block_min_z = min_z + pos->z - 0.01f;
        f32 block_max_z = pos->z + max_z - 0.01f;
        if (block_min_z >= object->apiobj.collision_position.z)
            continue;
        if (object->apiobj.collision_position.z >= block_max_z)
            continue;
        if ((block->flags_0ca & 0x20) != 0)
            continue;
        if ((block->flags_0ca & 0x90) == 0x90)
            continue;
        gap = dx - fabsf(min_x);
        goto have_gap;
    }
    mode1_geometry: {
        f32 block_max_z = max_z + pos->z - 0.01f;
        f32 block_min_z = pos->z + min_z - 0.01f;
        if (object->apiobj.collision_position.z >= block_max_z)
            continue;
        if (block_min_z >= object->apiobj.collision_position.z)
            continue;
        if ((block->flags_0ca & 0x20) != 0)
            continue;
        if ((block->flags_0ca & 0x50) == 0x50)
            continue;
        gap = fabsf(dx) - max_x;
        goto have_gap;
    }
    mode3_geometry: {
        f32 block_min_x = min_x + pos->x - 0.01f;
        f32 block_max_x = pos->x + max_x - 0.01f;
        if (block_min_x >= object->apiobj.collision_position.x)
            continue;
        if (object->apiobj.collision_position.x >= block_max_x)
            continue;
        if ((block->flags_0ca & 0x10) != 0)
            continue;
        if ((block->flags_0ca & 0x60) == 0x60)
            continue;
        gap = fabsf(dz) - max_z;
        goto have_gap;
    }
    mode2_geometry: {
        f32 block_max_x = max_x + pos->x - 0.01f;
        f32 block_min_x = pos->x + min_x - 0.01f;
        if (object->apiobj.collision_position.x >= block_max_x)
            continue;
        if (block_min_x >= object->apiobj.collision_position.x)
            continue;
        if ((block->flags_0ca & 0x10) != 0)
            continue;
        if ((block->flags_0ca & 0xa0) == 0xa0)
            continue;
        gap = dz - fabsf(min_z);
        goto have_gap;
    }
    have_gap: {
        if (gap > (object->apiobj.field_0x1dc + 0.01f) * 2.0f)
            continue;
        i32 yaw = NuAtan2D(delta.x, delta.z);
        i32 rot = 0x4000 - (yaw & 0xffff);
        NuVecRotateY(&delta, &delta, rot);
        i32 pitch = NuAtan2D(delta.x, delta.y);
        i16 yaw_diff = (i16)(yaw - object->apiobj.movement_facing_angle);
        i32 yaw_abs = yaw_diff;
        yaw_abs = yaw_abs < 0 ? -yaw_abs : yaw_abs;
        if ((u16)yaw_abs > 0x2000)
            continue;
        i32 pitch_diff = 0x4000 - (u16)pitch;
        pitch_diff = pitch_diff < 0 ? -pitch_diff : pitch_diff;
        if ((u16)pitch_diff > 0x4000)
            continue;
        f32 dist2 = dx * dx + dz * dz;
        if (range <= dist2)
            continue;
        if (nearest_dist <= dist2)
            continue;
        nearest_dist = dist2;
        nearest = block;
    }
    }
    return nearest;
}
__attribute__((optimize("O2"))) void GizmoPushBlockInitAndReset(WORLDINFO_s *world, void *progress) {
    world->push_block_position_count = 0;
    runoutofpostabspace = 0;
    pushposincrease = 0;
    world->push_block_positions = reinterpret_cast<NUVEC *>(world->giz_buffer.void_ptr);
    world->giz_buffer.addr =
        ALIGN(world->giz_buffer.addr + world->current_level->max_push_block_end_pos * sizeof(NUVEC), 4);
    pushblock_s *block = world->push_blocks;
    ResetPushProgress(world, progress);

    for (i32 index = 0; index < world->push_block_count; ++index, ++block) {
        block->runtime_flags_0c9 &= 0x8f;
        nuinstanim_s *animation = NuSpecialGetInstAnim(&block->special);
        if (animation != NULL) {
            i32 positions =
                static_cast<i32>(NuAnimEndFrameOld(block->special.scene->instance_animation_data[animation->anim_ix])) &
                7;
            block->output_count = positions;
            block->runtime_flags_0c9 = (block->runtime_flags_0c9 & 0x8f) | ((positions & 7) << 4);
            const i32 required = world->push_block_position_count + positions;
            if (required > world->current_level->max_push_block_end_pos) {
                pushposincrease +=
                    runoutofpostabspace == 0 ? required - world->current_level->max_push_block_end_pos : positions;
                runoutofpostabspace = 1;
            } else if (positions != 0 && runoutofpostabspace == 0) {
                block->snap_positions = &world->push_block_positions[world->push_block_position_count];
                for (i32 position = 0; position < ((block->runtime_flags_0c9 >> 4) & 7); ++position) {
                    NUMTX evaluated;
                    EvalAnim(&block->special, static_cast<f32>(position + 1), &evaluated, 0);
                    block->snap_positions[position] = *reinterpret_cast<NUVEC *>(&evaluated.m30);
                    evaluated = *NuSpecialGetMtx(&block->special);
                    block->snap_positions[position].x += evaluated.m30;
                    block->snap_positions[position].y += evaluated.m31;
                    block->snap_positions[position].z += evaluated.m32;
                    ++world->push_block_position_count;
                }
            }
        }
        ResetSinglePushBlock(world, block, index);
    }

    block = world->push_blocks;
    for (i32 index = 0; index < world->push_block_count; ++index, ++block) {
        NUVEC centre = *block->position;
        block->velocity = v000;
        block->target_velocity = v000;
        block->snap_origin = centre;

        const f32 left = fabsf(block->bounds_min.x) - 0.006f;
        const f32 right = fabsf(block->bounds_max.x) - 0.006f;
        const f32 back = fabsf(block->bounds_min.z) - 0.006f;
        const f32 front = fabsf(block->bounds_max.z) - 0.006f;
        const f32 y = centre.y - fabsf(block->bounds_min.y) + 0.001f + 0.025f;
        NUVEC corners[4] = {
            {centre.x - left, y, centre.z - back},
            {centre.x + right, y, centre.z - back},
            {centre.x + right, y, centre.z + front},
            {centre.x - left, y, centre.z + front},
        };
        PlatOnOff(block->platform_id, 0);
        NewTerrPlatformsOff();
        const f32 height0 = GameShadow(NULL, &corners[0], 0.1f, -1);
        block->terrain_info[0] = static_cast<i8>(ShadowInfo());
        block->extra_terrain_info[0] = static_cast<i8>(EShadowInfo());
        NewTerrPlatformsOff();
        const f32 height1 = GameShadow(NULL, &corners[1], 0.1f, -1);
        block->terrain_info[1] = static_cast<i8>(ShadowInfo());
        block->extra_terrain_info[1] = static_cast<i8>(EShadowInfo());
        NewTerrPlatformsOff();
        const f32 height2 = GameShadow(NULL, &corners[2], 0.1f, -1);
        block->terrain_info[2] = static_cast<i8>(ShadowInfo());
        block->extra_terrain_info[2] = static_cast<i8>(EShadowInfo());
        NewTerrPlatformsOff();
        const f32 height3 = GameShadow(NULL, &corners[3], 0.1f, -1);
        block->terrain_info[3] = static_cast<i8>(ShadowInfo());
        block->extra_terrain_info[3] = static_cast<i8>(EShadowInfo());
        PlatOnOff(block->platform_id, 1);

        if (height1 == height2 && height0 == height1 && height2 == height3) {
            NewTerrPlatformsOff();
            block->ground_height = GameShadow(NULL, &centre, 5.0f, -1);
            if (block->ground_height == 2000000.0f) {
                block->ground_height = 0.0f;
            }
        } else {
            block->ground_height = (height1 + height0 + height2 + height3) * 0.25f;
        }
        f32 support_heights[4];
        TerrainBlockOnBlock(world, block, corners, support_heights);
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

i32 PushBlock(GameObject_s *object) {
    pushblock_s *block = NearestFacingPushBlock(WORLD, object, 2.0f);
    if (block != NULL) {
        block->pushing_object = object;
        block->runtime_flags_0c8 |= 1;
        AlertSurroundingCreatures(object, &object->apiobj.collision_position);
        return 1;
    }
    object->character_context = -1;
    return 0;
}
