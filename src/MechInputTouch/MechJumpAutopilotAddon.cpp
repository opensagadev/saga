#include "decomp.h"
#include "MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/render/core/terrain.h"
#include "nu2api/numath/nutrig.h"

HashedKey MechJumpAutoPilotAddon::s_hashId("MechJumpAutopilotAddon");

void MechJumpAutoPilotAddon::AnalyseJumpTrajectory() {
    const float floor = field_24.y - 2.0f;
    float y = field_44.y;
    if (y < floor || state == 2) {
        state = 2;
        return;
    }

    for (int count = 50; count != 0; --count) {
        if (state == 2) break;

        VuVec next;
        next.x = field_44.x + field_54.x * 0.1f;
        next.y = y + field_54.y * 0.1f;
        next.z = field_44.z + field_54.z * 0.1f;
        LookForBottomInt(next);
        LookForTerrInt(next);

        field_44 = next;
        field_54.y += character->apiobj.character_data->game_character->gravity * 0.1f;
        y = next.y;
        if (y < floor) break;
    }
    if (y < floor || state == 2) state = 2;
}

void MechJumpAutoPilotAddon::CalculateModifiedJumpTrajectory() {
    float first_time;
    float second_time;
    const float gravity = character->apiobj.character_data->game_character->gravity;
    if (!TouchHacks::SolveRoot(gravity * 0.5f, field_34.y, -(field_84.y - field_24.y), first_time,
                               second_time)) {
        state = 6;
        return;
    }

    const float time = first_time - elapsed_time;
    const float dx = field_84.x - field_24.x;
    const float dz = field_84.z - field_24.z;
    const float vx = field_34.x * time;
    const float vz = field_34.z * time;
    speed_scale = ((dx * dx + dz * dz) * 0.5f) / (vx * vx + vz * vz);
    if (speed_scale < 1.0f && !(character->jump_input_flags & 0x10)) {
        speed_scale = 1.0f;
        character->jump_input_flags |= 0x10;
        state = 6;
        return;
    }
    if (speed_scale < 0.5f) speed_scale = 0.5f;
    if (speed_scale > 1.5f) speed_scale = 1.5f;
    state = 4;
}

void MechJumpAutoPilotAddon::LookForBottomInt(VuVec const &point) {
    if (field_9c) return;
    const float drop = field_24.y - field_44.y;
    if (drop < 1.1920928955078125e-7f) return;

    const float t = drop / (point.y - field_44.y);
    if (t <= 0.0f || t > 1.0f) return;
    field_64.x = field_44.x + (point.x - field_44.x) * t;
    field_64.y = field_24.y;
    field_64.z = field_44.z + (point.z - field_44.z) * t;
    field_64.w = 0.0f;
    field_9c = true;
}

void MechJumpAutoPilotAddon::LookForLandingPoint() {
    if (field_9d) {
        field_9d = false;
        if (LookForLandingSpotAroundPoint(field_74)) state = 3;
    } else if (field_9c) {
        field_9c = false;
        if (LookForLandingSpotAroundPoint(field_64)) state = 3;
        else state = 5;
    } else {
        state = 5;
    }
}

bool MechJumpAutoPilotAddon::LookForLandingSpotAroundPoint(VuVec const &point) {
    VuVec offsets[9] = {
        VuVec_Zero,
        VuVec(NuTrigTable[0x1000] * 0.2f, 0.0f, NuTrigTable[0x3000] * 0.2f, 1.0f),
        VuVec(NuTrigTable[0x2000] * 0.5f, 0.0f, NuTrigTable[0x4000] * 0.5f, 1.0f),
        VuVec(NuTrigTable[0x3000] * 0.2f, 0.0f, NuTrigTable[0x5000] * 0.2f, 1.0f),
        VuVec(NuTrigTable[0x4000] * 0.5f, 0.0f, NuTrigTable[0x6000] * 0.5f, 1.0f),
        VuVec(NuTrigTable[0x5000] * 0.2f, 0.0f, NuTrigTable[0x7000] * 0.2f, 1.0f),
        VuVec(NuTrigTable[0x6000] * 0.5f, 0.0f, NuTrigTable[0x0000] * 0.5f, 1.0f),
        VuVec(NuTrigTable[0x7000] * 0.2f, 0.0f, NuTrigTable[0x1000] * 0.2f, 1.0f),
        VuVec(NuTrigTable[0x0000] * 0.5f, 0.0f, NuTrigTable[0x2000] * 0.5f, 1.0f),
    };

    bool found = false;
    f32 best_score = -2000000000.0f;
    const f32 sweep_height = field_94 * 3.0f + 0.5f;
    const f32 ray_height = field_24.y + (sweep_height - 0.5f);
    const f32 ray_length = -sweep_height;

    for (i32 i = 0; i < 9; ++i) {
        VuVec origin(point.x + offsets[i].x, ray_height, point.z + offsets[i].z, 1.0f);
        VuVec ray(0.0f, ray_length, 0.0f, 1.0f);
        if (GameRayCast(&origin.xyz, &ray.xyz, 0.0f, 0)) {
            const f32 x = origin.x + ray.x;
            const f32 y = origin.y + ray.y;
            const f32 z = origin.z + ray.z;
            const f32 dx = x - field_24.x;
            const f32 dz = z - field_24.z;
            const f32 score = (dx * dx + dz * dz) * (y - field_24.y + 0.1f);
            if (score > best_score) {
                best_score = score;
                field_84 = VuVec(x, y, z, 0.0f);
            }
            found = true;
        }
    }
    return found;
}

void MechJumpAutoPilotAddon::LookForTerrInt(VuVec const &point) {
    if (field_9d) return;
    NUVEC displacement = {point.x - field_44.x, point.y - field_44.y, point.z - field_44.z};
    if (GameRayCast(&field_44.xyz, &displacement, 0.0f, 0) == 0) return;

    field_74.x = field_44.x + displacement.x;
    field_74.y = field_44.y + displacement.y;
    field_74.z = field_44.z + displacement.z;
    field_74.w = 0.0f;
    field_9d = true;
    VuVec normal = VuVec_Zero;
    NewRayCastGetImpactNormal(&normal.xyz);
    state = 2 + (normal.y > 0.8f) * 4;
}

MechJumpAutoPilotAddon::MechJumpAutoPilotAddon(MechObjectInterface &object)
    : MechAddon(object, s_hashId.value), character(object.GetCharacterObject()), state(0), elapsed_time(0.0f),
      speed_scale(1.0f), started(false) {
    character->jump_input_flags &= ~0x10;
}

void MechJumpAutoPilotAddon::ModifyJump() {
}

bool MechJumpAutoPilotAddon::OnProcess(MechAddon::ProcessStage, float delta_time) {
    if (character == NULL) return false;

    const bool is_jumping = character->character_context == LEGOCONTEXT_JUMP;
    if (!is_jumping && started) return false;
    if (is_jumping || !started) {
        if (state != 0) {
            const float x_speed = field_34.x * speed_scale;
            const float z_speed = field_34.z * speed_scale;
            character->apiobj.movement_direction.x = x_speed;
            character->target_velocity.x = x_speed;
            character->apiobj.velocity.x = x_speed;
            character->apiobj.movement_direction.z = z_speed;
            character->target_velocity.z = z_speed;
            character->apiobj.velocity.z = z_speed;
        }
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

    if (character->character_context == LEGOCONTEXT_JUMP && !started) {
        field_24.x = character->apiobj.position.x;
        field_24.y = character->apiobj.position.y;
        field_24.z = character->apiobj.position.z;
        field_24.w = 1.0f;
        field_34.x = character->apiobj.velocity.x;
        field_34.y = character->apiobj.velocity.y;
        field_34.z = character->apiobj.velocity.z;
        field_34.w = 1.0f;
        field_44 = field_24;
        field_54 = field_34;
        field_64.w = 1.0f;
        field_74.w = 1.0f;
        field_84.w = 1.0f;
        field_9c = false;
        field_9d = false;
        state = 1;
        elapsed_time = 0.0f;

        const float gravity = character->apiobj.character_data->game_character->gravity;
        const float vy = character->apiobj.velocity.y;
        field_94 = character->apiobj.position.y - (vy * vy) / (gravity + gravity);
    }

    started = character->character_context == LEGOCONTEXT_JUMP || started;
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
