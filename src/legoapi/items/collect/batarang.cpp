#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "nu2api/nucore/nustring.h"

extern BATARANG_s Batarang[8];
extern "C" i16 id_ROBIN;

void Batarangs_Draw() {
}

void Batarangs_Reset() {
    for (i32 i = 0; i < 8; ++i) {
        Batarang[i].field_0x7d = 0;
        Batarang[i].active = 0;
        Batarang[i].target_id = 0;
        Batarang[i].cooldown = 50;
    }
}

void Batarang_Release(GameObject_s *, i32) {
}

void Batarangs_Update() {
}

void Batarang_MoveCode(GameObject_s *) {
}

void Batarang_Ricochet(BATARANG_s *) {
}

void Batarang_GetSightInfo(i32 character, i32 *red, i32 *green, i32 *blue, char *text) {
    *red = 255;
    if (character == id_ROBIN) {
        *green = 0;
        *blue = 31;
        if (text != NULL)
            NuStrCpy(text, "\xc2\xb1");
    } else {
        *green = 223;
        *blue = 0;
        if (text != NULL)
            NuStrCpy(text, "\xc2\xa7");
    }
}

void Batarang_InitRicochet(BATARANG_s *, nuvec_s *) {
}

void Batarang_SeekToTarget(BATARANG_s *) {
}

void Batarangs_CheckLostData(void *) {
}

void Batarang_StartTargetting(GameObject_s *) {
}

void Batarang_StartThrowQuick(GameObject_s *) {
}

i32 Batarang_GetObjectFromCharID(i32 character) {
    return 0x32 + (character == id_ROBIN);
}

void GetShootDirection_Batman(GameObject_s *, nuvec_s *) {
}
