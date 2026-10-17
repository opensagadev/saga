#include "decomp.h"
#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"

extern char FS_LastFileName[64];
extern char FS_Path[256];
extern char FS_Title[64];
extern char FS_Filter[256];
extern char FS_FilterOut[256];
extern f32 FS_X;
extern f32 FS_Y;
extern f32 FS_Width;
extern f32 FS_W;
extern f32 FS_H;
extern u8 FS_Active;
extern u8 FS_RefreshDir;
extern u8 FS_ShowVolumes;
extern void (*FS_Callback)(char *, char *);

i32 SaveGizmoSys(GIZMOSYS_s *, char *, char *);

void SerialiseInt(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 1);
}

void SerialiseChar(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 1, 1);
}

void SerialiseFloat(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 1);
}

void SerialiseNuMtx(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 16);
}

void SerialiseNuVec(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 3);
}

void SerialiseShort(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 1);
}

void SerialiseVuMtx(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 16);
}

void SerialiseVuVec(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 3);
}

void SerialiseString(EdStream &stream, void *data, i32 count) {
    stream.SerialiseString(static_cast<char *>(data), count);
}

void SerialiseColour3(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 3);
}

// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.

void RenderFileSel3(i32 draw_title);

void RenderFileSel();

void SerialiseNuHSpecial(EdStream &stream, void *data, i32) {
    nuhspecial_s *special = static_cast<nuhspecial_s *>(data);
    if (stream.mode == 2) {
        char *name = NuSpecialGetName(special);
        stream.SerialiseString(name, 0);
    }

    if (stream.mode == 1) {
        char name[128];
        stream.SerialiseString(name, 128);
        NuSpecialFind(NULL, special, name, 0);
        theLevelEditor.GetScene(0);
        for (i32 index = 0; index < theLevelEditor.reset_pending; ++index) {
            if (NuSpecialFind(theLevelEditor.GetScene(index), special, name, 0))
                break;
        }
    }
}

void SerialiseStringAddr(EdStream &stream, void *data, i32 count) {
    stream.SerialiseString(static_cast<char **>(data), count);
}

i32 ProcessFileSel3(f32 elapsed, nupad_s *pad);

extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

void ProcessFileSel(f32 elapsed, nupad_s *pad);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

i32 LoadState(i32, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *) {
    return 0;
}

extern "C" {

i32 TriggerAutoSave(void);

} // extern "C"
