#pragma once

// Queries that translate stable level-file names into live game objects and
// their authored interaction positions.

GIZMO *find_gizmo(const char *name) {
    if (name == nullptr || WORLD == nullptr || WORLD->gizmo_sys == nullptr) {
        return nullptr;
    }
    return GizmoFindByName(WORLD->gizmo_sys, -1, const_cast<char *>(name));
}

bool force_gizmo_usable_by(GIZFORCE_s &force, GameObject_s &character) {
    const u8 required_flags = GIZFORCE_PROGRESS_ENABLED | GIZFORCE_PROGRESS_VISIBLE;
    const f32 distance_squared = NuVecDistSqr(&force.position, &character.apiobj.collision_position, nullptr);
    return (force.progress_flags & required_flags) == required_flags &&
           distance_squared <= force.interaction_radius * force.interaction_radius &&
           GizForce_StoodOnForce(&force, &character) == 0;
}

void log_loaded_gizmos() {
    if (WORLD == nullptr || WORLD->gizmo_sys == nullptr || gizmotypes == nullptr) {
        return;
    }
    for (i32 type_index = 0; type_index < gizmotypes->count; ++type_index) {
        GIZMOSET &set = WORLD->gizmo_sys->sets[type_index];
        for (i32 gizmo_index = 0; gizmo_index < set.count; ++gizmo_index) {
            GIZMO *gizmo = &set.gizmos[gizmo_index];
            const char *name = GizmoGetName(gizmo);
            const NUVEC *position = GizmoGetPos(WORLD->gizmo_sys, gizmo);
            const char *type_name = gizmotypes->types[type_index].name;
            if (name != nullptr && position != nullptr) {
                LOG_INFO("autoplay-inspect: type=%s name=%s position=(%.3f,%.3f,%.3f)", type_name, name, position->x,
                         position->y, position->z);
            } else if (name != nullptr) {
                LOG_INFO("autoplay-inspect: type=%s name=%s position=<none>", type_name, name);
            }
            if (gizmo->object != nullptr && SDL_strcasecmp(type_name, "GizObstacle") == 0) {
                const GIZOBSTACLE_s &obstacle = *static_cast<GIZOBSTACLE_s *>(gizmo->object);
                const i32 animation_state = obstacle.anim_set != nullptr ? obstacle.anim_set->state : -1;
                const unsigned animation_flags =
                    obstacle.anim_set != nullptr ? static_cast<unsigned>(obstacle.anim_set->flags) : 0;
                LOG_INFO("autoplay-inspect:   trigger=(%.3f,%.3f,%.3f) radius=%.3f box=(%.3f,%.3f,%.3f) "
                         "mode=%u trigger-mode=%u config=0x%x state=%u progress=0x%x control=0x%x runtime=0x%x "
                         "animation=(state=%d flags=0x%x) outputs=(end=%d not-start=%d proximity=%d start=%d "
                         "forward=%d)",
                         obstacle.secondary_position.x, obstacle.secondary_position.y, obstacle.secondary_position.z,
                         obstacle.trigger_radius, obstacle.trigger_box_half_extents.x,
                         obstacle.trigger_box_half_extents.y, obstacle.trigger_box_half_extents.z,
                         static_cast<unsigned>(obstacle.mode), static_cast<unsigned>(obstacle.trigger_mode),
                         obstacle.config_flags, static_cast<unsigned>(obstacle.state),
                         static_cast<unsigned>(obstacle.progress_flags), static_cast<unsigned>(obstacle.control_flags),
                         static_cast<unsigned>(obstacle.runtime_flags), animation_state, animation_flags,
                         GizmoGetOutput(WORLD->gizmo_sys, gizmo, GIZOBSTACLE_OUTPUT_AT_END, 1),
                         GizmoGetOutput(WORLD->gizmo_sys, gizmo, GIZOBSTACLE_OUTPUT_NOT_AT_START, 1),
                         GizmoGetOutput(WORLD->gizmo_sys, gizmo, GIZOBSTACLE_OUTPUT_PROXIMITY, 1),
                         GizmoGetOutput(WORLD->gizmo_sys, gizmo, GIZOBSTACLE_OUTPUT_AT_START, 1),
                         GizmoGetOutput(WORLD->gizmo_sys, gizmo, GIZOBSTACLE_OUTPUT_PLAYING_FORWARD, 1));
            } else if (gizmo->object != nullptr && SDL_strcasecmp(type_name, "GizForce") == 0) {
                GIZFORCE_s &force = *static_cast<GIZFORCE_s *>(gizmo->object);
                LOG_INFO("autoplay-inspect:   force-position=(%.3f,%.3f,%.3f) radius=%.3f progress=0x%x "
                         "runtime=0x%x state=0x%x config=0x%x animation-state=%d complete=%d",
                         force.position.x, force.position.y, force.position.z, force.interaction_radius,
                         static_cast<unsigned>(force.progress_flags), static_cast<unsigned>(force.runtime_flags),
                         static_cast<unsigned>(force.state_flags), force.config_flags,
                         force.anim_set != nullptr ? force.anim_set->state : -1, GizForce_Complete(&force));
            } else if (gizmo->object != nullptr && SDL_strcasecmp(type_name, "Panel") == 0) {
                GIZPANEL_s &panel = *static_cast<GIZPANEL_s *>(gizmo->object);
                NUVEC use_position{};
                GizPanel_GetAbsPlayerPos(&panel, &use_position);
                LOG_INFO("autoplay-inspect:   panel-position=(%.3f,%.3f,%.3f) use=(%.3f,%.3f,%.3f) variant=%u "
                         "flags=0x%x",
                         panel.position.x, panel.position.y, panel.position.z, use_position.x, use_position.y,
                         use_position.z, static_cast<unsigned>(panel.model_variant),
                         static_cast<unsigned>(panel.flags));
            } else if (gizmo->object != nullptr && SDL_strcasecmp(type_name, "Door") == 0) {
                const DOOR_s &door = *static_cast<DOOR_s *>(gizmo->object);
                LOG_INFO("autoplay-inspect:   door-position=(%.3f,%.3f,%.3f) normal=(%.3f,%.3f,%.3f) "
                         "radius=%.3f active=%u",
                         door.pos.x, door.pos.y, door.pos.z, door.normal.x, door.normal.y, door.normal.z, door.radius,
                         static_cast<unsigned>(door.active));
            } else if (gizmo->object != nullptr && SDL_strcasecmp(type_name, "ZipUp") == 0) {
                const ZIPUP &zipup = *static_cast<ZIPUP *>(gizmo->object);
                LOG_INFO("autoplay-inspect:   zip-up lower=(%.3f,%.3f,%.3f) upper=(%.3f,%.3f,%.3f) "
                         "hook=(%.3f,%.3f,%.3f) flags=0x%x runtime=0x%x",
                         zipup.lower_position.x, zipup.lower_position.y, zipup.lower_position.z, zipup.upper_position.x,
                         zipup.upper_position.y, zipup.upper_position.z, zipup.hook_origin.x, zipup.hook_origin.y,
                         zipup.hook_origin.z, static_cast<unsigned>(zipup.flags),
                         static_cast<unsigned>(zipup.runtime_flags));
            }
        }
    }
    if (Player[0] != nullptr) {
        LOG_INFO("autoplay-inspect: player id=%d position=(%.3f,%.3f,%.3f) lower=(%.3f,%.3f,%.3f) "
                 "collision=(%.3f,%.3f,%.3f) flags=0x%x room=%d",
                 Player[0]->id, Player[0]->apiobj.position.x, Player[0]->apiobj.position.y,
                 Player[0]->apiobj.position.z, Player[0]->apiobj.lower_position.x, Player[0]->apiobj.lower_position.y,
                 Player[0]->apiobj.lower_position.z, Player[0]->apiobj.collision_position.x,
                 Player[0]->apiobj.collision_position.y, Player[0]->apiobj.collision_position.z,
                 Player[0]->apiobj.field_0x1f8, static_cast<i32>(Player[0]->room_id));
    }
}

GameObject_s *find_character(const char *name) {
    if (name == nullptr || Obj == nullptr || Player[0] == nullptr) {
        return nullptr;
    }
    const i32 character_id = CharIDFromName(const_cast<char *>(name));
    if (character_id < 0) {
        return nullptr;
    }
    GameObject_s *nearest = nullptr;
    f32 nearest_distance = std::numeric_limits<f32>::max();
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
        GameObject_s *candidate = &Obj[index];
        if (candidate->id != character_id || (candidate->apiobj.field_0x1f8 & 0x1001) != 0x1001 ||
            candidate->apiobj.field_0x287 != 0) {
            continue;
        }
        const f32 dx = Player[0]->apiobj.position.x - candidate->apiobj.position.x;
        const f32 dz = Player[0]->apiobj.position.z - candidate->apiobj.position.z;
        const f32 distance = dx * dx + dz * dz;
        if (distance < nearest_distance) {
            nearest = candidate;
            nearest_distance = distance;
        }
    }
    return nearest;
}

std::optional<NUVEC> valid_position(NUVEC position) {
    if (!std::isfinite(position.x) || !std::isfinite(position.y) || !std::isfinite(position.z)) {
        return std::nullopt;
    }
    return position;
}

std::optional<NUVEC> gizmo_use_position(GIZMO *gizmo) {
    if (gizmo == nullptr || WORLD == nullptr) {
        return std::nullopt;
    }
    NUVEC position{};
    NUVEC *gizmo_position = GizmoGetPos(WORLD->gizmo_sys, gizmo);
    if (gizmo_position != nullptr) {
        LOG_INFO("autoplay: gizmo %s position=(%.3f,%.3f,%.3f)", GizmoGetName(gizmo), gizmo_position->x,
                 gizmo_position->y, gizmo_position->z);
    }
    if (gizmo->type_id == obstacle_gizmotype_id && gizmo->object != nullptr) {
        const GIZOBSTACLE_s &obstacle = *static_cast<GIZOBSTACLE_s *>(gizmo->object);
        position = obstacle.secondary_position;
        LOG_INFO("autoplay: gizmo %s trigger=(%.3f,%.3f,%.3f) radius=%.3f box=(%.3f,%.3f,%.3f) yaw=%d mode=%u",
                 GizmoGetName(gizmo), position.x, position.y, position.z, obstacle.trigger_radius,
                 obstacle.trigger_box_half_extents.x, obstacle.trigger_box_half_extents.y,
                 obstacle.trigger_box_half_extents.z, static_cast<i32>(obstacle.trigger_box_yaw),
                 static_cast<unsigned>(obstacle.mode));
        return valid_position(position);
    }
    if (gizmo->type_id == gizpanel_gizmotype_id && gizmo->object != nullptr) {
        GIZPANEL_s *panel = static_cast<GIZPANEL_s *>(gizmo->object);
        GizPanel_GetAbsPlayerPos(panel, &position);
        LOG_INFO("autoplay: panel %s player position=(%.3f,%.3f,%.3f)", GizmoGetName(gizmo), position.x, position.y,
                 position.z);
        return valid_position(position);
    }
    if (gizmo->type_id == gizbuildit_gizmotype_id && gizmo->object != nullptr) {
        const GIZBUILDIT_s &buildit = *static_cast<GIZBUILDIT_s *>(gizmo->object);
        position = buildit.start_position;
        LOG_INFO("autoplay: build-it %s interaction position=(%.3f,%.3f,%.3f)", GizmoGetName(gizmo), position.x,
                 position.y, position.z);
        return valid_position(position);
    }
    if (gizmo->type_id == blowup_gizmotype_id && gizmo->object != nullptr && gizmo_position != nullptr) {
        position = *gizmo_position;
        if (Player[0] != nullptr) {
            const f32 dx = Player[0]->apiobj.position.x - position.x;
            const f32 dz = Player[0]->apiobj.position.z - position.z;
            const f32 distance = std::sqrt(dx * dx + dz * dz);
            if (distance > 0.001f) {
                position.x += dx * (2.0f / distance);
                position.z += dz * (2.0f / distance);
            }
        }
        LOG_INFO("autoplay: blowup %s firing position=(%.3f,%.3f,%.3f)", GizmoGetName(gizmo), position.x, position.y,
                 position.z);
        return valid_position(position);
    }
    if (gizmo->object != nullptr && gizmotypes != nullptr &&
        SDL_strcasecmp(gizmotypes->types[gizmo->type_id].name, "Door") == 0) {
        position = static_cast<DOOR_s *>(gizmo->object)->pos;
        LOG_INFO("autoplay: door %s crossing position=(%.3f,%.3f,%.3f)", GizmoGetName(gizmo), position.x, position.y,
                 position.z);
        return valid_position(position);
    }
    if (gizmo->object != nullptr && gizmotypes != nullptr &&
        SDL_strcasecmp(gizmotypes->types[gizmo->type_id].name, "ZipUp") == 0) {
        const ZIPUP &zipup = *static_cast<ZIPUP *>(gizmo->object);
        position = zipup.lower_position;
        LOG_INFO("autoplay: zip-up %s lower endpoint=(%.3f,%.3f,%.3f)", GizmoGetName(gizmo), position.x, position.y,
                 position.z);
        return valid_position(position);
    }
    if (gizmo->type_id == force_gizmotype_id && gizmo->object != nullptr && gizmo_position != nullptr) {
        const GIZFORCE_s &force = *static_cast<GIZFORCE_s *>(gizmo->object);
        if (WORLD->ai_trigger_set_sys != nullptr) {
            for (i32 set_index = 0; set_index < 32; ++set_index) {
                AITRIGGERSET_s &set = WORLD->ai_trigger_set_sys->sets[set_index];
                if ((set.flags & 1) == 0) {
                    continue;
                }
                for (i32 trigger_index = 0; trigger_index < set.trigger_count; ++trigger_index) {
                    if (set.gizmos[trigger_index] == gizmo) {
                        position = set.targets[trigger_index].position;
                        LOG_INFO("autoplay: Force gizmo %s authored interaction locator=(%.3f,%.3f,%.3f)",
                                 GizmoGetName(gizmo), position.x, position.y, position.z);
                        return valid_position(position);
                    }
                }
            }
        }
        position = *gizmo_position;
        f32 direction_x = 0.0f;
        f32 direction_z = 1.0f;
        if (Player[0] != nullptr) {
            direction_x = Player[0]->apiobj.position.x - position.x;
            direction_z = Player[0]->apiobj.position.z - position.z;
            const f32 length = std::sqrt(direction_x * direction_x + direction_z * direction_z);
            if (length > 0.001f) {
                direction_x /= length;
                direction_z /= length;
            } else {
                direction_x = 0.0f;
                direction_z = 1.0f;
            }
        }
        // Stay clear of Force objects which also provide terrain: standing on
        // their collision makes the native targeting code deliberately reject
        // them even though the character is within the nominal radius.
        const f32 interaction_offset = std::fmax(0.75f, std::fmin(2.0f, force.interaction_radius * 0.65f));
        position.x += direction_x * interaction_offset;
        position.z += direction_z * interaction_offset;
        LOG_INFO("autoplay: Force gizmo %s interaction position=(%.3f,%.3f,%.3f), radius=%.3f", GizmoGetName(gizmo),
                 position.x, position.y, position.z, force.interaction_radius);
        return valid_position(position);
    }
    if (WORLD->ai_trigger_set_sys != nullptr) {
        for (i32 set_index = 0; set_index < 32; ++set_index) {
            AITRIGGERSET_s &set = WORLD->ai_trigger_set_sys->sets[set_index];
            if ((set.flags & 1) == 0) {
                continue;
            }
            for (i32 trigger_index = 0; trigger_index < set.trigger_count; ++trigger_index) {
                if (set.gizmos[trigger_index] == gizmo) {
                    position = set.targets[trigger_index].position;
                    LOG_INFO("autoplay: gizmo %s AI interaction locator=(%.3f,%.3f,%.3f)", GizmoGetName(gizmo),
                             position.x, position.y, position.z);
                    return valid_position(position);
                }
            }
        }
    }
    if (gizmo_position == nullptr) {
        return std::nullopt;
    }
    position = *gizmo_position;
    return valid_position(position);
}
