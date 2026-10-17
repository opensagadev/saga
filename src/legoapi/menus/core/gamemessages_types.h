#pragma once

// Types shared by legoapi/menus/core/gamemessages.cpp and legoapi/gizmo/object/gizmopickup.cpp.

/* The public game-message type intentionally only exposes the fields used by
 * callers.  This is the backing layout consumed by the original message
 * queue; its first 0x7c bytes are either a text pointer or inline text. */
struct GAME_MESSAGE_DATA {
    char *text;
    char text_buffer[0x78];
    nuvec_s position_a; // 0x7c
    nuvec_s target_position;
    nuvec_s position;
    nuvec_s start_position;
    f32 field_0xac;
    f32 target_scale;
    f32 field_0xb4;
    f32 field_0xb8;
    f32 elapsed;
    f32 duration;
    f32 field_0xc4;
    f32 field_0xc8;
    f32 field_0xcc;
    f32 field_0xd0;
    f32 field_0xd4;
    u32 flags;
    u32 score;
    u16 field_0xe0;
    u16 field_0xe2;
    u16 field_0xe4;
    u16 icon;
    union {
        nuvec_s color;
        nuhspecial_s special;
    };
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
    u8 active;
    u8 field_0xf9;
    u8 field_0xfa;
    u8 field_0xfb;
    u8 field_0xfc;
    u8 field_0xfd;
    u8 field_0xfe;
    u8 field_0xff;
    void (*delay_fn)(GAMEMESSAGE_s *);
    void (*tick_fn)(GAMEMESSAGE_s *);
    void (*update_fn)(GAMEMESSAGE_s *);
    void *field_0x10c;
    void (*end_fn)(GAMEMESSAGE_s *);
};

static_assert(offsetof(GAME_MESSAGE_DATA, tick_fn) == offsetof(GAMEMESSAGE_s, tick_fn), "game message backing callback offset");
static_assert(sizeof(GAME_MESSAGE_DATA) == sizeof(GAMEMESSAGE_s), "game message backing layout");
