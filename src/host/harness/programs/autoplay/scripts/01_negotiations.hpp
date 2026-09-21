#pragma once

// Episode I, Chapter 1: Negotiations. Included only by autoplay.cpp after the action
// vocabulary has been declared.

namespace scripts {

    using namespace dsl;

    const AutoplayScript negotiations = {
        "Play Negotiations from its opening cutscene through the native level exit",
        {
            wait_loaded(),
            wait_cutscene_end("negotiations_a", 30000),
            wait_time(1000),

            rail_to_gizmo("force1"),
            use_force_gizmo("force1"),
            rail_to_character("tc14"),
            switch_to_character("tc14"),
            rail_to_gizmo("panel4"),
            use_panel_gizmo("panel4"),
            wait_gizmo_output("obstacle40", GIZOBSTACLE_OUTPUT_AT_END, 1, 15000),

            switch_to_character("quigonjinn"),
            rail_to_gizmo("force28"),
            use_force_gizmo("force28"),
            wait_gizmo_output("obstacle42", GIZOBSTACLE_OUTPUT_AT_END, 1, 15000),
            rail_to_gizmo("buildit1"),
            use_buildit_gizmo("buildit1"),
            switch_to_character("tc14"),
            rail_to_gizmo("panel5"),
            use_panel_gizmo("panel5"),
            wait_gizmo_output("obstacle41", GIZOBSTACLE_OUTPUT_AT_END, 1, 15000),

            switch_to_character("quigonjinn"),
            rail_to_gizmo("force70"),
            use_force_gizmo("force70"),
            rail_through_gizmo("force70", 0.6f, 3.0f, 10000),
            wait_time(3000),
            wait_level("negotiations_c", 30000),
            rail_forward(3.5f),
            wait_time(250),

            // Let the two droidekas use the initially open gate, then wait
            // until their CLOSEGATE sequence has armed the player puzzle.
            rail_to_area("START_DROIDEKA", 5.0f, 15000, false),
            wait_time(1000),
            wait_gizmo_output("obstacle8", GIZOBSTACLE_OUTPUT_AT_END, 1, 20000),
            wait_time(3000),

            // The hangar's authored route uses the left Force step to get
            // over the active shield.  Holding the first pad asks the party
            // AI to help with the matching second pad; together they lower
            // obstacle8, exactly as wired in negotiations_c.git.
            rail_to_path_node("LEFT_STEP_FROM", 6.0f, 20000, false),
            rail_to_gizmo("force15", 5.0f, 15000, false),
            use_force_gizmo("force15"),
            rail_through_path_connection("LEFT_STEP_FROM", "LEFT_STEP_TO", 6.0f, 20000, false),
            rail_to_gizmo("obstacle1", 5.0f, 15000, false),
            native_jump_to_gizmo("obstacle1"),
            wait_gizmo_output("obstacle1", GIZOBSTACLE_OUTPUT_PROXIMITY, 1, 5000),
            hold_gizmo_until_output("obstacle1", "obstacle2", GIZOBSTACLE_OUTPUT_PROXIMITY, 1),
            wait_gizmo_output("obstacle8", GIZOBSTACLE_OUTPUT_AT_END, 1, 15000),

            // panel2 is TC-14's MTT door panel; obstacle3 is the door it
            // drives.  Finish through the authored door_mtt1 level portal.
            switch_to_character("tc14"),
            rail_through_path_connection("GATE_FROM", "GATE_TO", 6.0f, 20000, false),
            rail_to_gizmo("panel2", 6.0f, 20000, false),
            use_panel_gizmo("panel2"),
            wait_gizmo_output("obstacle3", GIZOBSTACLE_OUTPUT_AT_END, 1, 15000),
            switch_to_character("quigonjinn"),
            rail_through_gizmo("door_mtt11", 0.8f, 4.0f, 20000, false),
        },
    };

} // namespace scripts
