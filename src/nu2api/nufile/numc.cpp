#include "nu2api/nufile/nufile.h"

extern "C" i32 NuMcGetSlotMax(void) {
    return 0;
}

extern "C" i32 NuMcCreateDir(void) {
    return 0;
}

extern "C" void NuMcCloseDir(void) {
}

extern "C" i32 NuMcOpenDir(void) {
    return 0;
}

extern "C" i32 NuMcReadDir(void) {
    return 0;
}

i32 NuMcCheckCardPresent(i32 port, i32 slot) {
    return 0;
}

i32 NuMcCheckCardFormatted(i32 port, i32 slot) {
    return 0;
}

i32 NuMcCheckCardFreeSpace(i32 port, i32 slot) {
    return 0;
}

i32 NuMcOpen(i32 port, i32 slot, char *filepath, i32 mode, i32 async) {
    return 0;
}

i32 NuMcClose(i32 fd, i32 async) {
    return 0;
}

i32 NuMcSeek(i32 fd, i32 offset, NUFILESEEK mode, i32 async) {
    return 0;
}

i32 NuMcOpenSize(i32 fd) {
    return 0;
}

i32 NuMcRead(i32 fd, void *buf, i32 size, i32 async) {
    return 0;
}

i32 NuMcWrite(i32 fd, void *data, i32 size, i32 async) {
    return 0;
}

i32 NuMcFileOpenSize(NUFILE file) {
    file -= 0x1000;

    return NuMcOpenSize(file);
}

extern "C" i32 NuMcFormat(i32 port, i32 slot) {
    return 0;
}
