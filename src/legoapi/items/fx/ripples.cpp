// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "nu2api/numath/numtx.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/world/world.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"

#include <string.h>
#include "nu2api/nu3d/nutex.h"
#include "legoapi/items/collect/torpedo.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <stdio.h>
#include <math.h>
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "gameframework/saveload.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/mission.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/items/fx/ripples.h"
#include "legoapi/render/fx/game_deb.h"

static RIPPLEEFFECT_s *RE_rippleeffect;
static WORLDINFO_s *RE_worldinfo;
static ripple_node_s **eraselist;
static i32 erasecount;
static void RE_end_radius(NUFPAR *parser);
static void RE_start_radius(NUFPAR *parser);
static void RE_life(NUFPAR *parser);
static void RE_start_colour(NUFPAR *parser);
static void RE_end_colour(NUFPAR *parser);
static void RE_texture_name(NUFPAR *parser);
static void RE_effect_type(NUFPAR *parser);
static NUFPCOMJMP RippleEffect_ConfigKeywords[] = {{"texture_name", RE_texture_name},
                                                   {"effect_type", RE_effect_type},
                                                   {"start_colour", RE_start_colour},
                                                   {"end_colour", RE_end_colour},
                                                   {"life", RE_life},
                                                   {"start_radius", RE_start_radius},
                                                   {"end_radius", RE_end_radius},
                                                   {NULL, NULL}};

struct ripple_node_s;
struct ripple_set_s;

#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif
static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");

static void RE_end_radius(NUFPAR *parser) {
    RE_rippleeffect->end_size = NuFParGetFloat(parser);
}

static void RE_start_radius(NUFPAR *parser) {
    RE_rippleeffect->initial_size = NuFParGetFloat(parser);
}

static void RE_life(NUFPAR *parser) {
    RE_rippleeffect->lifetime = NuFParGetFloat(parser);
}

static void RE_end_colour(NUFPAR *parser) {
    RE_rippleeffect->end_color.r = NuFParGetInt(parser);
    RE_rippleeffect->end_color.g = NuFParGetInt(parser);
    RE_rippleeffect->end_color.b = NuFParGetInt(parser);
    RE_rippleeffect->end_color.a = NuFParGetInt(parser);
}

static void RE_start_colour(NUFPAR *parser) {
    RE_rippleeffect->start_color.r = NuFParGetInt(parser);
    RE_rippleeffect->start_color.g = NuFParGetInt(parser);
    RE_rippleeffect->start_color.b = NuFParGetInt(parser);
    RE_rippleeffect->start_color.a = NuFParGetInt(parser);
}

static void RE_texture_name(NUFPAR *parser) {
    if (NuFParGetWord(parser) && NuStrLen(parser->word_buf) <= 15)
        NuStrCpy(RE_rippleeffect->texture_name, parser->word_buf);
}

void RippleEffects_Configure(WORLDINFO_s *world, char *config) {
    world->ripple_effects = NULL;
    world->ripple_effect_count = 0;
    NUFPAR *parser = NuFParCreateMem("rippleeffects", config, 0xffff);
    if (parser == NULL)
        return;
    world->giz_buffer.addr = (world->giz_buffer.addr + 3) & ~static_cast<usize>(3);
    world->ripple_effects = reinterpret_cast<RIPPLEEFFECT_s *>(world->giz_buffer.addr);
    RIPPLEEFFECT_s *effect = world->ripple_effects;
    i32 active = 0;
    NuFParPushCom(parser, RippleEffect_ConfigKeywords);
    while (NuFParGetLine(parser)) {
        NuFParGetWord(parser);
        if (parser->word_buf[0] != 0) {
            if (active) {
                if (NuStrICmp(parser->word_buf, "rippleeffects_end") == 0) {
                    active = 0;
                    if (effect->texture_name[0] != 0) {
                        ++effect;
                        ++world->ripple_effect_count;
                    }
                } else {
                    NuFParInterpretWord(parser);
                }
            } else {
                if (world->ripple_effect_count > 1 || NuStrICmp(parser->word_buf, "rippleeffects_start") != 0)
                    continue;
                RE_worldinfo = world;
                effect->lifetime = 2.0f;
                effect->initial_size = 0.0f;
                effect->end_size = 1.0f;
                effect->texture_name[0] = 0;
                effect->start_color.r = 64;
                effect->start_color.g = 64;
                effect->start_color.b = 64;
                effect->start_color.a = 255;
                effect->end_color.r = 0;
                effect->end_color.g = 0;
                effect->end_color.b = 0;
                effect->end_color.a = 0;
                effect->material = NULL;
                RE_rippleeffect = effect;
                active = 1;
            }
        }
    }
    NuFParDestroy(parser);
    if (world->ripple_effect_count > 0) {
        world->giz_buffer.addr = (reinterpret_cast<usize>(world->ripple_effects + world->ripple_effect_count) + 15) &
                                 ~static_cast<usize>(15);
        for (i32 i = 0; i < world->ripple_effect_count; ++i)
            InitRippleMtl(world->ripple_effects[i].texture_name, &world->ripple_effects[i].material,
                          &world->giz_buffer, &world->unknown_0108);
    } else {
        world->ripple_effects = NULL;
    }
}

i32 LookupRippleEffectIndex(char *name) {
    if (NuStrICmp("water", name) == 0)
        return 0;
    return NuStrICmp("forcefield", name) == 0 ? 1 : -1;
}

static void RE_effect_type(NUFPAR *parser) {
    if (NuFParGetWord(parser)) {
        i32 index = LookupRippleEffectIndex(parser->word_buf);
        if (index != -1)
            RE_worldinfo->ripple_effect_indices[index] = RE_worldinfo->ripple_effect_count;
    }
}

void ResetRippleSet(ripple_set_s *set) {
    u16 count = set->count;
    ripple_node_s *nodes = set->nodes;

    memset(nodes, 0, count * sizeof(*nodes));
    memset(&set->reset_state, 0, sizeof(set->reset_state));
    set->current = NULL;
    set->field_0x0c = NULL;
    set->field_0x10 = NULL;
    set->nodes = nodes;
    set->count = count;

    if (nodes != NULL) {
        for (i32 i = 1; i < count - 1; ++i) {
            nodes[i].next = &nodes[i + 1];
            nodes[i].previous = &nodes[i - 1];
        }
        nodes[0].next = &nodes[1];
        nodes[0].previous = &nodes[count - 1];
        nodes[count - 1].next = &nodes[0];
        nodes[count - 1].previous = &nodes[count - 2];
    }

    set->current = nodes;
}

void InitRipples(ripple_set_s **result, variptr_u *buf, variptr_u *, i32 count) {
    ripple_set_s *set = reinterpret_cast<ripple_set_s *>((buf->addr + 3) & ~3u);
    buf->addr = reinterpret_cast<usize>(set + 1);
    *result = set;

    usize node_bytes;
    usize effect_bytes;
    i32 node_count;
    if (count < 1) {
        effect_bytes = 0;
        node_bytes = 0;
        node_count = 0;
    } else if (count < 0x40) {
        node_bytes = static_cast<usize>(count) * sizeof(ripple_node_s);
        effect_bytes = static_cast<usize>(count) * sizeof(*eraselist);
        node_count = count;
    } else {
        effect_bytes = 0x100;
        node_bytes = 0x2000;
        node_count = 0x40;
    }

    set->count = static_cast<u16>(node_count);
    set->free_count = 0;

    ripple_node_s *nodes = reinterpret_cast<ripple_node_s *>((buf->addr + 0x7f) & ~0x7fu);
    buf->addr = reinterpret_cast<usize>(nodes) + node_bytes;

    if (nodes != NULL) {
        for (i32 i = 1; i < node_count - 1; ++i) {
            nodes[i].next = &nodes[i + 1];
            nodes[i].previous = &nodes[i - 1];
        }
        nodes[0].next = &nodes[1];
        nodes[0].previous = &nodes[node_count - 1];
        nodes[node_count - 1].next = &nodes[0];
        nodes[node_count - 1].previous = &nodes[node_count - 2];
    }

    eraselist = reinterpret_cast<ripple_node_s **>((buf->addr + 3) & ~3u);
    buf->addr = reinterpret_cast<usize>(eraselist) + effect_bytes;
    (*result)->nodes = nodes;
    (*result)->current = (*result)->nodes;
    (*result)->field_0x0c = NULL;
    (*result)->field_0x10 = NULL;
}

void AddRipple(ripple_set_s *set, numtx_s *matrix, float size, float growth, float lifetime, float delay,
               RGBA start_color, RGBA end_color, i32 flags, numtl_s *material, nuvec_s *velocity) {
    if (material == NULL)
        return;
    if (set == NULL)
        return;
    ripple_node_s *newest = set->newest;
    u16 active_count = set->active_count;
    ripple_node_s *node = set->free_head;
    if (!(active_count < set->count)) {
        node = set->oldest;
        set->oldest = node->next;
        set->newest = node;
    } else {
        ripple_node_s *next_free;
        if (node != node->next) {
            node->next->previous = node->previous;
            node->previous->next = node->next;
            next_free = node->next;
        } else {
            next_free = NULL;
        }
        if (newest == NULL) {
            node->next = node;
            node->previous = node;
        } else {
            ripple_node_s *next = newest->next;
            node->next = next;
            newest->next = node;
            next->previous = node;
            node->previous = newest;
        }
        set->free_head = next_free;
        ++active_count;
        set->active_count = active_count;
        set->newest = node;
        if (active_count == set->count)
            set->free_head = NULL;
        if (set->oldest == NULL)
            set->oldest = node;
    }
    node->material = material;
    node->matrix = *matrix;
    node->initial_size = size;
    node->size = size;
    node->flags = static_cast<u16>(flags);
    node->lifetime = lifetime;
    node->start_color = start_color;
    node->growth = growth;
    node->end_color = end_color;
    node->color = start_color;
    node->age = 0.0f;
    node->delay = delay;
    if (velocity != NULL)
        node->velocity = *velocity;
    else
        node->velocity = v000;
}

void UpdateRippleSet(ripple_set_s *set) {
    if (set == NULL)
        return;
    ripple_node_s *node = set->newest;
    for (i32 i = 0; i < set->active_count; ++i) {
        if (node == NULL)
            continue;
        ripple_node_s *next = node->next;
        if (!(!(node->lifetime >= node->age))) {
            if (!(node->delay > 0.0f)) {
                f32 ratio = node->age / node->lifetime;
                if ((node->flags & 1) != 0)
                    node->size = node->initial_size + (node->growth - node->initial_size) * ratio;
                if ((node->flags & 2) != 0)
                    node->color.a = static_cast<u8>(
                        static_cast<f32>(static_cast<i32>(node->start_color.a)) +
                        static_cast<f32>(static_cast<i32>(node->end_color.a) - node->start_color.a) * ratio);
                if ((node->flags & 4) != 0) {
                    node->color.r = static_cast<u8>(
                        static_cast<f32>(static_cast<i32>(node->start_color.r)) +
                        static_cast<f32>(static_cast<i32>(node->end_color.r) - node->start_color.r) * ratio);
                    node->color.g = static_cast<u8>(
                        static_cast<f32>(static_cast<i32>(node->start_color.g)) +
                        static_cast<f32>(static_cast<i32>(node->end_color.g) - node->start_color.g) * ratio);
                    node->color.b = static_cast<u8>(
                        static_cast<f32>(static_cast<i32>(node->start_color.b)) +
                        static_cast<f32>(static_cast<i32>(node->end_color.b) - node->start_color.b) * ratio);
                }
                if ((node->flags & 8) != 0) {
                    node->matrix.m30 += node->velocity.x;
                    node->matrix.m31 += node->velocity.y;
                    node->matrix.m32 += node->velocity.z;
                }
            } else {
                node->delay -= FRAMETIME;
            }
            node->age += FRAMETIME;
        } else {
            eraselist[erasecount++] = node;
        }
        node = next;
    }
    for (i32 i = 0; i < erasecount; ++i) {
        node = eraselist[i];
        if (node != NULL) {
            if (node == set->oldest)
                set->oldest = node->next;
            if (set->active_count != 0) {
                ripple_node_s *newest = set->newest;
                ripple_node_s *free_head = set->free_head;
                if (node->previous != NULL)
                    node->previous->next = node->next;
                else
                    newest = node->next;
                if (node->next != NULL)
                    node->next->previous = node->previous;
                if (free_head == NULL) {
                    node->previous = node;
                    node->next = node;
                } else {
                    node->previous = free_head;
                    node->next = free_head->next;
                    free_head->next = node;
                    node->next->previous = node;
                }
                set->free_head = node;
                --set->active_count;
                set->newest = newest;
                if (set->active_count == 0) {
                    set->newest = NULL;
                    set->oldest = NULL;
                }
            }
        }
        eraselist[i] = NULL;
    }
    erasecount = 0;
}

void DrawRipple(ripple_node_s *node) {
    NURND_VERTEX3D vertices[4];
    vertices[0].position = {0.0f, node->size, 0.0f};
    vertices[1].position = {-node->size, 0.0f, 0.0f};
    vertices[2].position = {node->size, -0.0f, 0.0f};
    vertices[3].position = {-0.0f, -node->size, 0.0f};
    vertices[0].u = 1.0f;
    vertices[0].v = 0.0f;
    vertices[1].u = 0.0f;
    vertices[1].v = 0.0f;
    vertices[2].u = 1.0f;
    vertices[2].v = 1.0f;
    vertices[3].u = 1.0f;
    vertices[3].v = 0.0f;
    vertices[0].colour = node->color.value;
    vertices[1].colour = node->color.value;
    vertices[2].colour = node->color.value;
    vertices[3].colour = node->color.value;
    NUMTX matrix = node->matrix;
    NuRndrTriStrip3dClip(vertices, 4, &matrix, node->material);
}

void DrawRippleSet(ripple_set_s *set) {
    if (set == NULL)
        return;
    ripple_node_s *node = set->newest;
    for (i32 i = 0; i < set->active_count; ++i) {
        if (node != NULL) {
            DrawRipple(node);
            node = node->next;
        }
    }
}

void DestroyRippleMtls(WORLDINFO_s *world) {
    for (i32 i = 0; i < world->ripple_effect_count; ++i) {
        if (world->ripple_effects[i].material != NULL) {
            NuTexDestroy(world->ripple_effects[i].material->tex_id);
            NuMtlDestroy(world->ripple_effects[i].material);
        }
    }
}

