// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "MechInputTouch/MechInputTouch_types.h"

#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/ai/core/legoai.h"
#include "gamelib/util/gamelib_util_types.h"
#include "globals.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/characters/motion.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include <string.h>
#include <float.h>
#include <math.h>
#include "editor/edpath.h"
#include "batman.h"
#include <stdio.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "editor/aieditor_state.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "gameapi/edtools/edui.h"
#include "nu2api/nucore/nupad.h"
#include <stdint.h>
#include "nu2api/nu3d/numtl.h"
#include "editor/edpath_types.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/render/core/render.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/world/mission.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nuhgobj.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/contexts.h"
#include <stdlib.h>
#include "nu2api/nucore/nuanim3.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "gameframework/saveload.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include <new>
#include "legoapi/characters/motion/move.h"

i32 testStreakAlpha = 32;
f32 testColourSecF = 256.0f;
f32 testStreakTime = 0.5f;
f32 testStreakCrossSize2 = 0.15f;
f32 testStreakRotateSpeed = 65536.0f;
f32 generateNewStreakTime = 1.5f;
f32 testStreakMinDist = 1.0f;
f32 testStreakSpeed = 1.0f;
f32 testStreakCrossSize = 0.1f;
f32 testStreakClipTestRadius = 0.2f;

namespace {
    struct AutoJumpStreakLink {
        NULISTLNK link;
        MechAutoJumpConnection *connection;
        AIPATH_s *path;
        AIPATHCNX_s *path_connection;
        i32 direction;
        void *handle;
        void *reserved;
        NUVEC start;
        NUVEC end;
        u32 colour;
        f32 elapsed;
    };
    DECOMP_ASSERT(sizeof(AutoJumpStreakLink) == 0x40, "AutoJumpStreakLink size");
} // namespace

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

struct AIPATH_s;

struct EDCREATURE_s;

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// Retail keeps only the IPA-SRA clone, which takes the other object's x and z by address.

struct AISYS_s;

// NuFileExists @nufile (nucore_plain.cpp).

MechAutoJumpManager::MechAutoJumpManager(AISYS_s *ai_system) {
    if (ai_system != nullptr) {
        streak_time = 0.0f;
        ai_sys = ai_system;
        streaks.head = nullptr;
        streaks.tail = nullptr;
        jump_connections.head = nullptr;
        jump_connections.tail = nullptr;
    }
}

void MechAutoJumpManager::Init() {
}

void MechAutoJumpManager::DeleteJumpConnection(MechAutoJumpConnection *connection) {
    if (connection != NULL) {
        AutoJumpStreakLink *streak = reinterpret_cast<AutoJumpStreakLink *>(NuLinkedListGetHead(&streaks));
        while (streak != NULL) {
            if (streak->connection == connection) {
                streak->connection = NULL;
            }
            streak = reinterpret_cast<AutoJumpStreakLink *>(
                NuLinkedListGetNext(&streaks, reinterpret_cast<NULISTLNK *>(streak)));
        }

        NuLinkedListRemove(&jump_connections, reinterpret_cast<NULISTLNK *>(connection));
        delete connection;
    }
}

void MechAutoJumpManager::DeleteJumpConnectionsAndStreaks() {
    AutoJumpStreakLink *streak;
    while ((streak = reinterpret_cast<AutoJumpStreakLink *>(NuLinkedListGetHead(&streaks))) != NULL) {
        NuLinkedListRemove(&streaks, reinterpret_cast<NULISTLNK *>(streak));
        delete streak;
    }

    MechAutoJumpConnection *connection =
        reinterpret_cast<MechAutoJumpConnection *>(NuLinkedListGetHead(&jump_connections));
    while (connection != NULL) {
        DeleteJumpConnection(connection);
        connection = reinterpret_cast<MechAutoJumpConnection *>(NuLinkedListGetHead(&jump_connections));
    }
}

MechAutoJumpManager::~MechAutoJumpManager() {
    DeleteJumpConnectionsAndStreaks();
}

bool AISysNodeCanReachThisJumpConnection(GameObject_s &, AIPATH_s &path, unsigned char node_index,
                                         AIPATHCNX_s &jump_connection, i32 jump_direction) {
    u8 from_node = jump_connection.node_indices[jump_direction];
    u8 to_node = jump_connection.node_indices[jump_direction == 0];
    while (node_index != from_node) {
        if (node_index == to_node) {
            return 0;
        }
        u8 connection_index = path.route_matrix[node_index][from_node];
        if (connection_index == 0xff) {
            return 0;
        }
        AIPATHCNX *connection = path.nodes[node_index].connections[connection_index];
        if (connection == NULL) {
            return 0;
        }
        u8 direction = node_index == connection->node_indices[1];
        if ((connection->traversal_flags[direction] & mechAutoJumpCantReachFlags) != 0) {
            return 0;
        }
        node_index = connection->node_indices[direction ^ 1];
    }
    return 1;
}

bool AISysCharacterCanReachThisJumpConnection(GameObject_s &object, AIPATH_s &path, AIPATHCNX_s &jump_connection,
                                              i32 jump_direction) {
    if (object.ai.path_info.path != &path || object.ai.path_info.connection == NULL) {
        return 0;
    }
    u8 node_index = object.ai.path_info.dist < 0.5f ? object.ai.path_info.connection->node_indices[0]
                                                    : object.ai.path_info.connection->node_indices[1];
    return AISysNodeCanReachThisJumpConnection(object, path, node_index, jump_connection, jump_direction);
}

u32 DoSomeChecks(GameObject_s &object, AIPATH_s &path, AIPATHCNX_s &connection, i32 direction) {
    AIPATHNODE *node = &path.nodes[connection.node_indices[direction]];
    if (!(object.ai.terrain_origin.y > node->min_height && object.ai.terrain_origin.y < node->max_height)) {
        return 0;
    }
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    if (world != NULL && world->current_level == E1CHARACTERBONUSA_LDATA) {
        AIPATHNODE *other_node = &path.nodes[connection.node_indices[direction == 0]];
        if (node->position.y - other_node->position.y > 0.5f) {
            return 0;
        }
    }
    f32 distance = NuVecXZDistSqr(&object.ai.terrain_origin, &node->position, NULL);
    u32 result = 0;
    if (distance >= node->radius_squared) {
        if (distance < testAutoJumpXZCanUseRangeSqr) {
            result |= 1;
        }
        if (distance < testAutoJumpXZCanDisplayRangeSqr) {
            result |= 2;
        }
    } else if (distance < node->radius_squared) {
        result = 1;
        if (!(distance >= testAutoJumpXZCanDisplayRangeSqr)) {
            result |= 2;
        }
    }
    if (result == 0 || !AISysCharacterCanReachThisJumpConnection(object, path, connection, direction)) {
        return 0;
    }
    if ((connection.traversal_flags[direction] & object.ai.capabilities & mechAutoJumpFlags) != 0) {
        result |= 4;
    }
    return result;
}

MechAutoJumpConnection *MechAutoJumpManager::AddAutoJumpConnection(AIPATH_s *path, AIPATHCNX_s *connection,
                                                                   i32 direction, bool use_path_direction, i32 colour,
                                                                   bool allow_streak) {
    if (connection != NULL && path != NULL) {
        MechAutoJumpConnection *jump =
            reinterpret_cast<MechAutoJumpConnection *>(NuLinkedListGetHead(&jump_connections));
        while (jump != NULL) {
            if (jump->path == path && jump->connection == connection && jump->direction == direction) {
                break;
            }
            jump = reinterpret_cast<MechAutoJumpConnection *>(
                NuLinkedListGetNext(&jump_connections, reinterpret_cast<NULISTLNK *>(jump)));
        }

        if (jump == NULL) {
            jump = new MechAutoJumpConnection;
            jump->path = NULL;
            jump->connection = NULL;
            jump->active = false;
            jump->use_path_direction = false;
            jump->is_using = false;
            jump->link.prev = NULL;
            jump->link.next = NULL;
            jump->base_colour_components.y = (colour >> 8) & 0xff;
            jump->base_colour_components.x = colour & 0xff;
            jump->streak_handle = NULL;
            jump->cooldown = 0.0f;
            jump->streak_alpha = 0.0f;
            jump->streak_reserved = 0.0f;
            jump->base_colour = colour;
            jump->base_colour_components.z = (colour >> 16) & 0xff;
            NuLinkedListInsert(&jump_connections, reinterpret_cast<NULISTLNK *>(jump));
        }

        jump->path = path;
        jump->direction = direction;
        jump->use_path_direction = use_path_direction;
        jump->connection = connection;
        jump->active = true;
        jump->colour = colour;
        jump->allow_streak = allow_streak;
        return jump;
    }
    return NULL;
}

void MechAutoJumpManager::PreProcessJumpConnections() {
    MechAutoJumpConnection *jump = reinterpret_cast<MechAutoJumpConnection *>(NuLinkedListGetHead(&jump_connections));
    while (jump != NULL) {
        jump->active = false;
        jump->use_path_direction = false;
        jump->allow_streak = false;
        if (jump->cooldown > 0.0f) {
            jump->cooldown -= FRAMETIME;
            if (jump->cooldown < 0.0f) {
                jump->cooldown = 0.0f;
            }
        }

        if (jump->is_using != 0) {
            if (player == NULL || ((player->touch_task == NULL || player->touch_task->IsBigJumpTask() == 0) &&
                                   static_cast<i8>(player->character_context) != LEGOCONTEXT_BIGJUMP)) {
                jump->is_using = false;
            } else if ((jump->connection->traversal_flags[jump->direction == 0] & mechAutoJumpFlags) != 0) {
                WORLDINFO *world = WorldInfo_CurrentlyActive();
                if (world != NULL && world->mech_auto_jump_manager != NULL) {
                    MechAutoJumpConnection *active = world->mech_auto_jump_manager->AddAutoJumpConnection(
                        jump->path, jump->connection, jump->direction == 0, false,
                        (static_cast<u32>(static_cast<u8>(testStreakAlpha)) << 24) | 0x808080, false);
                    if (active != NULL) {
                        active->cooldown = 2.0f;
                    }
                }
            }
        }
        jump = reinterpret_cast<MechAutoJumpConnection *>(
            NuLinkedListGetNext(&jump_connections, reinterpret_cast<NULISTLNK *>(jump)));
    }
}

void MechAutoJumpManager::ProcessJumpConnections() {
    MechAutoJumpConnection *jump = reinterpret_cast<MechAutoJumpConnection *>(NuLinkedListGetHead(&jump_connections));
    while (jump != NULL) {
        MechAutoJumpConnection *next = reinterpret_cast<MechAutoJumpConnection *>(
            NuLinkedListGetNext(&jump_connections, reinterpret_cast<NULISTLNK *>(jump)));
        if (jump->active == 0 && jump->is_using == 0 && !(jump->cooldown > 0.0f)) {
            DeleteJumpConnection(jump);
        } else {
            const u32 colour = jump->colour;
            if (colour != jump->base_colour) {
                jump->base_colour_components.x = SeekLinearF(
                    jump->base_colour_components.x, static_cast<f32>(colour & 0xff), FRAMETIME * testColourSecF);
                jump->base_colour_components.y = SeekLinearF(
                    jump->base_colour_components.y, static_cast<f32>((colour >> 8) & 0xff), FRAMETIME * testColourSecF);
                jump->base_colour_components.z =
                    SeekLinearF(jump->base_colour_components.z, static_cast<f32>((colour >> 16) & 0xff),
                                FRAMETIME * testColourSecF);
                jump->streak_alpha = SeekLinearF(jump->streak_alpha, static_cast<f32>(static_cast<i32>(colour >> 24)),
                                                 FRAMETIME * testColourSecF);
                jump->base_colour =
                    (static_cast<u32>(static_cast<i32>(jump->base_colour_components.x)) & 0xff) |
                    ((static_cast<u32>(static_cast<i32>(jump->base_colour_components.y)) << 8) & 0xffff) |
                    ((static_cast<u32>(static_cast<i32>(jump->base_colour_components.z)) & 0xff) << 16) |
                    (static_cast<u32>(static_cast<i32>(jump->streak_alpha)) << 24);
            }
        }
        jump = next;
    }
}

void MechAutoJumpManager::Process() {
    streak_time += FRAMETIME;
    bool create_streak = false;
    if (streak_time >= generateNewStreakTime) {
        streak_time -= generateNewStreakTime;
        create_streak = true;
    }

    PreProcessJumpConnections();

    AIPATHSYS *paths = ai_sys != NULL ? ai_sys->path_sys : NULL;
    if (paths == NULL) {
        ProcessJumpConnections();
        return;
    }

    if (TouchHacks::TouchControlsActive) {
        AIPATH *path = paths->active_path;
        if (path != NULL) {
            show_autojump_hint = 0;
            for (i32 index = 0; index < path->connection_count; ++index) {
                AIPATHCNX *connection = &path->connections[index];
                for (i32 direction = 0; direction < 2; ++direction) {
                    i32 far_index = direction;
                    u32 flags = connection->traversal_flags[far_index];
                    if ((flags & (static_cast<u32>(LEGO_AIPATHCNX_DONT_JUMP_NOW) | 0x98000000u)) != 0 ||
                        (flags & static_cast<u32>(mechAutoJumpFlags)) == 0) {
                        continue;
                    }
                    AIPATHNODE *far_node = &path->nodes[connection->node_indices[far_index]];
                    if (NuCameraClipTestSphere(&far_node->position, testStreakClipTestRadius, &numtx_identity) != 0) {
                        continue;
                    }

                    u32 checks = DoSomeChecks(*player, *path, *connection, direction);
                    if (checks == 0) {
                        continue;
                    }
                    u32 colour = static_cast<u32>(static_cast<u8>(testStreakAlpha)) << 24;
                    bool allow_streak;
                    if ((checks & 4) != 0) {
                        colour |= 0xff00;
                        allow_streak = true;
                        if (show_autojump_hint == 0) {
                            show_autojump_hint = id_HINT_LSW_AUTOJUMP;
                        }
                    } else {
                        colour |= 0x808080;
                        allow_streak = false;
                        show_autojump_hint = id_HINT_LSW_AUTOJUMP_FAIL;
                    }
                    if ((checks & 2) == 0) {
                        continue;
                    }
                    bool use_path_direction = (checks & 1) != 0;
                    MechAutoJumpConnection *jump =
                        AddAutoJumpConnection(path, connection, direction, use_path_direction, colour, allow_streak);
                    if (use_path_direction && create_streak && jump != NULL && jump->cooldown <= 0.0f) {
                        AutoJumpStreakLink *new_streak = new AutoJumpStreakLink;
                        new_streak->link.prev = NULL;
                        new_streak->link.next = NULL;
                        new_streak->path = path;
                        new_streak->connection = jump;
                        new_streak->path_connection = connection;
                        new_streak->direction = direction;
                        new_streak->handle = NULL;
                        new_streak->reserved = NULL;
                        new_streak->colour = colour;
                        new_streak->elapsed = 0.0f;
                        NuLinkedListInsert(&streaks, reinterpret_cast<NULISTLNK *>(new_streak));
                    }
                }
            }
        }
    }

    AutoJumpStreakLink *streak = reinterpret_cast<AutoJumpStreakLink *>(NuLinkedListGetHead(&streaks));
    while (streak != NULL) {
        AutoJumpStreakLink *next = reinterpret_cast<AutoJumpStreakLink *>(
            NuLinkedListGetNext(&streaks, reinterpret_cast<NULISTLNK *>(streak)));
        streak->elapsed += FRAMETIME;
        AIPATHNODE *start_node = &streak->path->nodes[streak->path_connection->node_indices[streak->direction]];
        AIPATHNODE *end_node = &streak->path->nodes[streak->path_connection->node_indices[streak->direction == 0]];
        NUVEC cross;
        f32 distance = NuVecXZDist(&end_node->position, &start_node->position, &cross);
        if (distance < testStreakMinDist) {
            distance = testStreakMinDist;
        }
        f32 progress = streak->elapsed * testStreakSpeed;
        if (progress > distance) {
            NuLinkedListRemove(&streaks, reinterpret_cast<NULISTLNK *>(streak));
            delete streak;
        } else {
            f32 t = distance != 0.0f && progress != 0.0f ? progress / distance : 0.0f;
            NUVEC centre;
            centre.x = start_node->position.x + (end_node->position.x - start_node->position.x) * t;
            centre.y = start_node->position.y + (end_node->position.y - start_node->position.y) * t;
            centre.z = start_node->position.z + (end_node->position.z - start_node->position.z) * t;
            centre.y += NuTrigTable[(static_cast<i32>(t * 32768.0f) >> 1) & 0x7fff];
            i32 facing = NuAtan2D(end_node->position.x - centre.x, end_node->position.z - centre.z);
            cross = {testStreakCrossSize, 0.0f, 0.0f};
            NuVecRotateZ(&cross, &cross, static_cast<u16>(LevelTimer.time_elapsed * testStreakRotateSpeed));
            NuVecRotateY(&cross, &cross, static_cast<u16>(facing));
            NuVecAdd(&streak->start, &centre, &cross);
            NuVecSub(&streak->end, &centre, &cross);
            if (streak->connection != NULL) {
                streak->colour = streak->connection->base_colour;
            }
        }
        streak = next;
    }

    ProcessJumpConnections();
}

void MechAutoJumpManager::Render() {
    if (!TouchHacks::TouchControlsActive) {
        return;
    }

    AutoJumpStreakLink *streak = reinterpret_cast<AutoJumpStreakLink *>(NuLinkedListGetHead(&streaks));
    while (streak != NULL) {
        AutoJumpStreakLink *next = reinterpret_cast<AutoJumpStreakLink *>(
            NuLinkedListGetNext(&streaks, reinterpret_cast<NULISTLNK *>(streak)));
        AddStreakPoints(&streak->start, testStreakTime, streak->colour, &streak->handle, 0, player);
        streak = next;
    }

    MechAutoJumpConnection *jump = reinterpret_cast<MechAutoJumpConnection *>(NuLinkedListGetHead(&jump_connections));
    while (jump != NULL) {
        if (jump->cooldown <= 0.0f) {
            AIPATHNODE *node = &jump->path->nodes[jump->connection->node_indices[jump->direction]];
            NUVEC cross;
            cross.x = testStreakCrossSize2;
            cross.y = 0.0f;
            cross.z = 0.0f;
            NuVecRotateY(&cross, &cross, static_cast<u16>(LevelTimer.time_elapsed * testStreakRotateSpeed));
            NUVEC first;
            NUVEC second;
            NuVecAdd(&first, &node->position, &cross);
            NuVecSub(&second, &node->position, &cross);
            AddStreakPoints(&first, testStreakTime, jump->base_colour, &jump->streak_handle, 0, player);
        }
        jump = reinterpret_cast<MechAutoJumpConnection *>(
            NuLinkedListGetNext(&jump_connections, reinterpret_cast<NULISTLNK *>(jump)));
    }
}

MechAutoJumpConnection *MechAutoJumpGetBest(JumpTriggerPacket const &packet, i32 heading) {
    if (!TouchHacks::TouchControlsActive || WORLD == NULL || WORLD->mech_auto_jump_manager == NULL) {
        return NULL;
    }

    MechAutoJumpConnection *current = reinterpret_cast<MechAutoJumpConnection *>(
        NuLinkedListGetHead(&WORLD->mech_auto_jump_manager->jump_connections));
    MechAutoJumpConnection *best = NULL;

    if (packet.type == 3) {
        i32 desired = NuAtan2D(packet.end.x - packet.start.x, packet.end.y - packet.start.y);
        if (current == NULL) {
            return NULL;
        }
        i32 best_difference = 0x4000;
        do {
            if (current->allow_streak && current->use_path_direction) {
                AIPATHNODE *nodes = current->path->nodes;
                AIPATHCNX *connection = current->connection;
                i32 direction = current->direction;
                NUVEC *to_position = &nodes[connection->node_indices[direction == 0]].position;
                NUVEC *from_position = &nodes[connection->node_indices[direction]].position;
                NUVEC from_screen;
                NUVEC to_screen;
                NuCameraTransformScreenClip(&from_screen, from_position, 1, NULL);
                NuCameraTransformScreenClip(&to_screen, to_position, 1, NULL);
                i32 angle = NuAtan2D(to_screen.x - from_screen.x, to_screen.y - from_screen.y);
                i32 difference = RotDiff(static_cast<u16>(angle), static_cast<u16>(desired));
                difference = difference < 0 ? -difference : difference;
                if (difference < best_difference) {
                    best_difference = difference;
                    best = current;
                }
            }
            current = reinterpret_cast<MechAutoJumpConnection *>(NuLinkedListGetNext(
                &WORLD->mech_auto_jump_manager->jump_connections, reinterpret_cast<NULISTLNK *>(current)));
        } while (current != NULL);
        return best;
    }

    i32 best_difference = 0x2aab;
    while (current != NULL) {
        if (current->allow_streak && current->use_path_direction) {
            i32 angle = current->connection->rotation;
            if (current->direction != 0) {
                angle = NuAngAdd(0x8000, angle);
            }
            i32 difference = RotDiff(static_cast<u16>(heading), static_cast<u16>(angle));
            difference = difference < 0 ? -difference : difference;
            if (difference < best_difference) {
                best_difference = difference;
                best = current;
            }
        }
        current = reinterpret_cast<MechAutoJumpConnection *>(NuLinkedListGetNext(
            &WORLD->mech_auto_jump_manager->jump_connections, reinterpret_cast<NULISTLNK *>(current)));
    }
    return best;
}

void MechAutoJumpSetIsUsing(GameObject_s &object, MechAutoJumpConnection &connection) {
    object.ai.path_info.connection = connection.connection;
    object.ai.path_info.direction = connection.direction;

    WORLDINFO *world = WorldInfo_CurrentlyActive();
    if (world != NULL && world->mech_auto_jump_manager != NULL) {
        for (MechAutoJumpConnection *current = reinterpret_cast<MechAutoJumpConnection *>(
                 NuLinkedListGetHead(&world->mech_auto_jump_manager->jump_connections));
             current != NULL;
             current = reinterpret_cast<MechAutoJumpConnection *>(NuLinkedListGetNext(
                 &world->mech_auto_jump_manager->jump_connections, reinterpret_cast<NULISTLNK *>(current)))) {
            current->is_using = 0;
            current->cooldown = 0.0f;
        }
    }
    connection.is_using = 1;
}

