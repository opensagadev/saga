// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include <cstdio>

#include "decomp.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/screens/store.h"
#include "globals.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/menus/core/gamemessages_types.h"
#include "legoapi/render/core/render.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/menus/core/text.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "nu2api/nu3d/nurndr.h"
#include "gameframework/saveload.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/mission.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"

GAMEMESSAGE_s GameMessage[128] = {};

static i32 GameMessageIndex;

ADDGAMEMSG AddGameMsg_Default = {
    NULL, &v000, &v000, 1.0f, 1.0f, 0xff, 0xff, 0xff, 0x80, 0,  1.0f, 0.0f, 0, -1,
    NULL, 0,     0.0f,  0.0f, NULL, NULL, NULL, NULL, NULL, -1, 0,    0xff, 0,
};

void *AddGameMessage(char *text, nuvec_s *position, float scale, nuvec_s *target_position, float target_scale,
                     unsigned char red, unsigned char green, unsigned char blue, u32 flags, float duration);

struct GAMEMESSAGE_s;
struct nuhspecial_s;

nuhspecial_s *(*GameMsg_GetExtraObjFn)(GAMEMESSAGE_s *);

void ResetGameMessages() {
    for (i32 i = 0; i < 128; i++) {
        GameMessage[i].active = 0;
    }
}

GAMEMESSAGE_s *AddGameMsg(ADDGAMEMSG *message) {
    GAME_MESSAGE_DATA *slot;
    GAME_MESSAGE_DATA fallback;
    i32 temporary = 0;
    i32 slot_index = -1;
    f32 best = 1000000.0f;
    i32 index = GameMessageIndex;

    for (i32 count = 128; count != 0; --count) {
        slot = reinterpret_cast<GAME_MESSAGE_DATA *>(&GameMessage[index]);
        if (slot->active == 0) {
            GameMessageIndex = index;
            goto initialise_slot;
        }

        if ((slot->flags & 0x1000) == 0) {
            f32 value;
            if (slot->field_0xfa != 0) {
                value = 1.0f;
            } else if ((slot->elapsed == 0.0f) | (slot->duration == 0.0f)) {
                value = 0.0f;
            } else {
                value = slot->elapsed / slot->duration;
            }
            if (slot_index == -1 || value > best) {
                best = value;
                slot_index = index;
            }
        }

        index++;
        if (index == 128) {
            index = 0;
        }
    }

    if (slot_index == -1) {
        if (message->field_0x4d == 0)
            return NULL;
        memset(&fallback, 0, sizeof(fallback));
        slot = &fallback;
        temporary = 1;
    } else {
        GameMessageIndex = slot_index;
        slot = reinterpret_cast<GAME_MESSAGE_DATA *>(&GameMessage[slot_index]);
        if (slot->active != 0 && slot->score != 0 && slot->field_0xfd <= 1 && slot->end_fn != NULL) {
            slot->end_fn(reinterpret_cast<GAMEMESSAGE_s *>(slot));
        }
    }

initialise_slot:
    slot->text = NULL;
    i32 text_index = 0;
    if (message->special == NULL) {
        if (message->text == NULL) {
            return NULL;
        }

        i32 length = NuStrLen(message->text);
        if (length <= 0) {
            return NULL;
        }
        if ((message->flags & 0x2000) != 0) {
            slot->text = message->text;
        } else {
            for (; text_index < length && text_index <= 0x76; ++text_index) {
                slot->text_buffer[text_index] = message->text[text_index];
            }
        }
    } else {
        if (NuSpecialExistsFn(message->special) == 0) {
            return NULL;
        }
    }
    slot->text_buffer[text_index] = '\0';

    slot->field_0xd4 = 0.0f;
    slot->position = *message->position;
    slot->position_a = slot->position;
    slot->field_0xb4 = message->scale;
    slot->field_0xac = message->scale;
    slot->elapsed = 0.0f;
    if ((message->flags & 0x80) == 0) {
        slot->duration = message->duration;
    } else {
        slot->duration = 0.0f;
    }

    slot->red = message->red;
    slot->green = message->green;
    slot->blue = message->blue;
    slot->alpha = message->alpha;
    slot->flags = message->flags;

    nuvec_s *target_position = message->target_position;
    if (target_position == NULL)
        target_position = message->position;
    slot->target_position = *target_position;
    slot->target_scale = message->target_scale;
    if ((message->flags & 1) == 0) {
        slot->field_0xb8 = message->scale;
        slot->start_position = slot->position;
        slot->field_0xfb = 0;
    }
    slot->field_0xf9 = static_cast<u8>((message->flags & 1) == 0);
    slot->active = 1;
    slot->field_0xfa = 0;
    slot->field_0xfc = message->field_0x4f;
    slot->icon = static_cast<u16>(message->icon);
    if (message->special == NULL) {
        slot->special = nuhspecial_s();
    } else {
        slot->special = *message->special;
    }

    slot->field_0xfd = static_cast<u8>(message->player_index);
    slot->score = message->score;
    slot->field_0xc4 = message->field_0x30;
    slot->delay_fn = message->delay_fn;
    void (*end_fn)(GAMEMESSAGE_s *) = message->end_fn;
    slot->field_0xe0 = 0;
    slot->tick_fn = message->tick_fn;
    slot->update_fn = message->update_fn;
    slot->field_0xc8 = message->field_0x34;
    slot->field_0xe2 = 0;
    slot->field_0xe4 = 0;
    slot->field_0x10c = message->field_0x44;
    slot->end_fn = end_fn;
    slot->field_0xff = message->field_0x4d;
    slot->field_0xfe = message->field_0x4e;
    slot->field_0xd0 = message->field_0x20;
    slot->field_0xcc = 1.0f;

    if (temporary != 0) {
        if (end_fn != NULL)
            end_fn(reinterpret_cast<GAMEMESSAGE_s *>(slot));
        return NULL;
    }
    return reinterpret_cast<GAMEMESSAGE_s *>(slot);
}

void *AddGameMessage(char *text, nuvec_s *position, float scale, nuvec_s *target_position, float target_scale,
                     unsigned char red, unsigned char green, unsigned char blue, u32 flags, float duration) {
    ADDGAMEMSG message = AddGameMsg_Default;
    message.text = text;
    message.position = position;
    message.target_position = target_position;
    message.scale = scale;
    message.target_scale = target_scale;
    message.red = red;
    message.green = green;
    message.blue = blue;
    message.flags = flags;
    message.duration = duration;
    return AddGameMsg(&message);
}

void UpdateGameMessages() {
    using MessageFn = void (*)(GAMEMESSAGE_s *);
    GAME_MESSAGE_DATA *message = reinterpret_cast<GAME_MESSAGE_DATA *>(&GameMessage[0]);
    GAME_MESSAGE_DATA *end = reinterpret_cast<GAME_MESSAGE_DATA *>(&GameMessage[128]);
    for (; message != end; ++message) {
        if (message->active == 0) {
            continue;
        }
        if (message->field_0xfa != 0) {
            message->active = 0;
            if (message->end_fn != NULL && message->field_0xff == 0) {
                message->end_fn(reinterpret_cast<GAMEMESSAGE_s *>(message));
            }
            continue;
        }

        if ((message->flags & 0x80) != 0) {
            message->duration = 1.0f;
            message->elapsed = 1.0f;
            message->field_0xfa = 1;
        }
        if (message->field_0xd0 > 0.0f) {
            message->field_0xd0 -= FRAMETIME;
            if (!(message->field_0xd0 <= 0.0f)) {
                continue;
            }
            MessageFn callback = message->delay_fn;
            if (callback != NULL) {
                callback(reinterpret_cast<GAMEMESSAGE_s *>(message));
            }
            continue;
        }

        if (!(message->duration > message->elapsed)) {
            continue;
        }

        message->elapsed += FRAMETIME;
        if (message->elapsed >= message->duration) {
            message->elapsed = message->duration;
            message->field_0xfa = 1;
            if (message->end_fn != NULL && message->field_0xff != 0) {
                message->end_fn(reinterpret_cast<GAMEMESSAGE_s *>(message));
            }
        }

        const u32 flags = message->flags;
        if ((flags & 0x50) != 0) {
            f32 curve = message->elapsed / message->duration;
            if ((flags & 0x100) != 0) {
                curve = 1.0f - NU_SIN_LUT(static_cast<i32>(curve * 16384.0f + 16384.0f));
            } else if ((flags & 0x200) != 0) {
                curve = NU_SIN_LUT(static_cast<i32>(curve * 16384.0f));
            } else if ((flags & 0x400) != 0) {
                curve = NU_SIN_LUT(static_cast<i32>(curve * 32768.0f));
            } else if ((flags & 0x800) != 0) {
                curve = 1.0f - NU_SIN_LUT(static_cast<i32>(curve * 32768.0f));
            }
            if ((flags & 0x10) != 0) {
                message->position.x =
                    message->position_a.x + (message->target_position.x - message->position_a.x) * curve;
                message->position.y =
                    message->position_a.y + (message->target_position.y - message->position_a.y) * curve;
                message->position.z =
                    message->position_a.z + (message->target_position.z - message->position_a.z) * curve;
            }
            if ((flags & 0x40) != 0) {
                message->field_0xb4 = message->field_0xac + (message->target_scale - message->field_0xac) * curve;
            }
        }
        if ((flags & 0x8000) != 0) {
            const f32 alpha_curve =
                NU_SIN_LUT(static_cast<i32>(message->elapsed / message->duration * 16384.0f + 16384.0f));
            message->alpha = static_cast<u8>(alpha_curve * 128.0f);
        } else if ((flags & 0x4000) != 0) {
            message->alpha = static_cast<u8>((1.0f - message->elapsed / message->duration) * 128.0f);
        }
        MessageFn callback = message->tick_fn;
        if (callback != NULL) {
            callback(reinterpret_cast<GAMEMESSAGE_s *>(message));
        }
    }
}

void TransformGameMessages(nuvec_s *camera_position, nuvec_s *camera_scale, nuvec_s *camera_direction) {
    GAME_MESSAGE_DATA *message = reinterpret_cast<GAME_MESSAGE_DATA *>(&GameMessage[0]);
    GAME_MESSAGE_DATA *end = reinterpret_cast<GAME_MESSAGE_DATA *>(&GameMessage[128]);
    for (; message != end; ++message) {
        if (message->active == 0 || message->field_0xf9 != 0) {
            continue;
        }
        NuCameraTransformScreenClip(&message->start_position, &message->position, 1, NULL);
        const f32 depth = (message->position.x - camera_position->x) * camera_direction->x +
                          (message->position.y - camera_position->y) * camera_direction->y +
                          (message->position.z - camera_position->z) * camera_direction->z;
        if (!(depth <= 0.0f)) {
            message->field_0xfb = 0;
            if ((message->flags & 4) != 0) {
                const f32 offset = message->field_0xb4;
                NUVEC target = {message->position.x + offset * camera_scale->x,
                                message->position.y + offset * camera_scale->y,
                                message->position.z + offset * camera_scale->z};
                NUVEC transformed;
                NuCameraTransformScreenClip(&transformed, &target, 1, NULL);
                message->field_0xb8 = __builtin_fabsf(message->start_position.x - transformed.x) * 0.525f;
            }
        } else {
            message->field_0xfb = 1;
        }
        if ((message->flags & 2) == 0) {
            message->field_0xf9 = 1;
        }
        message->start_position.z = 1.0f;
    }
}

void DrawGameMessages() {
    extern GAMEMESSAGE_s GameMessage[128];
    extern i32 DrawPanel3DObjectNoAlpha(float, float, float, float, float, float, u16, u16, u16, nuhspecial_s *, i32);

    GAMEMESSAGE_s *message = GameMessage;
    GAMEMESSAGE_s *end = message + 128;
    for (; message != end; ++message) {
        if (message->active == 0) {
            continue;
        }
        if (message->field_0xfb != 0 && (message->flags & 0x40000) == 0) {
            continue;
        }
        if (message->field_0xd0 > 0.0f) {
            continue;
        }
        if (!(message->duration > message->elapsed) && message->field_0xfa == 0) {
            continue;
        }

        u32 flags = message->flags;
        f32 progress = message->elapsed / message->duration;
        if ((flags & 0x100) != 0) {
            progress = 1.0f - NU_SIN_LUT(static_cast<i32>(progress * 16384.0f + 16384.0f));
        } else if ((flags & 0x200) != 0) {
            progress = NU_SIN_LUT(static_cast<i32>(progress * 16384.0f));
        } else if ((flags & 0x400) != 0) {
            progress = NU_SIN_LUT(static_cast<i32>(progress * 32768.0f));
        } else if ((flags & 0x800) != 0) {
            progress = 1.0f - NU_SIN_LUT(static_cast<i32>(progress * 32768.0f));
        }

        f32 scale = (flags & 5) == 5 ? message->field_0xb8 : message->field_0xb4;
        NUVEC position = message->start_position;
        if (message->field_0xd4 != 0.0f && message->field_0xfb == 0) {
            const f32 limit = message->field_0xd4;
            if (position.x < -limit) {
                position.x = -limit;
            } else if (position.x > limit) {
                position.x = limit;
            }
            if (position.y < -limit) {
                position.y = -limit;
            } else if (position.y > limit) {
                position.y = limit;
            }
        }

        if ((flags & 8) != 0) {
            if (message->update_fn != NULL) {
                message->update_fn(message);
                flags = message->flags;
            }
            position.x = position.x + (message->target_position.x - position.x) * progress;
            position.y = position.y + (message->target_position.y - position.y) * progress;
            position.z = position.z + (message->target_position.z - position.z) * progress;
        }
        if ((flags & 0x20) != 0) {
            scale += (message->target_scale - scale) * progress;
        }
        position.z += message->field_0xc8;
        position.x += message->field_0xc4;

        if (message->draw_callback != NULL) {
            message->draw_callback(message, &position, scale);
            continue;
        }

        if (NuSpecialExistsFn(&message->special) != 0) {
            const u32 draw_flags = message->flags;
            if ((draw_flags & 0x20000) != 0) {
                DrawPanel3DObjectNoAlpha(position.x, position.y, position.z, scale, scale, scale, message->field_0xe0,
                                         message->rotation_y, message->field_0xe4, &message->special, 2);
            } else {
                const f32 alpha = (draw_flags & 0x10000) != 0 ? static_cast<f32>(message->alpha) / 128.0f : 1.0f;
                DrawPanel3DObject(position.x, position.y, position.z, scale, scale, scale, message->field_0xe0,
                                  message->rotation_y, message->field_0xe4, &message->special, 2, alpha);
            }
            if (GameMsg_GetExtraObjFn != NULL) {
                nuhspecial_s *extra = GameMsg_GetExtraObjFn(message);
                if (extra != NULL && NuSpecialExistsFn(extra) != 0) {
                    const u32 draw_flags = message->flags;
                    if ((draw_flags & 0x20000) != 0) {
                        DrawPanel3DObjectNoAlpha(position.x, position.y, position.z, scale, scale, scale,
                                                 message->field_0xe0, message->rotation_y, message->field_0xe4, extra,
                                                 2);
                    } else {
                        const f32 alpha =
                            (draw_flags & 0x10000) != 0 ? static_cast<f32>(message->alpha) / 128.0f : 1.0f;
                        DrawPanel3DObject(position.x, position.y, position.z, scale, scale, scale, message->field_0xe0,
                                          message->rotation_y, message->field_0xe4, extra, 2, alpha);
                    }
                }
            }
        } else {
            char *text = message->text != NULL ? message->text : message->text_buffer;
            Text3DEx(text, position.x, position.y, position.z, scale, scale, scale, message->field_0xfc, message->red,
                     message->green, message->blue, message->alpha);
        }
    }
}

i32 FindGameMsgsWithID(i32 id, i32 remove, i32 player, GAMEMESSAGE_s *exclude) {
    i32 count = 0;
    GAME_MESSAGE_DATA *message = reinterpret_cast<GAME_MESSAGE_DATA *>(&GameMessage[0]);
    GAME_MESSAGE_DATA *end = reinterpret_cast<GAME_MESSAGE_DATA *>(&GameMessage[128]);
    GAME_MESSAGE_DATA *excluded = reinterpret_cast<GAME_MESSAGE_DATA *>(exclude);

    if (remove == 0) {
        if (player != -1) {
            for (; message != end; ++message) {
                if (message->active != 0 && static_cast<i8>(message->field_0xfe) == id && message != excluded &&
                    static_cast<i8>(message->field_0xfd) == player) {
                    ++count;
                }
            }
        } else {
            for (; message != end; ++message) {
                if (message->active != 0 && static_cast<i8>(message->field_0xfe) == id && message != excluded) {
                    ++count;
                }
            }
        }
    } else {
        if (player != -1) {
            for (; message != end; ++message) {
                if (message->active != 0 && static_cast<i8>(message->field_0xfe) == id && message != excluded &&
                    static_cast<i8>(message->field_0xfd) == player) {
                    ++count;
                    message->active = 0;
                }
            }
        } else {
            for (; message != end; ++message) {
                if (message->active != 0 && static_cast<i8>(message->field_0xfe) == id && message != excluded) {
                    ++count;
                    message->active = 0;
                }
            }
        }
    }
    return count;
}

void AddGameMsgCount(nuvec_s *position, i32 count, i32 total, unsigned char red, unsigned char green,
                     unsigned char blue, float field_0xd4) {
    char text[32];
    sprintf(text, "%i/%i", count, total);

    void *message = position != NULL ? AddGameMessage(text, position, 0.6f, NULL, 0.8f, red, green, blue, 0x4023, 1.0f)
                                     : AddGameMessage(text, &v001, 0.6f, NULL, 0.8f, red, green, blue, 0x4020, 1.0f);
    if (message != NULL) {
        *reinterpret_cast<float *>(reinterpret_cast<char *>(message) + 0xd4) = field_0xd4;
    }
}

void AddFancyMessageRGB(char *text, float x, float y, float scale, float duration, i32 message_id, unsigned char red,
                        unsigned char green, unsigned char blue) {
    if (message_id != 0) {
        GameAudio_PlaySfx(0x2c, NULL, 0, 0);
        GameAudio_PlaySfx(0x26, NULL, 0, 0);
        NewRumbleAllPlayers(0.7f, 0.0f, 0, 0);
        GameCam_NewShake(GameCam, 1.0f, 1.0f, 1.0f);
    }

    NUVEC position = {x, y, 1.0f};
    scale += scale;
    const f32 scale_offset = scale * 0.5f;
    AddGameMessage(text, &position, scale, &position, scale + scale_offset, red, green, blue, 0x4020, duration);
    AddGameMessage(text, &position, scale, &position, scale - scale_offset, red, green, blue, 0x4020, duration);
}

void AddFancyMessage(char *text, float x, float y, float scale, float duration, i32 message_id, i32 colour_type) {
    const i32 red = 0xff;
    const i32 green = 0xff - ((static_cast<u32>(colour_type) < 1) << 6);
    const i32 blue = ~(-static_cast<i32>(static_cast<u32>(colour_type) < 1)) & 0xff;
    AddFancyMessageRGB(text, x, y, scale, duration, message_id, red, green, blue);
}

