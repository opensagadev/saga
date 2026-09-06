#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nuspecial.h"

#include <float.h>
#include <math.h>
#include <string.h>

extern "C" {
    // Shared suspend flag consulted by all timed debris emitters.
    i32 debris_suspended = 0;

    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern DEBRISGENERATOR gensorttab[13];
    extern DEBRISMOMENTUMADJUSTER gencodetab[7];
    extern f32 globaltime;
    extern f32 panelglobaltime;
    extern f32 timeincrement;
    f32 CameraEmitterDistance(NUVEC *);
    void SetSfxBit_On(i32);
    void PlaySfxByIdEx(i32, NUVEC *, f32, f32);
    void DebrisEmitterMomentum(i32, f32, f32, f32);
    void DebrisParticleMomentum(i32, f32, f32, f32);
    void AddFiniteShotDebrisEffect2(i32 *, i32, NUVEC *, NUVEC *, NUVEC *, i32);
    void AddVariableShotDebrisEffectMtx3(i32, NUVEC *, NUVEC *, i32, NUMTX *, NUMTX *);
    void AddVariableShotDebrisEffectMtx4(i32, NUVEC *, NUVEC *, i32, NUMTX *, NUMTX *, i16, u8);
    extern i32 debris_suspended;
    extern f32 debris_thinning_level;
    extern i32 forced_debris_thinning;
    extern i32 freedebkeyptr;
    extern i32 maxdebkeys;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern i32 freedebchkptr;
    extern i32 freedebchkptrg;
    extern dma_particle_chunk_s **freedebchunks;
    extern dma_particle_chunk_s **freedebchunksglass;
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern edpp_particle_s edpp_ptls[512];
    extern i32 edpp_page_used[8];
    extern i32 edpp_page_on[8];
    extern i32 edpp_instances_used;
    extern PART_s *Part;
    extern i32 MAXPARTS;
    extern i32 i_part;

    i32 DebAlloc(void);
    void DebrisStartOffset(i32, f32);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisOrientation(i32, i16, i16);
    void DebrisReflectionOrientation(i32, i16, i16, i16, f32);
    void DebrisSetTrigger(i32, i16, i16, i16);
    void DebrisEmitterOrientationMtx(i32, NUMTX *);
    i32 CreateScaledEffect(i32, f32);
    i32 NuCameraClipTestExtentsAxisAligned(NUVEC *, NUVEC *, f32);
    void NuVecAddScale(NUVEC *, NUVEC *, NUVEC *, f32);
    void LinkDmaParticalSets(dma_particle_chunk_s **, i32);
    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
    void AddVariableShotDebrisEffectTimed5(i32, NUVEC *, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *, i16, u8);
}

void AddDebrisEffectToStack(debkeydatatype_s *);
void AddChunkToRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
void AddChunkControlToStack(debris_chunk_control_s *, debris_chunk_control_s **);
void RemoveChunkControlFromStack(debris_chunk_control_s *, debris_chunk_control_s **);
void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
void DebrisGetControlStackLock();
void DebrisReleaseControlStackLock();
void FindAnglesZX(NUVEC *, u16 *, u16 *);
extern f32 AreaPickupGravity;
extern i16 temp_xrot;
extern i16 temp_zrot;

// Forward declarations for local (static) part/gizmo helper stubs.
struct CUSTOMPIECEANIM;
struct spacelevel_s;
struct quickboltinfo;

void PartObjectInterface::GetPos(VuVec &, i32) const {
}

void PartObjectInterface::GetTargetName() const {
}

PartObjectInterface::PartObjectInterface(PART_s &) {
}

PartObjectInterface::~PartObjectInterface() {
}

void PART_s::ClearMechObjectInterface() {
}

void PART_s::GetMechObjectInterface() {
}

// Local (static) part (PART_s), pickup (GIZMOPICKUP_s), gizmo flow
// (GIZFLOW_s/FLOWBOX_s), power-up and space/starfighter helpers. Stubbed as
// local `t` symbols matching res/libTTapp.so.

static __used__ void PartCollide(PART_s *, i32) {
}

static __used__ void TiePart_Kill(PART_s *, i32) {
}

static __used__ void TiePart_Move(PART_s *, f32) {
}

static __used__ void TiePart_Impact(PART_s *) {
}

static __used__ void TiePart_KillExplode(PART_s *, i32) {
}

static __used__ void TieSpinZPart_Move(PART_s *, f32) {
}

static __used__ void PartImpact_Coin(PART_s *) {
}

static __used__ void PartStolen_Coin(PART_s *) {
}

static __used__ void PartExtra_BlueCoin(PART_s *) {
}

static __used__ void PartExtra_PurpleCoin(PART_s *) {
}

static __used__ void PowerUp_DrawPart(PART_s *) {
}

static __used__ void PowerUp_ImpactPart(PART_s *) {
}

static __used__ void PowerUp_UpdatePart(PART_s *) {
}

static __used__ void PowerUp_EndMsg(GAMEMESSAGE_s *) {
}

static __used__ void PowerUp_UpdateMsg(GAMEMESSAGE_s *) {
}

static __used__ void SpeederPart_Draw(PART_s *) {
}

static __used__ void SpeederPart_Kill(PART_s *, i32) {
}

static __used__ void SpeederPart_Update(PART_s *) {
}

static __used__ void SuperCarry_PartKill(PART_s *, i32) {
}

static __used__ void SuperCarry_PartImpact(PART_s *) {
}

static __used__ void SuperCarry_TurnBlowupBackOn(GIZMOBLOWUP_s *, nuvec_s *, u16, i32) {
}

static __used__ void PartDraw_VehicleHeart(PART_s *) {
}

static __used__ void PartKill_DrawCreature(PART_s *) {
}

static __used__ void PartMove_VehicleHeart(PART_s *, f32) {
}

static __used__ void PartMove_VehiclePickup(PART_s *, f32) {
}

static __used__ void PartKill_EjectedCreature(PART_s *, i32) {
}

static __used__ void UpdateAnimTimer(CHARACTERMODEL_s *, ANIMPACKET_s *, i16, f32, f32, f32, i32, char *, i32, f32) {
}

static __used__ void UpdateCustomPieceAnim(CUSTOMPIECEANIM *, u16, u16) {
}

extern "C" {

    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z) {
        if (handle == NULL || effect_index < 0 || EDPP_MAX_TYPES <= effect_index || debtab == NULL ||
            debtab[effect_index] == NULL) {
            return;
        }
        debinftype *effect = debtab[effect_index];
        if (effect->disabled != 0) {
            return;
        }

        bool newly_allocated = false;
        i32 key_index = *handle;
        if (key_index == -1) {
            key_index = DebAlloc();
            *handle = key_index;
            if (key_index == -1) {
                return;
            }
            newly_allocated = true;
        }

        debkeydatatype_s &key = debkeydata[key_index];
        const f32 now = effect->time_group == 4 ? panelglobaltime : globaltime;
        key.field_184 = 0;
        key.effect_index = static_cast<i16>(effect_index);
        DebrisStartOffset(key_index, effect->emission_period);
        key.generator = gensorttab[static_cast<i8>(effect->generator_type)];
        key.momentum_adjuster = gencodetab[static_cast<i8>(effect->momentum_adjustment_type)];
        key.field_1d4 = 0;
        key.field_2cc = 0;
        key.emitter_rotation_x = 0;
        key.emitter_rotation_y = 0;
        key.field_2c8 = 0;
        for (i32 i = 0; i < effect->process_spheres; ++i) {
            key.process_spheres[i].time = -1.0f;
        }
        memset(&key.emission_position, 0, sizeof(key.emission_position));
        memset(&key.momentum, 0, sizeof(key.momentum));
        memset(&key.emitter_momentum, 0, sizeof(key.emitter_momentum));
        key.orientation_dirty = 0.0f;
        key.cutoff_distance = 1000000.0f;
        key.field_2f2 = -1;
        key.field_1d8 = 0;
        key.field_1da = 7;
        key.gscene = NULL;
        key.process_collision_sound = 0;
        key.last_update_time = now;
        key.field_2f9 = 1;
        key.field_32c = 0;
        key.field_2fa = 0;
        key.emission_epoch = key.field_1e4 < now ? key.field_1e4 : now;

        switch (effect->particle_type) {
            case 3:
                key.render_priority = static_cast<i16>(40000);
                break;
            case 7:
                key.render_priority = static_cast<i16>(20000);
                break;
            case 2:
                key.render_priority = static_cast<i16>(50000);
                break;
            default:
                key.render_priority = static_cast<i16>(30000);
                break;
        }
        for (i32 i = 0; i != 4; ++i) {
            key.collision_timers[i] = 9999;
            const i32 sound_id = effect->sound_data[i * 3];
            if (sound_id != -1) {
                const i32 mode = effect->sound_data[i * 3 + 1];
                if (mode == 3 || mode == 4) {
                    key.collision_timers[i] = 1;
                }
                key.process_collision_sound = 1;
            }
        }
        DebrisEmitterPos(key_index, x, y, z);
        DebrisEmitterOrientation(key_index, 0, 0, 0);
        DebrisOrientation(key_index, 0, 0);
        DebrisReflectionOrientation(key_index, 0, 0, 0, 0.9f);
        DebrisSetTrigger(key_index, 0, -1, 0);
        if (newly_allocated) {
            AddDebrisEffectToStack(debkeydata + key_index);
        }
    }

    void AddFiniteShotDebrisEffect(i32 *handle, i32 effect, NUVEC *position, i32 count) {
        AddFiniteShotDebrisEffect2(handle, effect, position, NULL, NULL, count);
    }

    void AddFiniteShotDebrisEffect2(i32 *handle, i32 effect, NUVEC *position,
                                  NUVEC *emitter_momentum, NUVEC *particle_momentum, i32 count) {
        if (debris_suspended != 0) return;
        AddDebrisEffect(handle, effect, position->x, position->y, position->z);
        if (*handle == -1) return;
        debkeydatatype_s &key = debkeydata[*handle];
        key.field_1d4 = count;
        const f32 now = debtab[effect]->time_group == 4 ? panelglobaltime : globaltime;
        key.field_184 = 1;
        key.field_1d8 = 0;
        const f32 offset = key.emission_time - now;
        key.emission_time -= offset;
        key.field_1e4 -= offset;
        key.emission_epoch = key.last_update_time;
        key.cutoff_distance = CameraEmitterDistance(position);
        if (key.process_collision_sound != 0) {
            f32 range = debtab[effect]->sound_range_override;
            if (range == 0.0f) range = debtab[effect]->sound_range;
            if (range == 0.0f) range = debtab[effect]->clip_extent;
            f32 volume = 0.0f;
            if (key.cutoff_distance < range) {
                for (i32 i = 0; i < 4; ++i) {
                    const i32 sound = debtab[effect]->sound_data[i * 3];
                    if (sound != -1) SetSfxBit_On(sound);
                }
                volume = (range - key.cutoff_distance) / range;
            }
            if (key.cutoff_distance < range) {
                for (i32 i = 0; i < 4; ++i) {
                    const i32 sound = debtab[effect]->sound_data[i * 3];
                    if (sound != -1 && debtab[effect]->sound_data[i * 3 + 1] == 1) {
                        PlaySfxByIdEx(sound, position, volume, 1.0f);
                    }
                }
            }
        }
        if (emitter_momentum != NULL) {
            DebrisEmitterMomentum(*handle, emitter_momentum->x, emitter_momentum->y, emitter_momentum->z);
        }
        if (particle_momentum != NULL) {
            DebrisParticleMomentum(*handle, particle_momentum->x, particle_momentum->y, particle_momentum->z);
        }
    }

    void AddFiniteShotDebrisEffectUserData(void) {
    }

    void AddFiniteShotPART(i32, void *, i32) {
    }

    void *AddGameDebris(APIDEBRISSYS_s *, i32, void *) {
        return NULL;
    }

    void AddGameDebrisMom(void) {
    }

    void AddGameDebrisMomentum(void) {
    }

    void AddGameDebrisMtx(void) {
    }

    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

    i32 AddGameDebrisRot(APIDEBRISSYS_s *system, i32 effect, NUVEC *position, i32 count,
                         i16 z_rotation, i16 y_rotation) {
        if (effect < 0 || effect >= system->capacity || system->entries[effect].effect == -1 || count < 1) {
            return 0;
        }
        AddVariableShotDebrisEffect(system->entries[effect].effect, position, count, z_rotation, y_rotation);
        return 1;
    }

    void AddGameDebrisXYZ(void) {
    }

    void AddMSituExtraTerrRot(void) {
    }

    void AddPARTEffect(void) {
    }

    void AddPart(void) {
    }

    void AddPickupTerr(void) {
    }

    void AddPickupTerrRot(void) {
    }

    void AddRotatedDebrisEffect(void) {
    }

    void AddScaledFiniteShotDebrisEffect(i32 *, i32, NUVEC *, i32, i32, i32, f32) {
    }

    void AddScaledFiniteShotPART(void) {
    }

    void AddScaledVariableShotDebrisEffect(void) {
    }

    void AddScaledVariableShotDebrisEffect1(void) {
    }

    i32 AddScaledVariableShotDebrisEffect2(i32 effect_index, NUVEC *position, i32 count, f32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation, f32 scale) {
        const i32 scaled_effect = CreateScaledEffect(effect_index, scale);
        if (scaled_effect != -1) {
            AddVariableShotDebrisEffectTimed3(scaled_effect, position, &nuvec_zero, count, time, emitter_orientation,
                                              particle_orientation);
        }
        return scaled_effect;
    }

    void AddScaledVariableShotDebrisEffect3(void) {
    }

    void AddScaledVariableShotDebrisEffect4(void) {
    }

    void AddScaledVariableShotDebrisEffect5(void) {
    }

    void AddScaledVariableShotPARTEffect(void) {
    }

    void AddVariableShotDebrisEffectMtx(i32, NUVEC *, i32, i16, i16, NUMTX *);

    void AddVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation) {
        AddVariableShotDebrisEffectMtx(effect, position, count, z_rotation, y_rotation, NULL);
    }

    void AddVariableShotDebrisEffectMtx(i32 effect, NUVEC *position, i32 count, i16 z_rotation,
                                      i16 y_rotation, NUMTX *particle_orientation) {
        NUMTX orientation;
        NuMtxSetIdentity(&orientation);
        NuMtxRotateZ(&orientation, z_rotation);
        NuMtxRotateY(&orientation, y_rotation);
        AddVariableShotDebrisEffectMtx3(effect, position, &nuvec_zero, count, &orientation, particle_orientation);
    }

    void AddVariableShotDebrisEffectMtx3(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                       NUMTX *emitter_orientation, NUMTX *particle_orientation) {
        if (effect < 0 || debtab[effect] == NULL) return;
        i16 priority = 20000;
        switch (debtab[effect]->particle_type) {
            case 2: priority = static_cast<i16>(40000); break;
            case 3: priority = 30000; break;
            case 7: priority = 10000; break;
        }
        AddVariableShotDebrisEffectMtx4(effect, position, momentum, count, emitter_orientation,
                                       particle_orientation, priority, 0);
    }

    void AddVariableShotDebrisEffectMtx4(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                       NUMTX *emitter_orientation, NUMTX *particle_orientation,
                                       i16 priority, u8 flags) {
        AddVariableShotDebrisEffectTimed5(effect, position, momentum, NULL, count * 30, timeincrement,
                                         emitter_orientation, particle_orientation, priority, flags);
    }

    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);

    void AddVariableShotDebrisEffectTimed1(i32 effect, NUVEC *position, i32 count, f32 time,
                                         i16 z_rotation, i16 y_rotation, NUMTX *particle_orientation) {
        NUMTX orientation;
        NuMtxSetIdentity(&orientation);
        NuMtxRotateZ(&orientation, z_rotation);
        NuMtxRotateY(&orientation, y_rotation);
        AddVariableShotDebrisEffectTimed3(effect, position, &nuvec_zero, count, time,
                                         &orientation, particle_orientation);
    }

    void AddVariableShotDebrisEffectTimed3(i32 effect_index, NUVEC *position, NUVEC *momentum, i32 count, f32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation) {
        if (effect_index < 0 || debtab[effect_index] == NULL) {
            return;
        }

        i16 render_priority = 20000;
        switch (debtab[effect_index]->particle_type) {
            case 2:
                render_priority = 40000;
                break;
            case 3:
                render_priority = 30000;
                break;
            case 4:
                render_priority = 20000;
                break;
            case 5:
                render_priority = 20000;
                break;
            case 6:
                render_priority = 20000;
                break;
            case 7:
                render_priority = 10000;
                break;
        }
        AddVariableShotDebrisEffectTimed5(effect_index, position, momentum, NULL, count, time, emitter_orientation,
                                          particle_orientation, render_priority, 0);
    }

    void AddVariableShotDebrisEffectTimed5(i32 effect_index, NUVEC *position, NUVEC *momentum, NUVEC *position_delta,
                                           i32 count, f32 duration, NUMTX *emitter_orientation,
                                           NUMTX *particle_orientation, i16 render_priority, u8 timed_flags) {
        if (debris_suspended != 0 || effect_index < 1 || EDPP_MAX_TYPES <= effect_index || debtab == NULL ||
            debtab[effect_index] == NULL || count < 1) {
            return;
        }

        debinftype *effect = debtab[effect_index];
        if (effect->disabled != 0) {
            return;
        }

        if (effect->time_group != 4) {
            NUVEC extent = {1.0f, 1.0f, 1.0f};
            if (NuCameraClipTestExtentsAxisAligned(position, &extent, effect->clip_extent) == 0) {
                return;
            }
        }

        f32 emission_interval = 0.0f;
        bool no_interval = true;
        const f32 thinning =
            forced_debris_thinning == 0
                ? (debris_thinning_level <= effect->thinning ? debris_thinning_level : effect->thinning)
                : debris_thinning_level;
        if (thinning != 0.0f && count != 0) {
            emission_interval = thinning / static_cast<f32>(count);
            no_interval = emission_interval == 0.0f;
        }

        if (emitter_orientation == NULL) {
            emitter_orientation = &numtx_identity;
        }
        if (particle_orientation == NULL) {
            particle_orientation = &numtx_identity;
        }

        const bool panel_time = effect->time_group == 4;
        const f32 now = panel_time ? panelglobaltime : globaltime;
        const f32 end_time = now + duration;
        const f32 elapsed_intervals = no_interval ? 0.0f : floorf(now / emission_interval);
        f32 emission_time = emission_interval + elapsed_intervals * emission_interval;
        if (end_time < emission_time) {
            return;
        }

        i32 emission_count = 0;
        f32 next_emission_time = emission_time;
        do {
            next_emission_time += emission_interval;
            ++emission_count;
        } while (next_emission_time <= end_time && emission_count != 99);

        const i32 particle_count = emission_count * (static_cast<i32>(effect->trail_count) + 1);
        const i32 particles_per_chunk = effect->particle_type == 7 ? 12 : 32;
        const i32 maximum_particles = effect->particle_type == 7 ? 0x180 : 0x400;

        i32 particle_key_slot = -1;
        debkeydatatype_s *key = NULL;
        for (i32 slot = 0; slot != 8; ++slot) {
            const i16 key_index = effect->particle_keys[slot];
            if (key_index == -1) {
                continue;
            }
            debkeydatatype_s *candidate = &debkeydata[key_index];
            if (candidate->particle_count + particle_count <= maximum_particles) {
                particle_key_slot = slot;
                key = candidate;
                break;
            }
        }

        if (key == NULL) {
            if (freedebkeyptr >= maxdebkeys) {
                return;
            }
            for (i32 slot = 0; slot != 8; ++slot) {
                if (effect->particle_keys[slot] == -1) {
                    const i32 key_index = DebAlloc();
                    if (key_index == -1) {
                        return;
                    }
                    particle_key_slot = slot;
                    effect->particle_keys[slot] = static_cast<i16>(key_index);
                    key = &debkeydata[key_index];
                    key->effect_index = static_cast<i16>(effect_index);
                    key->field_1d4 = 0;
                    key->generator = gensorttab[effect->generator_type];
                    key->momentum_adjuster = gencodetab[effect->momentum_adjustment_type];
                    key->effect_orientation = *particle_orientation;
                    key->effect_orientation.m30 = 0.0f;
                    key->effect_orientation.m31 = 0.0f;
                    key->effect_orientation.m32 = 0.0f;
                    DebrisEmitterPos(key_index, 0.0f, 0.0f, 0.0f);
                    key->timed_flags = timed_flags;
                    key->render_priority = render_priority;
                    break;
                }
            }
            if (key == NULL) {
                return;
            }
        }

        const i32 required_particles = key->particle_count + particle_count;
        const i32 required_chunks = (required_particles + particles_per_chunk - 1) / particles_per_chunk;
        i32 allocated_chunks = key->allocated_chunk_count;
        if (required_chunks > allocated_chunks) {
            const i32 new_chunk_count = required_chunks - allocated_chunks;
            i32 &free_chunk_count = effect->particle_type == 7 ? freedebchkptrg : freedebchkptr;
            const i32 available_chunk_count = effect->particle_type == 7 ? debrischunksglass : debrischunks;
            dma_particle_chunk_s **free_chunks = effect->particle_type == 7 ? freedebchunksglass : freedebchunks;
            if (available_chunk_count <= free_chunk_count + new_chunk_count || required_chunks > 32) {
                return;
            }

            for (i32 i = 0; i != new_chunk_count; ++i) {
                dma_particle_chunk_s *chunk = free_chunks[free_chunk_count + i];
                key->particle_chunks[allocated_chunks + i] = chunk;
                for (i32 particle = 0; particle != particles_per_chunk; ++particle) {
                    chunk->particles[particle].start_time = 0.0f;
                    chunk->particles[particle].inverse_lifetime = 32768.0f;
                }
            }
            free_chunk_count += new_chunk_count;
            key->allocated_chunk_count = static_cast<i16>(required_chunks);
            LinkDmaParticalSets(key->particle_chunks, required_chunks);

            // The renderer needs one entry per contiguous DMA chain, created
            // when the key receives its first chunk; subsequent growth merely
            // relinks that same chain.
            if (required_chunks == new_chunk_count) {
                const i32 total_chunk_count = debrischunks + debrischunksglass;
                particlechunkrendertype_s *render_chunk = NULL;
                for (i32 i = 0; i < total_chunk_count; ++i) {
                    if (ParticleChunkToRender[i].particle_chunk == NULL) {
                        render_chunk = &ParticleChunkToRender[i];
                        break;
                    }
                }
                if (render_chunk != NULL) {
                    render_chunk->particle_chunk = key->particle_chunks[0];
                    render_chunk->effect = effect;
                    render_chunk->key = key;
                    render_chunk->render_priority = render_priority;
                    AddChunkToRenderStack(render_chunk, &ParticleChunkRenderStack[effect->time_group]);
                }
            }
            allocated_chunks = required_chunks;
        }

        key->particle_count = static_cast<i16>(required_particles);
        if (momentum == NULL) {
            key->momentum = nuvec_zero;
        } else {
            key->momentum = *momentum;
        }
        DebrisEmitterOrientationMtx(effect->particle_keys[particle_key_slot], emitter_orientation);
        key->emission_epoch = elapsed_intervals * emission_interval;

        for (i32 i = 0; i != 100 && emission_time <= end_time; ++i) {
            if (position_delta == NULL) {
                key->emission_position = *position;
            } else {
                NuVecAddScale(&key->emission_position, position, position_delta, emission_time - end_time);
            }
            key->emission_time = emission_time;
            uv1deb *particle = key->generator(key, effect, emission_time);
            if (effect->process_spheres != 0 && particle != NULL && i == 0) {
                DebrisProcessSpheres(particle, emission_time, effect, key, 1);
            }
            emission_time = key->emission_epoch + emission_interval;
        }

        DebrisGetControlStackLock();
        while (key->controlled_chunk_count < key->allocated_chunk_count &&
               freechunkcontrolsptr < (debrischunks + debrischunksglass) * 2) {
            const i32 chunk_index = key->controlled_chunk_count;
            debris_chunk_control_s *control = freechunkcontrols[freechunkcontrolsptr++];
            control->particle_chunk = key->particle_chunks[chunk_index];
            control->active = 1;
            control->owner = key;
            control->expiry_time =
                now + effect->particle_lifetime + static_cast<f32>(effect->trail_count) * effect->trail_time;
            AddChunkControlToStack(control, &debris_chunk_control_stack[panel_time ? 1 : 0]);
            ++key->controlled_chunk_count;
        }

        if (key->controlled_chunk_count == key->allocated_chunk_count && key->controlled_chunk_count != 0) {
            debris_chunk_control_s **stack = &debris_chunk_control_stack[panel_time ? 1 : 0];
            const dma_particle_chunk_s *last_chunk = key->particle_chunks[key->controlled_chunk_count - 1];
            for (debris_chunk_control_s *control = *stack; control != NULL; control = control->next) {
                if (control->particle_chunk == last_chunk) {
                    RemoveChunkControlFromStack(control, stack);
                    control->expiry_time =
                        now + effect->particle_lifetime + static_cast<f32>(effect->trail_count) * effect->trail_time;
                    AddChunkControlToStack(control, stack);
                    break;
                }
            }
        }
        DebrisReleaseControlStackLock();

        key->previous_particle_count = key->particle_count;
        key->previous_allocated_chunk_count = key->allocated_chunk_count;
    }

    void AddVariableShotPARTEffect(void) {
    }

    void CastPart(void) {
    }

    void CheckPartCount(void) {
    }

    void DrawParts(i32) {
    }

    void FindPart(void) {
    }

    void GetMaxPartTypes(void) {
    }

    void GetPartCount(void) {
    }

    void GetPartName(void) {
    }

    void InitParts(i32, VARIPTR *, VARIPTR) {
    }

    void KillAllParts(void) {
    }

    void KillPart(void) {
    }

    void KillPartsByScene(void) {
    }

    void NewPartRotation(PART_s *) {
    }

    void PARTEmitterOrientation(void) {
    }

    void PARTEmitterPos(void) {
    }

    void PARTGetTotalOffTime(void) {
    }

    void PARTGetTotalOnTime(void) {
    }

    i32 PARTLookupType(char *) {
        return -1;
    }

    void PARTLookupTypePageOnly(void) {
    }

    void PARTStartOffset(void) {
    }

    void PartPlatId(void) {
    }

    void ParticleReset(void) {
        for (i32 i = 0; i < 512; ++i) {
            edpp_ptls[i].instance_id = -1;
        }
        memset(edpp_page_used, 0, sizeof(edpp_page_used));
        memset(edpp_page_on, 0, sizeof(edpp_page_on));
        edpp_instances_used = 0;
    }

    void ReassignPickupInst(void) {
    }

    void RemovePARTEffect(void) {
    }

    void ResetParts(void) {
        if (Part != NULL) {
            // Original multiplies by 0x224; PART_s layout is still opaque here.
            memset(Part, 0, static_cast<usize>(MAXPARTS) * 0x224);
        }
        i_part = 0;
    }

    void UpdateParts(f32) {
    }

} // extern "C"

void TargetPart(GameObject_s *, nuvec_s *, nuvec_s *, float, float, i32, i32) {
}

void PartTimeSlip() {
}

void InitPartTable(char **) {
}

void SetPartTarget(GameObject_s *, PART_s *) {
}

void AddHeartAsPart(GameObject_s *, nuvec_s *, nuvec_s *, float, float) {
}

void FindPartDebris(PARTDEBSYS_s *, char *) {
}

void MakePartVector(NUVEC *velocity, NUVEC *direction, f32 scale) {
    if (direction == NULL) {
        f32 random = static_cast<f32>(qrand()) * (1.0f / 65535.0f);
        velocity->x = random + random - 1.0f;
        velocity->y = AreaPickupGravity == 0.0f ? 0.0f : static_cast<f32>(qrand()) * (1.0f / 65535.0f) + 1.0f;
        random = static_cast<f32>(qrand()) * (1.0f / 65535.0f);
        velocity->z = random + random - 1.0f;
        NuVecScale(velocity, velocity, AreaPickupScale);
    } else {
        FindAnglesZX(direction, NULL, NULL);
        *velocity = v010;
        const i32 z_random = qrand();
        const i32 x_random = qrand();
        NuVecRotateZ(velocity, velocity, (z_random / 6 + static_cast<u16>(temp_zrot) - 0x1555) & 0xffff);
        NuVecRotateX(velocity, velocity, (x_random / 6 + static_cast<u16>(temp_xrot) - 0x1555) & 0xffff);
        NuVecScale(velocity, velocity, scale);
    }
}

void PartCollide_2D(PART_s *) {
}

void PartCollide_3D(PART_s *) {
}

void SetKillPartMom(nuvec_s *) {
}

void AddCoinsAsParts(i32, nuvec_s *, nuvec_s *, float, float) {
}

void UpdatePartEmits(float) {
}

i32 LineIntersectSphere(NUVEC *, NUVEC *, NUVEC *, f32, f32 *);

PART_s *FindIncomingPart(void *owner, NUVEC *position, f32 radius, u32 flags, f32 range) {
    PART_s *nearest = NULL;
    f32 nearest_distance = range > 0.0f ? range : 100.0f;
    for (i32 i = 0; i < MAXPARTS; ++i) {
        PART_s *part = &Part[i];
        if ((part->active & 1) == 0 || part->owner == owner || (part->flags & flags) != flags) continue;
        if (range <= 0.0f) {
            f32 speed = NuVecMag(&part->velocity);
            if (speed != 0.0f) {
                NUVEC direction;
                NuVecScale(&direction, &part->velocity, 1.0f / speed);
                f32 distance = NuVecDistSqr(&part->position, position, NULL);
                if (distance < speed * 0.5f * speed * 0.5f &&
                    LineIntersectSphere(&part->position, &direction, position, radius * radius, NULL)) {
                    nearest_distance = distance;
                    nearest = part;
                }
            }
        } else {
            f32 distance = NuVecDistSqr(&part->position, position, NULL);
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest = part;
            }
        }
    }
    return nearest;
}

void InstantKillParts(GameObject_s *, i32, float) {
}

void PartCleanupTypes() {
}

void PartImpact_Brick(PART_s *) {
}

void PartUpdate_Heart(PART_s *) {
}

void Asteroid_PartKill(PART_s *, i32) {
}

void PartDraw_Flickerer(PART_s *) {
}

void PartKill_ForceThrow(PART_s *, i32) {
}

void PartImpact_Basketball(PART_s *) {
}

void PartUpdate_Basketball(PART_s *) {
}

PART_s *Part_FindFromHSpecial(nuhspecial_s *special) {
    if (special != NULL) {
        for (i32 i = 0; i < MAXPARTS; ++i) {
            if ((Part[i].active & 1) != 0 && NuSpecialCompare(&Part[i].special, special)) return &Part[i];
        }
    }
    return NULL;
}

void NewPartOrderedRotation(PART_s *) {
}

void KillParts(GameObject_s *, i32, i32, i32, float, i32, u16 *) {
}
