#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/surfaces.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

f32 FRAMETIME = 0.1f;
extern "C" {
    TERRAIN_SURFACE_s TerSurface[32] = {};
}

namespace {
    rtldata_s fixture;
    GameObject_s *active_object;
    int service_calls;

    void require(bool condition, const char *message) {
        if (!condition) {
            std::fprintf(stderr, "LightGameObject regression: %s\n", message);
            std::exit(1);
        }
    }

    bool near(f32 actual, f32 expected) {
        return std::fabs(actual - expected) < 0.000001f;
    }

    void expect_vector(const NUVEC &actual, const NUVEC &expected) {
        require(near(actual.x, expected.x) && near(actual.y, expected.y) && near(actual.z, expected.z),
                "unexpected direction or ambient vector");
    }

    void prepare(GameObject_s &object, bool reset) {
        object = {};
        fixture = {};
        active_object = &object;
        service_calls = 0;
        object.apiobj.field_0x218 = 2000000.0f;
        object.field_0xefc = reset ? 0xc5 : 0x45;
        object.field_0xd6c = 0.75f;
        fixture.ambient = {2.0f, 4.0f, 6.0f};
        for (int i = 0; i < 3; ++i) {
            fixture.intensity[i] = {2.0f, 4.0f, 6.0f};
        }
    }

    void run(GameObject_s &object, bool reset) {
        LightGameObject(&object, &fixture);
        require(service_calls == 2, "RTL reset and apply must both run");
        require(object.field_0xefc == 0x45, "only the reset bit should clear");
        expect_vector(object.lighting_state.ambient, reset ? fixture.ambient : NUVEC{1.0f, 2.0f, 3.0f});
        for (int i = 0; i < 3; ++i) {
            const NUCOLOUR3 &colour = object.lighting_state.intensity[i];
            require(near(colour.r, reset ? 2.0f : 1.0f) && near(colour.g, reset ? 4.0f : 2.0f) &&
                        near(colour.b, reset ? 6.0f : 3.0f),
                    "colour reset or interpolation changed");
        }
        require(near(object.field_0xd6c, reset ? 0.0f : 0.45f), "surface fade reset or seek changed");
    }
} // namespace

// Controlled service inputs, not a simulation of RTL light selection or gameplay.
extern "C" void rtlResetEx(rtldata_s *data, i32 mode) {
    require(service_calls++ == 0 && data == &active_object->light_data && mode == 1, "unexpected RTL reset call");
    *data = {};
}

extern "C" void rtlApplySetScale(void *set, rtldata_s *data, NUVEC *position, NUMTX *matrix, i32 mask, f32 scale) {
    require(service_calls++ == 1 && set == &fixture && data == &active_object->light_data &&
                position == &active_object->apiobj.collision_position && matrix == NULL && mask == -1 && scale == 1.0f,
            "unexpected RTL apply call");
    *data = fixture;
}

int main() {
    static_assert(sizeof(void *) == 4, "This diagnostic uses the canonical 32-bit layout");
    require(SeekValF(-1.0f, 1.0f, 5.0f) == 0.0f, "real seek helper must produce the zero blend");

    GameObject_s object;
    prepare(object, false);
    fixture.direction[0] = {2.0f, 0.0f, 0.0f};
    fixture.direction[1] = {0.0f, 2.0f, 0.0f};
    fixture.direction[2] = {0.0f, 0.0f, 2.0f};
    for (int i = 0; i < 3; ++i) {
        object.lighting_state.direction[i] = {-fixture.direction[i].x, -fixture.direction[i].y,
                                              -fixture.direction[i].z};
    }
    run(object, false);
    for (int i = 0; i < 3; ++i) {
        // A zero blend copies the target verbatim, without normalization.
        expect_vector(object.lighting_state.direction[i], fixture.direction[i]);
    }

    prepare(object, false);
    fixture.direction[0] = {0.0f, 2.0f, 0.0f};
    object.lighting_state.direction[0] = {2.0f, 0.0f, 0.0f};
    fixture.direction[1] = {0.0f, 0.0f, 2.0f};
    object.lighting_state.direction[1] = {0.0f, 0.0f, 2.0f};
    // Channel 2 has a zero target and zero current direction.
    run(object, false);
    const f32 diagonal = std::sqrt(0.5f);
    expect_vector(object.lighting_state.direction[0], {diagonal, diagonal, 0.0f});
    expect_vector(object.lighting_state.direction[1], {0.0f, 0.0f, 1.0f});
    expect_vector(object.lighting_state.direction[2], {0.0f, 0.0f, 0.0f});

    prepare(object, true);
    fixture.direction[0] = {2.0f, 0.0f, 0.0f};
    fixture.direction[1] = {0.0f, 2.0f, 0.0f};
    run(object, true);
    expect_vector(object.lighting_state.direction[0], {1.0f, 0.0f, 0.0f});
    expect_vector(object.lighting_state.direction[1], {0.0f, 1.0f, 0.0f});
    expect_vector(object.lighting_state.direction[2], {0.0f, 0.0f, 0.0f});

    std::puts("LightGameObject regression: PASS");
    return 0;
}
