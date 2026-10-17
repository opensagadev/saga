// Strong replacement for the weak engine allocator in terrain_impact_test.cpp.
struct TERRAIN_TRACK_SLOT;
TERRAIN_TRACK_SLOT *ImpactFixtureAllocTerrId();
TERRAIN_TRACK_SLOT *AllocTerrId() {
    return ImpactFixtureAllocTerrId();
}
