#pragma once

// Episode I, Chapter 2: Invasion of Naboo. Included only by autoplay.cpp after the
// action vocabulary has been declared.

namespace scripts {

    using namespace dsl;

    const AutoplayScript gungan = {
        "Play Invasion of Naboo along its Story Mode route",
        {
            wait_loaded(),

            // Forest: lift the fallen tree, then heat the troop transport's
            // exposed engine three times as required by the story puzzle.
            wait_gizmo_output("obstacle3", GIZOBSTACLE_OUTPUT_AT_END, 1, 30000),
            rail_to_gizmo("force1", 3.5f, 20000),
            use_force_gizmo("force1"),
            rail_to_gizmo("force2", 3.5f, 20000),
            use_force_gizmo("force2"),
            use_force_gizmo("force3"),
            use_force_gizmo("force4"),

            // The authored path crosses the Jar Jar encounter trigger before
            // reaching the normal forest-to-cliffs portal.
            rail_through_gizmo("door_into_ruins11", 1.5f, 3.5f, 30000),
            wait_level("gungan_b", 30000),
            wait_time(500),

            // Cliffs: assemble the Gungan-face bridge, have Jar Jar lower the
            // first stone, then let Qui-Gon operate the revealed lift. Jar
            // Jar takes the authored high-jump route over the remaining
            // sinking stones and walks through the normal exit.
            rail_to_gizmo("force1", 3.5f, 20000),
            use_force_gizmo("force1"),
            switch_to_character("jarjarbinks"),
            rail_to_locator("JumpPrompt", 6.0f, 25000, false),
            native_jump_to_gizmo("obstacle1"),
            hold_gizmo_until_output("obstacle1", "obstacle1", GIZOBSTACLE_OUTPUT_AT_END, 1, 3.0f, 10000),
            switch_to_character("quigonjinn"),
            rail_to_gizmo("force2", 3.5f, 25000),
            use_force_gizmo("force2"),
            wait_gizmo_output("obstacle5", GIZOBSTACLE_OUTPUT_AT_END, 1, 15000),
            switch_to_character("jarjarbinks"),
            rail_through_path_connection("CNX3_A", "CNX3_B", 6.0f, 25000),
            hold_gizmo_until_output("obstacle2", "obstacle2", GIZOBSTACLE_OUTPUT_AT_END, 1, 3.0f, 10000),
            rail_through_path_connection("CNX4_A", "CNX4_B", 6.0f, 25000),
            hold_gizmo_until_output("obstacle3", "obstacle3", GIZOBSTACLE_OUTPUT_AT_END, 1, 3.0f, 10000),
            rail_through_path_connection("CNX5_A", "CNX5_B", 6.0f, 25000),
            hold_gizmo_until_output("obstacle4", "obstacle4", GIZOBSTACLE_OUTPUT_AT_END, 1, 3.0f, 10000),

            // The upper blocks are part of the Story route, not a detour.
            // A Jedi opens the final ledge, then Jar Jar takes the vertical
            // authored path through the exit.
            switch_to_character("quigonjinn"),
            rail_to_gizmo("force4", 6.0f, 25000),
            use_force_gizmo("force4"),
            switch_to_character("jarjarbinks"),
            rail_through_path_connection("TO_PLAT", "FROM_PLAT", 6.0f, 25000),
            rail_through_gizmo("door_into_ruins21", 1.5f, 6.0f, 30000),
            wait_level("gungan_c", 30000),
            wait_time(500),

            // Plateau: lower the entrance ramp and let its linked log bridge
            // move into place. Cross that exact authored connection, after
            // which Jar Jar can follow the normal high route to the dark exit
            // stone.
            switch_to_character("quigonjinn"),
            rail_to_gizmo("force1", 3.5f, 20000),
            use_force_gizmo("force1"),
            wait_gizmo_output("obstacle1", GIZOBSTACLE_OUTPUT_AT_END, 1, 10000),
            wait_time(1000),
            rail_through_path_connection("LOG_FROM", "LOG_TO", 6.0f, 20000),
            switch_to_character("jarjarbinks"),
            rail_to_gizmo("obstacle2", 6.0f, 30000),
            native_jump_to_gizmo("obstacle2"),
            hold_gizmo_until_output("obstacle2", "obstacle2", GIZOBSTACLE_OUTPUT_AT_END, 1, 3.0f, 10000),
            rail_through_gizmo("door_swamp1", 1.5f, 6.0f, 30000),

            // Lake: the underwater door is the native Story Mode level-end
            // trigger.  Crossing only succeeds when its cutscene/transition
            // actually starts; merely reaching the far side is a failure.
            wait_level("gungan_e", 30000),
            wait_time(500),
            rail_through_gizmo("door_under_water1", 1.5f, 6.0f, 30000),
        },
    };

} // namespace scripts
