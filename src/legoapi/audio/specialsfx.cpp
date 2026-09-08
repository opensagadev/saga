#include "decomp.h"
#include "globals.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void LetGoOfBalloon(GameObject_s *object);
i32 qrand(void);
extern "C" void PlaySfxByIdAndSetVolumeAndPitch(i32 sfx_id, nuvec_s *position, f32 volume, f32 pitch);

void PopBalloon(GameObject_s *object) {
    LetGoOfBalloon(object);
}

void Disorientate(GameObject_s *, nuvec_s *) {
}

extern "C" i32 GetSfxId(const char *name);
extern "C" void NuGScnGetSpecial(nuhspecial_s *special, NUGSCN *scene, i32 index);
extern "C" i32 IsSfxLooping(i32 sfx_id);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32 volume);
i32 GetAnimDirection(nuinstanim_s *animation);

i32 SpecialSfxAdd(i32 special_index) {
    if (WORLD->special_sfx_count < 0xc0) {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, WORLD->current_gscn, special_index);
        if (NuSpecialExistsFn(&special) != 0) {
            specialsfx_s *entry = &WORLD->special_sfx[WORLD->special_sfx_count];
            entry->flags &= 0xf0;
            entry->special = special;
            entry->events = NULL;
            entry->event_count = 0;
            return ++WORLD->special_sfx_count;
        }
    }
    return -1;
}

i32 SpecialSfxLoad(char *path, WORLDINFO_s *world) {
    if (world == NULL) {
        return 0;
    }

    EdFileSetMedia(1);
    if (EdFileOpen(path, NUFILE_READ) == 0) {
        return 0;
    }

    i32 version = EdFileReadInt();
    if (version > 2) {
        EdFileClose();
        return 0;
    }

    i32 count = EdFileReadInt();
    if (count > 0xc0) {
        count = 0xc0;
    }
    EdFileReadInt();
    world->special_sfx_count = 0;

    char name[72];
    for (i32 i = 0; i < count; i++) {
        specialsfx_s *entry = &world->special_sfx[i];
        i16 length = EdFileReadShort();
        EdFileRead(name, length);
        name[length] = '\0';
        NuSpecialFind(world->current_gscn, &entry->special, name, 1);
        entry->event_count = EdFileReadChar();
        if (version == 2) {
            entry->flags = (entry->flags & 0xf0) | (EdFileReadChar() & 0xf);
        }
        world->special_sfx_count++;
    }

    SPECIALSFXEVENT_s *event = world->special_sfx_events;
    for (i32 i = 0; i < count; i++) {
        specialsfx_s *entry = &world->special_sfx[i];
        for (i32 j = 0; j < entry->event_count; j++, event++) {
            i16 length = EdFileReadShort();
            EdFileRead(name, length);
            name[length] = '\0';
            event->sfx_id = static_cast<i16>(GetSfxId(name));
            event->flags = static_cast<u8>(EdFileReadChar());
            event->flags = (event->flags & ~2u) | ((IsSfxLooping(event->sfx_id) & 1) << 1);
            event->trigger_frame = EdFileReadFloat();
            event->previous_frame = EdFileReadFloat();
            world->special_sfx_event_count++;
        }
    }

    event = world->special_sfx_events;
    for (i32 i = 0; i < world->special_sfx_count; i++) {
        specialsfx_s *entry = &world->special_sfx[i];
        entry->events = event;
        for (i32 j = 0; j < entry->event_count; j++, event++) {
            event->next = (j + 1 < entry->event_count) ? event + 1 : NULL;
        }
    }

    EdFileClose();
    return 1;
}

void EngineNoiseCode(GameObject_s *object, i32 silent) {
    GAMECHARACTERDATA_s *character = object->apiobj.character_data->game_character;
    i32 sfx_id = character->sfx_engine;
    if (sfx_id == -1) {
        return;
    }

    f32 target = 0.0f;
    if (silent == 0) {
        f32 speed;
        if (static_cast<i8>(object->apiobj.flags_low) < 0) {
            speed = *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(&object->player_packet) + 0x714);
            if (speed < 0.0f) {
                speed = -speed;
            }
        } else {
            speed = object->apiobj.velocity_magnitude / character->movement_speed;
        }
        target = speed <= 1.0f ? speed : 1.0f;
    }

    f32 &engine_level = *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(&object->player_packet) + 0x6e0);
    engine_level = SeekLinearF(engine_level, target, FRAMETIME * 0.5f);
    f32 volume = engine_level * 0.5f + 0.5f;
    if (static_cast<i8>(object->apiobj.flags_low) >= 0) {
        volume *= 0.6f;
    }
    f32 variation = static_cast<f32>(qrand()) * 1.5259022e-5f * 0.03f + 0.985f;
    f32 pitch = variation *
                ((static_cast<f32>(object->apiobj.field_0x289) / 63.0f) * 0.05f - 0.025f + engine_level * 0.4f + 0.6f);
    PlaySfxByIdAndSetVolumeAndPitch(sfx_id, &object->apiobj.collision_position, volume, pitch);
}

void NewSeekHalfLife(i32 &current, i32 target, float fraction) {
    current = static_cast<i16>(static_cast<i16>(current) +
                               static_cast<i16>(static_cast<f32>(static_cast<i16>(target - current)) * fraction));
}

void DisorientateCode(GameObject_s *, nuvec_s *, float) {
}

void UpdateSpecialSfx(WORLDINFO_s *world) {
    for (i32 i = 0; i < world->special_sfx_count; i++) {
        specialsfx_s *entry = &world->special_sfx[i];
        SPECIALSFXEVENT_s *event = entry->events;
        if (NuSpecialExistsFn(&entry->special) == 0 || NuSpecialGetVisibilityFn(&entry->special) == 0) {
            continue;
        }

        nuinstanim_s *animation = NuSpecialGetInstAnim(&entry->special);
        if (animation == NULL) {
            continue;
        }
        nuanimdata_s *animation_data = entry->special.scene->instance_animation_data[animation->anim_ix];
        if (animation_data == NULL) {
            continue;
        }

        f32 end_frame = NuAnimEndFrameOld(animation_data);
        f32 frame = animation->ltime;
        if ((animation->flags & NUINSTANIM_FLAG_PLAYING) == 0 && entry->animation_playing == 0) {
            for (i32 j = 0; j < entry->event_count; j++) {
                if (event != NULL) {
                    event->flags &= ~1u;
                    if (frame < end_frame && (event->flags & 2) == 0) {
                        event->previous_frame = 0.0f;
                    }
                    event = event->next;
                }
            }
            entry->animation_playing = 0;
            continue;
        }

        NUVEC *position = NuSpecialGetDrawPos(&entry->special);
        i32 direction = GetAnimDirection(animation);
        for (i32 j = 0; j < entry->event_count; j++) {
            if (event == NULL) {
                continue;
            }

            bool play = false;
            u8 flags = event->flags;
            f32 previous = event->previous_frame;
            f32 trigger = event->trigger_frame;
            if ((flags & 2) != 0) {
                if (direction == 0) {
                    if ((flags & 4) != 0 && trigger <= frame && frame <= previous) {
                        play = true;
                    }
                } else if ((flags & 0xc) == 0xc && trigger <= frame && frame <= previous) {
                    play = true;
                } else if (direction == 1 && (flags & 8) != 0 && frame <= trigger && trigger <= previous) {
                    play = true;
                }
            } else if ((flags & 0xc) == 0xc) {
                if ((previous < trigger && trigger <= frame) || (trigger < previous && frame <= trigger)) {
                    play = true;
                }
            } else if ((flags & 4) != 0) {
                if ((previous < trigger && trigger <= frame) || (previous == 0.0f && trigger == 1.0f)) {
                    play = true;
                }
            } else if ((flags & 8) != 0) {
                if ((trigger < previous && frame <= trigger) || (end_frame == previous && end_frame == trigger)) {
                    play = true;
                }
            }

            if (play) {
                GameAudio_PlaySfxById(event->sfx_id, position, 0, 0);
                if ((event->flags & 2) != 0) {
                    event = event->next;
                    continue;
                }
            }
            event->previous_frame = frame;
            event = event->next;
        }
        entry->animation_playing = (animation->flags & NUINSTANIM_FLAG_PLAYING) != 0;
    }
}

void SetSpecialSfxBits(i32 *sfx_ids, i32 *sfx_count, WORLDINFO_s *world) {
    if (world == NULL) {
        return;
    }

    for (i32 i = 0; i < world->special_sfx_event_count; ++i) {
        i32 id = world->special_sfx_events[i].sfx_id;
        if (id != -1) {
            sfx_ids[(*sfx_count)++] = id;
        }
    }
}

extern "C" i32 GetSfxIdN(char *, i32);

template <typename T> static inline void EffectField(debinftype *effect, usize offset, T value) {
    *reinterpret_cast<T *>(reinterpret_cast<u8 *>(effect) + offset) = value;
}

void FileLoadSingleEffectType(debinftype *effect, i32 version, char category) {
    // All shipped Android general/character pages use version 41.  Keep this
    // branch expressed as the original typed reads: the file record is not a
    // byte-for-byte image of debinftype (runtime fields and padding differ).
    if (version != 41) {
        return;
    }

    u8 *bytes = reinterpret_cast<u8 *>(effect);
    EdFileRead(effect->name, sizeof(effect->name));
    effect->category = static_cast<u8>(category);
    effect->frequency = EdFileReadShort();
    effect->max_particles = EdFileReadShort();
    effect->emission_period = EdFileReadFloat();
    effect->emission_period_random = EdFileReadFloat();
    effect->emission_pause = EdFileReadFloat();
    effect->emission_pause_random = EdFileReadFloat();
    effect->start_offset_random = EdFileReadFloat();
    if (effect->frequency != 0) {
        const f32 minimum_period = 1.0f / static_cast<f32>(effect->frequency);
        if (effect->emission_period_random <= minimum_period && effect->emission_period_random != minimum_period) {
            effect->emission_period_random = minimum_period;
        }
    }

    effect->generator_type = static_cast<u8>(EdFileReadChar());
    effect->momentum_adjustment_type = static_cast<u8>(EdFileReadChar());
    effect->cutscene_only = static_cast<u8>(EdFileReadChar());
    effect->disabled = 0;
    effect->particle_type = static_cast<u8>(EdFileReadChar());
    effect->status = 1;
    effect->camera_facing = static_cast<u8>(EdFileReadChar());

    for (usize offset = 0x30; offset <= 0xa4; offset += sizeof(f32)) {
        EffectField<f32>(effect, offset, EdFileReadFloat());
    }
    EffectField<i16>(effect, 0xa8, EdFileReadShort());
    bytes[0xaa] = static_cast<u8>(EdFileReadChar());
    bytes[0xab] = static_cast<u8>(EdFileReadChar());
    for (usize offset = 0xac; offset <= 0xbc; offset += sizeof(f32)) {
        EffectField<f32>(effect, offset, EdFileReadFloat());
    }

    for (usize offset = 0xc0; offset <= 0xf8; offset += 8) {
        EffectField<f32>(effect, offset, EdFileReadFloat());
        bytes[offset + 4] = EdFileReadUnsignedChar();
        bytes[offset + 5] = EdFileReadUnsignedChar();
        bytes[offset + 6] = EdFileReadUnsignedChar();
        bytes[offset + 7] = EdFileReadUnsignedChar();
    }
    for (usize offset = 0x100; offset <= 0x13c; offset += sizeof(f32)) {
        EffectField<f32>(effect, offset, EdFileReadFloat());
    }
    EffectField<f32>(effect, 0x140, EdFileReadFloat());
    EffectField<f32>(effect, 0x144, EdFileReadFloat());
    for (usize offset = 0x148; offset <= 0x2a4; offset += sizeof(f32)) {
        EffectField<f32>(effect, offset, EdFileReadFloat());
    }
    for (usize offset = 0; offset < sizeof(effect->fields_2b0); offset += sizeof(f32)) {
        *reinterpret_cast<f32 *>(effect->fields_2b0 + offset) = EdFileReadFloat();
    }

    effect->process_spheres = static_cast<u8>(EdFileReadChar());
    effect->time_group = static_cast<u8>(EdFileReadChar());
    if (effect->particle_type == 7) {
        effect->time_group = 2;
    }
    effect->field_2f2 = static_cast<u8>(EdFileReadChar());
    effect->use_explicit_clip_box = static_cast<u8>(EdFileReadChar());
    *reinterpret_cast<f32 *>(effect->fields_2f8 + 0x0) = EdFileReadFloat();
    *reinterpret_cast<f32 *>(effect->fields_2f8 + 0x4) = EdFileReadFloat();
    *reinterpret_cast<f32 *>(effect->fields_2f8 + 0x8) = EdFileReadFloat();
    effect->thinning = EdFileReadFloat();
    for (usize offset = 0xc; offset < sizeof(effect->fields_2f8); offset += sizeof(f32)) {
        *reinterpret_cast<f32 *>(effect->fields_2f8 + offset) = EdFileReadFloat();
    }

    for (usize i = 0; i < sizeof(effect->particle_keys) / sizeof(effect->particle_keys[0]); ++i) {
        effect->particle_keys[i] = -1;
    }
    for (usize i = 0; i < sizeof(effect->sound_data) / sizeof(effect->sound_data[0]); i += 3) {
        effect->sound_data[i] = -1;
    }
    const i32 sound_count = EdFileReadInt();
    for (i32 i = 0; i < sound_count; ++i) {
        char sound_name[16];
        EdFileRead(sound_name, sizeof(sound_name));
        const usize index = static_cast<usize>(i) * 3;
        effect->sound_data[index] = GetSfxIdN(sound_name, sizeof(sound_name));
        effect->sound_data[index + 1] = EdFileReadInt();
        effect->sound_data[index + 2] = EdFileReadInt();
    }

    effect->trail_count = static_cast<u8>(EdFileReadChar());
    effect->trail_time = EdFileReadFloat();
    effect->radial_segments = static_cast<u8>(EdFileReadChar());
    effect->radial_floor = EdFileReadFloat();
    effect->scale_in_time = EdFileReadFloat();
    effect->scale = 1.0f;
    effect->unscaled_effect_index = 0;

    if (NuStrCmp(effect->name, (char *)"STARDESTROYER") == 0) {
        effect->frequency = 0;
    }
}
