// Exercise production impact dispatch and deflection with controlled track allocation.
// The allocation fixture can replace TerI, exposing stale query-pointer reads.
#ifndef TERRAIN_SOURCE
#define TERRAIN_SOURCE "legoapi/render/core/gameliball.cpp"
#endif
// The engine allocator lives in the same unit as the impact code, so it is made
// weak here and terrain_impact_alloc.cpp supplies the fixture allocator.
struct TERRAIN_TRACK_SLOT;
TERRAIN_TRACK_SLOT *AllocTerrId() __attribute__((weak));
#include TERRAIN_SOURCE
#include "nu2api/numath/nufloat_android.c"
i32 terrhitflags;
f32 NuTrigTable[NUTRIGTABLE_COUNT];
static TERRAIN_TRACK_SLOT allocated;
static TerrainQuery_s swapped;
static bool swap_query;
TERRAIN_TRACK_SLOT *ImpactFixtureAllocTerrId() {
    if (swap_query)
        TerI = &swapped;
    return &allocated;
}
static unsigned checks, failures;
static void check(bool ok, const char *label) {
    ++checks;
    if (!ok && failures++ < 10)
        fprintf(stderr, "FAIL %s\n", label);
}
int main() {
    TERRSET terrain = {};
    TERRAIN_GROUP group = {};
    TERRAIN_PLATFORM platform = {};
    TERRAIN_SHAPE shape = {};
    terrain.groups = &group;
    terrain.platforms = &platform;
    CurTerr = &terrain;
    group.scene_index = 0;
    shape.normals[0] = {0, 1, 0};
    shape.normals[1].y = 65536;
    shape.material[0] = 7;
    shape.material[1] = 11;
    shape.flags = 3;
    unsigned cases = 0;
    for (int type = 0; type < 256; ++type)
        for (int platform_mode = 0; platform_mode < 3; ++platform_mode)
            for (int normal = 0; normal < 2; ++normal)
                for (int previous = 0; previous < 2; ++previous) {
                    ++cases;
                    TerrainQuery_s query = {};
                    TerI = &query;
                    TERRAIN_TRACK_SLOT track = {};
                    CurTrackInfo = &track;
                    u8 flags[2] = {9, 11};
                    query.hit_flags = flags;
                    query.hit_type = type;
                    query.terrain_group_index = platform_mode ? 0 : -1;
                    query.surface = platform_mode ? &shape : NULL;
                    group.chunk_type = platform_mode == 2 ? 1 : 0;
                    query.shape_adjusted = 1;
                    query.position = {1, 2, 3};
                    query.movement = {.25f, -.5f, .75f};
                    query.movement_normal = normal ? NUVEC{0, 1, 0} : NUVEC{1, 0, 0};
                    query.impact_normal = query.movement_normal;
                    query.object_scale = 1;
                    query.collision_radius = .5f;
                    query.hit_time = .5f;
                    query.unclamped_hit_time = -.1f;
                    query.separation_epsilon = .01f;
                    query.flags = previous;
                    query.previous_movement_normal = {-1, 0, 0};
                    NUVEC position = {91, 92, 93}, movement = {.5f, -.25f, .75f};
                    terrhitflags = 0;
                    TerrWallInfo = 0;
                    platform.flags = 0;
                    TERRAIN_IMPACT_RECORD records[2] = {};
                    int record_count = 0;
                    TerImpactData = records;
                    TerImpactDataCount = &record_count;
                    TerImpactDataMax = 2;
                    TerrainImpact(&position, &movement, flags);
                    check(record_count == (type ? 1 : 0), "impact data recorded for every nonzero hit");
                    if (type == 0) {
                        check(position.x == 1.25f && position.y == 1.0f && position.z == 3.75f,
                              "no-hit output position");
                        check(flags[0] == 0 && flags[1] == 11, "no-hit flags");
                    }
                    bool valid = type >= 1 && type <= 4, penetrating = type >= 17 && type <= 20;
                    if (valid)
                        check((query.flags & 1) == 0, "first normal clears previous flag");
                    if (penetrating)
                        check((query.flags & 1) != 0, "penetrating hit retains previous normal");
                }

    int types[] = {1, 17};
    u8 old_id[2] = {}, new_id[2] = {};
    for (int type : types) {
        TerrainQuery_s original = {};
        original.hit_type = type;
        original.terrain_group_index = -1;
        original.shape_adjusted = 1;
        original.object_scale = 1;
        original.hit_time = .5f;
        original.impact_normal = {1, 0, 0};
        original.movement_normal = {1, 0, 0};
        original.hit_flags = old_id;
        swapped = original;
        swapped.impact_normal = {.2f, .3f, .4f};
        swapped.hit_flags = new_id;
        TerI = &original;
        CurTrackInfo = NULL;
        allocated = {};
        swap_query = true;
        TerImpactData = NULL;
        u8 flags[2] = {};
        NUVEC position = {}, movement = {};
        TerrainImpact(&position, &movement, flags);
        check(TerI == &swapped, "allocation switches current query");
        check(allocated.id == swapped.hit_flags, "track captures active query id");
        check(allocated.impact_normal.x == .2f && allocated.impact_normal.y == .3f && allocated.impact_normal.z == .4f,
              "track captures active query normal");
    }
    printf("terrain impact: %u ordinary cases and 2 callback cases, %u assertions, %u failures\n", cases, checks,
           failures);
    return failures ? 1 : 0;
}
