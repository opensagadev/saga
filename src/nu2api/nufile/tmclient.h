#pragma once

#include <stddef.h>

#include "nu2api/nucore/common.h"

// Target Manager's sixteen file handles occupy the first 0x200 bytes.
class TMClient {
  public:
    struct TM_MOUSE_AXIS {};

    TMClient(i32 use_target_manager, char *target_manager_mac_address);

    i32 AllocHandle();
    void Connect();
    i32 FClose(i32 handle);
    i32 FOpen(char const *path, char const *mode);
    void FRead(void *buffer, u32 size, u32 count, i32 handle);
    void FSeek(i32 handle, i64 offset, i32 origin);
    void FTell(i32 handle);
    void FWrite(void const *buffer, u32 size, u32 count, i32 handle);
    void FlushKeyBuffer();
    void GetKey(i32 *key);
    void GetMouseAxis(TM_MOUSE_AXIS axis);
    void GetMouseButtons();
    void SendTTY(char const *message, i32 length);
    void TestKey(i32 key);

  private:
    struct Handle {
        unsigned active : 1;
        unsigned : 31;
        u8 reserved[0x1c];
    };

    Handle handles[16];
    u8 reserved_200[0x314];
    char address_text[0x14];
    i32 tty_enabled;
    u8 reserved_52c[4];
};

static_assert(sizeof(TMClient) == 0x530, "TMClient size");

extern TMClient *the_tm_client;
