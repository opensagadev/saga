#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/numath/nuvec.h"
#include <string.h>

extern f32 FRAMETIME;

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

struct STREAK_s {
    STREAK_s *next;
    STREAK_s *prev;
    nuvec_s position;
    nuvec_s previous_position;
    nuvec_s start_tangent;
    nuvec_s end_tangent;
    nuvec_s positions[7];
    nuvec_s tangents[7];
    i32 segment_count;
    f32 remaining_time;
};

struct STREAKHDR_s {
    STREAKHDR_s *next;
    STREAKHDR_s *prev;
    STREAK_s *streaks;
    u16 index;
    u8 flags;
    u8 field_0xf;
    i32 has_new_streak;
    void **owner_slot;
    u32 colour;
};

DECOMP_ASSERT(sizeof(STREAK_s) == 0xe8, "STREAK_s size");
DECOMP_ASSERT(sizeof(STREAKHDR_s) == 0x1c, "STREAKHDR_s size");
DECOMP_ASSERT(offsetof(STREAK_s, positions) == 0x38, "Streak first edge samples");
DECOMP_ASSERT(offsetof(STREAK_s, tangents) == 0x8c, "Streak second edge samples");
DECOMP_ASSERT(offsetof(STREAK_s, remaining_time) == 0xe4, "Streak lifetime");
DECOMP_ASSERT(offsetof(STREAKHDR_s, owner_slot) == 0x14, "Streak owner slot");

static STREAKHDR_s streakhdrs[32];
static STREAKHDR_s *streakhdrs_free;
static STREAKHDR_s *streakhdrs_used;
static STREAK_s streaks[128];
static STREAK_s *streaks_free;
static STREAK_s *streaks_used;
numtl_s *streakmtl;
numtl_s *streakmtl_ref;

static void CalculateBezierPoint(NUVEC *result, NUVEC *start, NUVEC *end, NUVEC *start_tangent,
                                 NUVEC *end_tangent, f32 amount) {
    NUVEC control_a;
    NUVEC control_b;
    NUVEC negative_tangent;
    NuVecScale(&negative_tangent, end_tangent, -1.0f);
    NuVecAdd(&control_a, start, start_tangent);
    NuVecAdd(&control_b, end, &negative_tangent);

    const f32 inverse = 1.0f - amount;
    const f32 start_weight = inverse * inverse * inverse;
    const f32 control_a_weight = inverse * 3.0f * amount * inverse;
    const f32 control_b_weight = amount * 3.0f * amount * inverse;
    const f32 end_weight = amount * amount * amount;
    result->x = start->x * start_weight + control_a.x * control_a_weight +
                control_b.x * control_b_weight + end->x * end_weight;
    result->y = start->y * start_weight + control_a.y * control_a_weight +
                control_b.y * control_b_weight + end->y * end_weight;
    result->z = start->z * start_weight + control_a.z * control_a_weight +
                control_b.z * control_b_weight + end->z * end_weight;
}

static void CalculateStreakSegment(STREAK_s *newer, STREAK_s *segment) {
    for (i32 index = 1; index < segment->segment_count; ++index) {
        const f32 amount = static_cast<f32>(index) / static_cast<f32>(segment->segment_count);
        CalculateBezierPoint(&segment->positions[index - 1], &newer->position, &segment->position,
                             &newer->start_tangent, &segment->start_tangent, amount);
        CalculateBezierPoint(&segment->tangents[index - 1], &newer->previous_position,
                             &segment->previous_position, &newer->end_tangent, &segment->end_tangent, amount);
    }
}

static inline void UnlinkStreak(STREAK_s **head, STREAK_s *streak) {
    if (streak->prev != NULL) {
        streak->prev->next = streak->next;
    } else {
        *head = streak->next;
    }

    if (streak->next != NULL) {
        streak->next->prev = streak->prev;
    }
}

static inline void UnlinkStreakHeader(STREAKHDR_s **head, STREAKHDR_s *header) {
    if (header->prev != NULL) {
        header->prev->next = header->next;
    } else {
        *head = header->next;
    }

    if (header->next != NULL) {
        header->next->prev = header->prev;
    }
}

// Original 0x4a4ae0, 450 bytes.
void InitStreaks(variptr_u *buffer, variptr_u end, char *name) {
    for (i32 i = 0; i < 32; i++) {
        streakhdrs[i].index = i;
        streakhdrs[i].next = &streakhdrs[i + 1];
        streakhdrs[i].prev =
            reinterpret_cast<STREAKHDR_s *>(reinterpret_cast<usize>(&streakhdrs[i]) - sizeof(STREAKHDR_s));
    }

    streakhdrs_free = streakhdrs;
    streakhdrs[31].next = NULL;
    streakhdrs[0].prev = NULL;
    streakhdrs_used = NULL;

    for (i32 i = 0; i < 128; i++) {
        streaks[i].next = &streaks[i + 1];
        streaks[i].prev = reinterpret_cast<STREAK_s *>(reinterpret_cast<usize>(&streaks[i]) - sizeof(STREAK_s));
    }

    streaks_free = streaks;
    streaks[127].next = NULL;
    streaks[0].prev = NULL;
    streaks_used = NULL;

    streakmtl = NuMtlCreate3D(1);
    streakmtl->sort_pri = 500;
    streakmtl->attribs.unknown_1_1_2 = 0;
    streakmtl->attribs.unknown_1_4_8 = 0;
    streakmtl->attribs.z_mode = 1;
    streakmtl->opacity = 1.0f;
    streakmtl->attribs.alpha_mode = 2;
    streakmtl->attribs.filter_mode = 1;
    streakmtl->attribs.unknown_2_1_2 = 2;
    streakmtl->attribs.unknown_2_4 = 1;
    buffer->addr = ALIGN(buffer->addr, 16);
    const f32 five = 5.0f;
    memcpy(&streakmtl->filler3[4], &five, sizeof(five));
    streakmtl->particle_type_tag = -112;
    streakmtl->tex_id = NuTexRead(name, buffer, end);
    NuMtlUpdate(streakmtl);
    streakmtl_ref = NuMtlCreate3D(1);
    streakmtl_ref->particle_type_tag = -112;
    streakmtl_ref->opacity = 1.0f;
    streakmtl_ref->sort_pri = 1;
    streakmtl_ref->attribs.unknown_1_1_2 = 0;
    streakmtl_ref->attribs.unknown_1_4_8 = 0;
    streakmtl_ref->attribs.z_mode = 0;
    streakmtl_ref->attribs.alpha_mode = 2;
    streakmtl_ref->attribs.filter_mode = 1;
    memcpy(&streakmtl_ref->filler3[4], &five, sizeof(five));
    streakmtl_ref->attribs.unknown_2_1_2 = 2;
    streakmtl_ref->attribs.unknown_2_4 = 1;
    streakmtl_ref->tex_id = streakmtl->tex_id;
    NuMtlUpdate(streakmtl_ref);
}

void ResetStreaks() {
    STREAKHDR_s *used_headers = streakhdrs_used;
    STREAKHDR_s *header = used_headers;

    if (header == NULL) {
        return;
    }

    STREAKHDR_s *free_headers = streakhdrs_free;
    STREAK_s *free_streaks = streaks_free;

    do {
        while (header->streaks != NULL) {
            STREAK_s *streak = header->streaks;
            UnlinkStreak(&header->streaks, streak);
            streak->next = free_streaks;
            free_streaks = streak;
        }

        if (header->owner_slot != NULL) {
            if (*header->owner_slot == header) {
                *header->owner_slot = NULL;
            }
            header->owner_slot = NULL;
        }

        UnlinkStreakHeader(&used_headers, header);
        header->next = free_headers;
        free_headers = header;
        header = used_headers;
    } while (header != NULL);

    streaks_free = free_streaks;
    streakhdrs_used = NULL;
    streakhdrs_free = free_headers;
}

void UpdateStreaks(float elapsed) {
    if (elapsed == 0.0f) {
        return;
    }
    STREAKHDR_s *header = streakhdrs_used;
    while (header != NULL) {
        STREAKHDR_s *next_header = header->next;
        if (header->has_new_streak == 0 && header->owner_slot != NULL && *header->owner_slot == header) {
            *header->owner_slot = NULL;
            header->owner_slot = NULL;
        }
        header->has_new_streak = 0;

        STREAK_s *streak = header->streaks;
        while (streak != NULL) {
            STREAK_s *next_streak = streak->next;
            streak->remaining_time -= elapsed;
            if (streak->remaining_time <= 0.0f) {
                UnlinkStreak(&header->streaks, streak);
                streak->next = streaks_free;
                streaks_free = streak;
            }
            streak = next_streak;
        }

        if (header->streaks == NULL) {
            if (header->owner_slot != NULL) {
                if (*header->owner_slot == header) {
                    *header->owner_slot = NULL;
                }
                header->owner_slot = NULL;
            }
            UnlinkStreakHeader(&streakhdrs_used, header);
            header->next = streakhdrs_free;
            streakhdrs_free = header;
        }
        header = next_header;
    }
}

void DrawStreaks() {
    for (STREAKHDR_s *header = streakhdrs_used; header != NULL; header = header->next) {
        NURND_VERTEX3D vertices[254];
        i32 vertex_count = 0;
        STREAK_s *streak = header->streaks;

        while (vertex_count < 254 && streak != NULL) {
            const f32 fade = streak->remaining_time < 0.0f ? 0.0f : streak->remaining_time;
            i32 alpha = static_cast<i32>(static_cast<f32>(header->colour >> 24) * fade * 2.0f);
            if (alpha > 255)
                alpha = 255;
            const u32 colour = (header->colour & 0x00ffffff) | (static_cast<u32>(alpha) << 24);

            vertices[vertex_count].position = streak->position;
            vertices[vertex_count].colour = colour;
            vertices[vertex_count].u = 0.01f;
            vertices[vertex_count + 1].position = streak->previous_position;
            vertices[vertex_count + 1].colour = colour;
            vertices[vertex_count + 1].u = 0.99f;
            vertex_count += 2;

            streak = streak->next;
            if (streak == NULL)
                break;

            i32 segment_count = streak->segment_count;
            const i32 available = (256 - vertex_count) / 2;
            if (segment_count > available)
                segment_count = available;
            streak->segment_count = segment_count;
            if (segment_count < 2)
                continue;

            for (i32 index = 0; index < segment_count - 1; ++index) {
                const f32 segment_fade = fade < 0.0f ? 0.0f : fade;
                i32 segment_alpha =
                    static_cast<i32>(static_cast<f32>(header->colour >> 24) * segment_fade * 2.0f);
                if (segment_alpha > 255)
                    segment_alpha = 255;
                const u32 segment_colour =
                    (header->colour & 0x00ffffff) | (static_cast<u32>(segment_alpha) << 24);

                vertices[vertex_count].position = streak->positions[index];
                vertices[vertex_count].colour = segment_colour;
                vertices[vertex_count].u = 0.01f;
                vertices[vertex_count + 1].position = streak->tangents[index];
                vertices[vertex_count + 1].colour = segment_colour;
                vertices[vertex_count + 1].u = 1.01f;
                vertex_count += 2;
            }
        }

        for (i32 index = 0; index < vertex_count; index += 2) {
            const f32 v = static_cast<f32>(index) / static_cast<f32>(vertex_count) * 0.99f;
            vertices[index].v = v;
            vertices[index + 1].v = v;
        }

        NuRndrTriStrip3dClip(vertices, vertex_count, NULL, header->flags == 0 ? streakmtl : streakmtl_ref);
    }
}

void AddStreakPoints(nuvec_s *points, float duration, u32 colour, void **handle, i32 mode, void *) {
    STREAKHDR_s *header = static_cast<STREAKHDR_s *>(*handle);
    if (header == NULL) {
        if (streakhdrs_free == NULL)
            return;

        header = streakhdrs_free;
        streakhdrs_free = header->next;
        header->next = streakhdrs_used;
        if (streakhdrs_used != NULL)
            streakhdrs_used->prev = header;
        header->prev = NULL;
        header->streaks = NULL;
        header->owner_slot = handle;
        *handle = header;
        streakhdrs_used = header;
        header->colour = colour;
        header->flags = static_cast<u8>(mode);
    }

    header->has_new_streak = 1;
    header->colour = colour;

    STREAK_s *streak = streaks_free;
    if (streak != NULL) {
        streaks_free = streak->next;
        streak->next = header->streaks;
        if (header->streaks != NULL)
            header->streaks->prev = streak;
        streak->prev = NULL;
        header->streaks = streak;
    } else {
        streak = header->streaks;
        if (streak == NULL)
            return;
    }

    streak->position = points[0];
    streak->previous_position = points[1];
    streak->remaining_time = duration;
    if (streak->next == NULL)
        return;

    STREAK_s *older = streak->next;
    NuVecSub(&streak->start_tangent, &older->position, &streak->position);
    NuVecSub(&streak->end_tangent, &older->previous_position, &streak->previous_position);
    NuVecScale(&streak->start_tangent, &streak->start_tangent, 1.0f / 3.0f);
    NuVecScale(&streak->end_tangent, &streak->end_tangent, 1.0f / 3.0f);

    if (older->next == NULL) {
        NuVecSub(&older->start_tangent, &older->position, &streak->position);
        NuVecSub(&older->end_tangent, &older->previous_position, &streak->previous_position);
    } else {
        NUVEC incoming;
        NUVEC outgoing;
        NuVecSub(&incoming, &older->position, &streak->position);
        NuVecSub(&outgoing, &older->next->position, &older->position);
        NuVecAddScale(&older->start_tangent, &incoming, &outgoing, 0.5f);
        NuVecSub(&incoming, &older->previous_position, &streak->previous_position);
        NuVecSub(&outgoing, &older->next->previous_position, &older->previous_position);
        NuVecAddScale(&older->end_tangent, &incoming, &outgoing, 0.5f);
    }
    NuVecScale(&older->start_tangent, &older->start_tangent, 1.0f / 3.0f);
    NuVecScale(&older->end_tangent, &older->end_tangent, 1.0f / 3.0f);

    if (older->next != NULL && older->next->segment_count > 1)
        CalculateStreakSegment(older, older->next);

    NUVEC edge_a;
    NUVEC edge_b;
    NuVecSub(&edge_a, &streak->previous_position, &streak->position);
    NuVecSub(&edge_b, &older->previous_position, &older->position);
    NuVecNorm(&edge_a, &edge_a);
    NuVecNorm(&edge_b, &edge_b);

    older->segment_count = static_cast<i32>(60.0f * FRAMETIME + 60.0f * FRAMETIME);
    if (older->segment_count > 8)
        older->segment_count = 8;
    if (older->segment_count < 2)
        return;
    CalculateStreakSegment(streak, older);
}
