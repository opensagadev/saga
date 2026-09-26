#include "nu2api/nufile/tmclient.h"

#include "nu2api/nucore/nustring.h"

#include <stdio.h>

TMClient *the_tm_client;

TMClient::TMClient(i32, char *) {}

i32 TMClient::AllocHandle() {
    static i32 ix;
    for (i32 count = 0; count < 16; ++count) {
        ix = (ix + 1) & 15;
        if (!handles[ix].active) {
            handles[ix].active = 1;
            return ix;
        }
    }
    return -1;
}

void TMClient::Connect() {}

void TMClient::SendTTY(char const *message, i32 length) {
    if (this != NULL && (length | tty_enabled) != 0) {
        char command[0x10c];
        NuStrCpy(command, "TTNOT!");
        NuStrCat(command, address_text);
        NuStrCat(command, "TTY0");
        NuStrNCat(command, const_cast<char *>(message), 0x10b - NuStrLen(command));
    }
    printf(message);
}

i32 TMClient::FOpen(char const *, char const *) {
    return -1;
}

i32 TMClient::FClose(i32 handle) {
    const i32 index = handle - 0x8000;
    if (static_cast<u32>(index) < 16) {
        char command[0x2c];
        NuStrCpy(command, "TTNOT!");
        NuStrCat(command, address_text);
        NuStrCat(command, "CLOS");
        NuStrCatC(command, static_cast<char>(index > 9 ? index + 'W' : index + '0'));
        handles[index].active = 0;
    }
    return 0;
}

void TMClient::FRead(void *, u32, u32, i32) {}

void TMClient::FWrite(void const *, u32, u32, i32) {}

void TMClient::FSeek(i32, i64, i32) {}

void TMClient::FTell(i32) {}

void TMClient::GetKey(i32 *) {}

void TMClient::FlushKeyBuffer() {}

void TMClient::TestKey(i32) {}

void TMClient::GetMouseAxis(TM_MOUSE_AXIS) {}

void TMClient::GetMouseButtons() {}
