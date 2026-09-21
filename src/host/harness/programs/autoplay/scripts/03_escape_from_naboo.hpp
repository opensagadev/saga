#pragma once

// Episode I, Chapter 3: Escape from Naboo. Included only by autoplay.cpp after
// the action vocabulary has been declared.

namespace scripts {

    using namespace dsl;

    const AutoplayScript escape_from_naboo = {
        "Play Escape from Naboo along its Story Mode route",
        {
            wait_loaded(),

            // Courtyard rooftops: use the Queen's actual grapple transitions.
            // Two destructible anchors reveal the otherwise inactive third
            // and fourth grapples, matching the authored gizmo-flow chains.
            rail_to_gizmo("zipup1", 3.0f, 20000, true),
            use_zipup_gizmo("zipup1"),
            rail_to_gizmo("zipup2", 3.0f, 20000, true),
            use_zipup_gizmo("zipup2"),
            rail_to_gizmo("f_tub_by21", 3.0f, 20000, true),
            blaster_hit_gizmo("f_tub_by21"),
            rail_to_gizmo("zipup3", 3.0f, 20000, true),
            use_zipup_gizmo("zipup3"),
            rail_to_gizmo("pod21", 3.0f, 20000, true),
            blaster_hit_gizmo("pod21"),
            rail_to_gizmo("zipup4", 3.0f, 20000, true),
            use_zipup_gizmo("zipup4"),
            rail_through_gizmo("RescueA_RescueB1", 1.0f, 3.0f, 30000, true),
            wait_level("rescue_b", 30000),
            wait_time(500),

            // Semicircular roof: hit the named target across the bridge and
            // cross the gate it drives into the palace approach.
            rail_to_gizmo("target_a11", 3.5f, 30000, true),
            blaster_hit_gizmo("target_a11"),
            wait_gizmo_output("obstacle1", GIZOBSTACLE_OUTPUT_AT_END, 1, 10000),
            rail_through_gizmo("RescueB_RescueC1", 1.0f, 3.0f, 30000, true),
            wait_level("rescue_c", 30000),
            wait_time(500),

            // Palace gate: all four Story Mode targets are distinct blowups
            // feeding ExitGate. Approach and hit each exact authored object.
            rail_to_gizmo("target_a31", 3.5f, 30000, true),
            blaster_hit_gizmo("target_a31"),
            rail_to_gizmo("target_a51", 3.5f, 15000, true),
            blaster_hit_gizmo("target_a51"),
            rail_to_gizmo("target_a41", 3.5f, 15000, true),
            blaster_hit_gizmo("target_a41"),
            rail_to_gizmo("target_a61", 3.5f, 15000, true),
            blaster_hit_gizmo("target_a61"),
            wait_gizmo_output("obstacle3", GIZOBSTACLE_OUTPUT_AT_END, 1, 15000),
            rail_through_gizmo("RescueC_RescueE1", 1.0f, 3.0f, 30000, true),
            wait_level("rescue_e", 30000),
            wait_time(500),

            // Hangar roof: destroy the top-right green grille, then drop
            // through the vertical native story-exit portal below it.
            rail_to_gizmo("roof_light61", 4.0f, 30000),
            blaster_hit_gizmo("roof_light61"),
            rail_through_gizmo("poop_map1", 0.0f, 2.5f, 20000, true),
        },
        AutoplayOptions{},
        Milliseconds{180000},
    };

} // namespace scripts
