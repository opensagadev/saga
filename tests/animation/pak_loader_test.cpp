#include "nu2api/nucore/nuanim3.h"

#include <cstdio>
#include <cstring>

namespace {
    int failures;
    int cases;

    void Check(bool value, const char *name) {
        ++cases;
        if (!value) {
            ++failures;
            std::fprintf(stderr, "FAIL: %s\n", name);
        }
    }
} // namespace

int main() {
    static_assert(sizeof(void *) == 4, "Fixtures use the actual 32-bit pointer format");
    Check(NuAnimData2LoadBuffFromPAK(NULL, 0) == NULL, "zero-size null");
    u32 untouched[4] = {11, 22, 33, 44};
    const u32 expected[4] = {11, 22, 33, 44};
    Check(NuAnimData2LoadBuffFromPAK(untouched, 0) == NULL && std::memcmp(untouched, expected, sizeof(expected)) == 0,
          "zero-size leaves all data untouched");

    // Legacy offsets are relative to the buffer's previous base (zero here).
    alignas(16) u32 legacy[32] = {};
    legacy[2] = 16;
    nuanimdata2_s *animation = reinterpret_cast<nuanimdata2_s *>(legacy + 4);
    animation->duration = 3.0f;
    animation->curves = reinterpret_cast<nuanimcurve2_s *>(40);
    animation->curve_types = reinterpret_cast<u8 *>(44);
    animation->node_flags = reinterpret_cast<u8 *>(48);
    void *legacy_result = NuAnimData2LoadBuffFromPAK(legacy, sizeof(legacy));
    Check(legacy_result == animation, "legacy returned animation pointer");
    Check(legacy[0] == sizeof(legacy) && legacy[1] == reinterpret_cast<usize>(legacy) &&
              legacy[2] == reinterpret_cast<usize>(animation),
          "legacy header fixup");
    Check(animation->curves == reinterpret_cast<nuanimcurve2_s *>(legacy + 10) &&
              animation->curve_types == reinterpret_cast<u8 *>(legacy + 11) &&
              animation->node_flags == reinterpret_cast<u8 *>(legacy + 12),
          "legacy real-provider pointers");

    alignas(16) u32 nested[64] = {};
    nested[2] = 16;
    nuanimdata2_s *nested_animation = reinterpret_cast<nuanimdata2_s *>(nested + 4);
    nested_animation->duration = 3.0f;
    nested_animation->node_count = 1;
    nested_animation->curve_count = 1;
    nested_animation->curves = reinterpret_cast<nuanimcurve2_s *>(64);
    nested_animation->curve_types = reinterpret_cast<u8 *>(68);
    nested_animation->node_flags = reinterpret_cast<u8 *>(72);
    nuanimcurve2_s *curve = reinterpret_cast<nuanimcurve2_s *>(nested + 16);
    curve->data.curvedata = reinterpret_cast<nuanimcurvedata_s *>(80);
    reinterpret_cast<u8 *>(nested)[68] = 1;
    nuanimcurvedata_s *curve_data = reinterpret_cast<nuanimcurvedata_s *>(nested + 20);
    curve_data->key_mask = reinterpret_cast<u32 *>(96);
    curve_data->key_offsets = reinterpret_cast<u16 *>(100);
    curve_data->key_data = reinterpret_cast<void *>(104);
    Check(NuAnimData2LoadBuffFromPAK(nested, sizeof(nested)) == nested_animation && nested_animation->curves == curve &&
              curve->data.curvedata == curve_data && curve_data->key_mask == nested + 24 &&
              curve_data->key_offsets == reinterpret_cast<u16 *>(nested + 25) && curve_data->key_data == nested + 26,
          "legacy nested curve relocation");

    // One table entry relocates the nonzero pointer relative to its own slot.
    alignas(16) u32 block[16] = {};
    block[0] = 32;
    block[1] = 0x7fffffff;
    block[4] = 8;
    block[8] = 1;
    block[9] = static_cast<u32>(-20);
    Check(NuAnimData2LoadBuffFromPAK(block, sizeof(block)) == block + 1 &&
              block[4] == reinterpret_cast<usize>(block + 6),
          "pointer-block real relocation");

    alignas(16) u32 modern[32] = {};
    ani3_animheader_s *header = reinterpret_cast<ani3_animheader_s *>(modern);
    header->magic = ANI3_MAGIC_VERSION_4;
    header->constants = reinterpret_cast<i16 *>(64);
    header->keys = reinterpret_cast<u8 *>(80);
    Check(NuAnimData2LoadBuffFromPAK(modern, sizeof(modern)) == header, "ANI4 returns header");
    Check(header->constants == reinterpret_cast<i16 *>(modern + 16) &&
              header->keys == reinterpret_cast<u8 *>(modern + 20),
          "ANI4 real-provider pointers");

    std::printf("%d bounded actual-provider cases, %d failures\n", cases, failures);
    return failures != 0;
}
