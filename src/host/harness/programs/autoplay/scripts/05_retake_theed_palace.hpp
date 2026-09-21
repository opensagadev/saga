#pragma once

// Episode I, Chapter 5: Retake Theed Palace. Included only by autoplay.cpp
// after the action vocabulary has been declared.

namespace scripts {

    using namespace dsl;

    const AutoplayScript retake_theed_palace = {
        "Play Retake Theed Palace along its Story Mode route",
        {
            checkpoint("retake_a"),
            wait_loaded(),

            // Courtyard: clear the blockade, assemble the authored Jedi
            // staircase, then have R2 open the palace approach door.
            clear_nearby_hostiles(24.0f),
            rail_to_gizmo("force20", 6.0f, 30000),
            use_force_gizmo("force20"),
            switch_to_character("R2D2"),
            rail_to_gizmo("panel1", 6.0f, 30000),
            use_panel_gizmo("panel1"),
            wait_gizmo_output("obstacle1", GIZOBSTACLE_OUTPUT_AT_END, 1, 15000),
            switch_to_character("QuiGonJinn"),
            rail_through_gizmo("retakeA_retakeB1", 1.5f, 6.0f, 30000),
            wait_level("retake_b", 30000),
            checkpoint("retake_b"),
            wait_loaded(),
            wait_time(500),

            // Palace exterior: build the Jedi crossing and Padme's grapple,
            // regroup on the six party switches, then build and use the R2
            // panel which opens the exit.
            rail_to_gizmo("force3", 6.0f, 30000),
            use_force_gizmo("force3"),
            rail_to_gizmo("force2", 6.0f, 30000),
            use_force_gizmo("force2"),
            rail_through_path_connection("PLAT1_A", "PLAT1_B", 6.0f, 30000),
            rail_through_path_connection("PLAT2_A", "PLAT2_B", 6.0f, 30000),
            clear_nearby_hostiles(24.0f),
            hold_gizmo_until_output("obstacle3", "obstacle9", GIZOBSTACLE_OUTPUT_AT_END, 1, 6.0f, 20000),
            rail_to_gizmo("platform21", 6.0f, 30000),
            blaster_hit_gizmo("platform21"),
            rail_to_gizmo("buildit1", 6.0f, 15000),
            use_buildit_gizmo("buildit1"),
            switch_to_character("R2D2"),
            rail_to_gizmo("panel1", 6.0f, 30000),
            use_panel_gizmo("panel1"),
            wait_gizmo_output("obstacle10", GIZOBSTACLE_OUTPUT_AT_END, 1, 15000),
            switch_to_character("QuiGonJinn"),
            rail_through_gizmo("retakeB_retakeD1", 1.5f, 6.0f, 30000),
            wait_level("retake_d", 30000),
            checkpoint("retake_d"),
            wait_loaded(),
            wait_time(500),

            // Plaza: create the three traversal structures, move the story
            // crates, and have Anakin use the low route to open the gate.
            clear_nearby_hostiles(28.0f),
            rail_to_gizmo("force4", 6.0f, 30000),
            use_force_gizmo("force4"),
            rail_through_path_connection("STEP1_A", "STEP1_B", 6.0f, 20000),
            rail_through_path_connection("STEP1_B", "STEP1_C", 6.0f, 20000),
            rail_through_path_connection("STEP1_C", "STEP1_D", 6.0f, 20000),
            rail_through_path_connection("STEP1_D", "STEP1_E", 6.0f, 20000),
            rail_to_gizmo("force6", 6.0f, 30000),
            use_force_gizmo("force6"),
            rail_through_path_connection("BRIDGE1_A", "BRIDGE1_B", 6.0f, 20000),
            rail_to_gizmo("force7", 6.0f, 30000),
            use_force_gizmo("force7"),
            switch_to_character("Anakin_Boy"),
            rail_through_path_connection("VENT1_A", "VENT1_B", 6.0f, 30000),
            native_jump_to_gizmo("obstacle12"),
            hold_gizmo_until_output("obstacle12", "obstacle15", GIZOBSTACLE_OUTPUT_AT_END, 1, 6.0f, 20000),
            rail_through_gizmo("retakeD_retakeE1", 1.5f, 6.0f, 30000),
            wait_level("retake_e", 30000),
            checkpoint("retake_e"),
            wait_loaded(),
            wait_time(500),

            // Balcony: raise the authored ledge, use it to reach the upper
            // route, then occupy one of the six party switches. The native
            // party script brings every other Story character to the other
            // five switches and opens the shutter.
            switch_to_character("QuiGonJinn"),
            rail_to_gizmo("force4", 6.0f, 30000),
            use_force_gizmo("force4"),
            native_jump_to_gizmo("obstacle4"),
            hold_party_switches_until_output(
                {
                    {"QuiGonJinn", "obstacle4"},
                    {"ObiWanKenobi", "obstacle5"},
                    {"PadmeBattle", "obstacle6"},
                    {"CaptainPanaka", "obstacle7"},
                    {"R2D2", "obstacle8"},
                    {"Anakin_Boy", "obstacle9"},
                },
                "obstacle10", GIZOBSTACLE_OUTPUT_AT_END, 1, 15000),
            rail_through_gizmo("retakeE_retakeF1", 1.5f, 6.0f, 30000),
            wait_level("retake_f", 30000),
            checkpoint("retake_f"),
            wait_loaded(),
            wait_time(500),

            // Palace interior: destroy the three statues and use the middle
            // floor switch to open the route into the hangar.
            switch_to_character("PadmeBattle"),
            rail_to_gizmo("platform11", 6.0f, 30000),
            blaster_hit_gizmo("platform11"),
            rail_to_gizmo("platform21", 6.0f, 30000),
            blaster_hit_gizmo("platform21"),
            rail_to_gizmo("platform31", 6.0f, 30000),
            blaster_hit_gizmo("platform31"),
            hold_party_switches_until_output({{"PadmeBattle", "obstacle17"}}, "obstacle16", GIZOBSTACLE_OUTPUT_AT_END,
                                             1, 15000),
            // Follow LevelPath nodes 15 -> 20 -> 12 -> 21. The destroyed
            // middle pedestal remains solid in the current host
            // reconstruction, so use a normal native jump to rejoin at 23.
            rail_relative_route({{0.0f, 0.0f, 0.8f}, {0.3f, 0.0f, 2.9f}, {2.1f, 0.0f, 2.9f}}, 3.0f, 15000),
            native_jump_relative({2.0f, 0.0f, 0.0f}),
            rail_through_gizmo("Palace_Hangar1", 1.5f, 3.0f, 30000),
            wait_level("retake_g", 30000),
            checkpoint("retake_g"),
            wait_loaded(),
            wait_time(500),

            // Hangar: visit every captive guard position and defeat the
            // authored captors. The level script raises the final R2 panel
            // only after all six rescue counters have completed.
            rail_to_area("Camera_Cut", 6.0f, 30000),
            wait_time(6500),
            rail_to_locator("Locator1", 6.0f, 30000),
            destroy_named_ai_object("GUARD1_A"),
            clear_nearby_hostiles(12.0f),
            rail_to_locator("Locator2", 6.0f, 30000),
            destroy_named_ai_object("GUARD2_A"),
            clear_nearby_hostiles(12.0f),
            rail_to_locator("Locator3", 6.0f, 30000),
            destroy_named_ai_object("GUARD3_A"),
            clear_nearby_hostiles(12.0f),
            rail_to_locator("Locator4", 6.0f, 30000),
            clear_nearby_hostiles(12.0f),
            rail_to_locator("Locator5", 6.0f, 30000),
            destroy_named_ai_object("GUARD4_A"),
            destroy_named_ai_object("GUARD4_B"),
            clear_nearby_hostiles(12.0f),
            rail_to_locator("Locator6", 6.0f, 30000),
            clear_nearby_hostiles(12.0f),
            wait_ai_message("GuardsToRescue", 0, 60000),
            wait_ai_message("panel_up", 1, 30000),
            switch_to_character("R2D2"),
            rail_to_gizmo("panel1", 6.0f, 30000),
            use_panel_gizmo("panel1"),
            wait_gizmo_output("obstacle3", GIZOBSTACLE_OUTPUT_AT_END, 1, 30000),
        },
        AutoplayOptions{},
        480000,
    };

} // namespace scripts
