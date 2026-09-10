#include "MechInputTouch_types.h"

namespace {
    struct AutoJumpStreakLink {
        NULISTLNK link;
        MechAutoJumpConnection *connection;
    };
} // namespace

void MechAutoJumpManager::AddAutoJumpConnection(AIPATH_s *path, AIPATHCNX_s *connection, i32 direction,
                                                bool use_path_direction, i32 colour, bool allow_streak) {
    if (connection != NULL && path != NULL) {
        MechAutoJumpConnection *jump =
            reinterpret_cast<MechAutoJumpConnection *>(NuLinkedListGetHead(&jump_connections));
        while (jump != NULL && (jump->path != path || jump->connection != connection || jump->direction != direction)) {
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
            jump->base_colour_components.x = colour & 0xff;
            jump->base_colour_components.y = (colour >> 8) & 0xff;
            jump->streak_colour_components.y = 0.0f;
            jump->using_object = NULL;
            jump->streak_colour_components.x = 0.0f;
            jump->streak_colour_components.z = 0.0f;
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
    }
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

void MechAutoJumpManager::Init() {
}

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

void MechAutoJumpManager::PreProcessJumpConnections() {
}

void MechAutoJumpManager::Process() {
}

void MechAutoJumpManager::ProcessJumpConnections() {
}

void MechAutoJumpManager::Render() {
}

MechAutoJumpManager::~MechAutoJumpManager() {
    DeleteJumpConnectionsAndStreaks();
}
