#include "decomp.h"
#include "globals.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/numath/nurand.h"
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

static u8 disorientAxis = 7;

void Disorientate(GameObject_s *object, nuvec_s *) {
    const i32 elapsed_seconds = static_cast<i32>(object->context_animation_timer);
    if ((elapsed_seconds & 1) == 0 && static_cast<f32>(elapsed_seconds) != object->field_0x770) {
        object->field_0x770 = static_cast<f32>(elapsed_seconds);
        disorientAxis = 0;
        while ((disorientAxis & 5) == 0) {
            if (NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) < 0.5f) {
                disorientAxis |= 1;
            }
            if (NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) < 0.5f) {
                disorientAxis |= 2;
            }
            if (NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) < 0.5f) {
                disorientAxis |= 4;
            }
        }
    }

    if ((disorientAxis & 1) != 0) {
        object->apiobj.pitch_angle =
            SeekRot(object->apiobj.pitch_angle, static_cast<u16>(object->apiobj.pitch_angle + 0x4000), 3.0f);
    }
    if ((disorientAxis & 2) != 0) {
        object->apiobj.field_0x276 =
            SeekRot(object->apiobj.field_0x276, static_cast<u16>(object->apiobj.field_0x276 + 0x4000), 3.0f);
    }
    if ((disorientAxis & 4) != 0) {
        object->apiobj.roll_angle =
            SeekRot(object->apiobj.roll_angle, static_cast<u16>(object->apiobj.roll_angle + 0x4000), 3.0f);
    }

    NUVEC delta;
    const f32 distance_squared =
        NuVecDistSqr(&object->apiobj.collision_position, &object->disorientation_destination, &delta);
    const f32 speed = 75.0f / (distance_squared / 6.0f + 1.0f);
    const f32 fraction = 1.0f / ((delta.x + 0.01f) / (0.01f + delta.z));
    f32 x_velocity = (1.0f - fraction) * speed;
    f32 z_velocity = fraction * speed;
    if (x_velocity > 100.0f) {
        x_velocity = 100.0f;
    } else {
        z_velocity = MIN(100.0f, z_velocity);
    }
    if ((delta.x < 0.0f && x_velocity > 0.0f) || (delta.x > 0.0f && x_velocity < 0.0f)) {
        x_velocity = -x_velocity;
    }
    if ((delta.z < 0.0f && z_velocity > 0.0f) || (delta.z > 0.0f && z_velocity < 0.0f)) {
        z_velocity = -z_velocity;
    }
    object->apiobj.field_0x1fc = x_velocity;
    object->apiobj.field_0x200 = 0.0f;
    object->apiobj.field_0x204 = z_velocity;
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
            const i32 flags = EdFileReadChar();
            entry->flags = (entry->flags & 0xf0) | (flags & 0xf);
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
            const i32 looping = IsSfxLooping(event->sfx_id);
            event->flags = (event->flags & ~2u) | ((looping & 1) << 1);
            event->trigger_frame = EdFileReadFloat();
            event->previous_frame = EdFileReadFloat();
            world->special_sfx_event_count++;
        }
    }

    event = world->special_sfx_events;
    if (world->special_sfx_count > 0) {
        SPECIALSFXEVENT_s *event_end = event + world->special_sfx_event_count;
        specialsfx_s *entry = world->special_sfx;
        specialsfx_s *entry_end = entry + world->special_sfx_count;
        while (entry < entry_end) {
            SPECIALSFXEVENT_s *next = event == event_end
                                          ? NULL
                                          : reinterpret_cast<SPECIALSFXEVENT_s *>(reinterpret_cast<uintptr_t>(event) +
                                                                                  sizeof(SPECIALSFXEVENT_s));
            entry->events = event;
            if (entry->event_count > 1) {
                SPECIALSFXEVENT_s *previous = event;
                for (i32 j = 0; j < entry->event_count - 1; ++j) {
                    previous->next = next;
                    event = reinterpret_cast<SPECIALSFXEVENT_s *>(reinterpret_cast<uintptr_t>(next) +
                                                                  sizeof(SPECIALSFXEVENT_s));
                    previous = next;
                    next = event;
                }
                if (previous != NULL)
                    previous->next = NULL;
            } else {
                if (event != NULL)
                    event->next = NULL;
                event = next;
            }
            ++entry;
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
        if (object->apiobj.player_controlled) {
            speed = __builtin_fabsf(object->field_0xdc8);
        } else {
            speed = object->apiobj.velocity_magnitude / character->movement_speed;
        }
        target = 1.0f < speed ? 1.0f : speed;
    }

    f32 &engine_level = object->force_use_volume;
    engine_level = SeekLinearF(engine_level, target, FRAMETIME * 0.5f);
    f32 volume = engine_level * 0.5f + 0.5f;
    if (!object->apiobj.player_controlled) {
        volume *= 0.6f;
    }
    f32 pitch = engine_level * 0.4f + 0.6f;
    pitch += (static_cast<f32>(static_cast<i32>(object->apiobj.field_0x289)) / 63.0f) * 0.05f - 0.025f;
    f32 variation = static_cast<f32>(qrand()) * 1.5259022e-5f * 0.03f + 0.985f;
    pitch *= variation;
    PlaySfxByIdAndSetVolumeAndPitch(sfx_id, &object->apiobj.collision_position, volume, pitch);
}

void NewSeekHalfLife(i32 &current, i32 target, float fraction) {
    current = static_cast<i16>(static_cast<i16>(current) +
                               static_cast<i16>(static_cast<f32>(static_cast<i16>(target - current)) * fraction));
}

void StartTurn(GameObject_s *);

void DisorientateCode(GameObject_s *object, nuvec_s *target, f32 distance) {
    if (target == NULL) {
        goto continue_disorientation;
    }
    if (object->character_context != 0x40) {
        object->context_animation_timer = 0.0f;
    }
    object->character_context = 0x40;
    object->disorientation_destination = *target;

disorientate:
    Disorientate(object, &object->disorientation_destination);
    object->context_animation_timer += FRAMETIME;
    if (NuVecDistSqr(&object->apiobj.collision_position, &object->disorientation_destination, NULL) > distance * 1.5f &&
        __builtin_expect(object->context_animation_timer > 3.0f, false)) {
        goto start_turn;
    }
    return;

continue_disorientation:
    if (object->character_context != 0x40) {
        return;
    }
    goto disorientate;

start_turn:
    object->character_context = -1;
    StartTurn(object);
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
                if ((direction == 0 && (flags & 4) != 0) || (flags & 0xc) == 0xc) {
                    if (trigger <= frame && frame <= previous) {
                        play = true;
                    }
                } else if (direction == 1 && (flags & 8) != 0 && frame <= trigger && frame >= previous) {
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
            if ((event->flags & 2) == 0) {
                event->previous_frame = frame;
            }
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

void FileLoadSingleEffectType(debinftype *effect, i32 version, char category) {
    // Records are versioned field streams, not images of the runtime struct.
    EdFileRead(effect->name, sizeof(effect->name));
    effect->category = static_cast<u8>(category);
    if (version < 28) {
        const i16 rate = EdFileReadShort();
        effect->frequency = rate > 0 ? static_cast<i16>(rate * 60) : rate == 0 ? 0 : static_cast<i16>(-60 / rate);
        effect->max_particles = EdFileReadShort();
        effect->emission_period = static_cast<f32>(EdFileReadShort()) / 60.0f;
        effect->emission_period_random = static_cast<f32>(EdFileReadShort()) / 60.0f;
        effect->emission_pause = static_cast<f32>(EdFileReadShort()) / 60.0f;
        effect->emission_pause_random = static_cast<f32>(EdFileReadShort()) / 60.0f;
        effect->start_offset_random = static_cast<f32>(EdFileReadShort()) / 60.0f;
    } else {
        effect->frequency = EdFileReadShort();
        effect->max_particles = EdFileReadShort();
        effect->emission_period = EdFileReadFloat();
        effect->emission_period_random = EdFileReadFloat();
        effect->emission_pause = EdFileReadFloat();
        effect->emission_pause_random = EdFileReadFloat();
        effect->start_offset_random = EdFileReadFloat();
    }
    if (effect->frequency != 0) {
        const f32 minimum_period = 1.0f / static_cast<f32>(effect->frequency);
        if (effect->emission_period_random <= minimum_period && effect->emission_period_random != minimum_period)
            effect->emission_period_random = minimum_period;
    }

    effect->generator_type = static_cast<u8>(EdFileReadChar());
    effect->momentum_adjustment_type = static_cast<u8>(EdFileReadChar());
    if (version >= 35)
        effect->cutscene_only = static_cast<u8>(EdFileReadChar());
    else
        effect->cutscene_only = 0;
    effect->disabled = 0;
    effect->particle_type = static_cast<u8>(EdFileReadChar());
    if (version < 39)
        EdFileReadChar();
    effect->status = 1;
    if (version >= 40)
        effect->camera_facing = static_cast<u8>(EdFileReadChar());
    else
        effect->camera_facing = 0;

    *reinterpret_cast<f32 *>(effect->fields_030) = EdFileReadFloat();
    if (version >= 6) {
        effect->cut_on = EdFileReadFloat();
        effect->clip_extent = EdFileReadFloat();
    } else {
        effect->cut_on = 0.0f;
        effect->clip_extent = 25.0f;
    }
    if (version >= 10)
        effect->sound_range = EdFileReadFloat();
    else
        effect->sound_range = 0.0f;
    if (version >= 23)
        effect->sound_range_override = EdFileReadFloat();
    else
        effect->sound_range_override = 0.0f;
    if (version >= 24)
        effect->field_044 = EdFileReadFloat();
    else
        effect->field_044 = 0.5f;
    if (version < 7) {
        EdFileReadInt();
        EdFileReadInt();
    }
    effect->field_048 = EdFileReadFloat();
    effect->field_04c = EdFileReadFloat();
    effect->field_050 = EdFileReadFloat();
    effect->field_054 = EdFileReadFloat();
    if (version < 18) {
        EdFileReadFloat();
        EdFileReadFloat();
        EdFileReadFloat();
    }
    effect->field_058 = EdFileReadFloat();
    effect->field_05c = EdFileReadFloat();
    effect->field_060 = EdFileReadFloat();
    if (version < 18) {
        EdFileReadFloat();
        EdFileReadFloat();
        EdFileReadFloat();
    }
    if ((version == 18 || version == 19) && effect->generator_type == 6) {
        effect->field_054 += effect->field_054;
        effect->field_060 += effect->field_060;
    }
    if (version >= 29) {
        effect->emitter_velocity.x = EdFileReadFloat();
        effect->emitter_velocity.y = EdFileReadFloat();
        effect->emitter_velocity.z = EdFileReadFloat();
    } else {
        effect->emitter_velocity.x = effect->emitter_velocity.y = effect->emitter_velocity.z = 0.0f;
    }

    reinterpret_cast<f32 *>(effect->fields_070)[0] = EdFileReadFloat();
    reinterpret_cast<f32 *>(effect->fields_070)[1] = EdFileReadFloat();
    reinterpret_cast<f32 *>(effect->fields_070)[2] = EdFileReadFloat();
    reinterpret_cast<f32 *>(effect->fields_070)[3] = EdFileReadFloat();
    reinterpret_cast<f32 *>(effect->fields_070)[4] = EdFileReadFloat();
    reinterpret_cast<f32 *>(effect->fields_070)[5] = EdFileReadFloat();
    reinterpret_cast<f32 *>(effect->fields_070)[6] = EdFileReadFloat();
    reinterpret_cast<f32 *>(effect->fields_070)[7] = EdFileReadFloat();
    reinterpret_cast<f32 *>(effect->fields_070)[8] = EdFileReadFloat();
    reinterpret_cast<f32 *>(effect->fields_070)[9] = EdFileReadFloat();
    reinterpret_cast<f32 *>(effect->fields_070)[10] = EdFileReadFloat();
    reinterpret_cast<f32 *>(effect->fields_070)[11] = EdFileReadFloat();
    effect->field_0a0 = EdFileReadFloat();
    effect->particle_lifetime = EdFileReadFloat();
    effect->field_0a8 = EdFileReadShort();
    effect->field_0aa = static_cast<u8>(EdFileReadChar());
    effect->field_0ab = static_cast<u8>(EdFileReadChar());
    effect->field_0ac = EdFileReadFloat();
    effect->jib_x_frequency = EdFileReadFloat();
    effect->jib_x_amplitude = EdFileReadFloat();
    effect->jib_y_frequency = EdFileReadFloat();
    effect->jib_y_amplitude = EdFileReadFloat();
    if (version >= 33) {
        for (i32 i = 0; i < 8; ++i) {
            effect->colour_keys[i].time = EdFileReadFloat();
            effect->colour_keys[i].red = EdFileReadUnsignedChar();
            effect->colour_keys[i].green = EdFileReadUnsignedChar();
            effect->colour_keys[i].blue = EdFileReadUnsignedChar();
            effect->colour_keys[i].alpha = EdFileReadUnsignedChar();
        }
    } else {
        for (i32 i = 0; i < 8; ++i) {
            effect->colour_keys[i].time = EdFileReadFloat();
            effect->colour_keys[i].red = static_cast<u8>(static_cast<i32>(EdFileReadFloat()));
            effect->colour_keys[i].green = static_cast<u8>(static_cast<i32>(EdFileReadFloat()));
            effect->colour_keys[i].blue = static_cast<u8>(static_cast<i32>(EdFileReadFloat()));
        }
    }
    for (i32 i = 0; i < 8; ++i) {
        effect->alpha_keys[i].time = EdFileReadFloat();
        effect->alpha_keys[i].value = EdFileReadFloat();
    }
    if (version >= 21) {
        effect->field_140 = EdFileReadFloat();
        effect->field_144 = EdFileReadFloat();
    } else {
        effect->field_140 = 0.125f;
        effect->field_144 = 0.125f;
    }
    effect->min_size = EdFileReadFloat();
    effect->max_size = EdFileReadFloat();
    for (i32 i = 0; i < 8; ++i) {
        effect->width_keys[i].time = EdFileReadFloat();
        effect->width_keys[i].value = EdFileReadFloat();
    }
    for (i32 i = 0; i < 8; ++i) {
        effect->height_keys[i].time = EdFileReadFloat();
        effect->height_keys[i].value = EdFileReadFloat();
    }
    effect->min_rotation = EdFileReadFloat();
    effect->max_rotation = EdFileReadFloat();
    for (i32 i = 0; i < 8; ++i) {
        effect->rotation_keys[i].time = EdFileReadFloat();
        effect->rotation_keys[i].value = EdFileReadFloat();
    }
    for (i32 i = 0; i < 8; ++i) {
        effect->field_218_keys[i].time = EdFileReadFloat();
        effect->field_218_keys[i].value = EdFileReadFloat();
    }
    for (i32 i = 0; i < 8; ++i) {
        effect->field_258_keys[i].time = EdFileReadFloat();
        effect->field_258_keys[i].value = EdFileReadFloat();
    }
    effect->texture_u0 = EdFileReadFloat();
    effect->texture_v0 = EdFileReadFloat();
    effect->texture_u1 = EdFileReadFloat();
    effect->texture_v1 = EdFileReadFloat();

    if (version >= 3) {
        for (i32 i = 0; i < 8; ++i) {
            effect->collision_keys[i].time = EdFileReadFloat();
            effect->collision_keys[i].value = EdFileReadFloat();
        }
        effect->process_spheres = static_cast<u8>(EdFileReadChar());
    } else {
        effect->process_spheres = 0;
    }
    if (version >= 17)
        effect->time_group = static_cast<i8>(EdFileReadChar());
    else
        effect->time_group = 0;
    if (effect->particle_type == 7)
        effect->time_group = 2;
    if (version >= 31)
        effect->field_2f2 = static_cast<u8>(EdFileReadChar());
    else
        effect->field_2f2 = 3;
    if (version >= 32) {
        effect->use_explicit_clip_box = static_cast<u8>(EdFileReadChar());
        effect->repeat_box.x = EdFileReadFloat();
        effect->repeat_box.y = EdFileReadFloat();
        effect->repeat_box.z = EdFileReadFloat();
    } else {
        effect->use_explicit_clip_box = 0;
        effect->repeat_box.x = 1.0f;
        effect->repeat_box.y = 1.0f;
        effect->repeat_box.z = 1.0f;
    }
    if (version >= 36)
        effect->thinning = EdFileReadFloat();
    else
        effect->thinning = 4.0f;
    if (version == 36 && effect->thinning < 4.0f)
        effect->thinning = 4.0f;
    if (!(version >= 30)) {
        effect->torus_radius1 = 1.0f;
        effect->torus_radius2 = 0.1f;
        effect->torus_lifetime = 0.0f;
        effect->torus_keys1[0].time = effect->torus_keys1[0].value = 0.0f;
        effect->torus_keys1[1].time = effect->torus_keys1[1].value = 1.0f;
        effect->torus_keys2[0].time = effect->torus_keys2[0].value = 0.0f;
        effect->torus_keys2[1].time = effect->torus_keys2[1].value = 1.0f;
        effect->torus_keys3[0].time = effect->torus_keys3[0].value = 0.0f;
        effect->torus_keys3[1].time = effect->torus_keys3[1].value = 1.0f;
    } else {
        effect->torus_radius1 = EdFileReadFloat();
        effect->torus_radius2 = EdFileReadFloat();
        effect->torus_lifetime = EdFileReadFloat();
        for (i32 i = 0; i < 8; ++i) {
            effect->torus_keys1[i].time = EdFileReadFloat();
            effect->torus_keys1[i].value = EdFileReadFloat();
        }
        for (i32 i = 0; i < 8; ++i) {
            effect->torus_keys2[i].time = EdFileReadFloat();
            effect->torus_keys2[i].value = EdFileReadFloat();
        }
        for (i32 i = 0; i < 8; ++i) {
            effect->torus_keys3[i].time = EdFileReadFloat();
            effect->torus_keys3[i].value = EdFileReadFloat();
        }
    }

    for (i32 i = 0; i < 8; ++i)
        effect->particle_keys[i] = -1;
    if (version < 23) {
        if (version < 11) {
            for (i32 i = 0; i < 4; ++i) {
                effect->sound_data[i * 3] = -1;
                effect->sound_data[i * 3 + 1] = 0;
                effect->sound_data[i * 3 + 2] = 0;
            }
        } else {
            effect->sound_data[0] = EdFileReadInt();
            effect->sound_data[1] = EdFileReadInt();
            effect->sound_data[2] = EdFileReadInt();
            effect->sound_data[3] = EdFileReadInt();
            effect->sound_data[4] = EdFileReadInt();
            effect->sound_data[5] = EdFileReadInt();
            effect->sound_data[6] = EdFileReadInt();
            effect->sound_data[7] = EdFileReadInt();
            effect->sound_data[8] = EdFileReadInt();
            effect->sound_data[9] = EdFileReadInt();
            effect->sound_data[10] = EdFileReadInt();
            effect->sound_data[11] = EdFileReadInt();
        }
    } else {
        const i32 stored_sound_count = EdFileReadInt();
        const i32 sound_count = MAX(0, stored_sound_count);
        for (i32 i = 0; i < sound_count; ++i) {
            char sound_name[16];
            EdFileRead(sound_name, sizeof(sound_name));
            if (i < 4) {
                effect->sound_data[i * 3] = GetSfxIdN(sound_name, sizeof(sound_name));
                effect->sound_data[i * 3 + 1] = EdFileReadInt();
                effect->sound_data[i * 3 + 2] = EdFileReadInt();
            } else {
                GetSfxIdN(sound_name, sizeof(sound_name));
                EdFileReadInt();
                EdFileReadInt();
            }
        }
        for (i32 i = MIN(sound_count, 4); i < 4; ++i)
            effect->sound_data[i * 3] = -1;
    }
    if (version >= 16) {
        effect->trail_count = static_cast<i8>(version >= 41 ? EdFileReadChar() : EdFileReadInt());
        effect->trail_time = EdFileReadFloat();
    } else {
        effect->trail_count = 0;
        effect->trail_time = 0.0f;
    }
    if (version >= 25) {
        effect->radial_segments = static_cast<u8>(version >= 41 ? EdFileReadChar() : EdFileReadInt());
        effect->radial_floor = EdFileReadFloat();
    } else {
        effect->radial_segments = 5;
        effect->radial_floor = 0.5f;
    }
    if (version >= 26)
        effect->scale_in_time = EdFileReadFloat();
    else
        effect->scale_in_time = 0.0f;
    effect->scale = 1.0f;
    effect->unscaled_effect_index = 0;
    if (NuStrCmp(effect->name, (char *)"STARDESTROYER") == 0)
        effect->frequency = 0;
}
