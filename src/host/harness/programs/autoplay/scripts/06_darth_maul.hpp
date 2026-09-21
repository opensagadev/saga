#pragma once

// Episode I, Chapter 6: Darth Maul. Included only by autoplay.cpp after the
// action vocabulary has been declared.

namespace scripts {

    using namespace dsl;

    const AutoplayScript darth_maul = {
        "Play Darth Maul along its Story Mode route",
        {
            checkpoint("maul_a"),
            wait_loaded(),
            wait_time(500),
            switch_to_character("QuiGonJinn"),

            // Hangar: fight each scripted droid wave and Maul phase, then
            // raise and cross the authored bridge after he retreats.
            wait_time(1000),
            clear_named_characters("BattleDroid", 32.0f, 1000, 30000),
            damage_character_to_health("DarthMaul", 2, 8.0f, 30000),
            wait_ai_message("Hits", 2),
            wait_time(1000),
            clear_named_characters("BattleDroid", 32.0f, 1000, 30000),
            damage_character_to_health("DarthMaul", 1, 8.0f, 30000),
            wait_ai_message("Hits", 1),
            wait_time(1000),
            clear_named_characters("BattleDroid", 32.0f, 1000, 30000),
            damage_character_to_health("DarthMaul", 0, 8.0f, 30000),
            wait_ai_message("Hits", 0),
            wait_ai_message("MaulOnTheRun", 1),
            wait_ai_message("Bridge", 1),
            wait_gizmo_output("obstacle1", GIZOBSTACLE_OUTPUT_AT_END, 1, 30000),
            wait_gizmo_output("obstacle2", GIZOBSTACLE_OUTPUT_AT_END, 1, 30000),
            rail_to_gizmo("force2", 6.0f, 30000),
            use_force_gizmo("force2"),
            rail_through_path_connection("BRIDGE_A", "BRIDGE_B", 6.0f, 20000),
            rail_through_path_connection("BRIDGE_B", "BRIDGE_C", 6.0f, 20000),
            rail_through_path_connection("BRIDGE_C", "BRIDGE_D", 6.0f, 20000),
            rail_through_path_connection("BRIDGE_D", "BRIDGE_E", 6.0f, 20000),
            rail_through_gizmo("door_21", 1.5f, 6.0f, 30000),
            wait_level("maul_b", 30000),

            checkpoint("maul_b"),
            wait_loaded(),
            wait_time(500),

            // Chase Maul over the authored platforms. Following his named
            // locators exercises the jumps and raises each chase trigger.
            rail_to_locator("MAUL1", 6.0f, 30000),
            native_jump_to_locator("MAUL2", 15000),
            rail_to_locator("MAUL3", 6.0f, 30000),
            native_jump_to_locator("MAUL4", 15000),
            rail_to_locator("MAUL5", 6.0f, 30000),
            native_jump_to_locator("MAUL6", 15000),
            native_jump_to_locator("MAUL7", 15000),
            native_jump_to_locator("MAUL8", 15000),
            rail_to_locator("MAUL9", 6.0f, 30000),
            rail_through_locator("MAUL10", 6.0f, 30000),
            wait_level("maul_d", 30000),

            checkpoint("maul_d"),
            wait_loaded(),
            wait_time(500),

            // Central chamber: entering FLEE starts the commander/droid
            // ambush. Clear every wave, then keep both Jedi on the paired
            // floor switches until the path to the laser corridor opens.
            rail_to_area("FLEE", 6.0f, 30000),
            wait_time(4500),
            clear_nearby_hostiles(48.0f, 2500, 90000),
            hold_party_switches_until_output({{"QuiGonJinn", "obstacle10"}, {"ObiWanKenobi", "obstacle12"}},
                                             "obstacle8", GIZOBSTACLE_OUTPUT_AT_END, 1, 30000),
            rail_through_gizmo("door_181", 1.5f, 6.0f, 30000),
            wait_level("maul_e", 30000),

            checkpoint("maul_e"),
            wait_loaded(),
            wait_time(500),

            // Laser corridor: pull both levers belonging to each of the four
            // barriers. Completed partners are harmless no-ops, so this also
            // remains deterministic if the party AI finishes one first.
            switch_to_character("QuiGonJinn"),
            rail_to_gizmo("force1", 6.0f, 30000),
            switch_to_character("ObiWanKenobi"),
            rail_to_gizmo("force2", 6.0f, 30000),
            hold_party_forces_until_output({{"QuiGonJinn", "force1"}, {"ObiWanKenobi", "force2"}}, "obstacle8",
                                           GIZOBSTACLE_OUTPUT_AT_END, 1, 20000),
            wait_time(1000),
            switch_to_character("QuiGonJinn"),
            rail_to_gizmo("force9", 6.0f, 30000),
            switch_to_character("ObiWanKenobi"),
            rail_to_gizmo("force10", 6.0f, 30000),
            hold_party_forces_until_output({{"QuiGonJinn", "force9"}, {"ObiWanKenobi", "force10"}}, "obstacle5",
                                           GIZOBSTACLE_OUTPUT_AT_END, 1, 20000),
            wait_time(1000),
            switch_to_character("QuiGonJinn"),
            rail_to_gizmo("force11", 6.0f, 30000),
            switch_to_character("ObiWanKenobi"),
            rail_to_gizmo("force12", 6.0f, 30000),
            hold_party_forces_until_output({{"QuiGonJinn", "force11"}, {"ObiWanKenobi", "force12"}}, "obstacle6",
                                           GIZOBSTACLE_OUTPUT_AT_END, 1, 20000),
            wait_time(1000),
            switch_to_character("QuiGonJinn"),
            rail_to_gizmo("force13", 6.0f, 30000),
            switch_to_character("ObiWanKenobi"),
            rail_to_gizmo("force14", 6.0f, 30000),
            hold_party_forces_until_output({{"QuiGonJinn", "force13"}, {"ObiWanKenobi", "force14"}}, "obstacle7",
                                           GIZOBSTACLE_OUTPUT_AT_END, 1, 20000),
            wait_time(1000),
            rail_through_gizmo("door_81", 1.5f, 6.0f, 30000),
            wait_level("maul_f", 30000),

            checkpoint("maul_f"),
            wait_loaded(),
            wait_time(500),

            // Duel: fight Maul to his platform phase, lower all three
            // authored platforms with the Force, then finish the last phase.
            rail_to_character("DarthMaul", 2.5f, 6.0f, 30000),
            damage_character_to_health("DarthMaul", 4, 3.5f, 60000),
            wait_time(1500),
            rail_to_gizmo("force2", 6.0f, 30000),
            use_force_gizmo("force2"),
            rail_to_gizmo("force6", 6.0f, 30000),
            use_force_gizmo("force6"),
            rail_to_gizmo("force1", 6.0f, 30000),
            use_force_gizmo("force1"),
            wait_time(1500),
            rail_to_character("DarthMaul", 2.5f, 6.0f, 30000),
            damage_character_to_health("DarthMaul", 0, 3.5f, 60000),
            wait_time(4000),
        },
        AutoplayOptions{},
        720000,
    };

} // namespace scripts
