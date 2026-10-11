// Include the production owner so fixtures can exercise its internal query
// state without adding test-only symbols to the engine ABI. Unused engine
// sections are discarded by the diagnostic linker. Collision and square-root
// logic is real.
#ifndef TERRAIN_SOURCE
#define TERRAIN_SOURCE "legoapi/render/core/terrain.cpp"
#endif
#include TERRAIN_SOURCE
#include "nu2api/numath/nufloat_android.c"

TERRSET *CurTerr;
i32 terrhitflags;
f32 NuTrigTable[NUTRIGTABLE_COUNT];
static unsigned failures, checks, cases;
static void check(bool value, const char *label, int type) {
    ++checks;
    if (!value) {
        if (failures < 12)
            fprintf(stderr, "FAIL type=%d %s\n", type, label);
        ++failures;
    }
}
static bool near(float a, float b) {
    return fabsf(a - b) < 0.00001f;
}
static void vector_check(const NUVEC &actual, const NUVEC &expected, const char *label, int type) {
    check(near(actual.x, expected.x) && near(actual.y, expected.y) && near(actual.z, expected.z), label, type);
}
int main() {
    static_assert(sizeof(void *) == 4, "Diagnostic requires the target's 32-bit pointer width");
    // Exact quarter-turn samples are the only entries used by these fixtures.
    NuTrigTable[0] = 0.0f;
    NuTrigTable[8192] = 1.0f;
    NuTrigTable[16384] = 0.0f;
    NuTrigTable[24576] = -1.0f;
    TERRSET terrain = {};
    TERRAIN_GROUP group = {};
    TERRAIN_SHAPE shape = {};
    terrain.groups = &group;
    group.chunk_type = TERRAIN_CHUNK_GROUP_SECONDARY;
    group.scene_index = 42;
    shape.material[0] = 7;
    shape.material[1] = 11;
    shape.flags = 3;
    shape.normal_flags = 5;
    CurTerr = &terrain;
    const NUVEC sentinel = {91.0f, 92.0f, 93.0f};
    const float root_half = 0.7071067811865475f;
    for (int type = 0; type < 256; ++type) {
        const bool valid =
            type == 1 || type == 2 || type == 3 || type == 4 || type == 17 || type == 18 || type == 19 || type == 20;
        const bool face = type == 1 || type == 17;
        for (int scaled = 0; scaled < 2; ++scaled)
            for (int yaw = 0; yaw < 2; ++yaw)
                for (int zero = 0; zero < 2; ++zero) {
                    ++cases;
                    TerrainQuery_s query = {};
                    query.hit_type = type;
                    query.terrain_group_index = 0;
                    query.surface = &shape;
                    query.object_scale = scaled ? 2.0f : 1.0f;
                    query.inverse_object_scale = scaled ? 0.5f : 1.0f;
                    query.inverse_object_scale_sq = scaled ? 0.25f : 1.0f;
                    query.inverse_collision_radius = 0.5f;
                    query.movement_yaw = yaw ? 16384.0f : 0.0f;
                    query.movement_normal = zero ? NUVEC{0, 0, 0} : NUVEC{2, 4, 0};
                    query.impact_normal = sentinel;
                    const NUVEC input = query.movement_normal;
                    TerI = &query;
                    terrhitflags = 0x100;
                    TerrPoly = NULL;
                    TerrPolyObj = -9;
                    memset(&TerrPolyInfo, 0, sizeof(TerrPolyInfo));
                    PlatImpactId = -1;
                    PlatImpactNorm = sentinel;
                    // Explicit expected vectors for identity/quarter-turn and 2:1
                    // vertical scale.
                    NUVEC movement = input;
                    NUVEC output = sentinel;
                    if (valid) {
                        if (zero)
                            output = NUVEC{0, 0, 0};
                        else if (face)
                            output = scaled ? NUVEC{root_half, root_half, 0} : NUVEC{2, 4, 0};
                        else if (scaled)
                            output = yaw ? NUVEC{0, root_half, -root_half} : NUVEC{root_half, root_half, 0};
                        else if (type < 16)
                            output = yaw ? NUVEC{0, 2, -1} : NUVEC{1, 2, 0};
                        else
                            output = yaw ? NUVEC{0, 4, -2} : NUVEC{2, 4, 0};
                        if (!face && !zero)
                            movement = type < 16 ? (yaw ? NUVEC{0, 2, -1} : NUVEC{1, 2, 0})
                                                 : (yaw ? NUVEC{0, 4, -2} : NUVEC{2, 4, 0});
                    }
                    StorePlatImpact();
                    vector_check(query.movement_normal, input, "platform leaves query normal untouched", type);
                    vector_check(PlatImpactNorm, output, "platform output", type);
                    check(PlatImpactId == (valid ? 42 : -1), "platform id", type);
                    if (valid)
                        check(memcmp(&PlatImpactTer, &shape, sizeof(shape)) == 0, "platform surface", type);
                    TerrainImpactNorm();
                    vector_check(query.movement_normal, movement, "movement rotation/radius", type);
                    vector_check(query.impact_normal, output, "impact output", type);
                    check(terrhitflags == (0x100 | ((type == 4 || type == 20) ? 4 : 0)), "sphere flags", type);
                    check(TerrPoly == (valid ? &TerrPolyInfo : NULL), "metadata pointer", type);
                    check(TerrPolyObj == (valid ? 0 : -9), "metadata group", type);
                    if (valid)
                        check(TerrPolyInfo.material[0] == 7 && TerrPolyInfo.material[1] == 11 &&
                                  TerrPolyInfo.flags == 3 && TerrPolyInfo.normal_flags == 5,
                              "metadata contents", type);
                }
    }
    TerrainQuery_s subset_query = {};
    subset_query.inverse_object_scale = 1.0f;
    TerI = &subset_query;
    TERRAIN_PLATFORM platform = {};
    NUMTX matrix = {};
    platform.scene_object = &matrix;
    terrain.platforms = &platform;
    group.scene_index = 0;
    group.radius = 1.0f;
    group.bounds_min = NUVEC{-2, -2, -2};
    group.bounds_max = NUVEC{2, 2, 2};
    shape.min_x = shape.min_y = shape.min_z = -1;
    shape.max_x = shape.max_y = shape.max_z = 1;
    for (int mode = 0; mode < 4; ++mode)
        for (int outside = 0; outside < 2; ++outside)
            for (int masked = 0; masked < 2; ++masked) {
                ++cases;
                const bool rotating = mode == 3;
                group.chunk_type = mode == 0 ? 0 : TERRAIN_CHUNK_GROUP_SECONDARY;
                platform.scene_object = mode == 1 ? NULL : &matrix;
                alignas(4) unsigned char arena[0x2000] = {};
                alignas(4) unsigned char input_handle[16] = {};
                auto *input_header = reinterpret_cast<i16 *>(input_handle);
                input_header[0] = 1;
                *reinterpret_cast<TERRAIN_SHAPE **>(input_handle + 4) = &shape;
                TempScanStack = arena;
                TempStackPtr = arena;
                platform.flags = rotating ? TERRAIN_PLATFORM_FLAG_ROTATING : 0;
                shape.material[1] = masked ? 2 : 1;
                for (int v = 0; v < 4; ++v)
                    shape.vectors[v] = NUVEC{outside ? 20.0f : 0.0f, 0, 0};
                shape.min_x = outside ? 19.0f : -1.0f;
                shape.max_x = outside ? 21.0f : 1.0f;
                NUVEC position = {}, movement = {};
                i16 *result = NewScanHandelSubset(input_header, &position, &movement, 1.0f, 1);
                const int expected = !outside && !masked ? 1 : 0;
                check(result == reinterpret_cast<i16 *>(arena), "subset output allocation", rotating);
                check(result && result[0] == expected, "subset includes eligible rotating/static shape", rotating);
                if (result && result[0]) {
                    check(result[1] == 0, "subset group", rotating);
                    check(*reinterpret_cast<TERRAIN_SHAPE **>(arena + 4) == &shape, "subset shape pointer", rotating);
                }
                subset_query.scan_result = 1;
                subset_query.collision_radius = 1.0f;
                subset_query.object_scale = 1.0f;
                for (int disabled = 0; disabled < 2; ++disabled) {
                    TerrPlatDis = disabled ? 0 : -1;
                    ScanTerrainHandel(1, input_header);
                    auto *scan_header = reinterpret_cast<i16 *>(subset_query.scan_list_storage);
                    const int scan_expected = disabled && mode >= 2 ? 0 : expected;
                    check(scan_header[0] == scan_expected, "scan retains eligible shape / rejects disabled platform",
                          mode);
                    check(TerrPlatDis == -1, "scan clears platform exclusion", mode);
                }
            }
    printf("terrain collisions: %u cases, %u assertions, %u failures\n", cases, checks, failures);
    return failures ? 1 : 0;
}
