#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

struct ADAPTIVEDIFFICULTY_s {
    i32 difficulty;
    f32 multiplier;
    f32 elapsed;
};

extern i32 adaptivedifficulty[3];

i8 adtabentries[9][4] = {{-1, -1, -1, -1}, {-1, -1, -1, 0}, {-1, -1, 0, 0}, {-1, 0, 0, 0}, {0, 0, 0, 0},
                         {1, 0, 0, 0},     {1, 1, 0, 0},    {1, 1, 1, 0},   {1, 1, 1, 1}};
i8 (*adtab)[4] = &adtabentries[4];

i32 CheckPosAIArea(AIAREA_s *area, nuvec_s *position, float tolerance) {
    if (position == NULL || area == NULL) {
        return 0;
    }

    NUVEC local_position;
    NuVecSub(&local_position, position, &area->position);
    NuVecRotateY(&local_position, &local_position, -area->rotation);

    return local_position.x + tolerance >= -area->half_width && local_position.y + tolerance >= -0.1f &&
           local_position.z + tolerance >= -area->half_depth && local_position.x - tolerance <= area->half_width &&
           local_position.y - tolerance <= area->height && local_position.z - tolerance <= area->half_depth;
}

void ResetAdaptiveDifficulty() {
    ADAPTIVEDIFFICULTY_s *difficulty = (ADAPTIVEDIFFICULTY_s *)adaptivedifficulty;
    difficulty->multiplier = 0.5f;
    difficulty->elapsed = 0.0f;
    difficulty->difficulty = -4;
}

void LoopCode(GameObject_s *, i32, i32, GAMEPAD_s *, i32) {
}
