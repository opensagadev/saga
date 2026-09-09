#include "legoapi/legoapi_types.h"

extern "C" {
    extern u16 id_CLONEARC;
    extern u16 id_IMPERIALSHUTTLE;
    extern u16 id_NABOOSTARFIGHTER;
    extern u16 id_XWING;
    extern u16 id_SNOWSPEEDER;
    extern u16 id_MILLENNIUMFALCON;
    extern u16 id_NEW_REPUBLIC_GUNSHIP;
}

float PodSprint_RollMul(GameObject_s *object) {
    u16 id = object->id;
    if (id == id_CLONEARC || id == id_IMPERIALSHUTTLE || id == id_NABOOSTARFIGHTER)
        return 0.6f;
    if (id == id_XWING || id == id_SNOWSPEEDER || id == id_MILLENNIUMFALCON || id == id_NEW_REPUBLIC_GUNSHIP)
        return 0.8f;
    return 1.0f;
}
