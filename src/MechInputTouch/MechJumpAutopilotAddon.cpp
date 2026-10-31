#include "decomp.h"
#include "MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/render/core/terrain.h"
#include "nu2api/numath/nutrig.h"

HashedKey MechJumpAutoPilotAddon::s_hashId("MechJumpAutopilotAddon", HashedKey::kInlineHash);

void MechJumpAutoPilotAddon::AnalyseJumpTrajectory() {
    float y = field_44.y;
    float limit = field_24.y - 2.0f;
    if (y < limit || state == 2)
        goto finish;

    {
        float vy = field_54.y;
        float z = field_44.z;
        float x = field_44.x;
        for (int count = 50; count != 0; --count) {
            if (state == 2)
                break;

            VuVec next;
            next.x = x + field_54.x * 0.1f;
            next.z = z + field_54.z * 0.1f;
            next.y = y + vy * 0.1f;
            LookForBottomInt(next);
            LookForTerrInt(next);

            field_44.w = next.w;
            field_44.x = next.x;
            field_44.y = next.y;
            field_44.z = next.z;
            limit = field_24.y - 2.0f;
            vy = field_54.y + character->apiobj.character_data->game_character->gravity * 0.1f;
            field_54.y = vy;
            x = next.x;
            y = next.y;
            z = next.z;
            if (!(y >= limit))
                break;
        }
    }
finish:
    if (limit > y || state == 2)
        state = 2;
}

void MechJumpAutoPilotAddon::CalculateModifiedJumpTrajectory() {
    float first_time;
    float second_time;
    const float gravity = character->apiobj.character_data->game_character->gravity;
    if (__builtin_expect(!TouchHacks::SolveRoot(gravity * 0.5f, field_34.y, -(field_84.y - field_24.y), first_time, second_time), 0)) {
        state = 6;
        return;
    }

    const float time = first_time - elapsed_time;
    const float dx = field_84.x - field_24.x;
    const float dz = field_84.z - field_24.z;
    const float vx = field_34.x * time;
    const float vz = field_34.z * time;
    speed_scale = ((dx * dx + dz * dz) * 0.5f) / (vx * vx + vz * vz);
    if (!(speed_scale < 1.0f) && !(character->jump_input_flags & 0x10)) {
        speed_scale = 1.0f;
        character->jump_input_flags |= 0x10;
        state = 6;
        return;
    }
    if (!(speed_scale >= 0.5f))
        speed_scale = 0.5f;
    if (speed_scale > 1.5f)
        speed_scale = 1.5f;
    state = 4;
}

void MechJumpAutoPilotAddon::LookForBottomInt(VuVec const &point) {
    if (field_9c)
        return;
    const float drop = field_24.y - field_44.y;
    if (drop < 1.1920928955078125e-7f)
        return;

    const float t = drop / (point.y - field_44.y);
    if (!(t > 0.0f || t <= 1.0f))
        return;
    field_64.z = field_44.z + (point.z - field_44.z) * t;
    field_64.w = 0.0f;
    field_64.y = field_24.y;
    field_9c = true;
    field_64.x = field_44.x + (point.x - field_44.x) * t;
}

void MechJumpAutoPilotAddon::LookForLandingPoint() {
    if (field_9d) {
        field_9d = false;
        if (LookForLandingSpotAroundPoint(field_74))
            state = 3;
    } else if (__builtin_expect(field_9c != 0, 0)) {
        field_9c = false;
        if (!LookForLandingSpotAroundPoint(field_64)) {
            state = 5;
            return;
        }
        state = 3;
        return;
    } else {
        state = 5;
    }
}

// Retail stores each better landing spot as one unaligned four-float vector.
typedef f32 LandingSpotVec4 __attribute__((vector_size(16), aligned(4)));

bool MechJumpAutoPilotAddon::LookForLandingSpotAroundPoint(VuVec const &point) {
    // The centre is probed first, followed by eight directions at 45 degree
    // intervals. The near and far probes alternate between two radii.
    VuVec offsets[9];
    offsets[0].x = VuVec_Zero.x;
    offsets[0].y = VuVec_Zero.y;
    offsets[0].z = VuVec_Zero.z;
    offsets[0].w = VuVec_Zero.w;
    for (int i = 1; i < 9; i++) {
        const f32 radius = (i & 1) ? 0.2f : 0.5f;
        offsets[i].x = NU_SIN_LUT(i * 0x2000) * radius;
        offsets[i].y = 0.0f;
        offsets[i].z = NU_COS_LUT(i * 0x2000) * radius;
    }

    bool found = false;
    f32 best_score = -1000000000.0f;
    const f32 sweep_height = field_94 * 3.0f + 1.0f;
    const f32 ray_length = -sweep_height;
    const f32 ray_offset = sweep_height - 1.0f;

    for (int i = 0; i < 9; i++) {
        VuVec ray(0.0f, ray_length, 0.0f, 1.0f);
        const VuVec &offset = offsets[i];
        VuVec origin;
        origin.x = point.x + offset.x;
        origin.z = point.z + offset.z;
        origin.y = field_24.y + ray_offset;
        if (GameRayCast(&origin.xyz, &ray.xyz, 0.0f, 0)) {
            VuVec hit;
            hit.x = origin.x + ray.x;
            hit.z = origin.z + ray.z;
            hit.y = origin.y + ray.y;
            hit.w = 0.0f;
            const f32 dx = hit.x - field_24.x;
            const f32 dz = hit.z - field_24.z;
            const f32 dy = hit.y - field_24.y + 0.1f;
            const f32 score = dy * (dx * dx + dz * dz);
            if (score > best_score) {
                best_score = score;
                *reinterpret_cast<LandingSpotVec4 *>(&field_84) = (LandingSpotVec4){hit.x, hit.y, hit.z, hit.w};
            }
            found = true;
        }
    }
    return found;
}

void MechJumpAutoPilotAddon::LookForTerrInt(VuVec const &point) {
    NUVEC displacement;
    if (!field_9d) {

        displacement.x = point.x - field_44.x;
        displacement.y = point.y - field_44.y;
        displacement.z = point.z - field_44.z;
        if (GameRayCast(&field_44.xyz, &displacement, 0.0f, 0) == 0)
            return;

        field_74.w = 0.0f;
        VuVec normal = VuVec_Zero;
        field_74.z = field_44.z + displacement.z;
        field_9d = true;
        field_74.y = field_44.y + displacement.y;
        field_74.x = field_44.x + displacement.x;
        NewRayCastGetImpactNormal(&normal.xyz);
        state = 2 + (normal.y > 0.8f) * 4;
    }
}

MechJumpAutoPilotAddon::MechJumpAutoPilotAddon(MechObjectInterface &object)
    : MechAddon(object, s_hashId.value), character(object.GetCharacterObject()), state(0), elapsed_time(0.0f),
      speed_scale(1.0f), started(false) {
    character->jump_input_flags &= ~0x10;
}

void MechJumpAutoPilotAddon::ModifyJump() {
}

bool MechJumpAutoPilotAddon::OnProcess(MechAddon::ProcessStage, float delta_time) {
    if (character == NULL)
        return false;

    const bool is_jumping = character->character_context == LEGOCONTEXT_JUMP;
    if (!is_jumping && started)
        return false;
    if (state == 0) {
        elapsed_time += delta_time;
        if (__builtin_expect(character->character_context == LEGOCONTEXT_JUMP, 0) && !started) {
            field_24 =
                VuVec(character->apiobj.position.x, character->apiobj.position.y, character->apiobj.position.z, 1.0f);
            field_34 =
                VuVec(character->apiobj.velocity.x, character->apiobj.velocity.y, character->apiobj.velocity.z, 1.0f);
            field_44.x = field_24.x;
            field_44.y = field_24.y;
            field_44.z = field_24.z;
            field_44.w = 1.0f;
            field_54.x = field_34.x;
            field_54.y = field_34.y;
            field_54.z = field_34.z;
            field_54.w = 1.0f;
            state = 1;
            field_9c = false;
            field_9d = false;

            const float gravity = character->apiobj.character_data->game_character->gravity;
            field_94 = field_24.y + (-(field_34.y * field_34.y)) / (gravity + gravity);
            elapsed_time = 0.0f;
        }
    } else {
        GameObject_s *jump_character = character;
        const float x_speed = field_34.x * speed_scale;
        const float z_speed = field_34.z * speed_scale;
        jump_character->apiobj.movement_direction.x = x_speed;
        jump_character->target_velocity.x = x_speed;
        jump_character->apiobj.velocity.x = x_speed;
        jump_character->apiobj.movement_direction.z = z_speed;
        jump_character->target_velocity.z = z_speed;
        jump_character->apiobj.velocity.z = z_speed;
        elapsed_time += delta_time;

        switch (state) {
            case 1:
                AnalyseJumpTrajectory();
                break;
            case 2:
                LookForLandingPoint();
                break;
            case 3:
                CalculateModifiedJumpTrajectory();
                break;
            case 4:
                ModifyJump();
                break;
            case 5:
                ProcJumpingToCertainDoom();
                break;
            default:
                break;
        }
    }

    started |= character->character_context == LEGOCONTEXT_JUMP;
    return true;
}

void MechJumpAutoPilotAddon::ProcJumpingToCertainDoom() {
}

void MechJumpAutoPilotAddon::Recalculate() {
    speed_scale = 1.0f;
    state = 0;
    started = false;
    field_9c = false;
    field_9d = false;
}

MechJumpAutoPilotAddon::~MechJumpAutoPilotAddon() {
    character->jump_input_flags &= ~0x10;
}
